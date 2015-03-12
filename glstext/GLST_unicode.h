#pragma once

#include "GLST_std.h"
#include "utf8.h"
#include "GLST_gl.h"

bool IsSDLKeysymValidSymbol(SDL_keysym *keysym);
void DoBackspaceForString(std::string *value);
std::string GetStringFromUnicode(Uint16 code);
int utf8len(std::string & value);
