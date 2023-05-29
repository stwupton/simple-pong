#include <cassert>

#include "SDL2/SDL.h"

#include "types/base_types.hpp"
#include "types/vector.hpp"

#include "sdl_gl_renderer.hpp"

#include "game/settings.hpp"
#include "game/game_state.hpp"
#include "game/input.hpp"
#include "game/game.hpp"

#define SIM_TIME_MS (1.0f / 60 * 1000)

void update_input(Input *input) {
  // Reset input
  *input = {};

  const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

  // Left paddle
  if (keyboard_state[SDL_SCANCODE_A]) {
    input->left_paddle += 1;
  } 

  if (keyboard_state[SDL_SCANCODE_Q]) {
    input->left_paddle -= 1;
  }

  // Right paddle
  if (keyboard_state[SDL_SCANCODE_SEMICOLON]) {
    input->right_paddle += 1;
  } 

  if (keyboard_state[SDL_SCANCODE_P]) {
    input->right_paddle -= 1;
  }
}

int main(int argc, char *args[]) {
  int result = SDL_Init(SDL_INIT_EVERYTHING);
  if (result != 0) {
    SDL_Log(SDL_GetError());
  }

  SDL_DisplayMode display_mode;
  result = SDL_GetDesktopDisplayMode(0, &display_mode);
  if (result != 0) {
    SDL_Log(SDL_GetError());
  }

  int window_flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
  SDL_Window *window = SDL_CreateWindow("Simple Pong", 0, 0, display_mode.w, display_mode.h, window_flags);
  assert(window != nullptr);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  if (gl_context == NULL) {
    SDL_Log(SDL_GetError());
  }

  result = SDL_GL_SetSwapInterval(1);
  if (result != 0) {
    SDL_Log(SDL_GetError());
  }

  Game_State *game_state = new Game_State {};
  Game::setup(game_state);

  Game_State *previous_state = new Game_State {};
  *previous_state = *game_state;

  Input *input = new Input {};
  SDL_GL_Renderer renderer;

  u64 previous_time = SDL_GetTicks64();
  u64 time_accumulator = 0;
  u8 sim_speed = 1;

  bool should_close = false;
  while (!should_close) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE: {
            should_close = true;
          } break;

          case SDLK_EQUALS:
            sim_speed += 1;
            break;
          case SDLK_MINUS:
            sim_speed -= 1;
            break;
        }
      }
    }

    update_input(input);

    const u64 current_time = SDL_GetTicks64();
    time_accumulator += (current_time - previous_time) * sim_speed;
    previous_time = current_time;

    while (time_accumulator >= SIM_TIME_MS) {
      time_accumulator -= SIM_TIME_MS;
      *previous_state = *game_state;
      Game::update(game_state, SIM_TIME_MS / 1000, input);
    }

    const f32 alpha = time_accumulator / SIM_TIME_MS;
    renderer.render(window, *previous_state, *game_state, alpha);
  }

  return 0;
}