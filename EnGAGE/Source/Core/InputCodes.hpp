#pragma once


namespace Core
{
	namespace InputCodes {
		/* The unknown key */
		 constexpr unsigned int KEY_UNKNOWN = -1;
		 constexpr unsigned int KEY_SPACE = 32;
		 constexpr unsigned int KEY_APOSTROPHE = 39;  /* ' */
		 constexpr unsigned int KEY_COMMA = 44;  /* , */
		 constexpr unsigned int KEY_MINUS = 45;  /* - */
		 constexpr unsigned int KEY_PERIOD = 46;  /* . */
		 constexpr unsigned int KEY_SLASH = 47;  /* / */
		 constexpr unsigned int KEY_0 = 48;
		 constexpr unsigned int KEY_1 = 49;
		 constexpr unsigned int KEY_2 = 50;
		 constexpr unsigned int KEY_3 = 51;
		 constexpr unsigned int KEY_4 = 52;
		 constexpr unsigned int KEY_5 = 53;
		 constexpr unsigned int KEY_6 = 54;
		 constexpr unsigned int KEY_7 = 55;
		 constexpr unsigned int KEY_8 = 56;
		 constexpr unsigned int KEY_9 = 57;
		 constexpr unsigned int KEY_SEMICOLON = 59;  /* ; */
		 constexpr unsigned int KEY_EQUAL = 61;  /* = */
		 constexpr unsigned int KEY_A = 65;
		 constexpr unsigned int KEY_B = 66;
		 constexpr unsigned int KEY_C = 67;
		 constexpr unsigned int KEY_D = 68;
		 constexpr unsigned int KEY_E = 69;
		 constexpr unsigned int KEY_F = 70;
		 constexpr unsigned int KEY_G = 71;
		 constexpr unsigned int KEY_H = 72;
		 constexpr unsigned int KEY_I = 73;
		 constexpr unsigned int KEY_J = 74;
		 constexpr unsigned int KEY_K = 75;
		 constexpr unsigned int KEY_L = 76;
		 constexpr unsigned int KEY_M = 77;
		 constexpr unsigned int KEY_N = 78;
		 constexpr unsigned int KEY_O = 79;
		 constexpr unsigned int KEY_P = 80;
		 constexpr unsigned int KEY_Q = 81;
		 constexpr unsigned int KEY_R = 82;
		 constexpr unsigned int KEY_S = 83;
		 constexpr unsigned int KEY_T = 84;
		 constexpr unsigned int KEY_U = 85;
		 constexpr unsigned int KEY_V = 86;
		 constexpr unsigned int KEY_W = 87;
		 constexpr unsigned int KEY_X = 88;
		 constexpr unsigned int KEY_Y = 89;
		 constexpr unsigned int KEY_Z = 90;
		 constexpr unsigned int KEY_LEFT_BRACKET = 91;  /* [ */
		 constexpr unsigned int KEY_BACKSLASH = 92;  /* \ */
		 constexpr unsigned int KEY_RIGHT_BRACKET = 93;  /* ] */
		 constexpr unsigned int KEY_GRAVE_ACCENT = 96;  /* ` */
		 constexpr unsigned int KEY_WORLD_1 = 161; /* non-US #1 */
		 constexpr unsigned int KEY_WORLD_2 = 162; /* non-US #2 */
		 constexpr unsigned int KEY_ESCAPE = 256;
		 constexpr unsigned int KEY_ENTER = 257;
		 constexpr unsigned int KEY_TAB = 258;
		 constexpr unsigned int KEY_BACKSPACE = 259;
		 constexpr unsigned int KEY_INSERT = 260;
		 constexpr unsigned int KEY_DELETE = 261;
		 constexpr unsigned int KEY_RIGHT = 262;
		 constexpr unsigned int KEY_LEFT = 263;
		 constexpr unsigned int KEY_DOWN = 264;
		 constexpr unsigned int KEY_UP = 265;
		 constexpr unsigned int KEY_PAGE_UP = 266;
		 constexpr unsigned int KEY_PAGE_DOWN = 267;
		 constexpr unsigned int KEY_HOME = 268;
		 constexpr unsigned int KEY_END = 269;
		 constexpr unsigned int KEY_CAPS_LOCK = 280;
		 constexpr unsigned int KEY_SCROLL_LOCK = 281;
		 constexpr unsigned int KEY_NUM_LOCK = 282;
		 constexpr unsigned int KEY_PRINT_SCREEN = 283;
		 constexpr unsigned int KEY_PAUSE = 284;
		 constexpr unsigned int KEY_F1 = 290;
		 constexpr unsigned int KEY_F2 = 291;
		 constexpr unsigned int KEY_F3 = 292;
		 constexpr unsigned int KEY_F4 = 293;
		 constexpr unsigned int KEY_F5 = 294;
		 constexpr unsigned int KEY_F6 = 295;
		 constexpr unsigned int KEY_F7 = 296;
		 constexpr unsigned int KEY_F8 = 297;
		 constexpr unsigned int KEY_F9 = 298;
		 constexpr unsigned int KEY_F10 = 299;
		 constexpr unsigned int KEY_F11 = 300;
		 constexpr unsigned int KEY_F12 = 301;
		 constexpr unsigned int KEY_F13 = 302;
		 constexpr unsigned int KEY_F14 = 303;
		 constexpr unsigned int KEY_F15 = 304;
		 constexpr unsigned int KEY_F16 = 305;
		 constexpr unsigned int KEY_F17 = 306;
		 constexpr unsigned int KEY_F18 = 307;
		 constexpr unsigned int KEY_F19 = 308;
		 constexpr unsigned int KEY_F20 = 309;
		 constexpr unsigned int KEY_F21 = 310;
		 constexpr unsigned int KEY_F22 = 311;
		 constexpr unsigned int KEY_F23 = 312;
		 constexpr unsigned int KEY_F24 = 313;
		 constexpr unsigned int KEY_F25 = 314;
		 constexpr unsigned int KEY_KP_0 = 320;
		 constexpr unsigned int KEY_KP_1 = 321;
		 constexpr unsigned int KEY_KP_2 = 322;
		 constexpr unsigned int KEY_KP_3 = 323;
		 constexpr unsigned int KEY_KP_4 = 324;
		 constexpr unsigned int KEY_KP_5 = 325;
		 constexpr unsigned int KEY_KP_6 = 326;
		 constexpr unsigned int KEY_KP_7 = 327;
		 constexpr unsigned int KEY_KP_8 = 328;
		 constexpr unsigned int KEY_KP_9 = 329;
		 constexpr unsigned int KEY_KP_DECIMAL = 330;
		 constexpr unsigned int KEY_KP_DIVIDE = 331;
		 constexpr unsigned int KEY_KP_MULTIPLY = 332;
		 constexpr unsigned int KEY_KP_SUBTRACT = 333;
		 constexpr unsigned int KEY_KP_ADD = 334;
		 constexpr unsigned int KEY_KP_ENTER = 335;
		 constexpr unsigned int KEY_KP_EQUAL = 336;
		 constexpr unsigned int KEY_LEFT_SHIFT = 340;
		 constexpr unsigned int KEY_LEFT_CONTROL = 341;
		 constexpr unsigned int KEY_LEFT_ALT = 342;
		 constexpr unsigned int KEY_LEFT_SUPER = 343;
		 constexpr unsigned int KEY_RIGHT_SHIFT = 344;
		 constexpr unsigned int KEY_RIGHT_CONTROL = 345;
		 constexpr unsigned int KEY_RIGHT_ALT = 346;
		 constexpr unsigned int KEY_RIGHT_SUPER = 347;
		 constexpr unsigned int KEY_MENU = 348;
		 constexpr unsigned int KEY_LAST = KEY_MENU;
		 constexpr unsigned int NUM_KEYS = 500;

