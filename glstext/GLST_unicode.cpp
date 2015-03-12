#include "GLST_unicode.h"

using namespace std;

int utf8len(std::string & value)
{
	vector<int> utf32result;
	utf8::utf8to32(value.begin(), value.end(), back_inserter(utf32result)); 
	return (int)utf32result.size();
}

void DoBackspaceForString(string *value)
{
	// sukeliam simbolius i sarasa kaip integer
	vector<int> utf32result;
	utf8::utf8to32(value->begin(), value->end(), back_inserter(utf32result)); 

	*value = "";
	int count = utf32result.size();
	int i = 0;
	// einam per sarasa
	for (vector<int>::iterator it = utf32result.begin(); it != utf32result.end(); ++it)
	{
		if (i >= count - 1)
			break;
		// paverciam kiekviena simboli atgal i string
		utf8::append(*it, back_inserter(*value));
		i++;
	}
}

string GetStringFromUnicode(Uint16 code)
{
	try
	{
		bool valid = false;
		string temp;
		utf8::append(code, back_inserter(temp));
		string::iterator itval = utf8::find_invalid(temp.begin(), temp.end());
		if (itval == temp.end())
			valid = true;
		if (valid)
			return temp;
		return string("");
	} catch (...) {
		return string("");
	}
}
SDLKey numpadvals[] = { 
	SDLK_KP_PERIOD,
	SDLK_KP0,	SDLK_KP1,	SDLK_KP2,	SDLK_KP3,	SDLK_KP4,	SDLK_KP5,	SDLK_KP6,	SDLK_KP7,
	SDLK_KP8,	SDLK_KP9,
};

SDLKey cvals[] = { 
	SDLK_0,	SDLK_1,	SDLK_2,	SDLK_3,	SDLK_4,	SDLK_5,	SDLK_6,	SDLK_7,	SDLK_8,	SDLK_9,	SDLK_q,	SDLK_w,	SDLK_e,
	SDLK_r,	SDLK_t,	SDLK_y,	SDLK_u,	SDLK_i,	SDLK_o,	SDLK_p,	SDLK_a,	SDLK_s,	SDLK_d,	SDLK_f,	SDLK_g,	SDLK_h,
	SDLK_j,	SDLK_k,	SDLK_l,	SDLK_z,	SDLK_x,	SDLK_c,	SDLK_v,	SDLK_b,	SDLK_n,	SDLK_m,	SDLK_LEFTBRACKET,
	SDLK_RIGHTBRACKET,	SDLK_LEFTPAREN,	SDLK_RIGHTPAREN,	SDLK_SEMICOLON,	SDLK_QUOTE,	SDLK_QUOTEDBL,
	SDLK_ASTERISK,	SDLK_AMPERSAND,	SDLK_LESS,	SDLK_UNDERSCORE,	SDLK_SPACE,	SDLK_SLASH,	SDLK_QUESTION,
	SDLK_PLUS,	SDLK_PERIOD,	SDLK_MINUS,	SDLK_EQUALS,	SDLK_COMMA,	SDLK_COLON,	SDLK_SEMICOLON,
	SDLK_KP_PLUS,	SDLK_KP_DIVIDE,	SDLK_KP_MINUS,	SDLK_KP_EQUALS,	SDLK_KP_MULTIPLY,
};

bool IsSDLKeysymValidSymbol(SDL_keysym *keysym)
{
	bool valid = false;
	int i;
	if (!((keysym->mod & KMOD_ALT) || (keysym->mod & KMOD_CTRL) || (keysym->mod & KMOD_META)))
	{
		for (i = 0; i < (sizeof(cvals)/sizeof(SDLKey)); i++)
		{
			if (cvals[i] == keysym->sym)
			{
				valid = true;
				break;
			}
		}
		if (keysym->mod & KMOD_NUM)
		{
			for (i = 0; i < (sizeof(numpadvals)/sizeof(SDLKey)); i++)
			{
				if (numpadvals[i] == keysym->sym)
				{
					valid = true;
					break;
				}
			}			
		}
	}
	return valid;
}
