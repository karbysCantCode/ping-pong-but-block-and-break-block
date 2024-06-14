#pragma once
#include "KarbusVector2.h"
#include "cellManager.h"
#include "MathOperations.h"
#include <random>
#include <vector>
#include <iostream>
#include <SDL.h>

class balls {
private:
    struct ballStruct {
        vector2 position;
        vector2 velocity;
    };

    float solveOut(float position, float min, float max) {
        if (position > min + ((max - min) / 2)) return max + circleRadiusInPixels;
        return min - circleRadiusInPixels;
    }

    float positionToEscapeSide(float position, bool isX) {
        if (isX) {
            if (position >= screenX-10) return static_cast<float>(screenX - circleRadiusInPixels);
            return static_cast<float>(circleRadiusInPixels);
        }
        else {
            if (position >= screenY-10) return static_cast<float>(screenY - circleRadiusInPixels);
            return static_cast<float>(circleRadiusInPixels);
        }
    }

    std::vector<ballStruct> ballArray;
    SDL_Texture* circleTexture = nullptr;
    const int circleRadiusInPixels = 6;

    int totalGlitches = 0;
    int screenX;
    int screenY;

public:
    int ballDamage = 1;

    balls(SDL_Renderer* renderer, int scrnX, int scrnY) {
        screenX = scrnX;
        screenY = scrnY;    
        SDL_Surface* surface = SDL_CreateRGBSurface(0, circleRadiusInPixels * 2, circleRadiusInPixels * 2, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        Uint32* pixels = (Uint32*)surface->pixels; // Get the pixel array
        for (int y = -circleRadiusInPixels; y <= circleRadiusInPixels; ++y) {
            for (int x = -circleRadiusInPixels; x <= circleRadiusInPixels; ++x) {
                if (x * x + y * y <= circleRadiusInPixels * circleRadiusInPixels) { // Check if the point is inside the circle
                    pixels[((y + circleRadiusInPixels) * surface->w) + (x + circleRadiusInPixels)] = 0x000000FF; // Set the pixel color
                }
            }
        }
        circleTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    void createBall() {

        ballStruct newBall;
        newBall.position.x = static_cast<float>(screenX / 2);
        newBall.position.y = static_cast<float>(screenY / 2);//screenY - (screenY / 10);

        newBall.velocity.x = randomFloat(-10, 10);
        newBall.velocity.y = randomFloat(-10, 10);

        ballArray.push_back(newBall);
    }
    
    void processBalls(SDL_Renderer* renderer, cells& cellManager) {
        for (int i = 0; i < ballArray.size(); i++) {
            ballStruct& currentBall = ballArray[i];

            // Boundary collision detection
            if (currentBall.position.x - circleRadiusInPixels < 0 || currentBall.position.x + circleRadiusInPixels >= screenX) {
                currentBall.velocity.x = -currentBall.velocity.x;
                currentBall.position.x = positionToEscapeSide(currentBall.position.x, 1);
            }
            if (currentBall.position.y - circleRadiusInPixels < 0 || currentBall.position.y + circleRadiusInPixels >= screenY) {
                currentBall.velocity.y = -currentBall.velocity.y;
                currentBall.position.y = positionToEscapeSide(currentBall.position.y, 0);
            }

            vector2Int cellPosition(static_cast<int>(currentBall.position.x) / cellManager.singleCellX, static_cast<int>(currentBall.position.y) / cellManager.singleCellY);
            //std::cout << cellPosition << '\n';

            for (int y = 0; y < 3; ++y) {
                for (int x = 0; x < 3; ++x) {
                    const int currentCheckAroundCellY = static_cast<int>(cellPosition.y) - 1 + y;
                    const int currentCheckAroundCellX = static_cast<int>(cellPosition.x) - 1 + x;

                    if (currentCheckAroundCellY >= 0 && currentCheckAroundCellY < cellManager.cellsAlongY &&
                        currentCheckAroundCellX >= 0 && currentCheckAroundCellX < cellManager.cellsAlongX &&
                        cellManager.allCells[currentCheckAroundCellY * cellManager.cellsAlongX + currentCheckAroundCellX].cellType != CELL_NONE) {

                        singleCell& currentCell = cellManager.allCells[currentCheckAroundCellY * cellManager.cellsAlongX + currentCheckAroundCellX];

                        if (currentCell.cellX == cellPosition.x &&
                            currentCell.cellY == cellPosition.y) {
                            totalGlitches++;
                            //std::cout << totalGlitches << '\n';
                        }

                        int cX = clamp(static_cast<int>(currentBall.position.x), currentCheckAroundCellX * cellManager.singleCellX, currentCheckAroundCellX * cellManager.singleCellX + cellManager.singleCellX);
                        int cY = clamp(static_cast<int>(currentBall.position.y), currentCheckAroundCellY * cellManager.singleCellY, currentCheckAroundCellY * cellManager.singleCellY + cellManager.singleCellY);

                        float distanceSquared = ((cX - currentBall.position.x) * (cX - currentBall.position.x)) + ((cY - currentBall.position.y) * (cY - currentBall.position.y));

                        if (distanceSquared <= circleRadiusInPixels * circleRadiusInPixels || (cX == currentBall.position.x && cY == currentBall.position.y)) {
                            //std::cout << "collision" << '\n';
                            float absX = mathAbs(cX - currentBall.position.x);
                            float absY = mathAbs(cY - currentBall.position.y);
                            if (absX > absY) {
                                currentBall.velocity.x = -currentBall.velocity.x;
                                currentBall.position.x = solveOut(currentBall.position.x, static_cast<float>(currentCheckAroundCellX * cellManager.singleCellX), static_cast<float>(currentCheckAroundCellX * cellManager.singleCellX + cellManager.singleCellX));
                            }
                            else {
                                currentBall.velocity.y = -currentBall.velocity.y;
                                currentBall.position.y = solveOut(currentBall.position.y, static_cast<float>(currentCheckAroundCellY * cellManager.singleCellY), static_cast<float>(currentCheckAroundCellY * cellManager.singleCellY + cellManager.singleCellY));

                            }

                            // Decrease the cell health
                            currentCell.cellHealth -= ballDamage;
                            if (currentCell.cellHealth <= 0 && currentCell.cellType != CELL_NONE) {
                                currentCell.cellType = CELL_NONE;
                            }
                        }
                    }
                }
            }

            // Update ball position after collision detection
            currentBall.position.x += currentBall.velocity.x;
            currentBall.position.y += currentBall.velocity.y;

            // Render the ball
            SDL_Rect dstRect = { static_cast<int>(currentBall.position.x) - circleRadiusInPixels, static_cast<int>(currentBall.position.y) - circleRadiusInPixels, circleRadiusInPixels * 2, circleRadiusInPixels * 2 };
            SDL_RenderCopy(renderer, circleTexture, nullptr, &dstRect);
        }
    }
};
