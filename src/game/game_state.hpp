#pragma once 

#include "types/array.hpp"
#include "types/base_types.hpp"
#include "types/vector.hpp"

struct Ball {
  Vec2<f32> position = {};
  Vec2<f32> direction = {};
  Rgb colour = Rgb(1, 1, 1);
};

struct Game_State {
  Vec2<f32> left_paddle = {};
  Vec2<f32> right_paddle = {};
  Ball ball = {};
  f32 timeout = 0.f;
  f32 speed_multiplier = 1.f;
};