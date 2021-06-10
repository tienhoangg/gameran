#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <SDL2.h>
using namespace std;
#undef main
SDL_Window* window;
SDL_Event mainevent;
SDL_Renderer* renderer;
SDL_Rect filled_rect;
Mix_Chunk* chunk1;
Mix_Chunk* chunk2;
Mix_Music* music;
struct intt
{
	int x;
	int y;
};
queue<intt> wait;
int a[80][60], away = 2, colorr = 255, colorg = 255, colorb = 255, colorr2, colorg2, colorb2;
bool runing = true, p = true;
intt bait, location;
void createbait()
{
	do
	{
		bait.x = rand() % (79 - 0 + 1);
		bait.y = rand() % (59 - 0 + 1);
		if (a[bait.x][bait.y] == 0)
		{
			a[bait.x][bait.y] = 2;
			break;
		}
	} while (1);
	colorr2 = colorr;
	colorg2 = colorg;
	colorb2 = colorb;
	colorr = rand() % 255;
	colorg = rand() % 255;
	colorb = rand() % 255;
	SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
	filled_rect.x = bait.x * 10;
	filled_rect.y = bait.y * 10;
	filled_rect.w = 10;
	filled_rect.h = 10;
	SDL_RenderFillRect(renderer, &filled_rect);
}
void stomach(intt i)
{
	SDL_SetRenderDrawColor(renderer, colorr2, colorg2, colorb2, 255);
	filled_rect.x = i.x * 10;
	filled_rect.y = i.y * 10;
	filled_rect.w = 10;
	filled_rect.h = 10;
	SDL_RenderFillRect(renderer, &filled_rect);
}
void head(intt i)
{
	stomach(i);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	filled_rect.x = i.x * 10 + 3;
	filled_rect.y = i.y * 10 + 3;
	filled_rect.w = 4;
	filled_rect.h = 4;
	SDL_RenderFillRect(renderer, &filled_rect);
}
void background(intt i)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	filled_rect.x = i.x * 10;
	filled_rect.y = i.y * 10;
	filled_rect.w = 10;
	filled_rect.h = 10;
	SDL_RenderFillRect(renderer, &filled_rect);
}
void update()
{
	stomach(location);
	if (away == 1)
	{
		if (location.y == 0)
			location.y = 59;
		else
			location.y--;
	}
	if (away == 2)
	{
		if (location.x == 79)
			location.x = 0;
		else
			location.x++;
	}
	if (away == 3)
	{
		if (location.y == 59)
			location.y = 0;
		else
			location.y++;
	}
	if (away == 4)
	{
		if (location.x == 0)
			location.x = 79;
		else
			location.x--;
	}
	if (a[location.x][location.y] == 1)
	{
		Mix_PlayChannel(-1, chunk2, 0);
		Sleep(100);
		cout << "LOSER!!";
		SDL_DestroyWindow(window);
		Mix_CloseAudio();
		runing = false;
	}
	if (a[location.x][location.y] == 0)
	{
		background(wait.front());
		a[wait.front().x][wait.front().y] = 0;
		wait.pop();
	}
	if (a[location.x][location.y] == 2)
	{
		Mix_PlayChannel(-1, chunk1, 0);
		createbait();
	}
	a[location.x][location.y] = 1;
	head(location);
	wait.push(location);
}
int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("gamesnake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	chunk1 = Mix_LoadWAV("getpoint.wav");
	chunk2 = Mix_LoadWAV("changecolor.wav");
	music = Mix_LoadMUS("nhacnen.mp3");
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	filled_rect.x = 0;
	filled_rect.y = 0;
	filled_rect.w = 800;
	filled_rect.h = 600;
	SDL_RenderFillRect(renderer, &filled_rect);
	for (int i = 0; i < 80; i++)
		for (int j = 0; j < 60; j++)
			a[i][j] = 0;
	a[40][30] = a[39][30] = 1;
	location = { 40,30 };
	wait.push({ 39,30 });
	wait.push({ 40,30 });
	createbait();
	stomach({ 39,30 });
	head({ 40,30 });
	srand(time(NULL));
	SDL_RenderPresent(renderer);
	Mix_PlayMusic(music, -1);
	while (runing)
	{
		while (p)
		{
			Mix_PauseMusic();
			SDL_PollEvent(&mainevent);
			if (mainevent.key.keysym.sym == SDLK_p)
			{
				p = false;
				Mix_ResumeMusic();
			}
		}
		while (SDL_PollEvent(&mainevent))
		{
			switch (mainevent.type)
			{
			case SDL_QUIT:
			{
				SDL_DestroyWindow(window);
				Mix_CloseAudio();
				runing = false;
				break;
			}
			case SDL_KEYDOWN:
			{
				if (mainevent.key.keysym.sym == SDLK_SPACE)
					p = true;
				if (mainevent.key.keysym.sym == SDLK_UP && away != 3)
					away = 1;
				if (mainevent.key.keysym.sym == SDLK_DOWN && away != 1)
					away = 3;
				if (mainevent.key.keysym.sym == SDLK_LEFT && away != 2)
					away = 4;
				if (mainevent.key.keysym.sym == SDLK_RIGHT && away != 4)
					away = 2;
				if(mainevent.key.keysym.sym == SDLK_q)
				{
					SDL_DestroyWindow(window);
					Mix_CloseAudio();
					runing = false;
				}
				break;
			}
			}
		}
		update();
		SDL_RenderPresent(renderer);
		Sleep(100);
	}
}