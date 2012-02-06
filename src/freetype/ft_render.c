/*
  pygame - Python Game Library
  Copyright (C) 2009 Vicent Marti

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#define PYGAME_FREETYPE_INTERNAL

#include "ft_wrap.h"
#include "../pgview.h"
#include FT_MODULE_H
#include FT_OUTLINE_H

static const FontColor mono_opaque = {0, 0, 0, SDL_ALPHA_OPAQUE};
static const FontColor mono_transparent = {0, 0, 0, SDL_ALPHA_TRANSPARENT};

static void render(FreeTypeInstance *, FontText *, const FontRenderMode *,
                   const FontColor *, FontSurface *, unsigned, unsigned,
                   FT_Vector *, FT_Pos, FT_Fixed);

int
_PGFT_CheckStyle(FT_UInt32 style)
{
    const FT_UInt32 max_style =
        FT_STYLE_NORMAL |
        FT_STYLE_STRONG |
        FT_STYLE_OBLIQUE |
        FT_STYLE_UNDERLINE |
        FT_STYLE_WIDE;

    return style > max_style;
}

int
_PGFT_BuildRenderMode(FreeTypeInstance *ft,
                      PgFontObject *fontobj, FontRenderMode *mode,
                      int pt_size, int style, int rotation)
{
    int angle;

    if (pt_size == -1) {
        if (fontobj->ptsize == -1) {
            PyErr_SetString(PyExc_ValueError,
                  "No font point size specified"
                  " and no default font size in typeface");
            return -1;
        }

        pt_size = fontobj->ptsize;
    }
    if (pt_size <= 0) {
        PyErr_SetString(PyExc_ValueError, "Invalid point size for font.");
        return -1;
    }
    mode->pt_size = (FT_UInt16)pt_size;

    if (style == FT_STYLE_DEFAULT) {
        mode->style = fontobj->style;
    }
    else {
        if (_PGFT_CheckStyle((FT_UInt32)style)) {
            PyErr_SetString(PyExc_ValueError, "Invalid style value");
            return -1;
        }

        mode->style = (FT_UInt16)style;
    }
    mode->strength = DBL_TO_FX16(fontobj->strength);
    mode->underline_adjustment = DBL_TO_FX16(fontobj->underline_adjustment);
    mode->render_flags = fontobj->render_flags;
    angle = rotation % 360;
    while (angle < 0) angle += 360;
    mode->rotation_angle = INT_TO_FX16(angle);
    mode->transform = fontobj->transform;

    if (mode->rotation_angle != 0) {
        if (mode->style & FT_STYLE_WIDE) {
            PyErr_SetString(PyExc_ValueError,
                  "the wide style is unsupported for rotated text");
            return -1;
        }
        if (mode->style & FT_STYLE_UNDERLINE) {
            PyErr_SetString(PyExc_ValueError,
                  "the underline style is unsupported for rotated text");
            return -1;
        }
        if (mode->render_flags & FT_RFLAG_PAD) {
            PyErr_SetString(PyExc_ValueError,
                  "padding is unsupported for rotated text");
            return -1;
        }
    }

    if (mode->render_flags & FT_RFLAG_VERTICAL) {
        if (mode->style & FT_STYLE_UNDERLINE) {
            PyErr_SetString(PyExc_ValueError,
                  "the underline style is unsupported for vertical text");
            return -1;
        }
    }

    return 0;
}

void
_PGFT_GetRenderMetrics(const FontRenderMode *mode, FontText *text,
                       unsigned *w, unsigned *h, FT_Vector *offset,
                       FT_Pos *underline_top, FT_Fixed *underline_size)
{
    FT_Pos min_x = text->min_x;
    FT_Pos max_x = text->max_x;
    FT_Pos min_y = text->min_y;
    FT_Pos max_y = text->max_y;

    *underline_size = 0;
    if (mode->style & FT_STYLE_UNDERLINE) {
        FT_Fixed half_size = (text->underline_size + 1) / 2;
        FT_Fixed adjusted_pos;

        if (mode->underline_adjustment < 0) {
            adjusted_pos = FT_MulFix(text->ascender,
                                     mode->underline_adjustment);
        }
        else {
            adjusted_pos = FT_MulFix(text->underline_pos,
                                     mode->underline_adjustment);
        }
        if (adjusted_pos + half_size > max_y) {
            max_y = adjusted_pos + half_size;
        }
        if (adjusted_pos - half_size < min_y) {
            min_y = adjusted_pos - half_size;
        }
        *underline_size = text->underline_size;
        *underline_top = adjusted_pos - half_size;
    }

    offset->x = -min_x;
    offset->y = -min_y;
    *w = (unsigned)FX6_TRUNC(FX6_CEIL(max_x) - FX6_FLOOR(min_x));
    *h = (unsigned)FX6_TRUNC(FX6_CEIL(max_y) - FX6_FLOOR(min_y));
}


/*********************************************************
 *
 * Rendering on SDL-specific surfaces
 *
 *********************************************************/
