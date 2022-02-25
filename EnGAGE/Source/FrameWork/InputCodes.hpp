#pragma once


namespace FrameworkComponent
{
	class InputCodes {
	public:
		/* The unknown key */
		static constexpr uint16_t KEY_UNKNOWN = -1;
		/* Printable keys */
		static constexpr uint16_t KEY_SPACE = 32;
		static constexpr uint16_t KEY_APOSTROPHE = 39;  /* ' */
		static constexpr uint16_t KEY_COMMA = 44;  /* , */
		static constexpr uint16_t KEY_MINUS = 45;  /* - */
		static constexpr uint16_t KEY_PERIOD = 46;  /* . */
		static constexpr uint16_t KEY_SLASH = 47;  /* / */
		static constexpr uint16_t KEY_0 = 48;
		static constexpr uint16_t KEY_1 = 49;
		static constexpr uint16_t KEY_2 = 50;
		static constexpr uint16_t KEY_3 = 51;
		static constexpr uint16_t KEY_4 = 52;
		static constexpr uint16_t KEY_5 = 53;
		static constexpr uint16_t KEY_6 = 54;
		static constexpr uint16_t KEY_7 = 55;
		static constexpr uint16_t KEY_8 = 56;
		static constexpr uint16_t KEY_9 = 57;
		static constexpr uint16_t KEY_SEMICOLON = 59;  /* ; */
		static constexpr uint16_t KEY_EQUAL = 61;  /* = */
		static constexpr uint16_t KEY_A = 65;
		static constexpr uint16_t KEY_B = 66;
		static constexpr uint16_t KEY_C = 67;
		static constexpr uint16_t KEY_D = 68;
		static constexpr uint16_t KEY_E = 69;
		static constexpr uint16_t KEY_F = 70;
		static constexpr uint16_t KEY_G = 71;
		static constexpr uint16_t KEY_H = 72;
		static constexpr uint16_t KEY_I = 73;
		static constexpr uint16_t KEY_J = 74;
		static constexpr uint16_t KEY_K = 75;
		static constexpr uint16_t KEY_L = 76;
		static constexpr uint16_t KEY_M = 77;
		static constexpr uint16_t KEY_N = 78;
		static constexpr uint16_t KEY_O = 79;
		static constexpr uint16_t KEY_P = 80;
		static constexpr uint16_t KEY_Q = 81;
		static constexpr uint16_t KEY_R = 82;
		static constexpr uint16_t KEY_S = 83;
		static constexpr uint16_t KEY_T = 84;
		static constexpr uint16_t KEY_U = 85;
		static constexpr uint16_t KEY_V = 86;
		static constexpr uint16_t KEY_W = 87;
		static constexpr uint16_t KEY_X = 88;
		static constexpr uint16_t KEY_Y = 89;
		static constexpr uint16_t KEY_Z = 90;
		static constexpr uint16_t KEY_LEFT_BRACKET = 91;  /* [ */
		static constexpr uint16_t KEY_BACKSLASH = 92;  /* \ */
		static constexpr uint16_t KEY_RIGHT_BRACKET = 93;  /* ] */
		static constexpr uint16_t KEY_GRAVE_ACCENT = 96;  /* ` */
		static constexpr uint16_t KEY_WORLD_1 = 161; /* non-US #1 */
		static constexpr uint16_t KEY_WORLD_2 = 162; /* non-US #2 */
		/* Function keys */
		static constexpr uint16_t KEY_ESCAPE = 256;
		static constexpr uint16_t KEY_ENTER = 257;
		static constexpr uint16_t KEY_TAB = 258;
		static constexpr uint16_t KEY_BACKSPACE = 259;
		static constexpr uint16_t KEY_INSERT = 260;
		static constexpr uint16_t KEY_DELETE = 261;
		static constexpr uint16_t KEY_RIGHT = 262;
		static constexpr uint16_t KEY_LEFT = 263;
		static constexpr uint16_t KEY_DOWN = 264;
		static constexpr uint16_t KEY_UP = 265;
		static constexpr uint16_t KEY_PAGE_UP = 266;
		static constexpr uint16_t KEY_PAGE_DOWN = 267;
		static constexpr uint16_t KEY_HOME = 268;
		static constexpr uint16_t KEY_END = 269;
		static constexpr uint16_t KEY_CAPS_LOCK = 280;
		static constexpr uint16_t KEY_SCROLL_LOCK = 281;
		static constexpr uint16_t KEY_NUM_LOCK = 282;
		static constexpr uint16_t KEY_PRINT_SCREEN = 283;
		static constexpr uint16_t KEY_PAUSE = 284;
		static constexpr uint16_t KEY_F1 = 290;
		static constexpr uint16_t KEY_F2 = 291;
		static constexpr uint16_t KEY_F3 = 292;
		static constexpr uint16_t KEY_F4 = 293;
		static constexpr uint16_t KEY_F5 = 294;
		static constexpr uint16_t KEY_F6 = 295;
		static constexpr uint16_t KEY_F7 = 296;
		static constexpr uint16_t KEY_F8 = 297;
		static constexpr uint16_t KEY_F9 = 298;
		static constexpr uint16_t KEY_F10 = 299;
		static constexpr uint16_t KEY_F11 = 300;
		static constexpr uint16_t KEY_F12 = 301;
		static constexpr uint16_t KEY_F13 = 302;
		static constexpr uint16_t KEY_F14 = 303;
		static constexpr uint16_t KEY_F15 = 304;
		static constexpr uint16_t KEY_F16 = 305;
		static constexpr uint16_t KEY_F17 = 306;
		static constexpr uint16_t KEY_F18 = 307;
		static constexpr uint16_t KEY_F19 = 308;
		static constexpr uint16_t KEY_F20 = 309;
		static constexpr uint16_t KEY_F21 = 310;
		static constexpr uint16_t KEY_F22 = 311;
		static constexpr uint16_t KEY_F23 = 312;
		static constexpr uint16_t KEY_F24 = 313;
		static constexpr uint16_t KEY_F25 = 314;
		static constexpr uint16_t KEY_KP_0 = 320;
		static constexpr uint16_t KEY_KP_1 = 321;
		static constexpr uint16_t KEY_KP_2 = 322;
		static constexpr uint16_t KEY_KP_3 = 323;
		static constexpr uint16_t KEY_KP_4 = 324;
		static constexpr uint16_t KEY_KP_5 = 325;
		static constexpr uint16_t KEY_KP_6 = 326;
		static constexpr uint16_t KEY_KP_7 = 327;
		static constexpr uint16_t KEY_KP_8 = 328;
		static constexpr uint16_t KEY_KP_9 = 329;
		static constexpr uint16_t KEY_KP_DECIMAL = 330;
		static constexpr uint16_t KEY_KP_DIVIDE = 331;
		static constexpr uint16_t KEY_KP_MULTIPLY = 332;
		static constexpr uint16_t KEY_KP_SUBTRACT = 333;
		static constexpr uint16_t KEY_KP_ADD = 334;
		static constexpr uint16_t KEY_KP_ENTER = 335;
		static constexpr uint16_t KEY_KP_EQUAL = 336;
		static constexpr uint16_t KEY_LEFT_SHIFT = 340;
		static constexpr uint16_t KEY_LEFT_CONTROL = 341;
		static constexpr uint16_t KEY_LEFT_ALT = 342;
		static constexpr uint16_t KEY_LEFT_SUPER = 343;
		static constexpr uint16_t KEY_RIGHT_SHIFT = 344;
		static constexpr uint16_t KEY_RIGHT_CONTROL = 345;
		static constexpr uint16_t KEY_RIGHT_ALT = 346;
		static constexpr uint16_t KEY_RIGHT_SUPER = 347;
		static constexpr uint16_t KEY_MENU = 348;
		static constexpr uint16_t KEY_LAST = KEY_MENU;
		static constexpr uint16_t NUM_KEYS = 500;
	};
}