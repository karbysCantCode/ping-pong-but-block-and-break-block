#pragma once
#include "SDL.h"
#include <iostream>

struct KCS_Color {
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;

	KCS_Color(int R, int G, int B, int A) : r(R), g(G), b(B), a(A) {}
};

class KCS_SevenSegmentFont { // t = top, b = bottom, r right, l left, m middle
private:
	const uint8_t SEG_A = 0b00000001; // Top
	const uint8_t SEG_B = 0b00000010; // Top right
	const uint8_t SEG_C = 0b00000100; // Bottom right
	const uint8_t SEG_D = 0b00001000; // Bottom
	const uint8_t SEG_E = 0b00010000; // Bottom left
	const uint8_t SEG_F = 0b00100000; // Top left
	const uint8_t SEG_G = 0b01000000; // Middle

	const uint8_t sevenSegmentDigits[10] = {
	   static_cast<uint8_t>(SEG_A ^ SEG_B ^ SEG_C ^ SEG_D ^ SEG_E ^ SEG_F),       // 0
	   static_cast<uint8_t>(SEG_B ^ SEG_C),                                       // 1
	   static_cast<uint8_t>(SEG_A ^ SEG_B ^ SEG_G ^ SEG_E ^ SEG_D),               // 2
	   static_cast<uint8_t>(SEG_A ^ SEG_B ^ SEG_G ^ SEG_C ^ SEG_D),               // 3
	   static_cast<uint8_t>(SEG_F ^ SEG_G ^ SEG_B ^ SEG_C),                       // 4
	   static_cast<uint8_t>(SEG_A ^ SEG_F ^ SEG_G ^ SEG_C ^ SEG_D),               // 5
	   static_cast<uint8_t>(SEG_A ^ SEG_F ^ SEG_G ^ SEG_E ^ SEG_C ^ SEG_D),       // 6
	   static_cast<uint8_t>(SEG_A ^ SEG_B ^ SEG_C),                               // 7
	   static_cast<uint8_t>(SEG_A ^ SEG_B ^ SEG_C ^ SEG_D ^ SEG_E ^ SEG_F ^ SEG_G), // 8
	   static_cast<uint8_t>(SEG_A ^ SEG_B ^ SEG_C ^ SEG_D ^ SEG_F ^ SEG_G)        // 9
	};


	void drawSegment(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int size) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for testing
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}
public:
	void renderDigit(SDL_Renderer* renderer, int x, int y, int size, KCS_Color *textColor,char charDigit) {
		/*
		In ASCII, the numerical values representing characters '0' to '9' are contiguous and ordered.
		Specifically, the ASCII value of '0' is 48, '1' is 49, '2' is 50, and so on, up to '9' which is 57
		So when you subtract the ASCII value of '0' from the ASCII value of any digit character, 
		you're effectively subtracting 48 from the ASCII value of that character.
		'0' has an ASCII value of 48.
		'1' has an ASCII value of 49.
		'2' has an ASCII value of 50.
		...
		'9' has an ASCII value of 57.
		*/

		int digit = charDigit - '0';

		if (!(digit >= 0 && digit <= 9)) return;

		uint8_t segments = static_cast<uint8_t>(sevenSegmentDigits[digit]);

		int oneSize = 1 * size;
		if (segments & SEG_A) drawSegment(renderer, x + 1, y, x + size, y, size); // Top
		if (segments & SEG_B) drawSegment(renderer, x + 1 + size, y + 1, x + 1 + size, y + size, size); // Top right
		if (segments & SEG_C) drawSegment(renderer, x + 1 + size, y + 2 + size, x + 1 + size, y + 1 + (size * 2), size); // bottom right
		if (segments & SEG_D) drawSegment(renderer, x + 1, y+(size*2)+2, x + size, y + (size * 2)+2, size); // Bottom
		if (segments & SEG_E) drawSegment(renderer, x, y + 1 + (size * 2), x, y + 1 + size, size); // Bottom left
		if (segments & SEG_F) drawSegment(renderer, x, y + 1, x, y + 1 + size, size); // top left 
		if (segments & SEG_G) drawSegment(renderer, x + 1, y + 1 + size, x + 1 + size, y + 1 + size, size); // Middle






		
	}
};

