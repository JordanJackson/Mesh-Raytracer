#pragma once

template<typename T>
class Matrix4x4;

// Generic Vector3 class
template<typename T>
class Vec3
{
public:
	Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec3(T scalar) : x(scalar), y(scalar), z(scalar) {}
	Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

	Vec3 operator + (const Vec3 &v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}
	Vec3 operator - (const Vec3 &v) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}
	Vec3 operator - () const
	{
		return Vec3(-x, -y, -z);
	}

	Vec3 operator * (const T &r) const
	{
		return Vec3(x * r, y * r, z * r);
	}
	Vec3 operator * (const Vec3 &v) const
	{
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	Vec3& operator /= (const T &r)
	{
		x /= r, y /= r, z /= r; return *this;
	}
	Vec3& operator *= (const T &r)
	{
		x *= r, y *= r, z *= r; return *this;
	}

	T DotProduct(const Vec3<T> &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	Vec3 CrossProduct(const Vec3<T> &v) const
	{
		return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	T Norm() const
	{
		return x * x + y * y + z * z;
	}
	T Length() const
	{
		return sqrt(Norm());
	}
	Vec3& Normalize()
	{
		T n = Norm();
		if (n > 0)
		{
			T factor = 1 / sqrt(n);
			x *= factor, y *= factor, z *= factor;
		}
		return *this;
	}
	template<typename S>
	Vec3<S> operator * (const Matrix4x4<S> &mat)
	{
		S a, b, c;
		a = x * mat[0][0] + y * mat[1][0] + z * mat[2][0];
		b = x * mat[0][1] + y * mat[1][1] + z * mat[2][1];
		c = x * mat[0][2] + y * mat[1][2] + z * mat[2][2];

		return Vec3(a, b, c);
	}
	// Access operators
	const T& operator [] (uint8_t i) const { return (&x)[i]; }
	T& operator [] (uint8_t i) { return (&x)[i]; }

	// Friend functions
	friend Vec3 operator * (const T &r, const Vec3 &v)
	{
		return Vec3<T>(v.x * r, v.y * r, v.z * r);
	}
	friend Vec3 operator / (const T &r, const Vec3 &v)
	{
		return Vec3<T>(v.x / r, v.y / r, v.z / r);
	}

	friend std::ostream& operator << (std::ostream &s, const Vec3<T> &v)
	{
		return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
	}

	T x, y, z;
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;