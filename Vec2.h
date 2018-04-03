#pragma once

// Generic Vector2 class
template<typename T>
class Vec2
{
public:
	Vec2() : x(0), y(0) {}
	Vec2(T scalar) : x(scalar), y(scalar) {}
	Vec2(T x, T y) : x(x), y(y) {}

	Vec2 operator + (const Vec2 &v) const
	{
		return Vec2(x + v.x, y + v.y);
	}
	Vec2 operator - (const Vec2 &v) const
	{
		return Vec2(x - v.x, y - v.y);
	}
	Vec2 operator * (const T &s) const
	{
		return Vec2(x * s, y * s);
	}
	Vec2 operator / (const T &s) const
	{
		return Vec2(x / s, y / s);
	}

	Vec2 operator *= (const T &r)
	{
		x *= r, y *= r;
		return *this;
	}
	Vec2& operator /= (const T &r)
	{
		x /= r, y /= r;
		return *this;
	}

	friend std::ostream& operator << (std::ostream &s, const Vec2<T> &v)
	{
		return s << '[' << v.x << ' ' << v.y << ']';
	}
	friend Vec2 operator * (const T & r, const Vec2<T> &v)
	{
		return Vec2(v.x * r, v.y * r);
	}

	T x, y;
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;