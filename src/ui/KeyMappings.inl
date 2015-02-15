
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
                // TODO: Finish the mapping
                return true;
            }
            bool glfwMapping = glfwMapKeys();
//SPACE              32
//APOSTROPHE         39  /* ' */
//COMMA              44  /* , */
//MINUS              45  /* - */
//PERIOD             46  /* . */
//SLASH              47  /* / */
//0                  48
//1                  49
//2                  50
//3                  51
//4                  52
//5                  53
//6                  54
//7                  55
//8                  56
//9                  57
//SEMICOLON          59  /* ; */
//EQUAL              61  /* = */
//A                  65
//B                  66
//C                  67
//D                  68
//E                  69
//F                  70
//G                  71
//H                  72
//I                  73
//J                  74
//K                  75
//L                  76
//M                  77
//N                  78
//O                  79
//P                  80
//Q                  81
//R                  82
//S                  83
//T                  84
//U                  85
//V                  86
//W                  87
//X                  88
//Y                  89
//Z                  90
//LEFT_BRACKET       91  /* [ */
//BACKSLASH          92  /* \ */
//RIGHT_BRACKET      93  /* ] */
//GRAVE_ACCENT       96  /* ` */
//WORLD_1            161 /* non-US #1 */
//WORLD_2            162 /* non-US #2 */
//ESCAPE             256
//ENTER              257
//TAB                258
//BACKSPACE          259
//INSERT             260
//DELETE             261
//RIGHT              262
//LEFT               263
//DOWN               264
//UP                 265
//PAGE_UP            266
//PAGE_DOWN          267
//HOME               268
//END                269
//CAPS_LOCK          280
//SCROLL_LOCK        281
//NUM_LOCK           282
//PRINT_SCREEN       283
//PAUSE              284
//F1                 290
//F2                 291
//F3                 292
//F4                 293
//F5                 294
//F6                 295
//F7                 296
//F8                 297
//F9                 298
//F10                299
//F11                300
//F12                301
//F13                302
//F14                303
//F15                304
//F16                305
//F17                306
//F18                307
//F19                308
//F20                309
//F21                310
//F22                311
//F23                312
//F24                313
//F25                314
//KP_0               320
//KP_1               321
//KP_2               322
//KP_3               323
//KP_4               324
//KP_5               325
//KP_6               326
//KP_7               327
//KP_8               328
//KP_9               329
//KP_DECIMAL         330
//KP_DIVIDE          331
//KP_MULTIPLY        332
//KP_SUBTRACT        333
//KP_ADD             334
//KP_ENTER           335
//KP_EQUAL           336
//LEFT_SHIFT         340
//LEFT_CONTROL       341
//LEFT_ALT           342
//LEFT_SUPER         343
//RIGHT_SHIFT        344
//RIGHT_CONTROL      345
//RIGHT_ALT          346
//RIGHT_SUPER        347
//MENU               348
#endif
        }
    }
}

