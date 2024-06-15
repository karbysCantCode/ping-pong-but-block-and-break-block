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
	CELL_SPECIAL,
	CELL_COUNT
};

struct singleCell {
	int cellX = 0;
	int cellY = 0;
	int cellHealth = 0;
	int cellTypeValue = 0;
	CellType cellType = CELL_NONE;
};

class cells {
private:

	int averageHealth = 700;
	int m_scrnX;
	int m_scrnY;

	SDL_Texture* cellTextures[CELL_COUNT] = { nullptr };

	const int cellBorderWidth = 5;

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
	
	int currentScreenX;
	int currentScreenY;

	cells(SDL_Renderer* renderer,
			int health,
			int screenX,
			int screenY) {

		baseHealth = health;
		m_scrnX = screenX;
		m_scrnY = screenY;
		currentScreenX = m_scrnX;
		currentScreenY = m_scrnY;

		singleCellX = (m_scrnX / cellsAlongX) + 1;
		singleCellY = (m_scrnY / cellsAlongY) + 1;

		buildCellTextures(renderer);

		allCells.resize(cellsAlongX * cellsAlongY);
	}

	void buildCellTextures(SDL_Renderer* renderer) {
		for (int i = 0; i < CELL_COUNT; ++i) {
			SDL_Surface* surface = SDL_CreateRGBSurface(0,
				singleCellX,
				singleCellY,
				32,
				0x00FF0000,
				0x0000FF00,
				0x000000FF,
				0xFF000000);

			KCS_Color mainColor(255, 255, 255, 255);
			switch (i){
			case 2: // speed
				mainColor = { 255, 255, 0, 255 };
				break;
			case 3: // damage
				mainColor = { 255, 0, 0, 255 };
				break;
			case 4: // amount
				mainColor = { 0, 0, 255, 255 };
				break;
			case 5: // special
				mainColor = { 100, 100, 100, 255 };
				break;
			}
			KCS_Color borderColor(0, 0, 0, 255);
			KCS_SurfaceRectInsetBorder(surface,
				0,
				0,
				singleCellX,
				singleCellY,
				cellBorderWidth,
				borderColor,
				mainColor);

			cellTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		}
	}

	/* creates another row and col on the left and top,
	run before creating more cells
	*/
	void expandTotalCells() {
		averageHealth = (averageHealth + 1) * 2;
		cellsAlongX += 1;
		cellsAlongY += 1;

		singleCellX = (m_scrnX / cellsAlongX) + 1;
		singleCellY = (m_scrnY / cellsAlongY) + 1;

		std::vector<singleCell> newCells(cellsAlongX * cellsAlongY);

		for (int y = 0; y < cellsAlongY-1; ++y) {
			for (int x = 0; x < cellsAlongX-1; ++x) {
				newCells[(y+1) * cellsAlongX + (x + 1)] = allCells[y * (cellsAlongX-1) + x];
			}
		}

		allCells.resize(cellsAlongX * cellsAlongY);
		allCells = std::move(newCells);
		currentScreenX = cellsAlongX * singleCellX;
		currentScreenY = cellsAlongY * singleCellY;
		
	}

	bool createCell(int f_x,
					int f_y,
					int f_health,
					CellType f_cellType) {
		int cellValue = 0;
		if (f_cellType != CELL_NONE) {
			cellValue = randomInt(1, 3);
		}
		singleCell newCell = { f_x,f_y,f_health,cellValue,f_cellType };
		allCells[f_y * cellsAlongY + f_x] = newCell;
		return 1;
	}
	/*1 = lose
	  0 = sucess
	*/
	int generateRow() {
		std::cout << currentScreenX << "," << currentScreenY << '\n';
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
			SDL_Rect dstRect = { (i % cellsAlongX) * singleCellX,
								 (i / cellsAlongX) * singleCellY,
								 singleCellX,
								 singleCellY};

			SDL_RenderCopy(renderer, cellTextures[allCells[i].cellType], nullptr, &dstRect);
			const int f_FitX = mathMaxInt(singleCellX - (singleCellX * (cellsAlongX * (2 * cellBorderWidth)) / m_scrnX),1);
			const int f_FitY = mathMaxInt(singleCellY - (singleCellY * (cellsAlongY * (2 * cellBorderWidth)) / m_scrnY),1);
			font.KBF_RenderStringToFit(renderer,
									   abbreviateNumber(allCells[i].cellHealth),
									   (i % cellsAlongX) * singleCellX + static_cast<float>(singleCellX*(cellsAlongX * cellBorderWidth)/ m_scrnX),
									   (i / cellsAlongX) * singleCellY + static_cast<float>(singleCellY * (cellsAlongY * cellBorderWidth) / m_scrnY),
										f_FitX,
										f_FitY);
			
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
