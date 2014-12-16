#include <SDL/SDL_scancode.h>

/// An enum for defining all the virtual keys
enum VirtualKey : ui16 {
    VKEY_UNKNOWN = SDL_SCANCODE_UNKNOWN,
    VKEY_A = SDL_SCANCODE_A,
    VKEY_B = SDL_SCANCODE_B,
    VKEY_C = SDL_SCANCODE_C,
    VKEY_D = SDL_SCANCODE_D,
    VKEY_E = SDL_SCANCODE_E,
    VKEY_F = SDL_SCANCODE_F,
    VKEY_G = SDL_SCANCODE_G,
    VKEY_H = SDL_SCANCODE_H,
    VKEY_I = SDL_SCANCODE_I,
    VKEY_J = SDL_SCANCODE_J,
    VKEY_K = SDL_SCANCODE_K,
    VKEY_L = SDL_SCANCODE_L,
    VKEY_M = SDL_SCANCODE_M,
    VKEY_N = SDL_SCANCODE_N,
    VKEY_O = SDL_SCANCODE_O,
    VKEY_P = SDL_SCANCODE_P,
    VKEY_Q = SDL_SCANCODE_Q,
    VKEY_R = SDL_SCANCODE_R,
    VKEY_S = SDL_SCANCODE_S,
    VKEY_T = SDL_SCANCODE_T,
    VKEY_U = SDL_SCANCODE_U,
    VKEY_V = SDL_SCANCODE_V,
    VKEY_W = SDL_SCANCODE_W,
    VKEY_X = SDL_SCANCODE_X,
    VKEY_Y = SDL_SCANCODE_Y,
    VKEY_Z = SDL_SCANCODE_Z,
    VKEY_1 = SDL_SCANCODE_1,
    VKEY_2 = SDL_SCANCODE_2,
    VKEY_3 = SDL_SCANCODE_3,
    VKEY_4 = SDL_SCANCODE_4,
    VKEY_5 = SDL_SCANCODE_5,
    VKEY_6 = SDL_SCANCODE_6,
    VKEY_7 = SDL_SCANCODE_7,
    VKEY_8 = SDL_SCANCODE_8,
    VKEY_9 = SDL_SCANCODE_9,
    VKEY_0 = SDL_SCANCODE_0,
    VKEY_RETURN = SDL_SCANCODE_RETURN,
    VKEY_ESCAPE = SDL_SCANCODE_ESCAPE,
    VKEY_BACKSPACE = SDL_SCANCODE_BACKSPACE,
    VKEY_TAB = SDL_SCANCODE_TAB,
    VKEY_SPACE = SDL_SCANCODE_SPACE,
    VKEY_MINUS = SDL_SCANCODE_MINUS,
    VKEY_EQUALS = SDL_SCANCODE_EQUALS,
    VKEY_LEFTBRACKET = SDL_SCANCODE_LEFTBRACKET,
    VKEY_RIGHTBRACKET = SDL_SCANCODE_RIGHTBRACKET,
    VKEY_BACKSLASH = SDL_SCANCODE_BACKSLASH,
    VKEY_NONUSHASH = SDL_SCANCODE_NONUSHASH,
    VKEY_SEMICOLON = SDL_SCANCODE_SEMICOLON,
    VKEY_APOSTROPHE = SDL_SCANCODE_APOSTROPHE,
    VKEY_GRAVE = SDL_SCANCODE_GRAVE,
    VKEY_COMMA = SDL_SCANCODE_COMMA,
    VKEY_PERIOD = SDL_SCANCODE_PERIOD,
    VKEY_SLASH = SDL_SCANCODE_SLASH,
    VKEY_CAPSLOCK = SDL_SCANCODE_CAPSLOCK,
    VKEY_F1 = SDL_SCANCODE_F1,
    VKEY_F2 = SDL_SCANCODE_F2,
    VKEY_F3 = SDL_SCANCODE_F3,
    VKEY_F4 = SDL_SCANCODE_F4,
    VKEY_F5 = SDL_SCANCODE_F5,
    VKEY_F6 = SDL_SCANCODE_F6,
    VKEY_F7 = SDL_SCANCODE_F7,
    VKEY_F8 = SDL_SCANCODE_F8,
    VKEY_F9 = SDL_SCANCODE_F9,
    VKEY_F10 = SDL_SCANCODE_F10,
    VKEY_F11 = SDL_SCANCODE_F11,
    VKEY_F12 = SDL_SCANCODE_F12,
    VKEY_PRINTSCREEN = SDL_SCANCODE_PRINTSCREEN,
    VKEY_SCROLLLOCK = SDL_SCANCODE_SCROLLLOCK,
    VKEY_PAUSE = SDL_SCANCODE_PAUSE,
    VKEY_INSERT = SDL_SCANCODE_INSERT,
    VKEY_HOME = SDL_SCANCODE_HOME,
    VKEY_PAGEUP = SDL_SCANCODE_PAGEUP,
    VKEY_DELETE = SDL_SCANCODE_DELETE,
    VKEY_END = SDL_SCANCODE_END,
    VKEY_PAGEDOWN = SDL_SCANCODE_PAGEDOWN,
    VKEY_RIGHT = SDL_SCANCODE_RIGHT,
    VKEY_LEFT = SDL_SCANCODE_LEFT,
    VKEY_DOWN = SDL_SCANCODE_DOWN,
    VKEY_UP = SDL_SCANCODE_UP,
    VKEY_NUMLOCKCLEAR = SDL_SCANCODE_NUMLOCKCLEAR,
    VKEY_KP_DIVIDE = SDL_SCANCODE_KP_DIVIDE,
    VKEY_KP_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY,
    VKEY_KP_MINUS = SDL_SCANCODE_KP_MINUS,
    VKEY_KP_PLUS = SDL_SCANCODE_KP_PLUS,
    VKEY_KP_ENTER = SDL_SCANCODE_KP_ENTER,
    VKEY_KP_1 = SDL_SCANCODE_KP_1,
    VKEY_KP_2 = SDL_SCANCODE_KP_2,
    VKEY_KP_3 = SDL_SCANCODE_KP_3,
    VKEY_KP_4 = SDL_SCANCODE_KP_4,
    VKEY_KP_5 = SDL_SCANCODE_KP_5,
    VKEY_KP_6 = SDL_SCANCODE_KP_6,
    VKEY_KP_7 = SDL_SCANCODE_KP_7,
    VKEY_KP_8 = SDL_SCANCODE_KP_8,
    VKEY_KP_9 = SDL_SCANCODE_KP_9,
    VKEY_KP_0 = SDL_SCANCODE_KP_0,
    VKEY_KP_PERIOD = SDL_SCANCODE_KP_PERIOD,
    VKEY_NONUSBACKSLASH = SDL_SCANCODE_NONUSBACKSLASH,
    VKEY_APPLICATION = SDL_SCANCODE_APPLICATION,
    VKEY_POWER = SDL_SCANCODE_POWER,
    VKEY_KP_EQUALS = SDL_SCANCODE_KP_EQUALS,
    VKEY_F13 = SDL_SCANCODE_F13,
    VKEY_F14 = SDL_SCANCODE_F14,
    VKEY_F15 = SDL_SCANCODE_F15,
    VKEY_F16 = SDL_SCANCODE_F16,
    VKEY_F17 = SDL_SCANCODE_F17,
    VKEY_F18 = SDL_SCANCODE_F18,
    VKEY_F19 = SDL_SCANCODE_F19,
    VKEY_F20 = SDL_SCANCODE_F20,
    VKEY_F21 = SDL_SCANCODE_F21,
    VKEY_F22 = SDL_SCANCODE_F22,
    VKEY_F23 = SDL_SCANCODE_F23,
    VKEY_F24 = SDL_SCANCODE_F24,
    VKEY_EXECUTE = SDL_SCANCODE_EXECUTE,
    VKEY_HELP = SDL_SCANCODE_HELP,
    VKEY_MENU = SDL_SCANCODE_MENU,
    VKEY_SELECT = SDL_SCANCODE_SELECT,
    VKEY_STOP = SDL_SCANCODE_STOP,
    VKEY_AGAIN = SDL_SCANCODE_AGAIN,
    VKEY_UNDO = SDL_SCANCODE_UNDO,
    VKEY_CUT = SDL_SCANCODE_CUT,
    VKEY_COPY = SDL_SCANCODE_COPY,
    VKEY_PASTE = SDL_SCANCODE_PASTE,
    VKEY_FIND = SDL_SCANCODE_FIND,
    VKEY_MUTE = SDL_SCANCODE_MUTE,
    VKEY_VOLUMEUP = SDL_SCANCODE_VOLUMEUP,
    VKEY_VOLUMEDOWN = SDL_SCANCODE_VOLUMEDOWN,
    VKEY_KP_COMMA = SDL_SCANCODE_KP_COMMA,
    VKEY_KP_EQUALSAS400 = SDL_SCANCODE_KP_EQUALSAS400,
    VKEY_INTERNATIONAL1 = SDL_SCANCODE_INTERNATIONAL1,
    VKEY_INTERNATIONAL2 = SDL_SCANCODE_INTERNATIONAL2,
    VKEY_INTERNATIONAL3 = SDL_SCANCODE_INTERNATIONAL3,
    VKEY_INTERNATIONAL4 = SDL_SCANCODE_INTERNATIONAL4,
    VKEY_INTERNATIONAL5 = SDL_SCANCODE_INTERNATIONAL5,
    VKEY_INTERNATIONAL6 = SDL_SCANCODE_INTERNATIONAL6,
    VKEY_INTERNATIONAL7 = SDL_SCANCODE_INTERNATIONAL7,
    VKEY_INTERNATIONAL8 = SDL_SCANCODE_INTERNATIONAL8,
    VKEY_INTERNATIONAL9 = SDL_SCANCODE_INTERNATIONAL9,
    VKEY_LANG1 = SDL_SCANCODE_LANG1,
    VKEY_LANG2 = SDL_SCANCODE_LANG2,
    VKEY_LANG3 = SDL_SCANCODE_LANG3,
    VKEY_LANG4 = SDL_SCANCODE_LANG4,
    VKEY_LANG5 = SDL_SCANCODE_LANG5,
    VKEY_LANG6 = SDL_SCANCODE_LANG6,
    VKEY_LANG7 = SDL_SCANCODE_LANG7,
    VKEY_LANG8 = SDL_SCANCODE_LANG8,
    VKEY_LANG9 = SDL_SCANCODE_LANG9,
    VKEY_ALTERASE = SDL_SCANCODE_ALTERASE,
    VKEY_SYSREQ = SDL_SCANCODE_SYSREQ,
    VKEY_CANCEL = SDL_SCANCODE_CANCEL,
    VKEY_CLEAR = SDL_SCANCODE_CLEAR,
    VKEY_PRIOR = SDL_SCANCODE_PRIOR,
    VKEY_RETURN2 = SDL_SCANCODE_RETURN2,
    VKEY_SEPARATOR = SDL_SCANCODE_SEPARATOR,
    VKEY_OUT = SDL_SCANCODE_OUT,
    VKEY_OPER = SDL_SCANCODE_OPER,
    VKEY_CLEARAGAIN = SDL_SCANCODE_CLEARAGAIN,
    VKEY_CRSEL = SDL_SCANCODE_CRSEL,
    VKEY_EXSEL = SDL_SCANCODE_EXSEL,
    VKEY_KP_00 = SDL_SCANCODE_KP_00,
    VKEY_KP_000 = SDL_SCANCODE_KP_000,
    VKEY_THOUSANDSSEPARATOR = SDL_SCANCODE_THOUSANDSSEPARATOR,
    VKEY_DECIMALSEPARATOR = SDL_SCANCODE_DECIMALSEPARATOR,
    VKEY_CURRENCYUNIT = SDL_SCANCODE_CURRENCYUNIT,
    VKEY_CURRENCYSUBUNIT = SDL_SCANCODE_CURRENCYSUBUNIT,
    VKEY_KP_LEFTPAREN = SDL_SCANCODE_KP_LEFTPAREN,
    VKEY_KP_RIGHTPAREN = SDL_SCANCODE_KP_RIGHTPAREN,
    VKEY_KP_LEFTBRACE = SDL_SCANCODE_KP_LEFTBRACE,
    VKEY_KP_RIGHTBRACE = SDL_SCANCODE_KP_RIGHTBRACE,
    VKEY_KP_TAB = SDL_SCANCODE_KP_TAB,
    VKEY_KP_BACKSPACE = SDL_SCANCODE_KP_BACKSPACE,
    VKEY_KP_A = SDL_SCANCODE_KP_A,
    VKEY_KP_B = SDL_SCANCODE_KP_B,
    VKEY_KP_C = SDL_SCANCODE_KP_C,
    VKEY_KP_D = SDL_SCANCODE_KP_D,
    VKEY_KP_E = SDL_SCANCODE_KP_E,
    VKEY_KP_F = SDL_SCANCODE_KP_F,
    VKEY_KP_XOR = SDL_SCANCODE_KP_XOR,
    VKEY_KP_POWER = SDL_SCANCODE_KP_POWER,
    VKEY_KP_PERCENT = SDL_SCANCODE_KP_PERCENT,
    VKEY_KP_LESS = SDL_SCANCODE_KP_LESS,
    VKEY_KP_GREATER = SDL_SCANCODE_KP_GREATER,
    VKEY_KP_AMPERSAND = SDL_SCANCODE_KP_AMPERSAND,
    VKEY_KP_DBLAMPERSAND = SDL_SCANCODE_KP_DBLAMPERSAND,
    VKEY_KP_VERTICALBAR = SDL_SCANCODE_KP_VERTICALBAR,
    VKEY_KP_DBLVERTICALBAR = SDL_SCANCODE_KP_DBLVERTICALBAR,
    VKEY_KP_COLON = SDL_SCANCODE_KP_COLON,
    VKEY_KP_HASH = SDL_SCANCODE_KP_HASH,
    VKEY_KP_SPACE = SDL_SCANCODE_KP_SPACE,
    VKEY_KP_AT = SDL_SCANCODE_KP_AT,
    VKEY_KP_EXCLAM = SDL_SCANCODE_KP_EXCLAM,
    VKEY_KP_MEMSTORE = SDL_SCANCODE_KP_MEMSTORE,
    VKEY_KP_MEMRECALL = SDL_SCANCODE_KP_MEMRECALL,
    VKEY_KP_MEMCLEAR = SDL_SCANCODE_KP_MEMCLEAR,
    VKEY_KP_MEMADD = SDL_SCANCODE_KP_MEMADD,
    VKEY_KP_MEMSUBTRACT = SDL_SCANCODE_KP_MEMSUBTRACT,
    VKEY_KP_MEMMULTIPLY = SDL_SCANCODE_KP_MEMMULTIPLY,
    VKEY_KP_MEMDIVIDE = SDL_SCANCODE_KP_MEMDIVIDE,
    VKEY_KP_PLUSMINUS = SDL_SCANCODE_KP_PLUSMINUS,
    VKEY_KP_CLEAR = SDL_SCANCODE_KP_CLEAR,
    VKEY_KP_CLEARENTRY = SDL_SCANCODE_KP_CLEARENTRY,
    VKEY_KP_BINARY = SDL_SCANCODE_KP_BINARY,
    VKEY_KP_OCTAL = SDL_SCANCODE_KP_OCTAL,
    VKEY_KP_DECIMAL = SDL_SCANCODE_KP_DECIMAL,
    VKEY_KP_HEXADECIMAL = SDL_SCANCODE_KP_HEXADECIMAL,
    VKEY_LCTRL = SDL_SCANCODE_LCTRL,
    VKEY_LSHIFT = SDL_SCANCODE_LSHIFT,
    VKEY_LALT = SDL_SCANCODE_LALT,
    VKEY_LGUI = SDL_SCANCODE_LGUI,
    VKEY_RCTRL = SDL_SCANCODE_RCTRL,
    VKEY_RSHIFT = SDL_SCANCODE_RSHIFT,
    VKEY_RALT = SDL_SCANCODE_RALT,
    VKEY_RGUI = SDL_SCANCODE_RGUI,
    VKEY_MODE = SDL_SCANCODE_MODE,
    VKEY_AUDIONEXT = SDL_SCANCODE_AUDIONEXT,
    VKEY_AUDIOPREV = SDL_SCANCODE_AUDIOPREV,
    VKEY_AUDIOSTOP = SDL_SCANCODE_AUDIOSTOP,
    VKEY_AUDIOPLAY = SDL_SCANCODE_AUDIOPLAY,
    VKEY_AUDIOMUTE = SDL_SCANCODE_AUDIOMUTE,
    VKEY_MEDIASELECT = SDL_SCANCODE_MEDIASELECT,
    VKEY_WWW = SDL_SCANCODE_WWW,
    VKEY_MAIL = SDL_SCANCODE_MAIL,
    VKEY_CALCULATOR = SDL_SCANCODE_CALCULATOR,
    VKEY_COMPUTER = SDL_SCANCODE_COMPUTER,
    VKEY_AC_SEARCH = SDL_SCANCODE_AC_SEARCH,
    VKEY_AC_HOME = SDL_SCANCODE_AC_HOME,
    VKEY_AC_BACK = SDL_SCANCODE_AC_BACK,
    VKEY_AC_FORWARD = SDL_SCANCODE_AC_FORWARD,
    VKEY_AC_STOP = SDL_SCANCODE_AC_STOP,
    VKEY_AC_REFRESH = SDL_SCANCODE_AC_REFRESH,
    VKEY_AC_BOOKMARKS = SDL_SCANCODE_AC_BOOKMARKS,
    VKEY_BRIGHTNESSDOWN = SDL_SCANCODE_BRIGHTNESSDOWN,
    VKEY_BRIGHTNESSUP = SDL_SCANCODE_BRIGHTNESSUP,
    VKEY_DISPLAYSWITCH = SDL_SCANCODE_DISPLAYSWITCH,
    VKEY_KBDILLUMTOGGLE = SDL_SCANCODE_KBDILLUMTOGGLE,
    VKEY_KBDILLUMDOWN = SDL_SCANCODE_KBDILLUMDOWN,
    VKEY_KBDILLUMUP = SDL_SCANCODE_KBDILLUMUP,
    VKEY_EJECT = SDL_SCANCODE_EJECT,
    VKEY_SLEEP = SDL_SCANCODE_SLEEP,
    VKEY_HIGHEST_VALUE
};