#ifdef HAVE_PYGAME_SDL_VIDEO
int
_PGFT_Render_ExistingSurface(FreeTypeInstance *ft, PgFontObject *fontobj,
                             const FontRenderMode *mode, PGFT_String *text,
                             SDL_Surface *surface, int x, int y,
                             FontColor *fgcolor, FontColor *bgcolor,
                             SDL_Rect *r)
{
    static const FontRenderPtr __SDLrenderFuncs[] = {
        0,
        __render_glyph_RGB1,
        __render_glyph_RGB2,
        __render_glyph_RGB3,
        __render_glyph_RGB4
    };

    static const FontRenderPtr __MONOrenderFuncs[] = {
        0,
        __render_glyph_MONO1,
        __render_glyph_MONO2,
        __render_glyph_MONO3,
        __render_glyph_MONO4
    };

    static const FontFillPtr __RGBfillFuncs[] = {
        0,
        __fill_glyph_RGB1,
        __fill_glyph_RGB2,
        __fill_glyph_RGB3,
        __fill_glyph_RGB4
    };

    int locked = 0;
    unsigned width;
    unsigned height;
    FT_Vector offset;
    FT_Vector surf_offset;
    FT_Pos underline_top;
    FT_Fixed underline_size;

    FontSurface font_surf;
    FontText *font_text;

    if (PGFT_String_GET_LENGTH(text) == 0) {
        /* No rendering */
        r->x = 0;
        r->y = 0;
        r->w = 0;
        r->h = _PGFT_Font_GetHeightSized(ft, fontobj, mode->pt_size);
        return 0;
    }

    if (SDL_MUSTLOCK(surface)) {
        if (SDL_LockSurface(surface) == -1) {
            SDL_FreeSurface(surface);
            PyErr_SetString(PyExc_SDLError, SDL_GetError());
            return -1;
        }
        locked = 1;
    }

    /* build font text */
    font_text = _PGFT_LoadFontText(ft, fontobj, mode, text);
    if (!font_text) {
        if (locked) {
            SDL_UnlockSurface(surface);
        }
        return -1;
    }

    _PGFT_GetRenderMetrics(mode, font_text, &width, &height, &offset,
                           &underline_top, &underline_size);
    if (width == 0 || height == 0) {
        /* Nothing more to do. */
        if (locked) {
            SDL_UnlockSurface(surface);
        }
        r->x = 0;
        r->y = 0;
        r->w = 0;
        r->h = _PGFT_Font_GetHeightSized(ft, fontobj, mode->pt_size);
        return 0;
    }
    surf_offset.x = INT_TO_FX6(x);
    surf_offset.y = INT_TO_FX6(y);
    if (mode->render_flags & FT_RFLAG_ORIGIN) {
        x -= FX6_TRUNC(FX6_CEIL(offset.x));
        y -= FX6_TRUNC(FX6_CEIL(offset.y));
    }
    else {
        surf_offset.x += offset.x;
        surf_offset.y += offset.y;
    }

    /*
     * Setup target surface struct
     */
    font_surf.buffer = surface->pixels;
    font_surf.width = surface->w;
    font_surf.height = surface->h;
    font_surf.pitch = surface->pitch;
    font_surf.format = surface->format;
    font_surf.render_gray = __SDLrenderFuncs[surface->format->BytesPerPixel];
    font_surf.render_mono = __MONOrenderFuncs[surface->format->BytesPerPixel];
    font_surf.fill = __RGBfillFuncs[surface->format->BytesPerPixel];

    /*
     * if bg color exists, paint background
     */
    if (bgcolor) {
        if (bgcolor->a == SDL_ALPHA_OPAQUE) {
            SDL_Rect    bg_fill;
            FT_UInt32   fillcolor;

            fillcolor = SDL_MapRGBA(surface->format,
                    bgcolor->r, bgcolor->g, bgcolor->b, bgcolor->a);

            bg_fill.x = (FT_Int16)x;
            bg_fill.y = (FT_Int16)y;
            bg_fill.w = (FT_UInt16)width;
            bg_fill.h = (FT_UInt16)height;

            SDL_FillRect(surface, &bg_fill, fillcolor);
        }
        else {
            font_surf.fill(INT_TO_FX6(x), INT_TO_FX6(y),
                           INT_TO_FX6(width), INT_TO_FX6(height),
                           &font_surf, bgcolor);
        }
    }

    /*
     * Render!
     */
    render(ft, font_text, mode, fgcolor, &font_surf,
           width, height, &surf_offset, underline_top, underline_size);

    r->x = -(Sint16)FX6_TRUNC(FX6_FLOOR(offset.x));
    r->y = (Sint16)FX6_TRUNC(FX6_CEIL(offset.y));
    r->w = (Uint16)width;
    r->h = (Uint16)height;

    if (locked) {
        SDL_UnlockSurface(surface);
    }

    return 0;
}

