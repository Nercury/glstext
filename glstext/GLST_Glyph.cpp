#include "GLST_Glyph.h"

using namespace std;

GLST_Glyph::GLST_Glyph(const SDL_Rect rect, GLST_Surface *surface, TTF_Font *font, Uint16 char_code)
{
	this->rectOnSurface = rect;
	this->surfaceRef = surface;
	this->left = rect.x / (float)surface->width;
	this->top = rect.y / (float)surface->height;
	this->right = (rect.x + rect.w) / (float)surface->width;
	this->bottom = (rect.y + rect.h) / (float)surface->height;
	this->char_code = char_code;
	this->height = (float)rect.h;
	int minx;
	int maxx;
	int miny;
	int maxy;
	int advance;
	TTF_GlyphMetrics(font, char_code, &minx, &maxx, &miny, &maxy, &advance);
	//cout << "w:" << rect.w << " h:" << rect.h << endl;
	//cout << "minx:" << minx << " maxx:" << maxx << " miny:" << miny << " maxy:" << maxy << " adv:" << advance << endl;
	this->t_minx = (float)minx;
	this->t_maxx =  (float)maxx;
	this->t_advance = (float)advance;
}

GLST_Glyph::~GLST_Glyph(void)
{
	this->surfaceRef = NULL;
}
