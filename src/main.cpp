#include "SDL3/SDL_scancode.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <cmath>

#include <glad/glad.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "constants.h"
#include "renderer.h"
#include "camera.h"

typedef struct
{
    SDL_Window *window;
    SDL_GLContext glContext;
    Camera *camera;
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
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

    SDL_Window *window = SDL_CreateWindow("Learning OpenGL", SCREEN_WIDTH, SCREEN_HEIGHT,
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

    Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    AppState *state = static_cast<AppState *>(SDL_calloc(1, sizeof(AppState)));
    state->window = window;
    state->glContext = glContext;
    state->camera = camera;
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
        switch (event->key.scancode)
        {
        case SDL_SCANCODE_ESCAPE:
            return SDL_APP_SUCCESS;
        case SDL_SCANCODE_TAB:
            renderer::swapPolygonMode();
            break;
        case SDL_SCANCODE_SPACE:
            renderer::nextScene();
            break;
        case SDL_SCANCODE_W:
            state->camera->setForward(true);
            break;
        case SDL_SCANCODE_S:
            state->camera->setBack(true);
            break;
        case SDL_SCANCODE_A:
            state->camera->setLeft(true);
            break;
        case SDL_SCANCODE_D:
            state->camera->setRight(true);
            break;
        case SDL_SCANCODE_LSHIFT:
            state->camera->setSprint(true);
            break;
        default:
            break;
        }
        break;

    case SDL_EVENT_KEY_UP:
        switch (event->key.scancode)
        {
        case SDL_SCANCODE_W:
            state->camera->setForward(false);
            break;
        case SDL_SCANCODE_S:
            state->camera->setBack(false);
            break;
        case SDL_SCANCODE_A:
            state->camera->setLeft(false);
            break;
        case SDL_SCANCODE_D:
            state->camera->setRight(false);
            break;
        case SDL_SCANCODE_LSHIFT:
            state->camera->setSprint(false);
            break;
        default:
            break;
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

    float currentFrame = SDL_GetTicks() / 1000.0f;
    state->deltaTime = currentFrame - state->lastFrame;
    state->lastFrame = currentFrame;

    state->camera->updatePos(state->deltaTime);

    renderer::render(state->camera);

    SDL_GL_SwapWindow(state->window);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    renderer::cleanup();

    AppState *state = static_cast<AppState *>(appstate);
    if (!state)
    {
        return;
    }
    SDL_free(state);
    SDL_Quit();
}