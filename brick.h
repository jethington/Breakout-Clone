#ifndef BRICK_H_
#define BRICK_H_

#define NUMBER_OF_BRICKS 50
#define BRICK_WIDTH 64.0f
#define BRICK_HEIGHT 20.0f

#include <SDL.h>

struct brick {
  brick();
  ~brick();
  bool hit;
  Uint32 color;
};

#endif // BRICK_H_
