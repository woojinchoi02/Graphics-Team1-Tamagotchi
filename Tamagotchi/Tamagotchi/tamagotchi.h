#include <SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;

class Tamagotchi {
public:
    Tamagotchi() : hunger(100), happiness(100), cleanliness(100), lastUpdateTime(SDL_GetTicks()) {
        tamagotchiRect = { 350, 100, 100, 100 };
        playButton = { 100, 450, 200, 50 };
        feedButton = { 350, 450, 200, 50 };
        cleanButton = { 600, 450, 200, 50 };
    }

    void handleEvent(SDL_Event& e) {
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

    void update() {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastUpdateTime + 30000) { // 30초마다 감소
            hunger = std::max(0, hunger - 5);
            happiness = std::max(0, happiness - 5);
            cleanliness = std::max(0, cleanliness - 5);
            lastUpdateTime = currentTime;
        }
    }

    void render(SDL_Renderer* renderer) {
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

    bool isGameOver() {
        return (hunger == 0 || happiness == 0 || cleanliness == 0);
    }

private:
    int hunger;
    int happiness;
    int cleanliness;
    Uint32 lastUpdateTime;
    SDL_Rect tamagotchiRect;
    SDL_Rect playButton;
    SDL_Rect feedButton;
    SDL_Rect cleanButton;
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Tamagotchi", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        SDL_DestroyWindow(win);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event e;

    Tamagotchi tamagotchi;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            tamagotchi.handleEvent(e);
        }

        tamagotchi.update();

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        tamagotchi.render(ren);

        SDL_RenderPresent(ren);

        if (tamagotchi.isGameOver()) {
            std::cout << "Game Over!" << std::endl;
            running = false;
        }
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
