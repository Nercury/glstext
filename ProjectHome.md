GLSText 0.0.0

Uses ttf fonts to render text with OpenGL. To achieve better performance, text glyphs are automatically prerendered to textures. Supports utf8 text, uses lazy rendering for glyphs and automatically destroys font textures when font is no longer used.

Smooth blending to background, different text colors, alpha blending, alignment, text measurements, text can be rapidly changed at render time.

Initial version. Requires SDL, SDL\_ttf, OpenGL libraries.

To quickly check it out, you can look at included test project.
More details about the way this works are in GLST\_Text.h file. This file is also the main class you will need to include. It is also nice and small frontend for GLSText, and does the most obvious job if you would need to change the way rendering is performed.