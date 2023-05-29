#pragma once

#include "types/base_types.hpp"

#include "game/game_state.hpp"
#include "game/input.hpp"
#include "game/settings.hpp"

typedef s32 Debug_UID;
static Debug_UID debug_uid_count = 0;

template<size_t L>
Debug_UID _gen_debug_uid() {
  static Debug_UID uid = debug_uid_count++;
  return uid;
}

#define GEN_DEBUG_UID() _gen_debug_uid<__LINE__>()

namespace Game {
  // Forward declarations 
  f32 calculate_collision_penetration(f32 penetration, f32 velocity);
  f32 get_random_angle_degrees();
  void restart(Game_State *game_state);
  void setup(Game_State *game_state);
  void update(Game_State *game_state, Input *input);
  void update_paddle_position(Vec2<f32> *paddle, f32 input);
  void update_ball(Game_State *game_state, f32 delta);

  f32 calculate_collision_penetration(f32 penetration, f32 velocity) {
    return abs(penetration) / velocity;
  }

  f32 get_random_angle_degrees() {
    return rand() % 90 - 45;
  }

  void restart(Game_State *game_state) {
    game_state->timeout = 2.0f;
  }

  void setup(Game_State *game_state) {
    // Paddles
    const f32 start_y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    game_state->left_paddle = { 0.0f, start_y };
    game_state->right_paddle = { SCREEN_WIDTH - PADDLE_WIDTH, start_y };

    // Ball
    game_state->ball.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

    const f32 random_direction = rand() % 2 == 0 ? -1 : 1;
    game_state->ball.direction = Vec2<f32>(random_direction, 0.0f)
      .rotate(get_random_angle_degrees());
    game_state->speed_multiplier = 1.f;
  }

  void update(Game_State *game_state, f32 delta, Input *input) {
    update_paddle_position(&game_state->left_paddle, PADDLE_SPEED * input->left_paddle * delta);
    update_paddle_position(&game_state->right_paddle, PADDLE_SPEED * input->right_paddle * delta);

    if (game_state->timeout > 0) {
      game_state->timeout -= delta;
      game_state->timeout = fmax(0.f, game_state->timeout);
      if (game_state->timeout == 0) {
        game_state->ball.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
        
        const f32 random_direction = rand() % 2 == 0 ? -1 : 1;
        game_state->ball.direction = Vec2<f32>(random_direction, 0.0f)
          .rotate(get_random_angle_degrees());
        game_state->speed_multiplier = 1.f;
      }
      return;
    }

    update_ball(game_state, delta);
  }

  void update_ball(Game_State *game_state, f32 delta) {
    Ball *ball = &game_state->ball;
    const Vec2<f32> &left_paddle = game_state->left_paddle;
    const Vec2<f32> &right_paddle = game_state->right_paddle;

    const f32 speed = BALL_SPEED * game_state->speed_multiplier * delta;
    Vec2<f32> new_position = ball->position + ball->direction * speed;

    const f32 ball_top = new_position.y - BALL_RADIUS;
    const f32 ball_bottom = new_position.y + BALL_RADIUS;
    const f32 ball_left = new_position.x - BALL_RADIUS;
    const f32 ball_right = new_position.x + BALL_RADIUS;
    
    const bool hit_left_wall = ball_left < 0;
    const bool hit_right_wall = ball_right > SCREEN_WIDTH;
    const bool hit_top_wall = ball_top < 0;
    const bool hit_bottom_wall = ball_bottom > SCREEN_HEIGHT;

    const bool y_out_of_bounds = hit_top_wall || hit_bottom_wall;
    const bool x_out_of_bounds = hit_left_wall || hit_right_wall;

    const bool hit_left_paddle = 
      ball_left < left_paddle.x + PADDLE_WIDTH && 
      ball_top < left_paddle.y + PADDLE_HEIGHT && 
      ball_bottom > left_paddle.y;

    const bool hit_right_paddle = 
      ball_right > right_paddle.x &&
      ball_top < right_paddle.y + PADDLE_HEIGHT && 
      ball_bottom > right_paddle.y;

    // DVD easter egg, thanks Phil N
    if (y_out_of_bounds && (x_out_of_bounds || hit_left_paddle || hit_right_paddle)) {
      const f32 r = .25f + (f32)rand() / RAND_MAX / 2;
      const f32 g = .25f + (f32)rand() / RAND_MAX / 2;
      const f32 b = .25f + (f32)rand() / RAND_MAX / 2;
      ball->colour = Rgb(r, g, b);
    }

    if (hit_left_paddle || hit_right_paddle) {
      f32 penetration = 0;
      if (hit_left_paddle) {
        penetration = calculate_collision_penetration(left_paddle.x + PADDLE_WIDTH - ball_left, speed);
      } else if (hit_right_paddle) {
        penetration = calculate_collision_penetration(right_paddle.x - ball_right, speed);
      }

      ball->position += ball->direction * speed * (1.f - penetration);
      ball->direction = Vec2<f32>(hit_left_paddle ? 1 : -1, 0).rotate(get_random_angle_degrees());
      ball->position += ball->direction * speed * penetration;

      game_state->speed_multiplier += .1f;
    } else if (x_out_of_bounds) {
      f32 penetration = 0;
      if (hit_left_wall) {
        penetration = calculate_collision_penetration(ball_left, speed);
      } else if (hit_right_wall) {
        penetration = calculate_collision_penetration(ball_right - SCREEN_WIDTH, speed);
      }

      ball->position += ball->direction * speed * (1.f - penetration);
      restart(game_state);
    } else if (y_out_of_bounds) {
      f32 penetration;
      if (hit_top_wall) {
        penetration = calculate_collision_penetration(ball_top, speed);
      } else if (hit_bottom_wall) {
        penetration = calculate_collision_penetration(SCREEN_HEIGHT - ball_bottom, speed);
      }

      ball->position += ball->direction * speed * (1.f - penetration);
      ball->direction.y -= ball->direction.y * 2;
      ball->position += ball->direction * speed * penetration;
    } else {
      ball->position += ball->direction * speed;
    }
  }

  void update_paddle_position(Vec2<f32> *paddle, f32 y_motion) {
    paddle->y += y_motion;
    paddle->y = fmax(0, fmin(SCREEN_HEIGHT - PADDLE_HEIGHT, paddle->y));
  }
};