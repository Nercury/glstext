#pragma once

#include "GLST_std.h"
#include "GLST_gl.h"
#include "GLST_unicode.h"
#include <SDL/SDL_ttf.h>
#include "GLST_Factory.h"
#include "GLST_Glyph.h"

/*

Text object, draws single line of text, aligned to point.

*/

/* text alignment options
 */
enum GLST_TextAlign
{
	GLST_Middle_Center,
	GLST_Middle_Left,
	GLST_Middle_Right,
	GLST_Top_Left,
	GLST_Top_Right,
	GLST_Top_Center,
	GLST_Bottom_Left,
	GLST_Bottom_Right,
	GLST_Bottom_Center,
};

/*
 * This is main GLST object, responsible for text rendering.
 * 
 * When this object is created, it checks if GLST_Factory already
 * exists for this font. If it does, it assigns existing GLST_Factory,
 * otherwise, new GLST_Factory is created.
 * 
 * When text is assigned, glyph list is loaded from 
 * GLST_Factory. Every glyph is actually a rectangle on certain
 * OpenGL texture, managed by GLST_Surface. Factory keeps as many
 * surfaces as needed for it's font, and these surfaces are destroyed
 * when last GLST_Text instance using that font is destroyed.
 * 
 * So for every symbol there exist only single GLST_Glyph instance,
 * which is shared between GLST_Text objects. Therefore GLST_Text
 * itself is quite fast, because it basically loads existing glyphs
 * into a list (when text is changed), and renders that list 
 * sequentially. If some unfortunate glyph does not exist on texture
 * yet, GLST_Factory will take care of that by rendering and updating
 * textures as necessary.
 * 
 * Few requirements when using this:
 * 
 * 1. on window resize, call GLST_Refresh();
 * 2. before drawing any text in scene, call GLST_SetSceneStart();
 */
class GLST_Text
{
private:
	/* assigned text */
	std::string text;
	/* font factory which provides glyph textures */
	GLST_Factory *factory;
	/* glyphs for current text */
	std::vector<GLST_Glyph*> glyphs;
	/* current text width */
	float width;
	/* current text height */
	float height;
	/* color */
	float r;
	float g;
	float b;
	float a;
	/* text alignment */
	GLST_TextAlign align;
public:
	/* set text. this initializes glyph textures and creates glyph
	 * list ready for action */
	void SetText(std::string text);
	/* change text color */
	inline void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { this->r = r; this->g = g; this->b = b; this->a = a; };
	/* change text color, including alpha */
	inline void SetColor(GLfloat r, GLfloat g, GLfloat b) { this->r = r; this->g = g; this->b = b; this->a = 1.0f; };
	/* get currently assigned text */
	inline void SetAlpha(GLfloat a) { this->a = a; };
	std::string GetText();
	/* checks if it is possible to render string as utf8 (valid utf8) */
	bool CanRender(std::string utf8chars);
	/* render at position */
	void Render(GLint x, GLint y);
	/* get assigned text height */
	inline float Height() { return this->height; };
	/* get assigned text width */
	inline float Width() { return this->width; };
	GLST_Text(std::string font_file, int size, GLST_TextAlign align);
	~GLST_Text(void);
};