void KCS_RenderRectInsetBorder(SDL_Renderer* renderer,
							   int f_x,
							   int f_y,
							   int f_width,
							   int f_height,
							   int f_inset,
							   KCS_Color * f_borderColor,
							   KCS_Color* f_fillColor) {
	SDL_Rect border;
	border.x = f_x;
	border.y = f_y;
	border.w = f_width;
	border.h = f_height;

	SDL_SetRenderDrawColor(renderer, f_borderColor->r, f_borderColor->g, f_borderColor->b, f_borderColor->a);
	SDL_RenderFillRect(renderer, &border);

	SDL_Rect main;
	main.w = f_width - (f_inset * 2);
	main.h = f_height - (f_inset * 2);
	main.x = f_x + f_inset;
	main.y = f_y + f_inset;

	SDL_SetRenderDrawColor(renderer, f_fillColor->r, f_fillColor->g, f_fillColor->b, f_fillColor->a);
	SDL_RenderFillRect(renderer, &main);
	
}

void KCS_SurfaceRectInsetBorder(SDL_Surface* surface,
								int f_x,
								int f_y,
								int f_width,
								int f_height,
								int f_inset,
								KCS_Color& f_borderColor,
								KCS_Color& f_fillColor) {
	SDL_Rect border;
	border.x = f_x;
	border.y = f_y;
	border.w = f_width;
	border.h = f_height;

	SDL_FillRect(surface, &border, SDL_MapRGBA(surface->format, f_borderColor.r, f_borderColor.g, f_borderColor.b, f_borderColor.a));

	SDL_Rect main;
	main.w = f_width - (f_inset * 2);
	main.h = f_height - (f_inset * 2);
	main.x = f_x + f_inset;
	main.y = f_y + f_inset;

	SDL_FillRect(surface, &main, SDL_MapRGBA(surface->format, f_fillColor.r, f_fillColor.g, f_fillColor.b, f_fillColor.a));

}

/*Parameters:
	SDL_Renderer* = ptr to renderer
	portions = how smooth graident
	direction = false = X, true = Y
	xRes = res of x
	yRes = res of y
	startcolor = color to start
	endcolor = color to end*/
void KCS_BackgroundGradient(SDL_Renderer* renderer,
							int portions,
							bool direction,
							int xRes,
							int yRes,
							KCS_Color* startColor,
							KCS_Color* endColor) {

	SDL_SetRenderDrawColor(renderer, endColor->r, endColor->g, endColor->b, endColor->a);
	SDL_RenderClear(renderer);

	if (direction) {
		/*
		int remainder = yRes % portions;
		int portionSize = (yRes - remainder) / portions;
		if (remainder > portions / 2) {
			std::cout << "more " << portionSize << "\n";
			portions = remainder;
			portionSize = yRes / portions;
			std::cout << "NEW POrtion " << portionSize << " remainder " << yRes % portions;
		}
		*/
		//portions = portions + remainder;

		int remainder = yRes % portions;
		int portionSize = yRes / portions;

		if (!(remainder == 0)) portions += (yRes - (portions * portionSize)) / portionSize+1;

		//std::cout << portions;
		//portions += remainder;
		
		for (int i = portions; i > -1; i--)
		{
			//std::cout << portionSize << "\n" << portionSize * i << "\n";
			SDL_Rect rect;
			rect.x = 0;
			rect.y = portionSize * i;
			rect.w = xRes;
			rect.h = portionSize;

			float ratio = static_cast<float>(i) / (portions - 1);

			SDL_SetRenderDrawColor(renderer,
				static_cast<int>(startColor->r * (1 - ratio) + endColor->r * ratio),
				static_cast<int>(startColor->g * (1 - ratio) + endColor->g * ratio),
				static_cast<int>(startColor->b * (1 - ratio) + endColor->b * ratio),
				static_cast<int>(startColor->a * (1 - ratio) + endColor->a * ratio));

			SDL_RenderFillRect(renderer, &rect);
		}

	}

}
