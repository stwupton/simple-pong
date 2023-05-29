#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "types/base_types.hpp"

#include "game/game_state.hpp"
#include "game/settings.hpp"

// Using dedicated GPU
// https://stackoverflow.com/questions/68469954/how-to-choose-specific-gpu-when-create-opengl-context
extern "C" {
  __declspec(dllexport) DWORD NvOptimusEnablement = 1;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// TODO(steven): Useful elsewhere?
struct Rect {
  f32 x, y, width, height;
  Rect(f32 x, f32 y, f32 width, f32 height) : 
    x(x), y(y), width(width), height(height) {}
};

struct SDL_GL_Renderer {
  void draw_rect(const Rect &rect, const Rgb &colour) const {
    const f32 width_ratio = 2.f / SCREEN_WIDTH;
    const f32 height_ratio = -(2.f / SCREEN_HEIGHT);

    const f32 x0 = rect.x * width_ratio - 1;
    const f32 y0 = rect.y * height_ratio + 1;
    const f32 x1 = (rect.x + rect.width) * width_ratio - 1;
    const f32 y1 = (rect.y + rect.height) * height_ratio + 1;

    glColor3f(colour.r, colour.g, colour.b);
    glBegin(GL_QUADS);
    glVertex2f(x0, y0);
    glVertex2f(x1, y0);
    glVertex2f(x1, y1);
    glVertex2f(x0, y1);
    glEnd();
  }

  void draw_line(
    const Vec2<f32> &start, 
    const Vec2<f32> &end, 
    const f32 thickness, 
    const Rgb &colour
  ) {
    const f32 width_ratio = 2.f / SCREEN_WIDTH;
    const f32 height_ratio = -(2.f / SCREEN_HEIGHT);

    glColor3f(colour.r, colour.g, colour.b);
    glLineWidth(thickness);
    glBegin(GL_LINE_STRIP);
    glVertex2f(start.x * width_ratio - 1, start.y * height_ratio + 1);
    glVertex2f(end.x * width_ratio - 1, end.y * height_ratio + 1);
    glEnd();
  }

  void draw_circle(
    const Vec2<f32> &position, 
    const f32 &radius, 
    const Rgb &colour
  ) const {
    const f32 width_ratio = 2.f / SCREEN_WIDTH;
    const f32 height_ratio = -(2.f / SCREEN_HEIGHT);

    const u32 vert_count = radius;

    glColor3f(colour.r, colour.g, colour.b);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= vert_count; i++) {
      const f32 angle = M_PI * 2 * i / vert_count;
      const f32 x0 = cos(angle) * radius;
      const f32 y0 = sin(angle) * radius;
      glVertex2f(
        (position.x + x0) * width_ratio - 1, 
        (position.y + y0) * height_ratio + 1
      );
    }
    glEnd();
  }

  void draw_paddle(const Vec2<f32> &position) {
    draw_rect(
      Rect(position.x, position.y, PADDLE_WIDTH, PADDLE_HEIGHT), 
      Rgb(1.f, 1.f, 1.f)
    );
  }

  // TODO(steven): Move elsewhere 
  template<typename T>
  T lerp(T a, T b, f32 alpha) {
    alpha = fmin(1.0f, fmax(0.0f, alpha));
    return a * (1.0f - alpha) + b * alpha;
  }

  void render(
    SDL_Window *window, 
    const Game_State &previous_state,
    const Game_State &state,
    const f32 alpha
  ) {
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_rect(Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), Rgb(0.f, 0.f, 0.f));

    const Vec2<f32> left_paddle = this->lerp(
      previous_state.left_paddle, 
      state.left_paddle, 
      alpha
    );
    draw_paddle(left_paddle);

    const Vec2<f32> right_paddle = this->lerp(
      previous_state.right_paddle, 
      state.right_paddle, 
      alpha
    );
    draw_paddle(right_paddle);

    const Vec2<f32> ball = this->lerp(
      previous_state.ball.position, 
      state.ball.position, 
      alpha
    );
    draw_circle(ball, BALL_RADIUS, state.ball.colour);

    glFlush();
    SDL_GL_SwapWindow(window);
  }
};