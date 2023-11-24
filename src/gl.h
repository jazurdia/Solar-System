// gl.h
#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <SDL.h>
#include "color.h"
#include "fragment.h"
#include "noise.h"

float rsPlanets = 1.0f;  // Base speed of the planets
float osPlanets = 1.0f;  // Base speed of the planets

float raSun = 0.2f;

float oaEarth = 0.0f;
float earthDistanceToSun = 3.5f;
float raEarth = 0.0f;

float orMars = 0.0f;
float marsDistanceToSun = 5.0f;
float raMars = 0.0f;

float oaJupiter = 0.0f;
float jupiterDistanceToSun = 6.5f;
float raJupiter = 0.0f;

float oaUranus = 0.0f;
float uranusDistanceToSun = 8.0f;
float raUranus = 0.0f;

float oaNeptune = 0.0f;
float neptuneDistanceToSun = 9.5f;
float raNeptune = 0.0f;

struct Face {
    std::array<int, 3> vertexIndices;
    std::array<int, 3> normalIndices;
    std::array<int, 3> texIndices;
};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

Color clearColor = {0, 0, 0, 255}; // Initially set to black
std::array<std::array<float, SCREEN_WIDTH>, SCREEN_HEIGHT> zbuffer;

// Function to set a specific pixel in the framebuffer to the currentColor
void point(Fragment f) {
    if (f.position.z < zbuffer[f.position.y][f.position.x]) {
        SDL_SetRenderDrawColor(renderer, f.color.r, f.color.g, f.color.b, f.color.a);
        SDL_RenderDrawPoint(renderer, f.position.x, f.position.y);
        zbuffer[f.position.y][f.position.x] = f.position.z;
    }
}


float ox = 1200.0f;
float oy = 3000.0f;

// Function to clear the framebuffer with the clearColor
void clear() {
    SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(renderer);

    // Clean the zbuffer
    for (auto &row : zbuffer) {
        std::fill(row.begin(), row.end(), 99999.0f);
    }

    // Generate stars
    int numStars = 2500;
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    ox += 0.001f;
    oy += 0.001f;


    for (int i = 0; i < numStars; i += 5) {
        float x = noise.GetNoise((float)i + ox, oy) * SCREEN_WIDTH;
        float y = noise.GetNoise((float)i + oy, ox) * SCREEN_HEIGHT;
        float z = noise.GetNoise((float)i + ox, oy) * 100.0f;

        x = std::abs(x);
        y = std::abs(y);
        z = std::abs(z);

//        std::cout << x << ", " << y << ", " << z << std::endl;

        float size = noise.GetNoise((float)i + ox, oy) * 5.0f;

        Fragment f = {
            {x, y, z},
            Color{255, 255, 255},
            size
        };

        point(f);
    }
}



