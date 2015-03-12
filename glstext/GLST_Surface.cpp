#include "GLST_Surface.h"

using namespace std;

GLST_Surface::GLST_Surface(int width, int height, TTF_Font *font)
{
	this->symbol_left = 0;
	this->symbol_top = 0;
	this->symbol_bottom = 0;

	this->modified = false;
	this->assigned = false;
	this->texture_ok = false;

	this->width = width;
	this->height = height;
	this->font = font;
	/*this->surface = SDL_CreateRGBSurface(0, width, height, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);*/
	this->surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
	              width, height, 32,
                  0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000);
	//SDL_SetAlpha(this->surface, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);

	//SDL_SetAlpha(this->surface, SDL_SRCALPHA, 0);
}

void GLST_Surface::Refresh()
{
	//cout << "Refresh!" << endl;
	this->modified = true;
}

void GLST_Surface::RenderSurface(int x, int y)
{
	GLuint *texture = this->GetGLTexture();

	if (texture != NULL)
	{
		/* prepare to render our texture */
		glBindTexture(GL_TEXTURE_2D, *texture);

		/* Draw a quad at location */
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f((float)x, (float)y);
			glTexCoord2f(1.0f, 1.0f); glVertex2f((float)(x+128), (float)y);
			glTexCoord2f(1.0f, 0.0f); glVertex2f((float)(x+128), (float)(y+128));
			glTexCoord2f(0.0f, 0.0f); glVertex2f((float)x, (float)(y+128));
		glEnd();
	}
}

void GLST_Surface::SetSceneStart()
{
	if (this->modified)
	{
		if (this->assigned)
		{
			this->RenderTexture();
		}
	}
	this->texture_ok = glIsTexture(this->texture);
}

bool GLST_Surface::FindPlaceForGlyph(SDL_Rect* rect)
{
	if (rect->w > (this->width - this->symbol_left))
	{
		this->symbol_top = this->symbol_bottom;
		this->symbol_left = 0;
		if (this->symbol_top >= this->height)
		{
			return false;
		}
	}
	if (rect->h <= (this->height - this->symbol_top))
	{
		int new_bottom = this->symbol_top + rect->h;
		if (new_bottom > this->symbol_bottom)
		{
			this->symbol_bottom = new_bottom;
		}
		rect->x = this->symbol_left;
		rect->y = this->symbol_top;
		this->symbol_left += rect->w;
		this->modified = true;
		return true;
	}
	else
	{
		return false;
	}
}

void GLST_Surface::RenderTexture()
{
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*for (int i = 3; i < this->width * this->height; i+=4)
	{
		Uint8 r = ((Uint8*)this->surface->pixels)[i-1];
		Uint8 g = ((Uint8*)this->surface->pixels)[i-2];
		Uint8 b = ((Uint8*)this->surface->pixels)[i-3];
		//Uint8 alpha = 255-r;
		Uint8 alpha = ((Uint8*)this->surface->pixels)[i];
		//((Uint8*)this->surface->pixels)[i] = alpha;
		if (alpha > 0)
			cout << "Alpha found: " << alpha << endl;
	}*/

	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_BGRA_EXT, 
		GL_UNSIGNED_BYTE, this->surface->pixels );*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_BGRA_EXT, 
		GL_UNSIGNED_BYTE, this->surface->pixels );

	//cout << "Texture " << this->texture << " for font " << this->font << " rendered." << endl;

	this->modified = false;
}

GLuint *GLST_Surface::GetGLTexture()
{
	if (!this->texture_ok)
	{
		if (!this->assigned || this->modified)
		{
			glGenTextures(1, &this->texture);
			//cout << "Texture " << this->texture << " for font " << this->font << " created." << endl;

			this->RenderTexture();

			this->assigned = true;
		}
		return NULL;
	} else {
		return &this->texture;
	}
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

bool GLST_Surface::GreateGlyph(string utf8char, SDL_Rect* rect)
{
	SDL_Surface *initial;

	SDL_Color GlyphColor = {255, 255, 255};

	initial = TTF_RenderUTF8_Blended(this->font, utf8char.c_str(), GlyphColor);

	if (initial != NULL)
	{
		rect->h = initial->h;
		rect->w = initial->w;

		if (this->FindPlaceForGlyph(rect))
		{
			SDL_LockSurface(this->surface);

			int dx, dy;
			for (int iy = 0; iy < initial->h; iy++)
			{
				for (int ix = 0; ix < initial->w; ix++)
				{
					dx = ix + rect->x;
					dy = iy + rect->y;
					putpixel(this->surface, dx, dy, getpixel(initial, ix, iy));
				}
			}

			SDL_UnlockSurface(this->surface);
			//SDL_BlitSurface(initial, 0, this->surface, rect);
			SDL_FreeSurface(initial);
			return true;
		} 
		else 
		{
			SDL_FreeSurface(initial);
			return false;
		}
	} else {
		throw 55;
	}
}

GLST_Surface::~GLST_Surface(void)
{
	SDL_FreeSurface(this->surface);
	if (this->assigned)
	{
		glFinish();
		glDeleteTextures(1, &this->texture);
		//cout << "Texture for font " << this->font << " destroyed." << endl;
	}
}
