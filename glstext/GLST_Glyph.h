#pragma once

#include "GLST_unicode.h"
#include <SDL/SDL_ttf.h>
#include "GLST_Surface.h"
#include "GLST_Glyph.h"

/*

Simbolio formos aprasas su informacija kokiame Surface jis saugomas ir kurioje
vietoje.

*/

class GLST_Glyph
{
public:
	SDL_Rect rectOnSurface;
	Uint16 char_code;
	float t_minx;
	float t_maxx;
	float t_advance;
	float left;
	float top;
	float right;
	float bottom;
	float height;
	GLST_Surface *surfaceRef;
	GLST_Glyph(const SDL_Rect rect, GLST_Surface *surface, TTF_Font *font, Uint16 char_code);
	~GLST_Glyph(void);
};
