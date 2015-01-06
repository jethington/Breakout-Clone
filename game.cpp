#include "game.h"
#include <iostream>
#include <cstdlib>

// defined in two places, not good
#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

game::game(SDL_Surface* s) {
  screen = s;
  for (int i = 0; i < NUMBER_OF_BRICKS; i++) {
    bricks[i] = brick();
  }
  ballPos = Vector2f(0.0f, SCREEN_HEIGHT + 100.0f);
  ballSpeed = Vector2f(0.0f, 0.0f);

  paddlePos = SCREEN_WIDTH/2.0f;

  ballLost = false;
  collisionTimer = (int)PADDLE_HEIGHT;
  lives = 3;
  brickCount = NUMBER_OF_BRICKS;
}

game::~game() {

}

void game::draw() {
  // draw ball
  // ballPos is center of ball, right?
  SDL_Rect tall;
  tall.x = ballPos.x - BALL_RADIUS/2.0f;
  tall.y = ballPos.y - BALL_RADIUS/1.0f;
  tall.h = BALL_RADIUS * 2.0f;
  tall.w = BALL_RADIUS;

  SDL_Rect wide;
  wide.x = ballPos.x - BALL_RADIUS/1.0f;
  wide.y = ballPos.y - BALL_RADIUS/2.0f;
  wide.h = BALL_RADIUS;
  wide.w = BALL_RADIUS * 2.0f;

  SDL_FillRect(screen, &tall, 0x0000007F);
  SDL_FillRect(screen, &wide, 0x0000007F);

  // draw paddle
  SDL_Rect paddleRect; // this could be static, marginal improvement?
  paddleRect.x = paddlePos - PADDLE_WIDTH/2.0f;
  paddleRect.y = SCREEN_HEIGHT - PADDLE_HEIGHT;
  paddleRect.w = PADDLE_WIDTH;
  paddleRect.h = PADDLE_HEIGHT;
  SDL_FillRect(screen, &paddleRect, 0x007F7F7F);

  // draw bricks
  for (int i = 0; i < NUMBER_OF_BRICKS; i++) {
    if (bricks[i].hit == false) {
      SDL_Rect temp;
      temp.x = (i % BRICKS_PER_ROW) * BRICK_WIDTH + X_OFFSET;
      temp.y = (i / BRICKS_PER_ROW) * BRICK_HEIGHT + Y_OFFSET;

      temp.w = BRICK_WIDTH;
      temp.h = BRICK_HEIGHT;
      SDL_FillRect(screen, &temp, bricks[i].color);
    }
  }

  // draw lives
  for (int i = 0; i < lives; i++) {
    SDL_Rect temp;
    temp.x = SCREEN_WIDTH - i*20.0f - 20.0f;
    temp.y = SCREEN_HEIGHT - 20.0f;
    temp.w = 10.0f;
    temp.h = 10.0f;
    SDL_FillRect(screen, &temp, 0x00007F00);
  }
}

void game::spaceBar(void) {
  if (ballPos.y > SCREEN_HEIGHT) {
    startNextBall();
  }
}

void game::movePaddle(float delta) {
  paddlePos = paddlePos + delta;
  if ((paddlePos + PADDLE_WIDTH/2.0f) > SCREEN_WIDTH) {
    paddlePos = SCREEN_WIDTH - PADDLE_WIDTH/2.0f;
  }
  if ((paddlePos - PADDLE_WIDTH/2.0f) < 0.0f) {
    paddlePos = PADDLE_WIDTH/2.0f;
  }
}

void game::update(Uint32 timeElapsed) {
  collisionTimer++;
  if (collisionTimer > (int)PADDLE_WIDTH) {
    collisionTimer = (int)PADDLE_WIDTH;
  }

  if (ballPos.y > (SCREEN_HEIGHT + 100.0f)) {
    return;
  }
  //std::cout << timeElapsed << std::endl;
  ballPos.x = ballPos.x + ballSpeed.x;
  ballPos.y = ballPos.y + ballSpeed.y;

  if (collideWithPaddle()) {
    return;
  }
  if (ballPos.y >= SCREEN_HEIGHT) {
    if (ballLost == false) {
      ballLost = true;
      lives--;
      if (lives == 0) {
        exit(0);
      }
    }
    return;
  }
  if (collideWithWall()) {
    return;
  }
  // check for collision with brick
  else {
    bool bounced = false;
    for (int i = 0; i < NUMBER_OF_BRICKS; i++) {
      if (bricks[i].hit == false) {
        if (ballOverlaps(ballPos, i)) {
          bricks[i].hit = true;
          brickCount--;
          if (brickCount == 0) {
            exit(0);
          }
          if (bounced == false) {
            bounced = true;
            collideWithBrick(i);
          }
        }
        else {
          // do nothing
        }
      }
    }
  }
}

void game::startNextBall(void) {
  ballPos.x = SCREEN_WIDTH/2.0f;
  ballPos.y = SCREEN_HEIGHT - 40.0f;
  ballSpeed.x = 0.02f*SPEED_FACTOR;
  ballSpeed.y = -.1f*SPEED_FACTOR;
  ballLost = false;
}

