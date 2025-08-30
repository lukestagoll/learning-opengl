#define SDL_MAIN_USE_CALLBACKS 1
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

#include <glad/glad.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "renderer.h"

typedef struct
{
    SDL_Window *window;
    SDL_GLContext glContext;
} AppState;

typedef struct
{
    Uint64 last_ns;
    Uint64 frame_ns;
    unsigned frames;
} FPS;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow("Learning OpenGL", WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (!window)
    {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        return SDL_APP_FAILURE;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_Log("Failed to initialize GLAD");
        return SDL_APP_FAILURE;
    }

    AppState *state = static_cast<AppState *>(SDL_calloc(1, sizeof(AppState)));
    state->window = window;
    state->glContext = glContext;
    *appstate = state;

    renderer::init();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *state = static_cast<AppState *>(appstate);

    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
        if (event->key.scancode == SDL_SCANCODE_ESCAPE)
        {
            return SDL_APP_SUCCESS;
        }
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        glViewport(0, 0, event->window.data1, event->window.data2);
        break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *state = static_cast<AppState *>(appstate);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer::render();

    SDL_GL_SwapWindow(state->window);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    AppState *state = static_cast<AppState *>(appstate);
    if (!state)
    {
        return;
    }
    SDL_free(state);
    SDL_Quit();
}