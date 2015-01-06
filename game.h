#ifndef GAME_H_

#include <SDL.h>

#include "vec.h"
#include "brick.h"

#define PADDLE_SPEED 0.10f
#define PADDLE_WIDTH 80.0f
#define PADDLE_HEIGHT 20.0f

#define BALL_RADIUS 10.0f
#define BRICKS_PER_ROW 10
#define X_OFFSET 0.0f
#define Y_OFFSET 0.0f

#define SPEED_FACTOR 30.0f

class game {
public:
  game(SDL_Surface* s);
  ~game();
  void movePaddle(float delta);
  void update(Uint32 timeElapsed);
  void spaceBar();
  void draw();
protected:
  brick bricks[NUMBER_OF_BRICKS];
  bool ballOverlaps(Vector2f position, int i);
  bool collideWithPaddle();
  void startNextBall();
  bool collideWithWall();
  bool collideWithRectangle(Vector2f ball, Vector2f topLeft, float width, float height);
  void collideWithBrick(int i);
  Vector2f ballPos;
  Vector2f ballSpeed;
  float paddlePos;
  SDL_Surface* screen;
  SDL_Surface* ballImage;
  int lives;
  bool ballLost;
  int collisionTimer; // must be at least PADDLE_HEIGHT in order for a collision to occur, counts up each update() call
  int brickCount;
};

#endif
