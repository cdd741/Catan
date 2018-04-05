#include "graphics.hpp"

#include "board.hpp"

#ifdef USING_SDL2

using namespace GRAPHICS;

void GRAPHICS::start_threaded_sdl2(SharedContext * ctx)
{
	auto hWnd = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		640,                               // width, in pixels
		480,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);

	SDL_Event evt;
	SDL_Renderer* renderer = SDL_CreateRenderer(hWnd, -1, SDL_RENDERER_ACCELERATED);


	bool running = true;
	bool handling = false;
	Board * b = nullptr;
	while (running)
	{
		//Handle events on queue
		while (SDL_PollEvent(&evt) != 0)
		{
			//User requests quit
			if (evt.type == SDL_QUIT)
			{
				running = false;
			}
			else if (evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_LEFT)
			{
				// left button up
				// clicked
			}
		}

		Event s_evt;
		while ((s_evt = ctx->poll()).typ != Event::NONE)
		{
			// handle not none events
			switch (s_evt.typ)
			{
			case Event::QUIT:
				running = false;
				break;
			case Event::BOARD_ASSIGN:
				b = (Board*)s_evt.p;
				break;

			}
		}



		//Clear screen
		SDL_RenderClear(renderer);

		//Update screen
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(hWnd);
}

#endif
