#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

struct SDLState {
    SDL_Window* win;
    SDL_Renderer* renderer;
    int width, height, logW, logH;
};

bool initialize(SDLState& state);
void cleanup(SDLState& state);

int main(int argc, char* argv[]) {
    SDLState state;
    state.width = 1600;
    state.height = 900;
    state.logW = 640;
    state.logH = 320;

    if(!initialize(state)) {
        return 1;
    }
    
    // load game assets
    SDL_Texture* idleTex = IMG_LoadTexture(state.renderer, "../../data/idle.png");
    SDL_SetTextureScaleMode(idleTex, SDL_SCALEMODE_NEAREST);

    // setup game data
    const bool* keys = SDL_GetKeyboardState(nullptr);
    float playerX = 150;
    const float floor = state.logH;
    uint64_t prevTime = SDL_GetTicks();
    bool flipHorizontal = false;

    // game loop
    bool running = true;
    while(running) {
        uint64_t nowTime = SDL_GetTicks();
        float deltaTime = (nowTime - prevTime) / 1000.0f;
        SDL_Event event {0};
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_EVENT_QUIT: {
                running = false;
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED: {
                state.width = event.window.data1;
                state.height = event.window.data2;
                break;
            }
            
            default:
                break;
            }
        }

        // handle movement
        float moveAmount = 0;
        if(keys[SDL_SCANCODE_A]) {
            moveAmount += -75.0f;
            flipHorizontal = true;
        }
        if(keys[SDL_SCANCODE_D]) {
            moveAmount += 75.0f;
            flipHorizontal = false;
        }
        playerX +=moveAmount * deltaTime;

        // perform drawing commands
        SDL_SetRenderDrawColor(state.renderer, 20, 10, 30, 255);
        SDL_RenderClear(state.renderer);

        const float spriteSize = 32;
        SDL_FRect src {
            .x = 0,
            .y = 0,
            .w = spriteSize,
            .h = spriteSize
        };

        SDL_FRect dst {
            .x = playerX,
            .y = floor - spriteSize,
            .w = spriteSize,
            .h = spriteSize
        };

        SDL_RenderTextureRotated(state.renderer, idleTex, &src, &dst, 0, nullptr, 
            flipHorizontal? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        
        //swap buffers and present
        SDL_RenderPresent(state.renderer);
        prevTime = nowTime;
    }

    SDL_DestroyTexture(idleTex);
    cleanup(state);
    return 0;
}

bool initialize(SDLState& state) {
    bool initSuccess = true;
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
        initSuccess = false;
    }

    // create window
    state.win = SDL_CreateWindow("SDL Window", state.width, state.height, SDL_WINDOW_RESIZABLE);
    if(!state.win) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing window", state.win);
        cleanup(state);
        initSuccess = false;
    }

    // sdl renderer
    state.renderer = SDL_CreateRenderer(state.win, nullptr);
    if(!state.renderer) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing window", state.win);
        cleanup(state);
        initSuccess = false;
    }

    // configure presentation
    
    SDL_SetRenderLogicalPresentation(state.renderer, state.logW, state.logH, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return initSuccess;
}

void cleanup(SDLState& state) {
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.win);
    SDL_Quit();
}