#include "tamagotchi.h"
#include <iostream>

Tamagotchi::Tamagotchi() : hunger(100), happiness(100), cleanliness(100), lastUpdateTime(SDL_GetTicks()) {
    tamagotchiRect = { 350, 100, 100, 100 };
    playButton = { 100, 450, 200, 50 };
    feedButton = { 350, 450, 200, 50 };
    cleanButton = { 600, 450, 200, 50 };
}

void Tamagotchi::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= playButton.x && x < playButton.x + playButton.w &&
            y >= playButton.y && y < playButton.y + playButton.h) {
            happiness = std::min(100, happiness + 10);
        }
        if (x >= feedButton.x && x < feedButton.x + feedButton.w &&
            y >= feedButton.y && y < feedButton.y + feedButton.h) {
            hunger = std::min(100, hunger + 10);
        }
        if (x >= cleanButton.x && x < cleanButton.x + cleanButton.w &&
            y >= cleanButton.y && y < cleanButton.y + cleanButton.h) {
            cleanliness = std::min(100, cleanliness + 10);
        }
    }
}

void Tamagotchi::update() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastUpdateTime + 30000) { // 30초마다 감소
        hunger = std::max(0, hunger - 5);
        happiness = std::max(0, happiness - 5);
        cleanliness = std::max(0, cleanliness - 5);
        lastUpdateTime = currentTime;
    }
}

void Tamagotchi::render(SDL_Renderer* renderer) {
    // Render happiness bar
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect happinessBar = { 50, 50, happiness * 2, 20 };
    SDL_RenderFillRect(renderer, &happinessBar);

    // Render hunger bar
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect hungerBar = { 50, 100, hunger * 2, 20 };
    SDL_RenderFillRect(renderer, &hungerBar);

    // Render cleanliness bar
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect cleanlinessBar = { 50, 150, cleanliness * 2, 20 };
    SDL_RenderFillRect(renderer, &cleanlinessBar);

    // Render tamagotchi
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &tamagotchiRect);

    // Render buttons
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(renderer, &playButton);
    SDL_RenderFillRect(renderer, &feedButton);
    SDL_RenderFillRect(renderer, &cleanButton);
}

bool Tamagotchi::isGameOver() {
    return (hunger == 0 || happiness == 0 || cleanliness == 0);
}