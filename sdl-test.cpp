// g++ -s -Os sdl-test.cpp -lSDL2 -o sdl-test && ./sdl-test

#include <SDL2/SDL.h>
#include <memory>
#include <functional>
#include <iostream>

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "[ERROR] Can't init SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);

    auto window_title = "Test";
    auto window_width = 1280;
    auto window_height = 720;
    auto window = std::unique_ptr<SDL_Window, std::function<void (SDL_Window *)>>(SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_RESIZABLE), SDL_DestroyWindow);
    if (!window) {
        std::cerr << "[ERROR] Can't create the SDL window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    auto renderer = std::unique_ptr<SDL_Renderer, std::function<void (SDL_Renderer *)>>(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer);
    if (!renderer) {
        std::cerr << "[ERROR] Can't create the SDL renderer: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_Rect box = { 0, 0, 100, 100 };
    SDL_Color boxColor = { 255, 0, 0, 255 };

    auto running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
        	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                window_width = event.window.data1;
                window_height = event.window.data2;
            }

            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        box.x += 1;
        box.y += 2;

        SDL_Color color = { 17, 17, 17, 255 };
        SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);
        SDL_RenderClear(renderer.get());

        SDL_SetRenderDrawColor(renderer.get(), boxColor.r, boxColor.g, boxColor.b, boxColor.a);
        SDL_RenderFillRect(renderer.get(), &box);

        SDL_Rect cornerBox = { window_width - 50, window_height - 50, 49, 49 };
     	SDL_Color cornerColor = { 0, 255, 0, 255 };
        SDL_SetRenderDrawColor(renderer.get(), cornerColor.r, cornerColor.g, cornerColor.b, cornerColor.a);
        SDL_RenderFillRect(renderer.get(), &cornerBox);

        SDL_RenderPresent(renderer.get());
    }

    return EXIT_SUCCESS;
}