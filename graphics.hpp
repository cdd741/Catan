#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <thread>
#include <queue>
#include <string>
#include <shared_mutex>

namespace GRAPHICS
{
#ifdef USING_SDL2

#define SDL_MAIN_HANDLED 
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef _DEBUG
#pragma comment(lib, "SDL2maind.lib")
#pragma comment(lib, "SDL2d.lib")
#else
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2.lib")
#endif

	struct Event
	{
		enum Type
		{
			NONE,
			QUIT,
			BOARD_ASSIGN
		};

		Type typ;
		std::string msg;
		void * p = nullptr;
		Event(Type typ = NONE) : typ{ typ } {}
	};

	class SharedContext
	{
		std::queue<Event> q, r;
		std::mutex q_mtx, r_mtx;

	public:
		SharedContext()
		{
			
		}

		virtual ~SharedContext()
		{

		}

		void push(Event& evt)
		{
			q_mtx.lock();
			q.push(evt);
			q_mtx.unlock();
		}

		Event poll()
		{
			q_mtx.lock();
			Event e;
			if (q.size() != 0)
			{
				e = q.front();
				q.pop();
			}
			q_mtx.unlock();
			return e;
		}

		void push_r(Event& evt)
		{
			r_mtx.lock();
			r.push(evt);
			r_mtx.unlock();
		}

		Event poll_r()
		{
			r_mtx.lock();
			Event e;
			if (r.size() != 0)
			{
				e = r.front();
				r.pop();
			}
			r_mtx.unlock();
			return e;
		}
		
	};

	void start_threaded_sdl2(SharedContext * ctx);

#endif
};



#endif
