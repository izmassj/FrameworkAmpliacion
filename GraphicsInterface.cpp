#include "GraphicsInterface.h"
#include "Parameters.h"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "Game.h"

GraphicsInterface::GraphicsInterface()
{
	//Abrimos ventana e inicializamos gráficos
	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer(Parameters::window_title, Parameters::width, Parameters::height, 0, &window, &renderer);

	InitFontAndGlyphs();
}


void GraphicsInterface::InitFontAndGlyphs()
{
	TTF_Init();

	font = TTF_OpenFont("nasalization-rg.ttf", Parameters::bigFontSize);

	SDL_Color color = { 255, 255, 255 };
	for (char c = '('; c <= 'z'; c++)
	{
		SDL_Surface* srfc = TTF_RenderGlyph_Blended(font, c, color);
		if (srfc)
		{
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, srfc);
			float texW = 0;
			float texH = 0;
			SDL_GetTextureSize(texture, &texW, &texH);
			glyphs.insert({ c, new Glyph{texture, texW, texH} });
			SDL_DestroySurface(srfc);
		}
	}
}

void GraphicsInterface::DestroyFontAndGlyphs()
{
	for (auto pair : glyphs)
	{
		SDL_DestroyTexture(pair.second->glyphTexture);
		delete pair.second;
	}

	TTF_CloseFont(font);
	TTF_Quit();
}

int GraphicsInterface::PrintText(std::string text, int x, int y, bColor color, EFONT_SIZE fontsize)
{
	int font_size{};
	switch (fontsize)
	{
	case EFONT_SIZE::BIG:
		font_size = Parameters::bigFontSize;
		break;
	case EFONT_SIZE::MEDIUM:
		font_size = Parameters::mediumFontSize;
		break;
	case EFONT_SIZE::SMALL:
		font_size = Parameters::smallFontSize;
		break;
	default:
		font_size = Parameters::bigFontSize;
		break;
	}
	float fontScale = (1.f * font_size) / Parameters::bigFontSize;

	SDL_FRect dstrect = { x, y, 0, 0 };
	for (int i = 0; i < text.size(); i++)
	{
		char c = text.at(i);
		auto it = glyphs.find(c);
		if (it != glyphs.end())
		{
			Glyph* g = glyphs.at(c);
			SDL_SetTextureColorMod(g->glyphTexture, color.R, color.G, color.B);
			dstrect.w = g->width * fontScale;
			dstrect.h = g->height * fontScale;
			SDL_RenderTexture(renderer, g->glyphTexture, NULL, &dstrect);
			dstrect.x += std::min((int)(g->width * fontScale), font_size);
		}
		else
			dstrect.x += font_size;
	}
	return dstrect.x;
}

GraphicsInterface::~GraphicsInterface()
{
	for (auto pair : image_collection)
		SDL_DestroyTexture(pair.second);

	DestroyFontAndGlyphs();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool GraphicsInterface::LoadImage(std::string path)
{
	SDL_Texture *txt= IMG_LoadTexture(renderer, path.c_str());

	image_collection.insert(std::make_pair(path, txt));

	return false;
}

bool GraphicsInterface::MustWindowClose()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) return true;
	}

	return false;
}

void GraphicsInterface::DrawSprite(std::string img_name, Transform transform, Vector2 size)
{
	SDL_FRect rect;
	rect.w = size.x*transform.scale.x;
	rect.h = size.y*transform.scale.y;
	rect.x = transform.position.x - rect.w * 0.5f - Game::camera.position.x + Parameters::width * 0.5f;
	rect.y = transform.position.y - rect.h * 0.5f - Game::camera.position.y + Parameters::height * 0.5f;

	SDL_RenderTextureRotated(renderer, image_collection[img_name],
		NULL, &rect,
		transform.rotation, NULL,
		SDL_FlipMode::SDL_FLIP_NONE);

}
void GraphicsInterface::DrawSpritePart(std::string img_name, Transform transform, Vector2 sourcePosition, Vector2 size, Vector2 origin)
{
	SDL_FRect srcRect;
	srcRect.x = sourcePosition.x;
	srcRect.y = sourcePosition.y;
	srcRect.w = size.x;
	srcRect.h = size.y;

	SDL_FRect dstRect;
	dstRect.w = size.x * transform.scale.x;
	dstRect.h = size.y * transform.scale.y;
	dstRect.x = transform.position.x - origin.x * transform.scale.x - Game::camera.position.x + Parameters::width * 0.5f;
	dstRect.y = transform.position.y - origin.y * transform.scale.y - Game::camera.position.y + Parameters::height * 0.5f;

	SDL_RenderTextureRotated(renderer, image_collection[img_name],
		&srcRect, &dstRect,
		transform.rotation, NULL,
		SDL_FlipMode::SDL_FLIP_NONE);
}

void GraphicsInterface::ClearScreen(unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 255); // Negro
	SDL_RenderClear(renderer);
}

void GraphicsInterface::DrawFrame()
{
	SDL_RenderPresent(renderer);
}
