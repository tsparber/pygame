/*
    pygame - Python Game Library
    Copyright (C) 2000-2001  Pete Shinners

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

    Pete Shinners
    pete@shinners.org
*/
#define NO_PYGAME_C_API
#include "pygame.h"
#include "scrap.h"

/* macros used to create each constant */
#define DEC_CONST(x)  PyModule_AddIntConstant(module, #x, (int) SDL_##x);
#define DEC_CONSTK(x) PyModule_AddIntConstant(module, #x, (int) SDL##x);
#define DEC_CONSTN(x) PyModule_AddIntConstant(module, #x, (int) x);
#define DEC_CONSTS(x,y) PyModule_AddIntConstant(module, #x, (int) y);

#define ADD_STRING_CONST(x) PyModule_AddStringConstant(module, #x, x);

static PyMethodDef builtins[] =
{
    {NULL}
};

PYGAME_EXPORT
void initconstants(void)
{
    PyObject* module;
    
    module = Py_InitModule3
        ("constants", builtins,
         "Constants defined by SDL and needed in Pygame.\n");
    
    DEC_CONST(YV12_OVERLAY);
    DEC_CONST(IYUV_OVERLAY);
    DEC_CONST(YUY2_OVERLAY);
    DEC_CONST(UYVY_OVERLAY);
    DEC_CONST(YVYU_OVERLAY);
    
    DEC_CONST(SWSURFACE);
    DEC_CONST(HWSURFACE);
    DEC_CONST(RESIZABLE);
    DEC_CONST(ASYNCBLIT);
    DEC_CONST(OPENGL);
    DEC_CONST(OPENGLBLIT);
    DEC_CONST(ANYFORMAT);
    DEC_CONST(HWPALETTE);
    DEC_CONST(DOUBLEBUF);
    DEC_CONST(FULLSCREEN);
    DEC_CONST(HWACCEL);
    DEC_CONST(SRCCOLORKEY);
    DEC_CONST(RLEACCELOK);
    DEC_CONST(RLEACCEL);
    DEC_CONST(SRCALPHA);
    DEC_CONST(PREALLOC);
    DEC_CONST(NOFRAME);
    
    DEC_CONST(GL_RED_SIZE);
    DEC_CONST(GL_GREEN_SIZE);
    DEC_CONST(GL_BLUE_SIZE);
    DEC_CONST(GL_ALPHA_SIZE);
    DEC_CONST(GL_BUFFER_SIZE);
    DEC_CONST(GL_DOUBLEBUFFER);
    DEC_CONST(GL_DEPTH_SIZE);
    DEC_CONST(GL_STENCIL_SIZE);
    DEC_CONST(GL_ACCUM_RED_SIZE);
    DEC_CONST(GL_ACCUM_GREEN_SIZE);
    DEC_CONST(GL_ACCUM_BLUE_SIZE);
    DEC_CONST(GL_ACCUM_ALPHA_SIZE);
#if SDL_VERSION_ATLEAST(1, 2, 10)
    DEC_CONST(GL_SWAP_CONTROL);
#endif
#if SDL_VERSION_ATLEAST(1, 2, 5)
    DEC_CONST(GL_STEREO);
#else
    PyModule_AddIntConstant(module, "GL_STEREO", -1);
#endif
#if SDL_VERSION_ATLEAST(1, 2, 6)
    DEC_CONST(GL_MULTISAMPLEBUFFERS);
    DEC_CONST(GL_MULTISAMPLESAMPLES);
#else
    PyModule_AddIntConstant(module, "GL_MULTISAMPLEBUFFERS", -1);
    PyModule_AddIntConstant(module, "GL_MULTISAMPLESAMPLES", -1);
#endif

    DEC_CONSTN(TIMER_RESOLUTION);
    
    DEC_CONSTN(AUDIO_U8);
    DEC_CONSTN(AUDIO_S8);
    DEC_CONSTN(AUDIO_U16LSB);
    DEC_CONSTN(AUDIO_S16LSB);
    DEC_CONSTN(AUDIO_U16MSB);
    DEC_CONSTN(AUDIO_S16MSB);
    DEC_CONSTN(AUDIO_U16);
    DEC_CONSTN(AUDIO_S16);
    DEC_CONSTN(AUDIO_U16SYS);
    DEC_CONSTN(AUDIO_S16SYS);

#define SCRAP_TEXT PYGAME_SCRAP_TEXT
#define SCRAP_BMP PYGAME_SCRAP_BMP
#define SCRAP_PPM PYGAME_SCRAP_PPM
#define SCRAP_PBM PYGAME_SCRAP_PBM

    ADD_STRING_CONST(SCRAP_TEXT);
    ADD_STRING_CONST(SCRAP_BMP);
    ADD_STRING_CONST(SCRAP_PPM);
    ADD_STRING_CONST(SCRAP_PBM);
    PyModule_AddIntConstant(module, "SCRAP_CLIPBOARD", 0);
    PyModule_AddIntConstant(module, "SCRAP_SELECTION", 1);

#define PYGAME_BLEND_ADD 0x1
#define PYGAME_BLEND_ADD  0x1
#define PYGAME_BLEND_SUB  0x2
#define PYGAME_BLEND_MULT 0x3
#define PYGAME_BLEND_MIN  0x4
#define PYGAME_BLEND_MAX  0x5

    DEC_CONSTS(BLEND_ADD,  PYGAME_BLEND_ADD);
    DEC_CONSTS(BLEND_SUB,  PYGAME_BLEND_SUB);
    DEC_CONSTS(BLEND_MULT, PYGAME_BLEND_MULT);
    DEC_CONSTS(BLEND_MIN,  PYGAME_BLEND_MIN);
    DEC_CONSTS(BLEND_MAX,  PYGAME_BLEND_MAX);

    DEC_CONST(NOEVENT);
    DEC_CONST(ACTIVEEVENT);
    DEC_CONST(KEYDOWN);
    DEC_CONST(KEYUP);
    DEC_CONST(MOUSEMOTION);
    DEC_CONST(MOUSEBUTTONDOWN);
    DEC_CONST(MOUSEBUTTONUP);
    DEC_CONST(JOYAXISMOTION);
    DEC_CONST(JOYBALLMOTION);
    DEC_CONST(JOYHATMOTION);
    DEC_CONST(JOYBUTTONDOWN);
    DEC_CONST(JOYBUTTONUP);
    DEC_CONST(VIDEORESIZE);
    DEC_CONST(VIDEOEXPOSE);
    DEC_CONST(QUIT);
    DEC_CONST(SYSWMEVENT);
    DEC_CONST(USEREVENT);
    DEC_CONST(NUMEVENTS);
    
    DEC_CONST(HAT_CENTERED);
    DEC_CONST(HAT_UP);
    DEC_CONST(HAT_RIGHTUP);
    DEC_CONST(HAT_RIGHT);
    DEC_CONST(HAT_RIGHTDOWN);
    DEC_CONST(HAT_DOWN);
    DEC_CONST(HAT_LEFTDOWN);
    DEC_CONST(HAT_LEFT);
    DEC_CONST(HAT_LEFTUP);
    
    DEC_CONSTK(K_UNKNOWN);
    DEC_CONSTK(K_FIRST);
    DEC_CONSTK(K_BACKSPACE);
    DEC_CONSTK(K_TAB);
    DEC_CONSTK(K_CLEAR);
    DEC_CONSTK(K_RETURN);
    DEC_CONSTK(K_PAUSE);
    DEC_CONSTK(K_ESCAPE);
    DEC_CONSTK(K_SPACE);
    DEC_CONSTK(K_EXCLAIM);
    DEC_CONSTK(K_QUOTEDBL);
    DEC_CONSTK(K_HASH);
    DEC_CONSTK(K_DOLLAR);
    DEC_CONSTK(K_AMPERSAND);
    DEC_CONSTK(K_QUOTE);
    DEC_CONSTK(K_LEFTPAREN);
    DEC_CONSTK(K_RIGHTPAREN);
    DEC_CONSTK(K_ASTERISK);
    DEC_CONSTK(K_PLUS);
    DEC_CONSTK(K_COMMA);
    DEC_CONSTK(K_MINUS);
    DEC_CONSTK(K_PERIOD);
    DEC_CONSTK(K_SLASH);
    DEC_CONSTK(K_0);
    DEC_CONSTK(K_1);
    DEC_CONSTK(K_2);
    DEC_CONSTK(K_3);
    DEC_CONSTK(K_4);
    DEC_CONSTK(K_5);
    DEC_CONSTK(K_6);
    DEC_CONSTK(K_7);
    DEC_CONSTK(K_8);
    DEC_CONSTK(K_9);
    DEC_CONSTK(K_COLON);
    DEC_CONSTK(K_SEMICOLON);
    DEC_CONSTK(K_LESS);
    DEC_CONSTK(K_EQUALS);
    DEC_CONSTK(K_GREATER);
    DEC_CONSTK(K_QUESTION);
    DEC_CONSTK(K_AT);
    DEC_CONSTK(K_LEFTBRACKET);
    DEC_CONSTK(K_BACKSLASH);
    DEC_CONSTK(K_RIGHTBRACKET);
    DEC_CONSTK(K_CARET);
    DEC_CONSTK(K_UNDERSCORE);
    DEC_CONSTK(K_BACKQUOTE);
    DEC_CONSTK(K_a);
    DEC_CONSTK(K_b);
    DEC_CONSTK(K_c);
    DEC_CONSTK(K_d);
    DEC_CONSTK(K_e);
    DEC_CONSTK(K_f);
    DEC_CONSTK(K_g);
    DEC_CONSTK(K_h);
    DEC_CONSTK(K_i);
    DEC_CONSTK(K_j);
    DEC_CONSTK(K_k);
    DEC_CONSTK(K_l);
    DEC_CONSTK(K_m);
    DEC_CONSTK(K_n);
    DEC_CONSTK(K_o);
    DEC_CONSTK(K_p);
    DEC_CONSTK(K_q);
    DEC_CONSTK(K_r);
    DEC_CONSTK(K_s);
    DEC_CONSTK(K_t);
    DEC_CONSTK(K_u);
    DEC_CONSTK(K_v);
    DEC_CONSTK(K_w);
    DEC_CONSTK(K_x);
    DEC_CONSTK(K_y);
    DEC_CONSTK(K_z);
    DEC_CONSTK(K_DELETE);
    
    DEC_CONSTK(K_KP0);
    DEC_CONSTK(K_KP1);
    DEC_CONSTK(K_KP2);
    DEC_CONSTK(K_KP3);
    DEC_CONSTK(K_KP4);
    DEC_CONSTK(K_KP5);
    DEC_CONSTK(K_KP6);
    DEC_CONSTK(K_KP7);
    DEC_CONSTK(K_KP8);
    DEC_CONSTK(K_KP9);
    DEC_CONSTK(K_KP_PERIOD);
    DEC_CONSTK(K_KP_DIVIDE);
    DEC_CONSTK(K_KP_MULTIPLY);
    DEC_CONSTK(K_KP_MINUS);
    DEC_CONSTK(K_KP_PLUS);
    DEC_CONSTK(K_KP_ENTER);
    DEC_CONSTK(K_KP_EQUALS);
    DEC_CONSTK(K_UP);
    DEC_CONSTK(K_DOWN);
    DEC_CONSTK(K_RIGHT);
    DEC_CONSTK(K_LEFT);
    DEC_CONSTK(K_INSERT);
    DEC_CONSTK(K_HOME);
    DEC_CONSTK(K_END);
    DEC_CONSTK(K_PAGEUP);
    DEC_CONSTK(K_PAGEDOWN);
    DEC_CONSTK(K_F1);
    DEC_CONSTK(K_F2);
    DEC_CONSTK(K_F3);
    DEC_CONSTK(K_F4);
    DEC_CONSTK(K_F5);
    DEC_CONSTK(K_F6);
    DEC_CONSTK(K_F7);
    DEC_CONSTK(K_F8);
    DEC_CONSTK(K_F9);
    DEC_CONSTK(K_F10);
    DEC_CONSTK(K_F11);
    DEC_CONSTK(K_F12);
    DEC_CONSTK(K_F13);
    DEC_CONSTK(K_F14);
    DEC_CONSTK(K_F15);
    
    DEC_CONSTK(K_NUMLOCK);
    DEC_CONSTK(K_CAPSLOCK);
    DEC_CONSTK(K_SCROLLOCK);
    DEC_CONSTK(K_RSHIFT);
    DEC_CONSTK(K_LSHIFT);
    DEC_CONSTK(K_RCTRL);
    DEC_CONSTK(K_LCTRL);
    DEC_CONSTK(K_RALT);
    DEC_CONSTK(K_LALT);
    DEC_CONSTK(K_RMETA);
    DEC_CONSTK(K_LMETA);
    DEC_CONSTK(K_LSUPER);
    DEC_CONSTK(K_RSUPER);
    DEC_CONSTK(K_MODE);
    
    DEC_CONSTK(K_HELP);
    DEC_CONSTK(K_PRINT);
    DEC_CONSTK(K_SYSREQ);
    DEC_CONSTK(K_BREAK);
    DEC_CONSTK(K_MENU);
    DEC_CONSTK(K_POWER);
    DEC_CONSTK(K_EURO);
    DEC_CONSTK(K_LAST);
    
    DEC_CONSTN(KMOD_NONE);
    DEC_CONSTN(KMOD_LSHIFT);
    DEC_CONSTN(KMOD_RSHIFT);
    DEC_CONSTN(KMOD_LCTRL);
    DEC_CONSTN(KMOD_RCTRL);
    DEC_CONSTN(KMOD_LALT);
    DEC_CONSTN(KMOD_RALT);
    DEC_CONSTN(KMOD_LMETA);
    DEC_CONSTN(KMOD_RMETA);
    DEC_CONSTN(KMOD_NUM);
    DEC_CONSTN(KMOD_CAPS);
    DEC_CONSTN(KMOD_MODE);
    
    DEC_CONSTN(KMOD_CTRL);
    DEC_CONSTN(KMOD_SHIFT);
    DEC_CONSTN(KMOD_ALT);
    DEC_CONSTN(KMOD_META);
}