bool game::ballOverlaps(Vector2f position, int i) {
  // get brick position:
  // start at top, go across rows from left to right
  // like reading a book
  Vector2f topLeftCorner;
  topLeftCorner.x = (i % BRICKS_PER_ROW) * BRICK_WIDTH + X_OFFSET;
  topLeftCorner.y = (i / BRICKS_PER_ROW) * BRICK_HEIGHT + Y_OFFSET;

  return collideWithRectangle(position, topLeftCorner, BRICK_WIDTH, BRICK_HEIGHT);
}

bool game::collideWithPaddle(void) {
  if (collisionTimer < (int)PADDLE_HEIGHT) {
    return false;
  }

  Vector2f topLeftCorner;
  topLeftCorner.x = paddlePos - PADDLE_WIDTH/2.0f;
  topLeftCorner.y = SCREEN_HEIGHT - PADDLE_HEIGHT;

  bool result = collideWithRectangle(ballPos, topLeftCorner, PADDLE_WIDTH, PADDLE_HEIGHT);

  if (result) {
    collisionTimer = 0;
    ballSpeed.y = ballSpeed.y * -1;
    float dx = ballPos.x - paddlePos;
    // position of ball relative to paddle
    // if ball is right of center of paddle, then this will be positive

    float dv = dx / PADDLE_WIDTH * 0.2f;

    ballSpeed.x = ballSpeed.x + dv*SPEED_FACTOR;

    // keep it from getting ridiculous
    if (ballSpeed.x > (0.1f*SPEED_FACTOR)) {
      ballSpeed.x = 0.1f*SPEED_FACTOR;
    }
    else if (ballSpeed.x < (-0.1f*SPEED_FACTOR)) {
      ballSpeed.x = -0.1f*SPEED_FACTOR;
    }

    // if moving left and land on left side: dv < 0

    // if moving left and land on right side: dv > 0

    // if moving right and land on left side: dv < 0

    // if moving right and land on right side: dv > 0

  }

  return result;
}

bool game::collideWithRectangle(Vector2f ball, Vector2f topLeft, float width, float height) {
  Vector2f topRight;
  topRight.x = topLeft.x + width;
  topRight.y = topLeft.y;

  Vector2f bottomLeft;
  bottomLeft.x = topLeft.x;
  bottomLeft.y = topLeft.y + height;

  Vector2f bottomRight;
  bottomRight.x = topRight.x;
  bottomRight.y = bottomLeft.y;

  // vertical rectangle
  if ((topLeft.x < ballPos.x)
       && (topRight.x > ball.x)
       && ((topLeft.y - BALL_RADIUS) < ball.y)
       && ((bottomLeft.y + BALL_RADIUS) > ball.y)) {
    return true;
  }
  // horizontal rectangle; some overlap (but still fastest?)
  if (((topLeft.x - BALL_RADIUS) < ball.x)
        && ((topRight.x + BALL_RADIUS) > ball.x)
        && (topLeft.y < ball.y)
        && (bottomLeft.y > ball.y)) {
    return true;
  }

  // radius around corners
  if ((ball.distance(topLeft) <= BALL_RADIUS)
       || (ball.distance(topRight) <= BALL_RADIUS)
       || (ball.distance(bottomLeft) <= BALL_RADIUS)
       || (ball.distance(bottomRight) <= BALL_RADIUS)) {
    return true;
  }

  return false;
}

bool game::collideWithWall(void) {
  bool xCollision = false;
  bool yCollision = false;
  if ((ballPos.x < 0.0f) || (ballPos.x > SCREEN_WIDTH)) {
    xCollision = true;
  }

  if (ballPos.y < 0.0f) {
    yCollision = true;
  }
  else if (ballPos.y > SCREEN_HEIGHT)  {

  }

  if (xCollision && yCollision) {
    // inside corner
    ballSpeed.x = ballSpeed.x * -1.0f;
    ballSpeed.y = ballSpeed.y * -1.0f;
  }
  else if (xCollision) {
    ballSpeed.x = ballSpeed.x * -1.0f;
  }
  else if (yCollision) {
    ballSpeed.y = ballSpeed.y * -1.0f;
  }
  else {
    return false;
  }
  return true;
}

// i = index of brick that ball bounced off of
void game::collideWithBrick(int i) {
  bool xCollision = false;
  bool yCollision = false;

  Vector2f prevX(ballPos);
  prevX.x = prevX.x - ballSpeed.x;
  if (ballOverlaps(prevX, i)) {
    yCollision = true;
  }

  Vector2f prevY(ballPos);
  prevY.y = prevY.y - ballSpeed.y;
  if (ballOverlaps(prevY, i)) {
    xCollision = true;
  }

  // can make the code more efficient if I wanted to
  if (xCollision && yCollision) {
    // inside corner
    ballSpeed.x = ballSpeed.x * -1.0f;
    ballSpeed.y = ballSpeed.y * -1.0f;
  }
  else if (xCollision) {
    ballSpeed.x = ballSpeed.x * -1.0f;
  }
  else if (yCollision) {
    ballSpeed.y = ballSpeed.y * -1.0f;
  }
  else {
    // outside corner
    ballSpeed.x = ballSpeed.x * -1.0f;
    ballSpeed.y = ballSpeed.y * -1.0f;
  }
}