SDL_Surface *_PGFT_Render_NewSurface(FreeTypeInstance *ft,
                                     PgFontObject *fontobj,
                                     const FontRenderMode *mode,
                                     PGFT_String *text,
                                     FontColor *fgcolor, FontColor *bgcolor,
                                     SDL_Rect *r)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    FT_UInt32 rmask = 0xff000000;
    FT_UInt32 gmask = 0x00ff0000;
    FT_UInt32 bmask = 0x0000ff00;
    FT_UInt32 amask = 0x000000ff;
#else
    FT_UInt32 rmask = 0x000000ff;
    FT_UInt32 gmask = 0x0000ff00;
    FT_UInt32 bmask = 0x00ff0000;
    FT_UInt32 amask = 0xff000000;
#endif
    int locked = 0;
    FT_UInt32 fillcolor;
    SDL_Surface *surface = 0;
    FT_UInt32 bits_per_pixel =
        (bgcolor || mode->render_flags & FT_RFLAG_ANTIALIAS) ? 32 : 8;
    FT_UInt32 surface_flags = SDL_SWSURFACE;

    FontSurface font_surf;
    FontText *font_text;
    unsigned width;
    unsigned height;
    FT_Vector offset;
    FT_Pos underline_top;
    FT_Fixed underline_size;
    FontColor mono_fgcolor = {0, 0, 0, 1};
    FontColor mono_bgcolor = {0, 0, 0, 0};

    /* build font text */
    font_text = _PGFT_LoadFontText(ft, fontobj, mode, text);
    if (!font_text) {
        return 0;
    }

    if (font_text->length > 0) {
        _PGFT_GetRenderMetrics(mode, font_text, &width, &height, &offset,
                               &underline_top, &underline_size);
    }
    else {
        width = 1;
        height = _PGFT_Font_GetHeightSized(ft, fontobj, mode->pt_size);
        offset.x = -font_text->min_x;
        offset.y = -font_text->min_y;
    }

    surface = SDL_CreateRGBSurface(surface_flags, width, height,
                   bits_per_pixel, rmask, gmask, bmask,
                   bits_per_pixel == 32 ? amask : 0);
    if (!surface) {
        PyErr_SetString(PyExc_SDLError, SDL_GetError());
        return 0;
    }

    if (SDL_MUSTLOCK(surface)) {
        if (SDL_LockSurface(surface) == -1) {
            PyErr_SetString(PyExc_SDLError, SDL_GetError());
            SDL_FreeSurface(surface);
            return 0;
        }
        locked = 1;
    }

    font_surf.buffer = surface->pixels;
    font_surf.width = surface->w;
    font_surf.height = surface->h;
    font_surf.pitch = surface->pitch;
    font_surf.format = surface->format;
    if (bits_per_pixel == 32) {
        font_surf.render_gray = __render_glyph_RGB4;
        font_surf.render_mono = __render_glyph_MONO4;
        font_surf.fill = __fill_glyph_RGB4;
        /*
         * Fill our texture with the required bg color
         */
        if (bgcolor) {
            fillcolor = SDL_MapRGBA(
                surface->format,
                bgcolor->r, bgcolor->g, bgcolor->b, bgcolor->a);
        }
        else {
            fillcolor = SDL_MapRGBA(surface->format,
                                    0, 0, 0, SDL_ALPHA_TRANSPARENT);
        }
        SDL_FillRect(surface, 0, fillcolor);
    }
    else {
        SDL_Color colors[2];

        colors[1].r = fgcolor->r;  /* Foreground */
        colors[1].g = fgcolor->g;
        colors[1].b = fgcolor->b;
        colors[0].r = ~colors[1].r;  /* Background */
        colors[0].g = ~colors[1].g;
        colors[0].b = ~colors[1].b;
        if (!SDL_SetColors(surface, colors, 0, 2)) {
            PyErr_SetString(PyExc_SystemError,
                            "Pygame bug in _PGFT_Render_NewSurface: "
                            "SDL_SetColors failed");
            SDL_FreeSurface(surface);
            return 0;
        }
        SDL_SetColorKey(surface, SDL_SRCCOLORKEY, (FT_UInt32)0);
        if (fgcolor->a != SDL_ALPHA_OPAQUE) {
            SDL_SetAlpha(surface, SDL_SRCALPHA, fgcolor->a);
        }
        fgcolor = &mono_fgcolor;
        bgcolor = &mono_bgcolor;
        font_surf.render_gray = __render_glyph_GRAY_as_MONO1;
        font_surf.render_mono = __render_glyph_MONO_as_GRAY1;
        font_surf.fill = __fill_glyph_GRAY1;
        /*
         * Fill our texture with the required bg color
         */
        SDL_FillRect(surface, 0, 0);
    }

    /*
     * Render the text!
     */
    render(ft, font_text, mode, fgcolor, &font_surf,
           width, height, &offset, underline_top, underline_size);

    r->x = -(Sint16)FX6_TRUNC(FX6_FLOOR(offset.x));
    r->y = (Sint16)FX6_TRUNC(FX6_CEIL(offset.y));
    r->w = (Uint16)width;
    r->h = (Uint16)height;

    if (locked) {
        SDL_UnlockSurface(surface);
    }

    return surface;
}
#endif  /* #ifdef HAVE_PYGAME_SDL_VIDEO */




