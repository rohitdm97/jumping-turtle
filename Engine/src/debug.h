#pragma once

#define EXIT_ON_GL_ERROR
#define ENABLE_DEBUG_CONTEXT
#define CHECK_GL_ERRORS

void gl_check_error(const char* function, const char* file, int line);

#ifdef CHECK_GL_ERRORS
#define GCE gl_check_error(__FUNCTION__, __FILE__, __LINE__);
#else
#define GCE
#endif // CHECK_GL_ERRORS

#define ALLOW_UKNOWN_UNIFORMS
