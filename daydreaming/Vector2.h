#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <math.h>


///Vector struct
///Partly modified, source from:-
///Ather Omar(April 30, 2017). "SDL 2.0 Tutorials: Galaga - 12.Enemy Paths using Bezier Curves"
///Available at: https://www.youtube.com/watch?v=qoIYkCwWLkA&t=148s
struct Vector2 {
	float x;
	float y;
	Vector2(float _x = 0.0f, float _y = 0.0f) :
		x(_x), y(_y) {}
	float magnitude()
	{
		return (float)sqrt(x * x + y * y);
	}
	Vector2 normalized()
	{
		float _magnitude = magnitude();
		Vector2 result{ x / _magnitude, y / _magnitude };
		return result;
	}
};
inline Vector2 operator +(const Vector2& left, const Vector2& right)
{
	return Vector2(left.x + right.x, left.y + right.y);
}
inline Vector2 operator -(const Vector2& left, const Vector2& right)
{
	return Vector2(left.x - right.x, left.y - right.y);
}
inline Vector2 operator *(const Vector2& left, const float& right)
{
	return Vector2(left.x * right, left.y * right);
}
inline bool operator !=(const Vector2& left, const Vector2& right)
{
	return left.x != right.x && left.y != right.y ? true : false;
}

inline Vector2 lerp(const Vector2& start, const Vector2& target, float time)
{
	Vector2 direction{ (target - start).normalized() };
	float magnitude{ (target - start).magnitude() };
	return { start + direction * magnitude*time };
}

#endif