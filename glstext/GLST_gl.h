#pragma once

#ifdef WIN32
#  include <windows.h>
#endif

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#ifndef GL_BGRA_EXT
#  include <GL/glext.h>
#endif

#include <SDL/SDL.h>
