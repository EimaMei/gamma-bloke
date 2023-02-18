#ifndef u8
#define u8 unsigned char
#endif

typedef void* ESGL_Handle;

#define EK_MOUSE_LBUTTON                       0x01
#define EK_MOUSE_RBUTTON                       0x02
#define EK_MOUSE_MBUTTON                       0x10
#define EK_MOUSE_XBUTTON1                      0x20
#define EK_MOUSE_XBUTTON2                      0x40

#define EK_Q                                   0x51
#define EK_W                                   0x57
#define EK_E                                   0x45
#define EK_R                                   0x52
#define EK_T                                   0x54
#define EK_Y                                   0x59
#define EK_U                                   0x55
#define EK_I                                   0x49
#define EK_O                                   0x4F
#define EK_P                                   0x50
#define EK_A                                   0X41
#define EK_S                                   0x53
#define EK_D                                   0x44
#define EK_F                                   0x46
#define EK_G                                   0x47
#define EK_H                                   0x48
#define EK_J                                   0x4A
#define EK_K                                   0x4B
#define EK_L                                   0x4C
#define EK_Z                                   0x5A
#define EK_X                                   0x58
#define EK_C                                   0x43
#define EK_V                                   0x56
#define EK_B                                   0x42
#define EK_N                                   0x4E
#define EK_M                                   0x4D

#define EK_ESCAPE                              0x1b
#define EK_SPACE                               0x20
#define EK_BACKSPACE                           0x08
#define EK_TAB                                 0x09
#define EK_CLEAR                               0x0C
#define EK_RETURN                              0x0D
#define EK_ENTER                               0x0D
#define EK_CTRL                                0x11
#define EK_CONTROL                             0x11
#define EK_MENU                                0x12
#define EK_PAUSE                               0x13
#define EK_CAPS                                0x14
#define EK_ALT                                 0xA4
#define EK_L_ALT                               0xA4
#define EK_R_ALT                               0xA5

#define EK_L_CTRL                              0xA2
#define EK_R_CTRL                              0xA3
#define EK_L_CONTROL                           0xA2
#define EK_R_CONTROL                           0xA3
#define EK_SHIFT                               0x10
#define EK_L_SHIFT                             0xA0
#define EK_R_SHIFT                             0xA1
#define EK_WINKEY                              0x5B
#define EK_R_WINKEY                            0x5C
#define EK_APPS                                0x5D
#define EK_SLEEP                               0x5F
#define EK_SCROLL                              0x91

#define EK_UP                                  0x26
#define EK_DOWN                                0x28
#define EK_LEFT                                0x25
#define EK_RIGHT                               0x27

#define EK_0                                   0x30
#define EK_1                                   0x31
#define EK_2                                   0x32
#define EK_3                                   0x33
#define EK_4                                   0x34
#define EK_5                                   0x35
#define EK_6                                   0x36
#define EK_7                                   0x37
#define EK_8                                   0x38
#define EK_9                                   0x39

#define EK_NUMPAD_0                            0x60
#define EK_NUMPAD_1                            0x61
#define EK_NUMPAD_2                            0x62
#define EK_NUMPAD_3                            0x63
#define EK_NUMPAD_4                            0x64
#define EK_NUMPAD_5                            0x65
#define EK_NUMPAD_6                            0x66
#define EK_NUMPAD_7                            0x67
#define EK_NUMPAD_8                            0x68
#define EK_NUMPAD_9                            0x69
#define EK_MULTIPLY                            0x6A
#define EK_ADD                                 0x6B
#define EK_SEPARATOR                           0x6C
#define EK_SUBTRACT                            0x6D
#define EK_DECIMAL                             0x6E
#define EK_DIVIDE                              0x6F
#define EK_NUMLOCK                             0x90

#define EK_INSERT                              0x2D
#define EK_DELETE                              0x2E
#define EK_PAGE_UP                             0x21
#define EK_PAGE_DOWN                           0x22
#define EK_HOME                                0x24
#define EK_END                                 0x23
#define EK_HELP                                0x2F

#define EK_SELECT                              0x29
#define EK_PRINT                               0x2A
#define EK_EXECUTE                             0x2B
#define EK_SNAPSHOT                            0x2C
#define EK_SCREENSHOT                          0X2C

#define EK_F1                                  0x70
#define EK_F2                                  0x71
#define EK_F3                                  0x72
#define EK_F4                                  0x73
#define EK_F5                                  0x74
#define EK_F6                                  0x75
#define EK_F7                                  0x76
#define EK_F8                                  0x77
#define EK_F9                                  0x78
#define EK_F10                                 0x79
#define EK_F11                                 0x7A
#define EK_F12                                 0x7B
#define EK_F13                                 0x7C
#define EK_F14                                 0x7D
#define EK_F15                                 0x7E
#define EK_F16                                 0x7F
#define EK_F17                                 0x80
#define EK_F18                                 0x81
#define EK_F19                                 0x82
#define EK_F20                                 0x83
#define EK_F21                                 0x84
#define EK_F22                                 0x85
#define EK_F23                                 0x86
#define EK_F24                                 0x87

#define EK_KANA                                0x15
#define EK_HANGUL                              0x15
#define EK_IME_ON                              0x16
#define EK_JUNJA                               0x17
#define EK_FINAL                               0x18
#define EK_HANJA                               0x19
#define EK_KANJI                               0x19
#define EK_IME_OFF                             0x1A
#define EK_CONVERT                             0x1C
#define EK_NON_CONVERT                         0x1D
#define EK_ACCEPT                              0x1E
#define EK_MODE_CHANGE                         0x1F

/* Used for miscellaneous characters; can vary by keyboard. */
#define EK_OEM_1                               0xBA	  /* For the US standard keyboard, the ';:' key */
#define EK_OEM_2                               0xBF	  /* For the US standard keyboard, the '/?' key */
#define EK_OEM_3                               0xC0	  /* For the US standard keyboard, the '`~' key */
#define EK_OEM_4                               0xDB	  /* For the US standard keyboard, the '[{' key */
#define EK_OEM_5                               0xDC   /* For the US standard keyboard, the '\|' key */
#define EK_OEM_6                               0xDD   /* For the US standard keyboard, the ']}' key */
#define EK_OEM_7                               0xDE   /* For the US standard keyboard, the `'"` key */
#define EK_OEM_8                               0xDF   /* Varies by keybaord */
#define EK_OEM_9                               0xE2   /* The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard */

#define EK_PLUS                                0xBB
#define EK_COMMA                               0xBC
#define EK_MINUS                               0xBD
#define EK_PERIOD                              0xBE