/*********************************************************
 *
 * Rendering on generic arrays
 *
 *********************************************************/

PyObject *_PGFT_Render_PixelArray(FreeTypeInstance *ft, PgFontObject *fontobj,
                                  const FontRenderMode *mode,
                                  PGFT_String *text, int invert,
                                  int *_width, int *_height)
{
    FT_Byte *buffer = 0;
    PyObject *array = 0;
    FontSurface surf;

    FontText *font_text;
    unsigned width;
    unsigned height;
    FT_Vector offset;
    FT_Pos underline_top;
    FT_Fixed underline_size;
    int array_size;

    if (PGFT_String_GET_LENGTH(text) == 0) {
        /* Empty array */
        *_width = 0;
        *_height = _PGFT_Font_GetHeight(ft, fontobj);
        return Bytes_FromStringAndSize("", 0);
    }

    /* build font text */
    font_text = _PGFT_LoadFontText(ft, fontobj, mode, text);

    if (!font_text) {
        return 0;
    }

    _PGFT_GetRenderMetrics(mode, font_text, &width, &height, &offset,
                           &underline_size, &underline_top);

    array_size = width * height;
    if (array_size == 0) {
        /* Empty array */
        *_width = 0;
        *_height = height;
        return Bytes_FromStringAndSize("", 0);
    }

    /* Create an uninitialized string whose buffer can be directly set. */
    array = Bytes_FromStringAndSize(0, array_size);
    if (!array) {
        return 0;
    }
    buffer = (FT_Byte *)Bytes_AS_STRING(array);
    if (invert) {
        memset(buffer, SDL_ALPHA_OPAQUE, (size_t)array_size);
    }
    else {
        memset(buffer, SDL_ALPHA_TRANSPARENT, (size_t)array_size);
    }
    surf.buffer = buffer;
    surf.width = width;
    surf.height = height;
    surf.pitch = (int)surf.width;
    surf.format = 0;
    surf.render_gray = __render_glyph_GRAY1;
    surf.render_mono = __render_glyph_MONO_as_GRAY1;
    surf.fill = __fill_glyph_GRAY1;

    render(ft, font_text, mode, invert ? &mono_transparent : &mono_opaque,
           &surf, width, height, &offset, underline_top, underline_size);

    *_width = width;
    *_height = height;

    return array;
}

