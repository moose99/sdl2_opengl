#include "sdl2_util.h"

const Uint8* gCurrentKeyStates = nullptr;

constexpr unsigned int wndWidth{ 800 }, wndHeight{ 600 };

class Game
{
private:
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	TTF_Font *font15 = nullptr;
	SDL_Texture *textLives = nullptr;

public:
	Game()	{ }

	// returns -1 on error
	int init()
	{
		// init SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			logSDLError(std::cout, "SDL_Init");
			return -1;
		}

		// init TTF system
		if (TTF_Init() != 0)
		{
			logSDLError(std::cout, "TTF_Init");
			SDL_Quit();
			return -1;
		}

		// Create window
		window = SDL_CreateWindow("SDL2_OPENGL", 100, 100, wndWidth, wndHeight, SDL_WINDOW_SHOWN);
		if (window == nullptr)
		{
			logSDLError(std::cout, "CreateWindow");
			SDL_Quit();
			return -1;
		}

		// create renderer
		renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == nullptr)
		{
			logSDLError(std::cout, "CreateRenderer");
			SDL_DestroyWindow(window);
			SDL_Quit();
			return -1;
		}
		if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0)
		{
			logSDLError(std::cout, "SetRenderDrawBlendMode");
			SDL_DestroyWindow(window);
			SDL_Quit();
			return -1;
		}

		//Open the font
		font15 = TTF_OpenFont("assets/calibri.ttf", 15);
		if (font15 == nullptr)
		{
			logSDLError(std::cout, "TTF_OpenFont");
			return -1;
		}

		SDL_Color white = { 255, 255, 255, 255 };
		textLives = createText("Lives: 3", font15, white, renderer);
		if (textLives == nullptr)
		{
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			SDL_Quit();
			return -1;
		}

		return 0;	// ok
	}

	void run()
	{
		SDL_Color white = { 255, 255, 255, 255 };
		SDL_Event e;
		bool quit = false;
		while (!quit)
		{
			while (SDL_PollEvent(&e))
			{
				//If user closes the window, presses escape
				if (e.type == SDL_QUIT ||
					(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				{
					quit = true;
				}
			}

			// key state
			gCurrentKeyStates = SDL_GetKeyboardState(NULL);

			/* Select the color for drawing. */
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			//First clear the renderer with the draw color
			SDL_RenderClear(renderer);

			if (gCurrentKeyStates[SDL_SCANCODE_ESCAPE])
				break;

//			manager.update();
//			manager.draw(renderer);

			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			SDL_Rect rect = { 200,200,	// x, y
				100,100 };				// w, h
			SDL_RenderFillRect(renderer, &rect);

			renderTexture(textLives, renderer, 10, 10);

			//Update the screen
			SDL_RenderPresent(renderer);
		}


		// cleanup
		TTF_CloseFont(font15);
		SDL_DestroyTexture(textLives);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

};

int main(int, char **)
{
	Game game;
	if (game.init() < 0)
	{
		return -1;
	}
	game.run();
	return 0;
}

