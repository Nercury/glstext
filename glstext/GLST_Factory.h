#pragma once

#include "GLST_gl.h"
#include "GLST_std.h"
#include "GLST_unicode.h"
#include <SDL/SDL_ttf.h>
#include "GLST_Glyph.h"
#include "GLST_Surface.h"

/*

Object to render specific font type. It provides glyphs, measurements,
and render methods.

*/

class GLST_Factory
{
private:
	std::string font_file;
	int size;

	GLST_Surface *MakeSurface();
	GLST_Glyph *MakeGlyph(std::string utf_char, int char_code);
	GLST_Glyph *GetGlyph(std::string utf_char, int char_code);

	std::map<std::string, GLST_Glyph*> glyph_map;
	std::vector<GLST_Surface*> surfaces;

	GLST_Surface* surface;
public:
	TTF_Font* font;
	void GetTextGlyphs(std::vector<GLST_Glyph*> *glyphs, std::string text);
	std::string getFontFile() const;
	int getFontSize();
	bool isFontLoaded();
	void SetSceneStart();
	void Refresh();
	bool CanRender(std::string utf8chars);
	void MeasureAdvance(GLST_Glyph* first, GLST_Glyph* second, float *char_offset, float *advance);
	float MeasureWidth(std::vector<GLST_Glyph*> *glyphs, std::vector<GLST_Glyph*>::size_type start, std::vector<GLST_Glyph*>::size_type end);
	void RenderSurfaces(int x, int y);
	int use_count;
	GLST_Factory(std::string font_file, int size);
	~GLST_Factory(void);
};

void GLST_SetSceneStart();
void GLST_Refresh();

GLST_Factory* UseFactory(std::string font_file, int size);
void UnuseFactory(GLST_Factory *factory);
void RenderSurfaces(int x, int y);
