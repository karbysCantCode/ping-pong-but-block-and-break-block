#include "SDL_image.h"
#include "SDL.h"
#include "MathOperations.h"
#include <iostream>
#include <string>

#define concat(first, second) first second


struct K_RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct KBF_surfaceAndData {
    SDL_Surface* surface;
    int charX;
    int charY;
};

void holdForError(std::string errorString) {
    int holdProgram;
    std::cerr << errorString << '\n';
    std::cin >> holdProgram;
}

bool checkFontDirectoryExists() {
    if (!std::filesystem::exists("FontTextures")) {
        if (!std::filesystem::create_directory("FontTextures")) {
            holdForError("Failed to create directory");
            return 1;
        }
        return 0;
    }
    return 0;
}

void makeNonBlackTransparent(SDL_Surface* surface) {
    SDL_LockSurface(surface); // Lock the surface to allow direct access to pixel data

    Uint32* pixels = (Uint32*)surface->pixels;
    int pixelCount = surface->w * surface->h;

    for (int i = 0; i < pixelCount; ++i) {
        Uint32 pixel = pixels[i];

        // Extract the RGB components of the pixel
        Uint8 red, green, blue, alpha;
        SDL_GetRGBA(pixel, surface->format, &red, &green, &blue, &alpha);

        // Check if the pixel is not black
        if (!(red == 0 && green == 0 && blue == 0)) {
            // Make the pixel fully transparent
            alpha = 0;
        }
        else {
            // Make sure black pixels are fully opaque
            alpha = 255;
        }

        // Set the modified pixel back to the surface
        pixels[i] = SDL_MapRGBA(surface->format, red, green, blue, alpha);
    }

    SDL_UnlockSurface(surface); // Unlock the surface after accessing pixel data
}

KBF_surfaceAndData* KBF_openFontAsSurface(std::string fileName) {

    if (checkFontDirectoryExists()) {
        holdForError("directory dont exist");
        return nullptr;
    }
    std::string filePath = "FontTextures/" + fileName + ".png";
    SDL_Surface* surface = IMG_Load(filePath.c_str());

    if (!surface) {
        std::cerr << "Failed to load surface: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    // Convert surface to a format with an alpha channel
    SDL_Surface* surfaceWithAlpha = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface); // Free the original surface
    surface = surfaceWithAlpha;

    makeNonBlackTransparent(surface);

    uint32_t* pixels = static_cast<uint32_t*>(surface->pixels);
    uint32_t target_pixel = pixels[0];
    K_RGBA colorValues;
    SDL_GetRGB(target_pixel, surface->format, &colorValues.r, &colorValues.g, &colorValues.b);

    KBF_surfaceAndData* result = new KBF_surfaceAndData;
    result->surface = surface;
    result->charX = colorValues.r;
    result->charY = colorValues.g;

    return result;
}

class KBF_Font {
private:
    struct textureInfo {
        SDL_Rect positionAndSize;
        SDL_Texture* texture;
    };

    int fontX = 0;
    int fontY = 0;
    SDL_Surface* openedSurface = nullptr;
    textureInfo charTextureArray[95] = { 0 };

public:
    KBF_Font(SDL_Renderer* renderer, SDL_Color& color, std::string fontFileName) {

        KBF_surfaceAndData* surfaceAndData = KBF_openFontAsSurface(fontFileName);

        if (surfaceAndData == nullptr) {
            std::cout << "font did not load" << '\n';
            return;
        }

        openedSurface = surfaceAndData->surface;
        fontX = surfaceAndData->charX;
        fontY = surfaceAndData->charY;

        std::cout << fontX << "::" << fontY;

        // create table of textures for all chars (32-127)
        for (int i = 0; i < 95; ++i) {
            textureInfo cTextureInfo;
            cTextureInfo.positionAndSize.x = (fontX + 1) * i + 1;
            cTextureInfo.positionAndSize.y = 1;
            cTextureInfo.positionAndSize.w = fontX;
            cTextureInfo.positionAndSize.h = fontY;

            SDL_Surface* extractSurface = SDL_CreateRGBSurface(0, fontX, fontY, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
            SDL_BlitSurface(openedSurface, &cTextureInfo.positionAndSize, extractSurface, nullptr);

            // Replace black pixels with specified color
            Uint32* pixels = (Uint32*)extractSurface->pixels;
            int pixelCount = extractSurface->w * extractSurface->h;
            for (int j = 0; j < pixelCount; ++j) {
                Uint32 pixel = pixels[j];
                Uint8 red, green, blue, alpha;
                SDL_GetRGBA(pixel, extractSurface->format, &red, &green, &blue, &alpha);

                if (red == 0 && green == 0 && blue == 0) {
                    pixels[j] = SDL_MapRGBA(extractSurface->format, color.r, color.g, color.b, 255); // Fully opaque color
                }
            }

            cTextureInfo.texture = SDL_CreateTextureFromSurface(renderer, extractSurface);
            charTextureArray[i] = cTextureInfo;
            SDL_FreeSurface(extractSurface);
        }
    }



    void KBF_RenderLetter(SDL_Renderer* renderer, char character, int x, int y, int scale) {
        //if (color.r != 0 || color.b != 0 || color.g != 0) {

        //} incomplete implementation for colored fonts
        SDL_Rect dstRect = { x, y, fontX * scale, fontY * scale };
        SDL_RenderCopy(renderer, charTextureArray[character - 32].texture, nullptr, &dstRect);

    }

    void KBF_RenderStringToFit(SDL_Renderer* renderer, std::string string, int x, int y, int xFit, int yFit) {
        int scale = mathMinInt(xFit / fontX, yFit / fontY);
        //std::cout << yFit / fontY << '\n';
        KBF_RenderString(renderer, string, x, y, scale);
    }

    void KBF_RenderString(SDL_Renderer* renderer, std::string string, int x, int y, int scale) {
        for (int i = 0; i < string.length(); ++i) {
            KBF_RenderLetter(renderer, string[i], x + (5 * i) * scale, y, scale);
        }
    }
};