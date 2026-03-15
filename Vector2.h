#pragma once

#include <cmath>

class Vector2
{
public:
	float x, y;

	Vector2() { x = 0.f; y = 0.f; }
	Vector2(float x, float y) :x(x), y(y) {}
	Vector2(float v) { x = y = v; }
	Vector2(const Vector2& v) { x = v.x; y = v.y; }

	float Module() const
	{
		return std::sqrt(x * x + y * y);
	}

	float SquareModule() const
	{
		return x * x + y * y;
	}

	Vector2 Normalized() const
	{
		return Vector2(x / Module(), y / Module());
	}

	Vector2 normalize()
	{
		float m = Module();
		x /= m;
		y /= m;

		return *this;
	}

	Vector2 rotate(float degrees) const
	{
		float rads = degrees * 3.14159 / 180;
		float _x = x * cos(rads) - y * sin(rads);
		float _y = x * sin(rads) + y * cos(rads);
		
		return Vector2(_x, _y);
	}

	Vector2 operator+=(const Vector2& v2)
	{
		x += v2.x;
		y += v2.y;

		return *this;
	}

	Vector2 operator-=(const Vector2& v2)
	{
		x -= v2.x;
		y -= v2.y;

		return *this;
	}

	Vector2 operator+(const Vector2& v2) const
	{
		return Vector2(x + v2.x, y + v2.y);
	}

	Vector2 operator-(const Vector2& v2) const
	{
		return Vector2(x - v2.x, y - v2.y);
	}

	Vector2 operator*(float scl) const
	{
		return Vector2(x*scl, y*scl);
	}

	Vector2 operator/(float scl) const
	{
		float inv = 1 / scl;
		return *this*inv;
	}
};

