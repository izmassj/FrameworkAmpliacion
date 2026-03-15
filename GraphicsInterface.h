#pragma once

#include <iostream>
#include <map>
#include "Transform.h"

struct bColor
{
	unsigned char R, G, B, A;
};

struct fColor
{
	float r, g, b, a;
};

enum class EFONT_SIZE
{
	SMALL, MEDIUM, BIG
};

class GraphicsInterface
{
	class SDL_Window* window;
	class SDL_Renderer* renderer;

	std::map<std::string, class SDL_Texture*> image_collection;

	struct Glyph
	{
		class SDL_Texture* glyphTexture;
		float width, height;
	};
	std::map<char, Glyph*> glyphs;
	class TTF_Font* font = NULL;


	void InitFontAndGlyphs();
	void DestroyFontAndGlyphs();

public:

	GraphicsInterface();
	~GraphicsInterface();

	bool LoadImage(std::string path);

	bool MustWindowClose();

	void DrawSprite(std::string img_name, Transform transform, Vector2 size);
	void DrawSpritePart(std::string img_name, Transform transform, Vector2 sourcePosition, Vector2 size, Vector2 origin);

	void DrawFrame();

	void ClearScreen(unsigned char r, unsigned char g, unsigned char b);

	int PrintText(std::string text, int x, int y, bColor color, EFONT_SIZE fontsize);
};