int
_PGFT_Render_Array(FreeTypeInstance *ft, PgFontObject *fontobj,
                   const FontRenderMode *mode, PyObject *arrayobj,
                   PGFT_String *text, int invert,
                   int x, int y, SDL_Rect *r)
{
    static int view_init = 0;

    PyObject *cobj = 0;
    PyArrayInterface *inter_p;

    unsigned width;
    unsigned height;
    int itemsize;
    FT_Vector offset;
    FT_Vector array_offset;
    FT_Pos underline_top;
    FT_Fixed underline_size;

    FontSurface font_surf;
    SDL_PixelFormat format;
    FontText *font_text;

    /* Get target buffer */
    if (!view_init) {
        import_pygame_view();
        if (PyErr_Occurred()) {
            return -1;
        }
    }
    if (Pg_GetArrayInterface(arrayobj, &cobj, &inter_p)) {
        return -1;
    }
    if (inter_p->nd != 2) {
        Py_DECREF(cobj);
        PyErr_Format(PyExc_ValueError,
                     "expecting a 2d target array: got %id array instead",
                     (int)inter_p->nd);
        return -1;
    }
    switch (inter_p->typekind) {

    case 'i':  /* integer */
        break;
    case 'u':  /* unsigned integer */
        break;
    case 'S':  /* fixed length character field */
        break;
    default:
        Py_DECREF(cobj);
        PyErr_Format(PyExc_ValueError, "unsupported target array type '%c'",
                     inter_p->typekind);
        return -1;
    }

    width = inter_p->shape[0];
    height = inter_p->shape[1];
    itemsize = inter_p->itemsize;

    /* if empty string, then nothing more to do */
    if (PGFT_String_GET_LENGTH(text) == 0) {
        Py_DECREF(cobj);
        r->x = 0;
        r->y = 0;
        r->w = 0;
        r->h = _PGFT_Font_GetHeightSized(ft, fontobj, mode->pt_size);
        return 0;
    }

    /* build font text */
    font_text = _PGFT_LoadFontText(ft, fontobj, mode, text);
    if (!font_text) {
        Py_DECREF(cobj);
        return -1;
    }

    _PGFT_GetRenderMetrics(mode, font_text, &width, &height, &offset,
                           &underline_top, &underline_size);
    if (width == 0 || height == 0) {
        /* Nothing more to do. */
        Py_DECREF(cobj);
        r->x = 0;
        r->y = 0;
        r->w = 0;
        r->h = _PGFT_Font_GetHeightSized(ft, fontobj, mode->pt_size);
        return 0;
    }
    array_offset.x = INT_TO_FX6(x);
    array_offset.y = INT_TO_FX6(y);
    if (mode->render_flags & FT_RFLAG_ORIGIN) {
        x -= FX6_TRUNC(FX6_CEIL(offset.x));
        y -= FX6_TRUNC(FX6_CEIL(offset.y));
    }
    else {
        array_offset.x += offset.x;
        array_offset.y += offset.y;
    }

    /*
     * Setup target surface struct
     */
    format.BytesPerPixel = itemsize;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    format.Ashift = ((inter_p->flags & PAI_NOTSWAPPED) ?
                     0 : (itemsize - 1) * 8);
#else
    format.Ashift = ((inter_p->flags & PAI_NOTSWAPPED) ?
                     (itemsize - 1) * 8 : 0);
#endif
    font_surf.buffer = inter_p->data;
    font_surf.width = inter_p->shape[0];
    font_surf.height = inter_p->shape[1];
    font_surf.item_stride = inter_p->strides[0];
    font_surf.pitch = inter_p->strides[1];
    font_surf.format = &format;
    font_surf.render_gray = __render_glyph_INT;
    font_surf.render_mono = __render_glyph_MONO_as_INT;
    font_surf.fill = __fill_glyph_INT;

    render(ft, font_text, mode, invert ? &mono_transparent : &mono_opaque,
           &font_surf, width, height, &array_offset, underline_top,
           underline_size);

    Py_DECREF(cobj);
    r->x = -(Sint16)FX6_TRUNC(FX6_FLOOR(offset.x));
    r->y = (Sint16)FX6_TRUNC(FX6_CEIL(offset.y));
    r->w = (Uint16)width;
    r->h = (Uint16)height;

    return 0;
}

