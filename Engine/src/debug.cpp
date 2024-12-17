#include "debug.h"

#include <string>



std::string lookup(int val) {
	switch (val) {
	case 0x1903: return "GL_RED";
	case 0x1904: return "GL_GREEN";
	case 0x1905: return "GL_BLUE";
	case 0x1906: return "GL_ALPHA";
	case 0x1907: return "GL_RGB";
	case 0x1908: return "GL_RGBA";

	case 32: return "GLFW_KEY_SPACE";
	case 39: return "GLFW_KEY_APOSTROPHE";
	case 44: return "GLFW_KEY_COMMA";
	case 45: return "GLFW_KEY_MINUS";
	case 46: return "GLFW_KEY_PERIOD";
	case 47: return "GLFW_KEY_SLASH";
	case 48: return "GLFW_KEY_0";
	case 49: return "GLFW_KEY_1";
	case 50: return "GLFW_KEY_2";
	case 51: return "GLFW_KEY_3";
	case 52: return "GLFW_KEY_4";
	case 53: return "GLFW_KEY_5";
	case 54: return "GLFW_KEY_6";
	case 55: return "GLFW_KEY_7";
	case 56: return "GLFW_KEY_8";
	case 57: return "GLFW_KEY_9";
	case 59: return "GLFW_KEY_SEMICOLON";
	case 61: return "GLFW_KEY_EQUAL";
	case 65: return "GLFW_KEY_A";
	case 66: return "GLFW_KEY_B";
	case 67: return "GLFW_KEY_C";
	case 68: return "GLFW_KEY_D";
	case 69: return "GLFW_KEY_E";
	case 70: return "GLFW_KEY_F";
	case 71: return "GLFW_KEY_G";
	case 72: return "GLFW_KEY_H";
	case 73: return "GLFW_KEY_I";
	case 74: return "GLFW_KEY_J";
	case 75: return "GLFW_KEY_K";
	case 76: return "GLFW_KEY_L";
	case 77: return "GLFW_KEY_M";
	case 78: return "GLFW_KEY_N";
	case 79: return "GLFW_KEY_O";
	case 80: return "GLFW_KEY_P";
	case 81: return "GLFW_KEY_Q";
	case 82: return "GLFW_KEY_R";
	case 83: return "GLFW_KEY_S";
	case 84: return "GLFW_KEY_T";
	case 85: return "GLFW_KEY_U";
	case 86: return "GLFW_KEY_V";
	case 87: return "GLFW_KEY_W";
	case 88: return "GLFW_KEY_X";
	case 89: return "GLFW_KEY_Y";
	case 90: return "GLFW_KEY_Z";
	case 91: return "GLFW_KEY_LEFT_BRACKET";
	case 92: return "GLFW_KEY_BACKSLASH";
	case 93: return "GLFW_KEY_RIGHT_BRACKET";
	case 96: return "GLFW_KEY_GRAVE_ACCENT";
	case 161: return "GLFW_KEY_WORLD_1";
	case 162: return "GLFW_KEY_WORLD_2";
	case 256: return "GLFW_KEY_ESCAPE";
	case 257: return "GLFW_KEY_ENTER";
	case 258: return "GLFW_KEY_TAB";
	case 259: return "GLFW_KEY_BACKSPACE";
	case 260: return "GLFW_KEY_INSERT";
	case 261: return "GLFW_KEY_DELETE";
	case 262: return "GLFW_KEY_RIGHT";
	case 263: return "GLFW_KEY_LEFT";
	case 264: return "GLFW_KEY_DOWN";
	case 265: return "GLFW_KEY_UP";
	case 266: return "GLFW_KEY_PAGE_UP";
	case 267: return "GLFW_KEY_PAGE_DOWN";
	case 268: return "GLFW_KEY_HOME";
	case 269: return "GLFW_KEY_END";
	case 280: return "GLFW_KEY_CAPS_LOCK";
	case 281: return "GLFW_KEY_SCROLL_LOCK";
	case 282: return "GLFW_KEY_NUM_LOCK";
	case 283: return "GLFW_KEY_PRINT_SCREEN";
	case 284: return "GLFW_KEY_PAUSE";
	case 290: return "GLFW_KEY_F1";
	case 291: return "GLFW_KEY_F2";
	case 292: return "GLFW_KEY_F3";
	case 293: return "GLFW_KEY_F4";
	case 294: return "GLFW_KEY_F5";
	case 295: return "GLFW_KEY_F6";
	case 296: return "GLFW_KEY_F7";
	case 297: return "GLFW_KEY_F8";
	case 298: return "GLFW_KEY_F9";
	case 299: return "GLFW_KEY_F10";
	case 300: return "GLFW_KEY_F11";
	case 301: return "GLFW_KEY_F12";
	case 302: return "GLFW_KEY_F13";
	case 303: return "GLFW_KEY_F14";
	case 304: return "GLFW_KEY_F15";
	case 305: return "GLFW_KEY_F16";
	case 306: return "GLFW_KEY_F17";
	case 307: return "GLFW_KEY_F18";
	case 308: return "GLFW_KEY_F19";
	case 309: return "GLFW_KEY_F20";
	case 310: return "GLFW_KEY_F21";
	case 311: return "GLFW_KEY_F22";
	case 312: return "GLFW_KEY_F23";
	case 313: return "GLFW_KEY_F24";
	case 314: return "GLFW_KEY_F25";
	case 320: return "GLFW_KEY_KP_0";
	case 321: return "GLFW_KEY_KP_1";
	case 322: return "GLFW_KEY_KP_2";
	case 323: return "GLFW_KEY_KP_3";
	case 324: return "GLFW_KEY_KP_4";
	case 325: return "GLFW_KEY_KP_5";
	case 326: return "GLFW_KEY_KP_6";
	case 327: return "GLFW_KEY_KP_7";
	case 328: return "GLFW_KEY_KP_8";
	case 329: return "GLFW_KEY_KP_9";
	case 330: return "GLFW_KEY_KP_DECIMAL";
	case 331: return "GLFW_KEY_KP_DIVIDE";
	case 332: return "GLFW_KEY_KP_MULTIPLY";
	case 333: return "GLFW_KEY_KP_SUBTRACT";
	case 334: return "GLFW_KEY_KP_ADD";
	case 335: return "GLFW_KEY_KP_ENTER";
	case 336: return "GLFW_KEY_KP_EQUAL";
	case 340: return "GLFW_KEY_LEFT_SHIFT";
	case 341: return "GLFW_KEY_LEFT_CONTROL";
	case 342: return "GLFW_KEY_LEFT_ALT";
	case 343: return "GLFW_KEY_LEFT_SUPER";
	case 344: return "GLFW_KEY_RIGHT_SHIFT";
	case 345: return "GLFW_KEY_RIGHT_CONTROL";
	case 346: return "GLFW_KEY_RIGHT_ALT";
	case 347: return "GLFW_KEY_RIGHT_SUPER";
	case 348: return "GLFW_KEY_MENU";
	default: return "UNKNOWN";
	}
}

#include <glad/glad.h>

void gl_check_error(const char* function, const char* file, int line) {
	GLenum error = 0;
	bool isError = false;
	while ((error = glGetError()) != GL_NO_ERROR) {
		isError = true;
		switch (error) {
		case GL_INVALID_ENUM:
			printf("GL_INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:
			printf("GL_INVALID_VALUE");
			break;
		case GL_INVALID_OPERATION:
			printf("GL_INVALID_OPERATION");
			break;
		case GL_STACK_OVERFLOW:
			printf("GL_STACK_OVERFLOW");
			break;
		case GL_STACK_UNDERFLOW:
			printf("GL_STACK_UNDERFLOW");
			break;
		case GL_OUT_OF_MEMORY:
			printf("GL_OUT_OF_MEMORY");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			printf("GL_INVALID_FRAMEBUFFER_OPERATION");
			break;
		case GL_CONTEXT_LOST:
			printf("GL_CONTEXT_LOST");
			break;
		default:
			printf("Unknown error code %d", error);
		}
		printf(" found at function '%s' (%s:%d)\n", function, file, line);
	}
#ifdef EXIT_ON_GL_ERROR
	if (isError) {
		exit(1);
	}
#endif // EXIT_ON_GL_ERROR
}
