#pragma once


namespace Core
{
	namespace InputCodes {
		/* The unknown key */
		 constexpr uint16_t KEY_UNKNOWN = -1;
		 constexpr uint16_t KEY_SPACE = 32;
		 constexpr uint16_t KEY_APOSTROPHE = 39;  /* ' */
		 constexpr uint16_t KEY_COMMA = 44;  /* , */
		 constexpr uint16_t KEY_MINUS = 45;  /* - */
		 constexpr uint16_t KEY_PERIOD = 46;  /* . */
		 constexpr uint16_t KEY_SLASH = 47;  /* / */
		 constexpr uint16_t KEY_0 = 48;
		 constexpr uint16_t KEY_1 = 49;
		 constexpr uint16_t KEY_2 = 50;
		 constexpr uint16_t KEY_3 = 51;
		 constexpr uint16_t KEY_4 = 52;
		 constexpr uint16_t KEY_5 = 53;
		 constexpr uint16_t KEY_6 = 54;
		 constexpr uint16_t KEY_7 = 55;
		 constexpr uint16_t KEY_8 = 56;
		 constexpr uint16_t KEY_9 = 57;
		 constexpr uint16_t KEY_SEMICOLON = 59;  /* ; */
		 constexpr uint16_t KEY_EQUAL = 61;  /* = */
		 constexpr uint16_t KEY_A = 65;
		 constexpr uint16_t KEY_B = 66;
		 constexpr uint16_t KEY_C = 67;
		 constexpr uint16_t KEY_D = 68;
		 constexpr uint16_t KEY_E = 69;
		 constexpr uint16_t KEY_F = 70;
		 constexpr uint16_t KEY_G = 71;
		 constexpr uint16_t KEY_H = 72;
		 constexpr uint16_t KEY_I = 73;
		 constexpr uint16_t KEY_J = 74;
		 constexpr uint16_t KEY_K = 75;
		 constexpr uint16_t KEY_L = 76;
		 constexpr uint16_t KEY_M = 77;
		 constexpr uint16_t KEY_N = 78;
		 constexpr uint16_t KEY_O = 79;
		 constexpr uint16_t KEY_P = 80;
		 constexpr uint16_t KEY_Q = 81;
		 constexpr uint16_t KEY_R = 82;
		 constexpr uint16_t KEY_S = 83;
		 constexpr uint16_t KEY_T = 84;
		 constexpr uint16_t KEY_U = 85;
		 constexpr uint16_t KEY_V = 86;
		 constexpr uint16_t KEY_W = 87;
		 constexpr uint16_t KEY_X = 88;
		 constexpr uint16_t KEY_Y = 89;
		 constexpr uint16_t KEY_Z = 90;
		 constexpr uint16_t KEY_LEFT_BRACKET = 91;  /* [ */
		 constexpr uint16_t KEY_BACKSLASH = 92;  /* \ */
		 constexpr uint16_t KEY_RIGHT_BRACKET = 93;  /* ] */
		 constexpr uint16_t KEY_GRAVE_ACCENT = 96;  /* ` */
		 constexpr uint16_t KEY_WORLD_1 = 161; /* non-US #1 */
		 constexpr uint16_t KEY_WORLD_2 = 162; /* non-US #2 */
		 constexpr uint16_t KEY_ESCAPE = 256;
		 constexpr uint16_t KEY_ENTER = 257;
		 constexpr uint16_t KEY_TAB = 258;
		 constexpr uint16_t KEY_BACKSPACE = 259;
		 constexpr uint16_t KEY_INSERT = 260;
		 constexpr uint16_t KEY_DELETE = 261;
		 constexpr uint16_t KEY_RIGHT = 262;
		 constexpr uint16_t KEY_LEFT = 263;
		 constexpr uint16_t KEY_DOWN = 264;
		 constexpr uint16_t KEY_UP = 265;
		 constexpr uint16_t KEY_PAGE_UP = 266;
		 constexpr uint16_t KEY_PAGE_DOWN = 267;
		 constexpr uint16_t KEY_HOME = 268;
		 constexpr uint16_t KEY_END = 269;
		 constexpr uint16_t KEY_CAPS_LOCK = 280;
		 constexpr uint16_t KEY_SCROLL_LOCK = 281;
		 constexpr uint16_t KEY_NUM_LOCK = 282;
		 constexpr uint16_t KEY_PRINT_SCREEN = 283;
		 constexpr uint16_t KEY_PAUSE = 284;
		 constexpr uint16_t KEY_F1 = 290;
		 constexpr uint16_t KEY_F2 = 291;
		 constexpr uint16_t KEY_F3 = 292;
		 constexpr uint16_t KEY_F4 = 293;
		 constexpr uint16_t KEY_F5 = 294;
		 constexpr uint16_t KEY_F6 = 295;
		 constexpr uint16_t KEY_F7 = 296;
		 constexpr uint16_t KEY_F8 = 297;
		 constexpr uint16_t KEY_F9 = 298;
		 constexpr uint16_t KEY_F10 = 299;
		 constexpr uint16_t KEY_F11 = 300;
		 constexpr uint16_t KEY_F12 = 301;
		 constexpr uint16_t KEY_F13 = 302;
		 constexpr uint16_t KEY_F14 = 303;
		 constexpr uint16_t KEY_F15 = 304;
		 constexpr uint16_t KEY_F16 = 305;
		 constexpr uint16_t KEY_F17 = 306;
		 constexpr uint16_t KEY_F18 = 307;
		 constexpr uint16_t KEY_F19 = 308;
		 constexpr uint16_t KEY_F20 = 309;
		 constexpr uint16_t KEY_F21 = 310;
		 constexpr uint16_t KEY_F22 = 311;
		 constexpr uint16_t KEY_F23 = 312;
		 constexpr uint16_t KEY_F24 = 313;
		 constexpr uint16_t KEY_F25 = 314;
		 constexpr uint16_t KEY_KP_0 = 320;
		 constexpr uint16_t KEY_KP_1 = 321;
		 constexpr uint16_t KEY_KP_2 = 322;
		 constexpr uint16_t KEY_KP_3 = 323;
		 constexpr uint16_t KEY_KP_4 = 324;
		 constexpr uint16_t KEY_KP_5 = 325;
		 constexpr uint16_t KEY_KP_6 = 326;
		 constexpr uint16_t KEY_KP_7 = 327;
		 constexpr uint16_t KEY_KP_8 = 328;
		 constexpr uint16_t KEY_KP_9 = 329;
		 constexpr uint16_t KEY_KP_DECIMAL = 330;
		 constexpr uint16_t KEY_KP_DIVIDE = 331;
		 constexpr uint16_t KEY_KP_MULTIPLY = 332;
		 constexpr uint16_t KEY_KP_SUBTRACT = 333;
		 constexpr uint16_t KEY_KP_ADD = 334;
		 constexpr uint16_t KEY_KP_ENTER = 335;
		 constexpr uint16_t KEY_KP_EQUAL = 336;
		 constexpr uint16_t KEY_LEFT_SHIFT = 340;
		 constexpr uint16_t KEY_LEFT_CONTROL = 341;
		 constexpr uint16_t KEY_LEFT_ALT = 342;
		 constexpr uint16_t KEY_LEFT_SUPER = 343;
		 constexpr uint16_t KEY_RIGHT_SHIFT = 344;
		 constexpr uint16_t KEY_RIGHT_CONTROL = 345;
		 constexpr uint16_t KEY_RIGHT_ALT = 346;
		 constexpr uint16_t KEY_RIGHT_SUPER = 347;
		 constexpr uint16_t KEY_MENU = 348;
		 constexpr uint16_t KEY_LAST = KEY_MENU;
		 constexpr uint16_t NUM_KEYS = 500;

		 inline static constexpr const char* toString(uint16_t code)
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
