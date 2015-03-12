#pragma once

/*

Pavirsiaus objektas, kuriame laikomi atrenderinti simboliai.

*/

#include "GLST_std.h"
#include "GLST_gl.h"
#include "GLST_unicode.h"
#include <SDL/SDL_ttf.h>

class GLST_Surface
{
private:
	SDL_Surface *surface;
	int symbol_top;
	int symbol_bottom;
	int symbol_left;
	TTF_Font *font;
	GLuint texture;
	bool modified;
	bool assigned;
	bool texture_ok;
	bool FindPlaceForGlyph(SDL_Rect* rect);
	void RenderTexture();
public:
	int width;
	int height;
	void Refresh();
	void RenderSurface(int x, int y);
	void SetSceneStart();
	bool GreateGlyph(std::string utf8char, SDL_Rect* rect);
	GLuint *GetGLTexture();
	GLST_Surface(int width, int height, TTF_Font *font);
	~GLST_Surface(void);
};
