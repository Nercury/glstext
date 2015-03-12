#include "GLST_Factory.h"

using namespace std;

GLST_Factory::GLST_Factory(string font_file, int size)
{
	this->surface = NULL;
	this->use_count = 0;
	this->font_file = font_file;
	this->size = size;
	if(!(this->font = TTF_OpenFont(font_file.c_str(), size))) {
		cout << "Error loading font: " << TTF_GetError() << endl;
	}
}

GLST_Factory::~GLST_Factory(void)
{
	for (map<string, GLST_Glyph*>::iterator it = this->glyph_map.begin(); it != this->glyph_map.end(); ++it)
	{
		delete it->second;
	}
	this->glyph_map.clear();
	for (vector<GLST_Surface*>::iterator it = this->surfaces.begin(); it != this->surfaces.end(); ++it)
	{
		delete (*it);
	}
	this->surfaces.clear();
	if (this->font != NULL)
	{
		TTF_CloseFont(this->font);
	}
}

void GLST_Factory::MeasureAdvance(GLST_Glyph *first, GLST_Glyph *second, float *char_offset, float *advance)
{
	if (second == NULL)
		return;
	float tw = second->rectOnSurface.w;
	float width = tw;
	if (width > second->t_maxx - second->t_minx)
	{
		width = second->t_maxx - second->t_minx;
	}
	if (first != NULL)
	{
		int kerning = 0;
		/* if used sdl_ttf with posibility to get kerning info */
		/*TTF_Kerning_64(this->font, first->char_code, second->char_code, &kerning);
		*advance += (float)kerning;*/
	} else if (second->t_minx < 0) {
		*advance -= second->t_minx;
	}
	if (second->t_minx < 0)
	{
		*char_offset = *advance + second->t_minx;
	} else {
		*char_offset = *advance;
	}
	*advance += second->t_advance;
}

float GLST_Factory::MeasureWidth(vector<GLST_Glyph*> *glyphs, vector<GLST_Glyph*>::size_type start, vector<GLST_Glyph*>::size_type end)
{
	if (glyphs->size() == 0 || start < 0 || end < 0 || start > end || start >= glyphs->size() || end > glyphs->size())
		return 0.0f;
	GLST_Glyph *previous;
	if (start == 0)
	{
		previous = NULL;
	} else {
		previous = glyphs->at(start - 1);
	}

	float measured_width = 0.0f;
	float offset = measured_width;

	for (vector<GLST_Glyph*>::size_type i = start; i < end; i++)
	{
		GLST_Glyph *current = glyphs->at(i);
		
		this->MeasureAdvance(previous, current, &offset, &measured_width);

		previous = current;
	}

	return measured_width;
}

GLST_Surface *GLST_Factory::MakeSurface()
{
	if (this->size > 50)
	{
		return new GLST_Surface(512, 512, this->font);
	} 
	else if (this->size > 90)
	{
		return new GLST_Surface(1024, 1024, this->font);
	}
	else
	{
		return new GLST_Surface(256, 256, this->font);
	}
}

bool GLST_Factory::CanRender(string utf8chars)
{
	SDL_Surface *initial;
	SDL_Color c;
	c.r = 255;
	c.g = 255;
	c.b = 255;
	initial = TTF_RenderUTF8_Solid(this->font, utf8chars.c_str(), c);
	if (initial == NULL)
	{
		return false;
	} else {
		SDL_FreeSurface(initial);
		return true;
	}
}

/*
Sukuria nauja Glyph objekta pagal nurodyta teksta
*/
GLST_Glyph *GLST_Factory::MakeGlyph(string utf_char, int char_code)
{
	if (this->surface == NULL)
	{
		this->surface = this->MakeSurface();
		this->surfaces.push_back(this->surface);
	}
	SDL_Rect rect;
	
	if (this->surface->GreateGlyph(utf_char, &rect))
	{
		GLST_Glyph *glyph = new GLST_Glyph(rect, this->surface, this->font, char_code);
		return glyph;
	} else {
		this->surface = this->MakeSurface();
		this->surfaces.push_back(this->surface);
		if (this->surface->GreateGlyph(utf_char, &rect))
		{
			GLST_Glyph *glyph = new GLST_Glyph(rect, this->surface, this->font, char_code);
			return glyph;
		}
	}
	return NULL;
}

/*
Gauna nurodyta Glyph pagal simbolio teksta
*/
GLST_Glyph *GLST_Factory::GetGlyph(string utf_char, int char_code)
{
	map<string, GLST_Glyph*>::iterator glyph_item;
	glyph_item = this->glyph_map.find(utf_char);
	GLST_Glyph *glyph = NULL;
	if (glyph_item == this->glyph_map.end())
	{
		glyph = this->MakeGlyph(utf_char, char_code);
		if (glyph != NULL)
		{
			this->glyph_map[utf_char] = glyph;
		}
	} else {
		glyph = glyph_item->second;
	}
	return glyph;
}

