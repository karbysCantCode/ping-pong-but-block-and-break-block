#pragma once
#include <iostream>
#include <filesystem>
#include <random>

#include "SDL.h"
#include "SDL_image.h"

#include "karbusComplexShapes.h"
#include "karbusFonts.h"
#include "KarbusVector2.h"
#include "KBalls.h"
#include "cellManager.h"

void renderTPS(SDL_Renderer* renderer, KBF_Font& font, int diff, int x, int y) {
	std::string stringOut = "TPS:" + std::to_string(diff);
	font.KBF_RenderString(renderer, stringOut, x, y, 3);
}

void renderGameData(SDL_Renderer* renderer, KBF_Font& font, int x, int y) {

}

int updateFrame(SDL_Renderer* renderer, balls& ballManager, cells& cellManager,KBF_Font& font, int screenX, int screenY) {

	cellManager.renderCells(renderer, font);
	ballManager.processBalls(renderer, cellManager);
	return 0;
}

int startGame(int screenX, int screenY, int gameX, int gameY, int secondsBetweenGenerates, int secondsBetweenExpands) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Karbus' Block Breaking Game!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenX, screenY, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		holdForError("no window");
		return -1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr) {
		holdForError("no renderer");
		return -1;
	}

	// create objects for game
	SDL_Color fontColor = { 0,0,255,255 };
	KBF_Font defaultFont(renderer, fontColor, "default");

	balls ballManager(renderer, gameX,gameY);

	cells cellManager(renderer,100, gameX, gameY);

	//for testing
	int totalBalls = 0;
	std::cout << '\n' << "how many balls do you want: ";
	std::cin >> totalBalls;
	for (int i = 0; i < totalBalls; ++i) {
		ballManager.createBall();
	}


	bool quit = false;
	SDL_Event evt;

	const int ticksPerSecond = 120;

	int ticksSinceLastGenerate = 0;
	const int ticksBetweenGenerates = secondsBetweenGenerates * ticksPerSecond;

	int ticksSinceLastExpand = 0;
	const int ticksBetweenExpand = secondsBetweenExpands * ticksPerSecond;

	const int tickDuration = 1000 / ticksPerSecond;
	uint64_t lastTick = SDL_GetTicks64();

	while (!quit) { // game loop
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		uint64_t currentTick = SDL_GetTicks64();
		if (currentTick >= (lastTick + tickDuration)) {
			ticksSinceLastGenerate++;
			ticksSinceLastExpand++;
			int diff = (1000 / (int)(currentTick - lastTick));
			lastTick = currentTick;
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);

			if (ticksSinceLastGenerate >= ticksBetweenGenerates) {
				ticksSinceLastGenerate = 0;
				cellManager.generateRow();
			}
			if (ticksSinceLastExpand >= ticksBetweenExpand) {
				ticksSinceLastExpand = 0;
				cellManager.expandTotalCells();
			}
			
			renderTPS(renderer, defaultFont, diff, screenX - 100, 5);

			updateFrame(renderer, ballManager, cellManager, defaultFont, gameX, gameY);

			SDL_RenderPresent(renderer);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}