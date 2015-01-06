#include "vec.h"
#include <math.h>

Vector2f::Vector2f() {
	x = 0.0f;
	y = 0.0f;
}

Vector2f::Vector2f(float xVal, float yVal) {
	x = xVal;
	y = yVal;
}

Vector2f::~Vector2f() {

}

float Vector2f::distance(Vector2f other) {
  float dx = x - other.x;
  float dy = y - other.y;
  return (float)sqrt(dx*dx + dy*dy);
}
