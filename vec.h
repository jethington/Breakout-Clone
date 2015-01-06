#ifndef VEC_H_
#define VEC_H_

struct Vector2f {
  Vector2f();
  Vector2f(float xVal, float yVal);
  ~Vector2f();
  float x;
  float y;
  float distance(Vector2f other);
};



#endif // VEC_H_