		 inline static constexpr const char* toString(unsigned int code)
		 {
			 switch (code)
			 {
				 case KEY_UNKNOWN:		return "KEY_UNKNOWN";
				 case KEY_SPACE:		return "KEY_SPACE";
				 case KEY_APOSTROPHE:	return "KEY_APOSTROPHE";  /* ' */
				 case KEY_COMMA:		return "KEY_COMMA";   /* , */
				 case KEY_MINUS:		return "KEY_MINUS";   /* - */
				 case KEY_PERIOD:		return "KEY_PERIOD";   /* . */
				 case KEY_SLASH:		return "KEY_SLASH";   /* / */
				 case KEY_0:			return "KEY_0"; 
				 case KEY_1:			return "KEY_1"; 
				 case KEY_2:			return "KEY_2"; 
				 case KEY_3:			return "KEY_3"; 
				 case KEY_4:			return "KEY_4"; 
				 case KEY_5:			return "KEY_5"; 
				 case KEY_6:			return "KEY_6"; 
				 case KEY_7:			return "KEY_7"; 
				 case KEY_8:			return "KEY_8"; 
				 case KEY_9:			return "KEY_9"; 
				 case KEY_SEMICOLON:	return "KEY_SEMICOLON";   /* ; */
				 case KEY_EQUAL:		return "KEY_EQUAL";   /* = */
				 case KEY_A :			return "KEY_A"; 
				 case KEY_B :			return "KEY_B"; 
				 case KEY_C :			return "KEY_C"; 
				 case KEY_D :			return "KEY_D"; 
				 case KEY_E :			return "KEY_E"; 
				 case KEY_F :			return "KEY_F"; 
				 case KEY_G :			return "KEY_G"; 
				 case KEY_H :			return "KEY_H"; 
				 case KEY_I :			return "KEY_I"; 
				 case KEY_J :			return "KEY_J"; 
				 case KEY_K :			return "KEY_K"; 
				 case KEY_L :			return "KEY_L"; 
				 case KEY_M :			return "KEY_M"; 
				 case KEY_N :			return "KEY_N"; 
				 case KEY_O :			return "KEY_O"; 
				 case KEY_P :			return "KEY_P"; 
				 case KEY_Q :			return "KEY_Q"; 
				 case KEY_R :			return "KEY_R"; 
				 case KEY_S :			return "KEY_S"; 
				 case KEY_T :			return "KEY_T"; 
				 case KEY_U :			return "KEY_U"; 
				 case KEY_V :			return "KEY_V"; 
				 case KEY_W :			return "KEY_W"; 
				 case KEY_X :			return "KEY_X"; 
				 case KEY_Y :			return "KEY_Y"; 
				 case KEY_Z :			return "KEY_Z"; 
				 case KEY_LEFT_BRACKET: return "KEY_LEFT_BRACKET";   /* [ */
				 case KEY_BACKSLASH:	return "KEY_BACKSLASH";   /* \ */
				 case KEY_RIGHT_BRACKET:return "KEY_RIGHT_BRACKET";   /* ] */
				 case KEY_GRAVE_ACCENT: return "KEY_GRAVE_ACCENT";   /* ` */
				 case KEY_WORLD_1:		return "KEY_WORLD_1";
				 case KEY_WORLD_2:		return "KEY_WORLD_2";
				 case KEY_ESCAPE:		return "KEY_ESCAPE";
				 case KEY_ENTER:		return "KEY_ENTER";
				 case KEY_TAB:			return "KEY_TAB";
				 case KEY_BACKSPACE:	return "KEY_BACKSPACE";
				 case KEY_INSERT:		return "KEY_INSERT";
				 case KEY_DELETE:		return "KEY_DELETE";
				 case KEY_RIGHT:		return "KEY_RIGHT";
				 case KEY_LEFT:			return "KEY_LEFT";
				 case KEY_DOWN:			return "KEY_DOWN";
				 case KEY_UP:			return "KEY_UP";
				 case KEY_PAGE_UP:		return "KEY_PAGE_UP";
				 case KEY_PAGE_DOWN:	return "KEY_PAGE_DOWN";
				 case KEY_HOME:			return "KEY_HOME";
				 case KEY_END:			return "KEY_END";
				 case KEY_CAPS_LOCK:	return "KEY_CAPS_LOCK";
				 case KEY_SCROLL_LOCK:  return "KEY_SCROLL_LOCK";
				 case KEY_NUM_LOCK:		return "KEY_NUM_LOCK";
				 case KEY_PRINT_SCREEN: return "KEY_PRINT_SCREEN";
				 case KEY_PAUSE:		return "KEY_PAUSE";
				 case KEY_F1 :			return "KEY_F1"; 
				 case KEY_F2 :			return "KEY_F2"; 
				 case KEY_F3 :			return "KEY_F3"; 
				 case KEY_F4 :			return "KEY_F4"; 
				 case KEY_F5 :			return "KEY_F5"; 
				 case KEY_F6 :			return "KEY_F6"; 
				 case KEY_F7 :			return "KEY_F7"; 
				 case KEY_F8 :			return "KEY_F8"; 
				 case KEY_F9 :			return "KEY_F9"; 
				 case KEY_F10:			return "KEY_F10"; 
				 case KEY_F11:			return "KEY_F11"; 
				 case KEY_F12:			return "KEY_F12"; 
				 case KEY_F13:			return "KEY_F13"; 
				 case KEY_F14:			return "KEY_F14"; 
				 case KEY_F15:			return "KEY_F15"; 
				 case KEY_F16:			return "KEY_F16"; 
				 case KEY_F17:			return "KEY_F17"; 
				 case KEY_F18:			return "KEY_F18"; 
				 case KEY_F19:			return "KEY_F19"; 
				 case KEY_F20:			return "KEY_F20"; 
				 case KEY_F21:			return "KEY_F21"; 
				 case KEY_F22:			return "KEY_F22"; 
				 case KEY_F23:			return "KEY_F23"; 
				 case KEY_F24:			return "KEY_F24"; 
				 case KEY_F25:			return "KEY_F25"; 
				 case KEY_KP_0 :		return "KEY_KP_0"; 
				 case KEY_KP_1 :		return "KEY_KP_1"; 
				 case KEY_KP_2 :		return "KEY_KP_2"; 
				 case KEY_KP_3 :		return "KEY_KP_3"; 
				 case KEY_KP_4 :		return "KEY_KP_4"; 
				 case KEY_KP_5 :		return "KEY_KP_5"; 
				 case KEY_KP_6 :		return "KEY_KP_6"; 
				 case KEY_KP_7 :		return "KEY_KP_7"; 
				 case KEY_KP_8 :		return "KEY_KP_8"; 
				 case KEY_KP_9 :		return "KEY_KP_9"; 
				 case KEY_KP_DECIMAL:	return "KEY_KP_DECIMAL";
				 case KEY_KP_DIVIDE:	return "KEY_KP_DIVIDE";
				 case KEY_KP_MULTIPLY:	return "KEY_KP_MULTIPLY";
				 case KEY_KP_SUBTRACT:	return "KEY_KP_SUBTRACT";
				 case KEY_KP_ADD:		return "KEY_KP_ADD";
				 case KEY_KP_ENTER:		return "KEY_KP_ENTER";
				 case KEY_KP_EQUAL:		return "KEY_KP_EQUAL";
				 case KEY_LEFT_SHIFT:	return "KEY_LEFT_SHIFT";
				 case KEY_LEFT_CONTROL: return "KEY_LEFT_CONTROL";
				 case KEY_LEFT_ALT:		return "KEY_LEFT_ALT";
				 case KEY_LEFT_SUPER:	return "KEY_LEFT_SUPER";
				 case KEY_RIGHT_SHIFT:	return "KEY_RIGHT_SHIFT";
				 case KEY_RIGHT_CONTROL:return "KEY_RIGHT_CONTROL";
				 case KEY_RIGHT_ALT:	return "KEY_RIGHT_ALT";
				 case KEY_RIGHT_SUPER:	return "KEY_RIGHT_SUPER";
				 case KEY_MENU:			return "KEY_MENU";
				 case NUM_KEYS:			return "NUM_KEYS";
				default: return "";
			 }
		 }
	};
}
