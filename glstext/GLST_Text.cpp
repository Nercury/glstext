#include "GLST_Text.h"

using namespace std;

GLST_Text::GLST_Text(string font_file, int size, GLST_TextAlign align)
{
	this->text = "";
	this->factory = UseFactory(font_file, size);
	this->align = align;
	this->width = 0.0f;
	this->height = 0.0f;
	this->r = 1.0f;
	this->g = 1.0f;
	this->b = 1.0f;
	this->a = 1.0f;
}

GLST_Text::~GLST_Text(void)
{
	UnuseFactory(this->factory);
}

bool GLST_Text::CanRender(string utf8chars)
{
	return this->factory->CanRender(utf8chars);
}

void GLST_Text::SetText(string text)
{
	this->text = text;
	if (this->factory->isFontLoaded())
	{
		this->factory->GetTextGlyphs(&this->glyphs, text);
		float mx = 0;
		float offsetx = mx;
		GLST_Glyph* previous = NULL;

		this->height = 0.0f;
		
		for (vector<GLST_Glyph*>::iterator it = this->glyphs.begin(); it != this->glyphs.end(); ++it)
		{
			//cout << "Set glyph" << endl;
			GLST_Glyph *glyph = *it;
			GLST_Surface *surface = glyph->surfaceRef;
			GLuint *texture = surface->GetGLTexture();
			
			if (this->factory->font != NULL)
			{
				this->factory->MeasureAdvance(previous, glyph, &offsetx, &mx);
				if (glyph->height > this->height)
					this->height = glyph->height;
			}
		}
		this->width = mx;
	} else {
		cout << "Can't set font text - font factory is not initialized." << endl;
	}
}

string GLST_Text::GetText()
{
	return this->text;
}

void GLST_Text::Render(GLint x, GLint y)
{
	if (this->factory->isFontLoaded())
	{
		glColor4f(r, g, b, a);
		if (this->align == GLST_Middle_Center)
		{
			x -= ((GLint)this->width) / 2;
			y -= ((GLint)this->height) / 2;
		} else if (this->align == GLST_Middle_Left)
		{
			y -= ((GLint)this->height) / 2;
		} else if (this->align == GLST_Middle_Right)
		{
			x -= (GLint)this->width;
			y -= ((GLint)this->height) / 2;
		} else if (this->align == GLST_Top_Left)
		{
		} else if (this->align == GLST_Top_Right)
		{
			x -= (GLint)this->width;
		} else if (this->align == GLST_Top_Center)
		{
			x -= ((GLint)this->width) / 2;
		} else if (this->align == GLST_Bottom_Left)
		{
			y -= (GLint)this->height;
		} else if (this->align == GLST_Bottom_Right)
		{
			x -= (GLint)this->width;
			y -= (GLint)this->height;
		} else if (this->align == GLST_Bottom_Center)
		{
			x -= ((GLint)this->width) / 2;
			y -= (GLint)this->height;
		} else return;

		GLfloat mx = (GLfloat)x;
		GLfloat offsetx = (GLfloat)mx;
		GLST_Glyph* previous = NULL;

		for (vector<GLST_Glyph*>::iterator it = this->glyphs.begin(); it != this->glyphs.end(); ++it)
		{
			GLST_Glyph *glyph = *it;
			GLST_Surface *surface = glyph->surfaceRef;
			GLuint *texture = surface->GetGLTexture();

			if (texture != NULL && this->factory->font != NULL)
			{
				this->factory->MeasureAdvance(previous, glyph, &offsetx, &mx);
				previous = glyph;

				GLfloat right = offsetx + glyph->rectOnSurface.w;
				GLfloat top = (GLfloat)(y + glyph->rectOnSurface.h);

				glBindTexture(GL_TEXTURE_2D, *texture);
				
				glBegin(GL_QUADS);
					glTexCoord2f(glyph->left, glyph->bottom); glVertex2f(offsetx, top);
					glTexCoord2f(glyph->right, glyph->bottom); glVertex2f(right, top);
					glTexCoord2f(glyph->right, glyph->top); glVertex2f(right, (GLfloat)y);
					glTexCoord2f(glyph->left, glyph->top); glVertex2f(offsetx, (GLfloat)y);
				glEnd();
			}
		}
	} else {
		//cout << "Font is not initialized." << endl;
	}
}
