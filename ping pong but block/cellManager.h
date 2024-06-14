#pragma once
#include <iostream>
#include <vector>

#include "SDL.h"

#include "karbusComplexShapes.h"
#include "MathOperations.h"

// chances of types: % of 100
const int CELL_NONE_CHANCE = 50;
const int CELL_NORMAL_CHANCE = 30;
const int CELL_SPEED_CHANCE = 5;
const int CELL_DAMAGE_CHANCE = 5;
const int CELL_AMOUNT_CHANCE = 5;
const int CELL_SPECIAL_CHANCE = 5;

enum CellType {
	CELL_NONE,
	CELL_NORMAL,
	CELL_SPEED,
	CELL_DAMAGE,
	CELL_AMOUNT,
	CELL_SPECIAL
};

struct singleCell {
	int cellX = 0;
	int cellY = 0;
	int cellHealth = 0;
	CellType cellType = CELL_NONE;
};

class cells {
private:

	int averageHealth = 1;
	int scrnX;
	int scrnY;

	SDL_Texture* cellTexture = nullptr;

	CellType generateCellType() {
		int randInt = randomInt(0, 100);
		int cumulativeChance = 0;
		cumulativeChance += CELL_NONE_CHANCE;
		if (randInt <= cumulativeChance) return CELL_NONE;

		cumulativeChance += CELL_NORMAL_CHANCE;
		if (randInt <= cumulativeChance) return CELL_NORMAL;

		cumulativeChance += CELL_SPEED_CHANCE;
		if (randInt <= cumulativeChance) return CELL_SPEED;

		cumulativeChance += CELL_DAMAGE_CHANCE;
		if (randInt <= cumulativeChance) return CELL_DAMAGE;

		cumulativeChance += CELL_AMOUNT_CHANCE;
		if (randInt <= cumulativeChance) return CELL_AMOUNT;

		cumulativeChance += CELL_SPECIAL_CHANCE;
		if (randInt <= cumulativeChance) return CELL_SPECIAL;

		// Default case (should not be reached with correct input probabilities)
		return CELL_NONE;
	}
public:
	std::vector<singleCell> allCells;

	int baseHealth = 0;
	int cellsAlongX = 9;
	int cellsAlongY = 12;

	int singleCellX;
	int singleCellY;

	cells(SDL_Renderer* renderer, int health, int screenX, int screenY) {
		baseHealth = health;
		scrnX = screenX;
		scrnY = screenY;

		singleCellX = (scrnX / cellsAlongX) + 1;
		singleCellY = (scrnY / cellsAlongY) + 1;

		buildCellTexture(renderer);

		allCells.resize(cellsAlongX * cellsAlongY);
	}

	void buildCellTexture(SDL_Renderer* renderer) {
		SDL_Surface* surface = SDL_CreateRGBSurface(0, singleCellX, singleCellY, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

		KCS_Color mainColor(255, 255, 255, 255);
		KCS_Color borderColor(0, 0, 0, 255);
		KCS_SurfaceRectInsetBorder(surface, 0, 0, singleCellX, singleCellY, 5, borderColor, mainColor);

		cellTexture = SDL_CreateTextureFromSurface(renderer, surface);
	}

	/* creates another row and col on the left and top,
	run before creating more cells
	*/
	void expandTotalCells() {
		averageHealth = (averageHealth + 1) * 2;
		cellsAlongX += 1;
		cellsAlongY += 1;

		singleCellX = (scrnX / cellsAlongX) + 1;
		singleCellY = (scrnY / cellsAlongY) + 1;

		std::vector<singleCell> newCells(cellsAlongX * cellsAlongY);

		for (int y = 0; y < cellsAlongY-1; ++y) {
			for (int x = 0; x < cellsAlongX-1; ++x) {
				newCells[(y+1) * cellsAlongX + (x + 1)] = allCells[y * (cellsAlongX-1) + x];
			}
		}

		allCells.resize(cellsAlongX * cellsAlongY);
		allCells = std::move(newCells);
	}

	bool createCell(int x, int y, int health, CellType m_cellType) { // third value set the health of the brick based on current avg or whatever
		singleCell newCell = { x,y,health,m_cellType };
		allCells[y * cellsAlongY + x] = newCell;
		return 1;
	}
	/*1 = lose
	  0 = sucess
	*/
	int generateRow() {
		for (int x = 0; x < cellsAlongX; ++x) {
			//std::cout << x << '\n';
			if (allCells[(cellsAlongY - 1) * cellsAlongX + x].cellHealth != 0) {
				if (baseHealth <= 0) return 1;
				baseHealth--;
			}
		}

		for (int y = cellsAlongY - 2; y >= 0; --y) {
			for (int x = cellsAlongX - 1; x >= 0; --x) {
				allCells[(y + 1) * cellsAlongX + x] = allCells[y * cellsAlongX + x];
			}
		}

		for (int x = 0; x < cellsAlongX; ++x) {
			CellType cellType = generateCellType();
			if (cellType == CELL_NONE) {
				createCell(x, 0, 0, cellType);
			}
			else {
				int health = averageHealth + randomInt(averageHealth - (averageHealth / 3), averageHealth + (averageHealth / 3));
				createCell(x, 0, health, cellType);
			}
		}

		return 0;
	}

	bool renderCells(SDL_Renderer* renderer, KBF_Font &font) {
		for (int i = 0; i < allCells.size(); ++i) {
			if (allCells[i].cellType == CELL_NONE) continue;
			SDL_Rect dstRect = { (i % cellsAlongX) * singleCellX, (i / cellsAlongX) * singleCellY, singleCellX, singleCellY };
			SDL_RenderCopy(renderer, cellTexture, nullptr, &dstRect);
			font.KBF_RenderStringToFit(renderer, std::to_string(allCells[i].cellHealth), (i % cellsAlongX) * singleCellX + static_cast<float>(singleCellX*(cellsAlongX * 5)/scrnX), (i / cellsAlongX) * singleCellY + static_cast<float>(singleCellY * (cellsAlongY * 5) / scrnY), singleCellX- (singleCellY * (cellsAlongY * 5) / scrnY), singleCellY- (singleCellY * (cellsAlongY * 5) / scrnY));
			
		}
		return 1;
	}

	void printCells() {
		for (int y = 0; y < cellsAlongY; ++y) {
			for (int x = 0; x < cellsAlongX; ++x) {
				if (allCells[y * cellsAlongY + x].cellHealth != 0) {
					std::cout << " #";
				}
				else
				{
					std::cout << " .";
				}
			}
			std::cout << '\n';
		}
	}




};
