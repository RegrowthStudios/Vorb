
namespace vorb {
    namespace ui {
        namespace impl {
#if defined(VORB_IMPL_UI_SFML)
            VirtualKey mapping[] = {
                VirtualKey::VKEY_UNKNOWN,
                VirtualKey::VKEY_A,
                VirtualKey::VKEY_B,
                VirtualKey::VKEY_C,
                VirtualKey::VKEY_D,
                VirtualKey::VKEY_E,
                VirtualKey::VKEY_F,
                VirtualKey::VKEY_G,
                VirtualKey::VKEY_H,
                VirtualKey::VKEY_I,
                VirtualKey::VKEY_J,
                VirtualKey::VKEY_K,
                VirtualKey::VKEY_L,
                VirtualKey::VKEY_M,
                VirtualKey::VKEY_N,
                VirtualKey::VKEY_O,
                VirtualKey::VKEY_P,
                VirtualKey::VKEY_Q,
                VirtualKey::VKEY_R,
                VirtualKey::VKEY_S,
                VirtualKey::VKEY_T,
                VirtualKey::VKEY_U,
                VirtualKey::VKEY_V,
                VirtualKey::VKEY_W,
                VirtualKey::VKEY_X,
                VirtualKey::VKEY_Y,
                VirtualKey::VKEY_Z,
                VirtualKey::VKEY_0,
                VirtualKey::VKEY_1,
                VirtualKey::VKEY_2,
                VirtualKey::VKEY_3,
                VirtualKey::VKEY_4,
                VirtualKey::VKEY_5,
                VirtualKey::VKEY_6,
                VirtualKey::VKEY_7,
                VirtualKey::VKEY_8,
                VirtualKey::VKEY_9,
                VirtualKey::VKEY_ESCAPE,
                VirtualKey::VKEY_LCTRL,
                VirtualKey::VKEY_LSHIFT,
                VirtualKey::VKEY_LALT,
                VirtualKey::VKEY_LGUI,
                VirtualKey::VKEY_RCTRL,
                VirtualKey::VKEY_RSHIFT,
                VirtualKey::VKEY_RALT,
                VirtualKey::VKEY_RGUI,
                VirtualKey::VKEY_MENU,
                VirtualKey::VKEY_LEFTBRACKET,
                VirtualKey::VKEY_RIGHTBRACKET,
                VirtualKey::VKEY_SEMICOLON,
                VirtualKey::VKEY_COMMA,
                VirtualKey::VKEY_PERIOD,
                VirtualKey::VKEY_APOSTROPHE,
                VirtualKey::VKEY_SLASH,
                VirtualKey::VKEY_BACKSLASH,
                VirtualKey::VKEY_GRAVE,
                VirtualKey::VKEY_EQUALS,
                VirtualKey::VKEY_MINUS,
                VirtualKey::VKEY_SPACE,
                VirtualKey::VKEY_RETURN,
                VirtualKey::VKEY_BACKSPACE,
                VirtualKey::VKEY_TAB,
                VirtualKey::VKEY_PAGEUP,
                VirtualKey::VKEY_PAGEDOWN,
                VirtualKey::VKEY_END,
                VirtualKey::VKEY_HOME,
                VirtualKey::VKEY_INSERT,
                VirtualKey::VKEY_DELETE,
                VirtualKey::VKEY_KP_PLUS,
                VirtualKey::VKEY_KP_MINUS,
                VirtualKey::VKEY_KP_MULTIPLY,
                VirtualKey::VKEY_KP_DIVIDE,
                VirtualKey::VKEY_LEFT,
                VirtualKey::VKEY_RIGHT,
                VirtualKey::VKEY_UP,
                VirtualKey::VKEY_DOWN,
                VirtualKey::VKEY_KP_0,
                VirtualKey::VKEY_KP_1,
                VirtualKey::VKEY_KP_2,
                VirtualKey::VKEY_KP_3,
                VirtualKey::VKEY_KP_4,
                VirtualKey::VKEY_KP_5,
                VirtualKey::VKEY_KP_6,
                VirtualKey::VKEY_KP_7,
                VirtualKey::VKEY_KP_8,
                VirtualKey::VKEY_KP_9,
                VirtualKey::VKEY_F1,
                VirtualKey::VKEY_F2,
                VirtualKey::VKEY_F3,
                VirtualKey::VKEY_F4,
                VirtualKey::VKEY_F5,
                VirtualKey::VKEY_F6,
                VirtualKey::VKEY_F7,
                VirtualKey::VKEY_F8,
                VirtualKey::VKEY_F9,
                VirtualKey::VKEY_F10,
                VirtualKey::VKEY_F11,
                VirtualKey::VKEY_F12,
                VirtualKey::VKEY_F13,
                VirtualKey::VKEY_F14,
                VirtualKey::VKEY_F15,
                VirtualKey::VKEY_PAUSE
            };
#elif defined(VORB_IMPL_UI_GLFW)
            VirtualKey mapping[350];
            extern bool glfwMapping;
            bool glfwMapKeys() {
                for(size_t i = 0;i < 350;i++) mapping[i] = VirtualKey::VKEY_UNKNOWN;
#define MAP_KEY(KEYGLFW, KEYMINE) mapping[GLFW_KEY_##KEYGLFW + 1] = VirtualKey::VKEY_##KEYMINE
                MAP_KEY(SPACE, SPACE);
                MAP_KEY(APOSTROPHE, APOSTROPHE);
                MAP_KEY(COMMA, COMMA);
                MAP_KEY(MINUS, MINUS);
                MAP_KEY(PERIOD, PERIOD);
                MAP_KEY(SLASH, SLASH);
                MAP_KEY(0, 0);
                MAP_KEY(1, 1);
                MAP_KEY(2, 2);
                MAP_KEY(3, 3);
                MAP_KEY(4, 4);
                MAP_KEY(5, 5);
                MAP_KEY(6, 6);
                MAP_KEY(7, 7);
                MAP_KEY(8, 8);
                MAP_KEY(9, 9);
                MAP_KEY(SEMICOLON, SEMICOLON);
                MAP_KEY(EQUAL, EQUALS);
                MAP_KEY(A, A);
                MAP_KEY(B, B);
                MAP_KEY(C, C);
                MAP_KEY(D, D);
                MAP_KEY(E, E);
                MAP_KEY(F, F);
                MAP_KEY(G, G);
                MAP_KEY(H, H);
                MAP_KEY(I, I);
                MAP_KEY(J, J);
                MAP_KEY(K, K);
                MAP_KEY(L, L);
                MAP_KEY(M, M);
                MAP_KEY(N, N);
                MAP_KEY(O, O);
                MAP_KEY(P, P);
                MAP_KEY(Q, Q);
                MAP_KEY(R, R);
                MAP_KEY(S, S);
                MAP_KEY(T, T);
                MAP_KEY(U, U);
                MAP_KEY(V, V);
                MAP_KEY(W, W);
                MAP_KEY(X, X);
                MAP_KEY(Y, Y);
                MAP_KEY(Z, Z);
                MAP_KEY(LEFT_BRACKET, LEFTBRACKET);
                MAP_KEY(BACKSLASH, BACKSLASH);
                MAP_KEY(RIGHT_BRACKET, RIGHTBRACKET);
                MAP_KEY(GRAVE_ACCENT, GRAVE);
                MAP_KEY(WORLD_1, NONUSBACKSLASH);
                MAP_KEY(WORLD_2, NONUSHASH);
                MAP_KEY(ESCAPE, ESCAPE);
                MAP_KEY(ENTER, RETURN);
                MAP_KEY(TAB, TAB);
                MAP_KEY(BACKSPACE, BACKSPACE);
                MAP_KEY(INSERT, INSERT);
                mapping[GLFW_KEY_DELETE + 1] = VirtualKey::VKEY_DELETE;
                MAP_KEY(RIGHT, RIGHT);
                MAP_KEY(LEFT, LEFT);
                MAP_KEY(DOWN, DOWN);
                MAP_KEY(UP, UP);
                MAP_KEY(PAGE_UP, PAGEUP);
                MAP_KEY(PAGE_DOWN, PAGEDOWN);
                MAP_KEY(HOME, HOME);
                MAP_KEY(END, END);
                MAP_KEY(CAPS_LOCK, CAPSLOCK);
                MAP_KEY(SCROLL_LOCK, SCROLLLOCK);
                MAP_KEY(NUM_LOCK, NUMLOCKCLEAR);
                MAP_KEY(PRINT_SCREEN, PRINTSCREEN);
                MAP_KEY(PAUSE, PAUSE);
                MAP_KEY(F1, F1);
                MAP_KEY(F2, F2);
                MAP_KEY(F3, F3);
                MAP_KEY(F4, F4);
                MAP_KEY(F5, F5);
                MAP_KEY(F6, F6);
                MAP_KEY(F7, F7);
                MAP_KEY(F8, F8);
                MAP_KEY(F9, F9);
                MAP_KEY(F10, F10);
                MAP_KEY(F11, F11);
                MAP_KEY(F12, F12);
                MAP_KEY(F13, F13);
                MAP_KEY(F14, F14);
                MAP_KEY(F15, F15);
                MAP_KEY(F16, F16);
                MAP_KEY(F17, F17);
                MAP_KEY(F18, F18);
                MAP_KEY(F19, F19);
                MAP_KEY(F20, F20);
                MAP_KEY(F21, F21);
                MAP_KEY(F22, F22);
                MAP_KEY(F23, F23);
                MAP_KEY(F24, F24);
                MAP_KEY(F25, UNKNOWN);
                MAP_KEY(KP_0, KP_0);
                MAP_KEY(KP_1, KP_1);
                MAP_KEY(KP_2, KP_2);
                MAP_KEY(KP_3, KP_3);
                MAP_KEY(KP_4, KP_4);
                MAP_KEY(KP_5, KP_5);
                MAP_KEY(KP_6, KP_6);
                MAP_KEY(KP_7, KP_7);
                MAP_KEY(KP_8, KP_8);
                MAP_KEY(KP_9, KP_9);
                MAP_KEY(KP_DECIMAL, KP_DECIMAL);
                MAP_KEY(KP_DIVIDE, KP_DIVIDE);
                MAP_KEY(KP_MULTIPLY, KP_MULTIPLY);
                MAP_KEY(KP_SUBTRACT, KP_MINUS);
                MAP_KEY(KP_ADD, KP_PLUS);
                MAP_KEY(KP_ENTER, KP_ENTER);
                MAP_KEY(KP_EQUAL, KP_EQUALS);
                MAP_KEY(LEFT_SHIFT, LSHIFT);
                MAP_KEY(LEFT_CONTROL, LCTRL);
                MAP_KEY(LEFT_ALT, LALT);
                MAP_KEY(LEFT_SUPER, LGUI);
                MAP_KEY(RIGHT_SHIFT, RSHIFT);
                MAP_KEY(RIGHT_CONTROL, RCTRL);
                MAP_KEY(RIGHT_ALT, RALT);
                MAP_KEY(RIGHT_SUPER, RGUI);
                MAP_KEY(MENU, MENU);
#undef MAP_KEY
                return true;
            }
            bool glfwMapping = glfwMapKeys();
#elif defined(VORB_IMPL_UI_SDL)
            VirtualKey mapping[300];
            extern bool sdlMapping;
            bool sdlMapKeys() {
                for (size_t i = 0; i < 300; i++) mapping[i] = VirtualKey::VKEY_UNKNOWN;
#define MAP_KEY(KEYSDL, KEYMINE) mapping[SDL_SCANCODE_##KEYSDL + 1] = VirtualKey::VKEY_##KEYMINE
                MAP_KEY(SPACE, SPACE);
                MAP_KEY(UNKNOWN, UNKNOWN);
                MAP_KEY(A, A);
                MAP_KEY(B, B);
                MAP_KEY(C, C);
                MAP_KEY(D, D);
                MAP_KEY(E, E);
                MAP_KEY(F, F);
                MAP_KEY(G, G);
                MAP_KEY(H, H);
                MAP_KEY(I, I);
                MAP_KEY(J, J);
                MAP_KEY(K, K);
                MAP_KEY(L, L);
                MAP_KEY(M, M);
                MAP_KEY(N, N);
                MAP_KEY(O, O);
                MAP_KEY(P, P);
                MAP_KEY(Q, Q);
                MAP_KEY(R, R);
                MAP_KEY(S, S);
                MAP_KEY(T, T);
                MAP_KEY(U, U);
                MAP_KEY(V, V);
                MAP_KEY(W, W);
                MAP_KEY(X, X);
                MAP_KEY(Y, Y);
                MAP_KEY(Z, Z);
                MAP_KEY(1, 1);
                MAP_KEY(2, 2);
                MAP_KEY(3, 3);
                MAP_KEY(4, 4);
                MAP_KEY(5, 5);
                MAP_KEY(6, 6);
                MAP_KEY(7, 7);
                MAP_KEY(8, 8);
                MAP_KEY(9, 9);
                MAP_KEY(0, 0);
                MAP_KEY(RETURN, RETURN);
                MAP_KEY(ESCAPE, ESCAPE);
                MAP_KEY(BACKSPACE, BACKSPACE);
                MAP_KEY(TAB, TAB);
                MAP_KEY(SPACE, SPACE);
                MAP_KEY(MINUS, MINUS);
                MAP_KEY(EQUALS, EQUALS);
                MAP_KEY(LEFTBRACKET, LEFTBRACKET);
                MAP_KEY(RIGHTBRACKET, RIGHTBRACKET);
                MAP_KEY(BACKSLASH, BACKSLASH);
                MAP_KEY(NONUSHASH, NONUSHASH);
                MAP_KEY(SEMICOLON, SEMICOLON);
                MAP_KEY(APOSTROPHE, APOSTROPHE);
                MAP_KEY(GRAVE, GRAVE);
                MAP_KEY(COMMA, COMMA);
                MAP_KEY(PERIOD, PERIOD);
                MAP_KEY(SLASH, SLASH);
                MAP_KEY(CAPSLOCK, CAPSLOCK);
                MAP_KEY(F1, F1);
                MAP_KEY(F2, F2);
                MAP_KEY(F3, F3);
                MAP_KEY(F4, F4);
                MAP_KEY(F5, F5);
                MAP_KEY(F6, F6);
                MAP_KEY(F7, F7);
                MAP_KEY(F8, F8);
                MAP_KEY(F9, F9);
                MAP_KEY(F10, F10);
                MAP_KEY(F11, F11);
                MAP_KEY(F12, F12);
                MAP_KEY(PRINTSCREEN, PRINTSCREEN);
                MAP_KEY(SCROLLLOCK, SCROLLLOCK);
                MAP_KEY(PAUSE, PAUSE);
                MAP_KEY(INSERT, INSERT);
                MAP_KEY(HOME, HOME);
                MAP_KEY(PAGEUP, PAGEUP);
                mapping[SDL_SCANCODE_DELETE + 1] = VirtualKey::VKEY_DELETE;
                MAP_KEY(END, END);
                MAP_KEY(PAGEDOWN, PAGEDOWN);
                MAP_KEY(RIGHT, RIGHT);
                MAP_KEY(LEFT, LEFT);
                MAP_KEY(DOWN, DOWN);
                MAP_KEY(UP, UP);
                MAP_KEY(NUMLOCKCLEAR, NUMLOCKCLEAR);
                MAP_KEY(KP_DIVIDE, KP_DIVIDE);
                MAP_KEY(KP_MULTIPLY, KP_MULTIPLY);
                MAP_KEY(KP_MINUS, KP_MINUS);
                MAP_KEY(KP_PLUS, KP_PLUS);
                MAP_KEY(KP_ENTER, KP_ENTER);
                MAP_KEY(KP_1, KP_1);
                MAP_KEY(KP_2, KP_2);
                MAP_KEY(KP_3, KP_3);
                MAP_KEY(KP_4, KP_4);
                MAP_KEY(KP_5, KP_5);
                MAP_KEY(KP_6, KP_6);
                MAP_KEY(KP_7, KP_7);
                MAP_KEY(KP_8, KP_8);
                MAP_KEY(KP_9, KP_9);
                MAP_KEY(KP_0, KP_0);
                MAP_KEY(KP_PERIOD, KP_PERIOD);
                MAP_KEY(NONUSBACKSLASH, NONUSBACKSLASH);
                MAP_KEY(APPLICATION, APPLICATION);
                MAP_KEY(POWER, POWER);
                MAP_KEY(KP_EQUALS, KP_EQUALS);
                MAP_KEY(F13, F13);
                MAP_KEY(F14, F14);
                MAP_KEY(F15, F15);
                MAP_KEY(F16, F16);
                MAP_KEY(F17, F17);
                MAP_KEY(F18, F18);
                MAP_KEY(F19, F19);
                MAP_KEY(F20, F20);
                MAP_KEY(F21, F21);
                MAP_KEY(F22, F22);
                MAP_KEY(F23, F23);
                MAP_KEY(F24, F24);
                MAP_KEY(EXECUTE, EXECUTE);
                MAP_KEY(HELP, HELP);
                MAP_KEY(MENU, MENU);
                MAP_KEY(SELECT, SELECT);
                MAP_KEY(STOP, STOP);
                MAP_KEY(AGAIN, AGAIN);
                MAP_KEY(UNDO, UNDO);
                MAP_KEY(CUT, CUT);
                MAP_KEY(COPY, COPY);
                MAP_KEY(PASTE, PASTE);
                MAP_KEY(FIND, FIND);
                MAP_KEY(MUTE, MUTE);
                MAP_KEY(VOLUMEUP, VOLUMEUP);
                MAP_KEY(VOLUMEDOWN, VOLUMEDOWN);
                MAP_KEY(KP_COMMA, KP_COMMA);
                MAP_KEY(KP_EQUALSAS400, KP_EQUALSAS400);
                MAP_KEY(INTERNATIONAL1, INTERNATIONAL1);
                MAP_KEY(INTERNATIONAL2, INTERNATIONAL2);
                MAP_KEY(INTERNATIONAL3, INTERNATIONAL3);
                MAP_KEY(INTERNATIONAL4, INTERNATIONAL4);
                MAP_KEY(INTERNATIONAL5, INTERNATIONAL5);
                MAP_KEY(INTERNATIONAL6, INTERNATIONAL6);
                MAP_KEY(INTERNATIONAL7, INTERNATIONAL7);
                MAP_KEY(INTERNATIONAL8, INTERNATIONAL8);
                MAP_KEY(INTERNATIONAL9, INTERNATIONAL9);
                MAP_KEY(LANG1, LANG1);
                MAP_KEY(LANG2, LANG2);
                MAP_KEY(LANG3, LANG3);
                MAP_KEY(LANG4, LANG4);
                MAP_KEY(LANG5, LANG5);
                MAP_KEY(LANG6, LANG6);
                MAP_KEY(LANG7, LANG7);
                MAP_KEY(LANG8, LANG8);
                MAP_KEY(LANG9, LANG9);
                MAP_KEY(ALTERASE, ALTERASE);
                MAP_KEY(SYSREQ, SYSREQ);
                MAP_KEY(CANCEL, CANCEL);
                MAP_KEY(CLEAR, CLEAR);
                MAP_KEY(PRIOR, PRIOR);
                MAP_KEY(RETURN2, RETURN2);
                MAP_KEY(SEPARATOR, SEPARATOR);
                mapping[SDL_SCANCODE_OUT + 1] = VirtualKey::VKEY_OUT;
                MAP_KEY(OPER, OPER);
                MAP_KEY(CLEARAGAIN, CLEARAGAIN);
                MAP_KEY(CRSEL, CRSEL);
                MAP_KEY(EXSEL, EXSEL);
                MAP_KEY(KP_00, KP_00);
                MAP_KEY(KP_000, KP_000);
                MAP_KEY(THOUSANDSSEPARATOR, THOUSANDSSEPARATOR);
                MAP_KEY(DECIMALSEPARATOR, DECIMALSEPARATOR);
                MAP_KEY(CURRENCYUNIT, CURRENCYUNIT);
                MAP_KEY(CURRENCYSUBUNIT, CURRENCYSUBUNIT);
                MAP_KEY(KP_LEFTPAREN, KP_LEFTPAREN);
                MAP_KEY(KP_RIGHTPAREN, KP_RIGHTPAREN);
                MAP_KEY(KP_LEFTBRACE, KP_LEFTBRACE);
                MAP_KEY(KP_RIGHTBRACE, KP_RIGHTBRACE);
                MAP_KEY(KP_TAB, KP_TAB);
                MAP_KEY(KP_BACKSPACE, KP_BACKSPACE);
                MAP_KEY(KP_A, KP_A);
                MAP_KEY(KP_B, KP_B);
                MAP_KEY(KP_C, KP_C);
                MAP_KEY(KP_D, KP_D);
                MAP_KEY(KP_E, KP_E);
                MAP_KEY(KP_F, KP_F);
                MAP_KEY(KP_XOR, KP_XOR);
                MAP_KEY(KP_POWER, KP_POWER);
                MAP_KEY(KP_PERCENT, KP_PERCENT);
                MAP_KEY(KP_LESS, KP_LESS);
                MAP_KEY(KP_GREATER, KP_GREATER);
                MAP_KEY(KP_AMPERSAND, KP_AMPERSAND);
                MAP_KEY(KP_DBLAMPERSAND, KP_DBLAMPERSAND);
                MAP_KEY(KP_VERTICALBAR, KP_VERTICALBAR);
                MAP_KEY(KP_DBLVERTICALBAR, KP_DBLVERTICALBAR);
                MAP_KEY(KP_COLON, KP_COLON);
                MAP_KEY(KP_HASH, KP_HASH);
                MAP_KEY(KP_SPACE, KP_SPACE);
                MAP_KEY(KP_AT, KP_AT);
                MAP_KEY(KP_EXCLAM, KP_EXCLAM);
                MAP_KEY(KP_MEMSTORE, KP_MEMSTORE);
                MAP_KEY(KP_MEMRECALL, KP_MEMRECALL);
                MAP_KEY(KP_MEMCLEAR, KP_MEMCLEAR);
                MAP_KEY(KP_MEMADD, KP_MEMADD);
                MAP_KEY(KP_MEMSUBTRACT, KP_MEMSUBTRACT);
                MAP_KEY(KP_MEMMULTIPLY, KP_MEMMULTIPLY);
                MAP_KEY(KP_MEMDIVIDE, KP_MEMDIVIDE);
                MAP_KEY(KP_PLUSMINUS, KP_PLUSMINUS);
                MAP_KEY(KP_CLEAR, KP_CLEAR);
                MAP_KEY(KP_CLEARENTRY, KP_CLEARENTRY);
                MAP_KEY(KP_BINARY, KP_BINARY);
                MAP_KEY(KP_OCTAL, KP_OCTAL);
                MAP_KEY(KP_DECIMAL, KP_DECIMAL);
                MAP_KEY(KP_HEXADECIMAL, KP_HEXADECIMAL);
                MAP_KEY(LCTRL, LCTRL);
                MAP_KEY(LSHIFT, LSHIFT);
                MAP_KEY(LALT, LALT);
                MAP_KEY(LGUI, LGUI);
                MAP_KEY(RCTRL, RCTRL);
                MAP_KEY(RSHIFT, RSHIFT);
                MAP_KEY(RALT, RALT);
                MAP_KEY(RGUI, RGUI);
                MAP_KEY(MODE, MODE);
                MAP_KEY(AUDIONEXT, AUDIONEXT);
                MAP_KEY(AUDIOPREV, AUDIOPREV);
                MAP_KEY(AUDIOSTOP, AUDIOSTOP);
                MAP_KEY(AUDIOPLAY, AUDIOPLAY);
                MAP_KEY(AUDIOMUTE, AUDIOMUTE);
                MAP_KEY(MEDIASELECT, MEDIASELECT);
                MAP_KEY(WWW, WWW);
                MAP_KEY(MAIL, MAIL);
                MAP_KEY(CALCULATOR, CALCULATOR);
                MAP_KEY(COMPUTER, COMPUTER);
                MAP_KEY(AC_SEARCH, AC_SEARCH);
                MAP_KEY(AC_HOME, AC_HOME);
                MAP_KEY(AC_BACK, AC_BACK);
                MAP_KEY(AC_FORWARD, AC_FORWARD);
                MAP_KEY(AC_STOP, AC_STOP);
                MAP_KEY(AC_REFRESH, AC_REFRESH);
                MAP_KEY(AC_BOOKMARKS, AC_BOOKMARKS);
                MAP_KEY(BRIGHTNESSDOWN, BRIGHTNESSDOWN);
                MAP_KEY(BRIGHTNESSUP, BRIGHTNESSUP);
                MAP_KEY(DISPLAYSWITCH, DISPLAYSWITCH);
                MAP_KEY(KBDILLUMTOGGLE, KBDILLUMTOGGLE);
                MAP_KEY(KBDILLUMDOWN, KBDILLUMDOWN);
                MAP_KEY(KBDILLUMUP, KBDILLUMUP);
                MAP_KEY(EJECT, EJECT);
                MAP_KEY(SLEEP, SLEEP);
#undef MAP_KEY
                return true;
            }
            bool sdl = sdlMapKeys();
#endif
        }
    }
}