/*********************************************************
 *
 * New rendering algorithm (full thickness underlines)
 *
 *********************************************************/
static void
render(FreeTypeInstance *ft, FontText *text, const FontRenderMode *mode,
       const FontColor *fg_color, FontSurface *surface,
       unsigned width, unsigned height, FT_Vector *offset,
       FT_Pos underline_top, FT_Fixed underline_size)
{
    FT_Pos top;
    FT_Pos left;
    int x;
    int y;
    int n;
    int length = text->length;
    FontGlyph **glyphs = text->glyphs;
    FT_BitmapGlyph image;
    FT_Vector *posns = text->posns;
    FontRenderPtr render_gray = surface->render_gray;
    FontRenderPtr render_mono = surface->render_mono;
    int is_underline_gray = 0;

    if (length <= 0) {
        return;
    }
    left = offset->x;
    top = offset->y;
    for (n = 0; n < length; ++n) {
        image = glyphs[n]->image;
        x = FX6_TRUNC(FX6_CEIL(left + posns[n].x));
        y = FX6_TRUNC(FX6_CEIL(top + posns[n].y));
        if (image->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY) {
            render_gray(x, y, surface, &(image->bitmap), fg_color);
            is_underline_gray = 1;
        }
        else {
            render_mono(x, y, surface, &(image->bitmap), fg_color);
        }
    }

    if (underline_size > 0) {
        if (is_underline_gray) {
            surface->fill(left + text->min_x, top + underline_top,
                          INT_TO_FX6(width), underline_size,
                          surface, fg_color);
        }
        else {
            surface->fill(FX6_CEIL(left + text->min_x),
                          FX6_CEIL(top + underline_top),
                          INT_TO_FX6(width), FX6_CEIL(underline_size),
                          surface, fg_color);
        }
    }
}