/*
Uzpildo simboliu masyva Glyph objektais pagal duota teksta
*/
void GLST_Factory::GetTextGlyphs(vector<GLST_Glyph*> *glyphs, string text)
{
	glyphs->clear();

	// sukeliam simbolius i sarasa kaip integer
	vector<int> utf32result;
	utf8::utf8to32(text.begin(), text.end(), back_inserter(utf32result)); 

	// einam per sarasa
	for (vector<int>::iterator it = utf32result.begin(); it != utf32result.end(); ++it)
	{
		// paverciam kiekviena simboli atgal i string
		string res;
		utf8::append(*it, back_inserter(res));
		GLST_Glyph *glyph = NULL;
		try
		{
			glyph = this->GetGlyph(res, *it);
		}
		catch (...) {}
		if (glyph != NULL)
		{
			glyphs->push_back(glyph);
		}
	}
}

bool GLST_Factory::isFontLoaded()
{
	return this->font != NULL;
}

string GLST_Factory::getFontFile() const
{
	return this->font_file;
}

int GLST_Factory::getFontSize() 
{
	return this->size;
}

void GLST_Factory::SetSceneStart()
{
	for (vector<GLST_Surface*>::iterator it = this->surfaces.begin(); it != this->surfaces.end(); ++it)
	{
		(*it)->SetSceneStart();
	}
}

void GLST_Factory::RenderSurfaces(int x, int y)
{
	int dx = x;
	for (vector<GLST_Surface*>::iterator it = this->surfaces.begin(); it != this->surfaces.end(); ++it)
	{
		(*it)->RenderSurface(dx, y);
		dx += 128;
	}
}

void GLST_Factory::Refresh()
{
	//cout << "Refresh ???" << endl;
	for (vector<GLST_Surface*>::iterator it = this->surfaces.begin(); it != this->surfaces.end(); ++it)
	{
		(*it)->Refresh();
	}
}

map<const GLST_Factory*, string> glst_factory_strings;
map<const string, GLST_Factory*> glst_factories;

GLST_Factory* UseFactory(string font_file, int size)
{
	// convert size to string
	std::stringstream sout;
	sout << size;
	
	// create factory key which represents factory based on file and font size
	string factory_key = font_file + sout.str();

	//cout << "Fact. key: " << factory_key << endl;

	GLST_Factory *f = NULL;

	map<const string, GLST_Factory*>::iterator cur;
	// find if such factory already exists
	cur = glst_factories.find(factory_key);
	if (cur == glst_factories.end()) // if it does not
	{
		// create new factory
		f = new GLST_Factory(font_file, size);
		// and put it into map which is based on key
		glst_factories[factory_key] = f;
		// also add it to factory map, which is basically the list of all factories used
		glst_factory_strings[f] = factory_key;
	} else {
		// if factory exists, return it
		f = cur->second;
	}
	// and increase use count
	f->use_count++;
	//cout << "Factory " << f->getFontFile() << " used " << f->use_count << " time(s)." << endl;
	return f;
}

void UnuseFactory(GLST_Factory *factory)
{
	if (factory != NULL)
	{
		// decrease factory use count
		factory->use_count--;

		//cout << "Factory " << factory->getFontFile() << " used " << factory->use_count << " time(s)." << endl;

		// if use count reached zero
		if (factory->use_count == 0)
		{
			map<const GLST_Factory*, string>::iterator str_item;

			// find factory in map
			str_item = glst_factory_strings.find(factory);

			if (str_item != glst_factory_strings.end()) // if it exists
			{
				map<const string, GLST_Factory*>::iterator cur;
				cur = glst_factories.find(str_item->second); // find item in first map based on key
				if (cur != glst_factories.end()) // if it exists
				{
					glst_factories.erase(cur); // erase it from key->factory map
				}
				glst_factory_strings.erase(str_item); // and erase from factory -> key map
			}
			//cout << "Destroy factory " << factory->getFontFile() << "." << endl;
			delete factory;
		}
	}
}

void GLST_SetSceneStart()
{
	for (map<const string, GLST_Factory*>::iterator it = glst_factories.begin(); it != glst_factories.end(); ++it)
	{
		it->second->SetSceneStart();
	}
}

void GLST_Refresh()
{
	//cout << "Refresh ??" << endl;
	for (map<const string, GLST_Factory*>::iterator it = glst_factories.begin(); it != glst_factories.end(); ++it)
	{
		it->second->Refresh();
	}
}

void RenderSurfaces(int x, int y)
{
	int dy = y;
	for (map<const string, GLST_Factory*>::iterator it = glst_factories.begin(); it != glst_factories.end(); ++it)
	{
		it->second->RenderSurfaces(x, dy);
		dy += 128;
	}
}
