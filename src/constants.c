/*
    pygame - Python Game Library
    Copyright (C) 2000  Pete Shinners

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

#include "pygame.h"



/* macros used to create each constant */
#define DEC_CONST(x)  PyModule_AddIntConstant(module, #x, SDL_##x);
#define DEC_CONSTK(x) PyModule_AddIntConstant(module, #x, SDL##x);
#define DEC_CONSTN(x) PyModule_AddIntConstant(module, #x, x);


static PyMethodDef builtins[] =
{
	{NULL}
};


    /*DOC*/ static char doc_pygame_constants_MODULE[] =
    /*DOC*/    "These constants are defined by SDL, and needed in pygame. Note\n"
    /*DOC*/    "that many of the flags for SDL are not needed in pygame, and are\n"
    /*DOC*/    "not included here. These constants are generally accessed from\n"
    /*DOC*/    "the pygame.locals module. This module is automatically placed in\n"
    /*DOC*/    "the pygame namespace, but you will usually want to place them\n"
    /*DOC*/    "directly into your module's namespace with the following command,\n"
    /*DOC*/    "'from pygame.locals import *'.\n"
    /*DOC*/ ;


void initconstants(void)
{
	PyObject* module;

	PyGAME_C_API[0] = PyGAME_C_API[0]; /*this cleans up compiler warning*/


	module = Py_InitModule3("constants", builtins, doc_pygame_constants_MODULE);

	DEC_CONST(SWSURFACE);
	DEC_CONST(HWSURFACE);
	DEC_CONST(RESIZABLE);
	DEC_CONST(ASYNCBLIT);
	DEC_CONST(OPENGL);
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

#if 0 /*documentation only*/
    /*DOC*/ static char doc_display[] =
    /*DOC*/    "pygame.constants.display (constants)\n"
    /*DOC*/    "The following constants are used by the display module and Surfaces\n"
    /*DOC*/    "\n"
    /*DOC*/    "HWSURFACE - surface in hardware video memory. (equal to 1)<br>\n"
    /*DOC*/    "RESIZEABLE - display window is resizeable<br>\n"
    /*DOC*/    "ASYNCBLIT - surface blits happen asynchronously (threaded)<br>\n"
    /*DOC*/    "OPENGL - display surface will be controlled by opengl<br>\n"
    /*DOC*/    "OPENGLBLIT - opengl controlled display surface will allow sdl\n"
    /*DOC*/    "blits<br>\n"
    /*DOC*/    "HWPALETTE - display surface has animatable hardware palette\n"
    /*DOC*/    "entries<br>\n"
    /*DOC*/    "DOUBLEBUF - hardware display surface is page flippable<br>\n"
    /*DOC*/    "FULLSCREEN - display surface is fullscreen (nonwindowed)<br>\n"
    /*DOC*/    "RLEACCEL - compile for quick alpha blits, only set in alpha or\n"
    /*DOC*/    "colorkey funcs<br>\n"
    /*DOC*/ ;

    /*DOC*/ static char doc_events[] =
    /*DOC*/    "pygame.constants.events (constants)\n"
    /*DOC*/    "These constants define the various event types\n"
    /*DOC*/    "\n"
    /*DOC*/    "NOEVENT - no event, represents an empty event list, equal to 0<br>\n"
    /*DOC*/    "ACTIVEEVENT - window has gain/lost mouse/keyboard/visiblity focus<br>\n"
    /*DOC*/    "KEYDOWN - keyboard button has been pressed (or down and repeating)<br>\n"
    /*DOC*/    "KEYUP - keyboard button has been released<br>\n"
    /*DOC*/    "MOUSEMOTION - mouse has moved<br>\n"
    /*DOC*/    "MOUSEBUTTONDOWN- mouse button has been pressed<br>\n"
    /*DOC*/    "MOUSEBUTTONUP - mouse button has been released<br>\n"
    /*DOC*/    "JOYAXISMOTION - an opened joystick axis has changed<br>\n"
    /*DOC*/    "JOYBALLMOTION - an opened joystick ball has moved<br>\n"
    /*DOC*/    "JOYHATMOTION - an opened joystick hat has moved<br>\n"
    /*DOC*/    "JOYBUTTONDOWN - an opened joystick button has been pressed<br>\n"
    /*DOC*/    "JOYBUTTONUP - an opened joystick button has been released<br>\n"
    /*DOC*/    "VIDEORESIZE - the display window has been resized by the user<br>\n"
    /*DOC*/    "QUIT - the user has requested the game to quit<br>\n"
    /*DOC*/    "SYSWMEVENT - currently unsupported, system dependant<br>\n"
    /*DOC*/    "USEREVENTS - all user messages are this or higher<br>\n"
    /*DOC*/    "NUMEVENTS - all user messages must be lower than this, equal to 32<br>\n"
    /*DOC*/ ;

    /*DOC*/ static char doc_keyboard[] =
    /*DOC*/    "pygame.constants.keyboard (constants)\n"
    /*DOC*/    "These constants represent the keys on the keyboard.\n"
    /*DOC*/    "\n"
    /*DOC*/    "There are many keyboard constants, they are used to represent\n"
    /*DOC*/    "keys on the keyboard. The following is a list of all keyboard\n"
    /*DOC*/    "constants\n"
    /*DOC*/    "\n"
    /*DOC*/    "K_UNKNOWN, K_FIRST, K_BACKSPACE, K_TAB, K_CLEAR, K_RETURN,\n"
    /*DOC*/    "K_PAUSE,<br>\n"
    /*DOC*/    ".... list unfinished, sorry :-P\n"
    /*DOC*/ ;

    /*DOC*/ static char doc_modifiers[] =
    /*DOC*/    "pygame.constants.modifiers (constants)\n"
    /*DOC*/    "These constants represent the modifier keys on the keyboard.\n"
    /*DOC*/    "\n"
    /*DOC*/    "Their states are treated slightly differently than normal\n"
    /*DOC*/    "keyboard button states, and you can temporarily set their states.\n"
    /*DOC*/    "\n"
    /*DOC*/    "KMOD_NONE, KMOD_LSHIFT, KMOD_RSHIFT, KMOD_SHIFT, KMOD_CAPS,<br>\n"
    /*DOC*/    "KMOD_LCTRL, KMOD_RCTRL, KMOD_CTRL, KMOD_LALT, KMOD_RALT,<br>\n"
    /*DOC*/    "KMOD_ALT, KMOD_LMETA, KMOD_RMETA, KMOD_META, KMOD_NUM, kMOD_MODE<br>\n"
    /*DOC*/ ;

    /*DOC*/ static char doc_zdeprecated[] =
    /*DOC*/    "pygame.constants.zdepracated (constants)\n"
    /*DOC*/    "The following constants are made available, but generally not needed\n"
    /*DOC*/    "\n"
    /*DOC*/    "The flags labeled as readonly should never be used,\n"
    /*DOC*/    "except when comparing checking flags against Surface.get_flags().\n"
    /*DOC*/    "\n"
    /*DOC*/    "SWSURFACE - not really usable as a surface flag, equates to 0 and\n"
    /*DOC*/    "is always default<br>\n"
    /*DOC*/    "ANYFORMAT - used to create surfaces, pygame defaults to this flag\n"
    /*DOC*/    "if you don't specifya bit depth<br>\n"
    /*DOC*/    "HWACCEL - surface is hardware accelerated, readonly<br>\n"
    /*DOC*/    "SRCCOLORKEY- surface has a colorkey for blits, readonly<br>\n"
    /*DOC*/    "SRCALPHA - surface has alpha enabled, readonly<br>\n"
    /*DOC*/    "RLEACCELOK - surface is rle accelerated, but hasn't been compiled\n"
    /*DOC*/    "yet, readonly<br>\n"
    /*DOC*/    "PREALLOC - not even sure?<br>\n"
    /*DOC*/ ;

#endif
