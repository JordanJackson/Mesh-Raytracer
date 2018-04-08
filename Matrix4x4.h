#pragma once

#include <iomanip>
#include <iostream>

template<typename T>
class Vec3;

// Generic 4x4 Matrix class
template<typename T>
class Matrix4x4
{
public:
	T x[4][4] =
	{
		{ 1,0,0,0 },
		{ 0,1,0,0 },
		{ 0,0,1,0 },
		{ 0,0,0,1 }
	};

	Matrix4x4() {}

	Matrix4x4(T a, T b, T c, T d, T e, T f, T g, T h,
		T i, T j, T k, T l, T m, T n, T o, T p)
	{
		x[0][0] = a;
		x[0][1] = b;
		x[0][2] = c;
		x[0][3] = d;
		x[1][0] = e;
		x[1][1] = f;
		x[1][2] = g;
		x[1][3] = h;
		x[2][0] = i;
		x[2][1] = j;
		x[2][2] = k;
		x[2][3] = l;
		x[3][0] = m;
		x[3][1] = n;
		x[3][2] = o;
		x[3][3] = p;
	}

	const T* operator [] (uint8_t i) const { return x[i]; }
	T* operator [] (uint8_t i) { return x[i]; }

	// Multiply the current matrix with another matrix (rhs)
	Matrix4x4 operator * (const Matrix4x4 &v) const
	{
		Matrix4x4 temp;
		multiply(*this, v, temp);

		return temp;
	}

	static void multiply(const Matrix4x4<T> &a, const Matrix4x4 &b, Matrix4x4 &c)
	{
		// Restrict qualified pointers promises compilers that for duration of scope
		// the target of the pointer will only be accessed through that pointer and
		// derivative pointers
		const T * __restrict ap = &a.x[0][0];
		const T * __restrict bp = &b.x[0][0];
		T * __restrict cp = &c.x[0][0];

		T a0, a1, a2, a3;

		a0 = ap[0];
		a1 = ap[1];
		a2 = ap[2];
		a3 = ap[3];

		cp[0] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
		cp[1] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
		cp[2] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
		cp[3] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

		a0 = ap[4];
		a1 = ap[5];
		a2 = ap[6];
		a3 = ap[7];

		cp[4] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
		cp[5] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
		cp[6] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
		cp[7] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

		a0 = ap[8];
		a1 = ap[9];
		a2 = ap[10];
		a3 = ap[11];

		cp[8] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
		cp[9] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
		cp[10] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
		cp[11] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

		a0 = ap[12];
		a1 = ap[13];
		a2 = ap[14];
		a3 = ap[15];

		cp[12] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
		cp[13] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
		cp[14] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
		cp[15] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];
	}

	Matrix4x4 Transpose() const
	{
		return Matrix4x4(x[0][0],
			x[1][0],
			x[2][0],
			x[3][0],
			x[0][1],
			x[1][1],
			x[2][1],
			x[3][1],
			x[0][2],
			x[1][2],
			x[2][2],
			x[3][2],
			x[0][3],
			x[1][3],
			x[2][3],
			x[3][3]);
	}

	Matrix4x4 Transpose()
	{
		Matrix4x4 temp(x[0][0],
			x[1][0],
			x[2][0],
			x[3][0],
			x[0][1],
			x[1][1],
			x[2][1],
			x[3][1],
			x[0][2],
			x[1][2],
			x[2][2],
			x[3][2],
			x[0][3],
			x[1][3],
			x[2][3],
			x[3][3]);
		*this = temp;

		return *this;
	}

	template<typename S>
	Vec3<S> operator * (const Vec3<S> &vec)
	{
		S a, b, c, w;
		a = vec[0] * x[0][0] + vec[1] * x[1][0] + vec[2] * x[2][0] + x[3][0];
		b = vec[0] * x[0][1] + vec[1] * x[1][1] + vec[2] * x[2][1] + x[3][1];
		c = vec[0] * x[0][2] + vec[1] * x[1][2] + vec[2] * x[2][2] + x[3][2];
		w = vec[0] * x[0][3] + vec[1] * x[1][3] + vec[2] * x[2][3] + x[3][3];

		return Vec3(a / w, b / w, c / w);
	}

	template<typename S>
	void MultPointVec(const Vec3<S> &vec, Vec3<S> &dst) const
	{
		S a, b, c, w;
		a = vec[0] * x[0][0] + vec[1] * x[1][0] + vec[2] * x[2][0] + x[3][0];
		b = vec[0] * x[0][1] + vec[1] * x[1][1] + vec[2] * x[2][1] + x[3][1];
		c = vec[0] * x[0][2] + vec[1] * x[1][2] + vec[2] * x[2][2] + x[3][2];
		w = vec[0] * x[0][3] + vec[1] * x[1][3] + vec[2] * x[2][3] + x[3][3];

		dst.x = a / w;
		dst.y = b / w;
		dst.z = c / w;
	}

	template<typename S>
	void MultDirVec(const Vec3<S> &vec, Vec3<S> &dst) const
	{
		S a, b, c;
		a = vec[0] * x[0][0] + vec[1] * x[1][0] + vec[2] * x[2][0];
		b = vec[0] * x[0][1] + vec[1] * x[1][1] + vec[2] * x[2][1];
		c = vec[0] * x[0][2] + vec[1] * x[1][2] + vec[2] * x[2][2];

		dst.x = a;
		dst.y = b;
		dst.z = c;
	}

	template<typename S>
	Matrix4x4<S> Translate(Matrix4x4<S> &matrix, Vec3<S> &vec)
	{
		Matrix4x4<S> result;
		result[3] = 
			vec[0] * matrix[0] +
			vec[1] * matrix[1] +
			vec[2] * matrix[2] +
			matrix[3];
		return result;
	}

	Matrix4x4 Inverse() const
	{
		int i, j, k;
		Matrix4x4 s;
		Matrix4x4 t(*this);

		// Forward elimination
		for (i = 0; i < 3; i++) {
			int pivot = i;

			T pivotsize = t[i][i];

			if (pivotsize < 0)
				pivotsize = -pivotsize;

			for (j = i + 1; j < 4; j++) {
				T tmp = t[j][i];

				if (tmp < 0)
					tmp = -tmp;

				if (tmp > pivotsize) {
					pivot = j;
					pivotsize = tmp;
				}
			}

			if (pivotsize == 0) {
				// Cannot invert singular matrix
				return Matrix4x4();
			}

			if (pivot != i) {
				for (j = 0; j < 4; j++) {
					T tmp;

					tmp = t[i][j];
					t[i][j] = t[pivot][j];
					t[pivot][j] = tmp;

					tmp = s[i][j];
					s[i][j] = s[pivot][j];
					s[pivot][j] = tmp;
				}
			}

			for (j = i + 1; j < 4; j++) {
				T f = t[j][i] / t[i][i];

				for (k = 0; k < 4; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		// Backward substitution
		for (i = 3; i >= 0; --i) {
			T f;

			if ((f = t[i][i]) == 0) {
				// Cannot invert singular matrix
				return Matrix4x4();
			}

			for (j = 0; j < 4; j++) {
				t[i][j] /= f;
				s[i][j] /= f;
			}

			for (j = 0; j < i; j++) {
				f = t[j][i];

				for (k = 0; k < 4; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		return s;
	}

	const Matrix4x4<T>& Invert()
	{
		*this = Inverse();
		return *this;
	}

	friend std::ostream& operator << (std::ostream &s, const Matrix4x4 &m)
	{
		std::ios_base::fmtflags oldFlags = s.flags();
		int width = 12; // total with of the displayed number
		s.precision(5); // control the number of displayed decimals
		s.setf(std::ios_base::fixed);

		s << "[" << std::setw(width) << m[0][0] <<
			" " << std::setw(width) << m[0][1] <<
			" " << std::setw(width) << m[0][2] <<
			" " << std::setw(width) << m[0][3] << "\n" <<

			" " << std::setw(width) << m[1][0] <<
			" " << std::setw(width) << m[1][1] <<
			" " << std::setw(width) << m[1][2] <<
			" " << std::setw(width) << m[1][3] << "\n" <<

			" " << std::setw(width) << m[2][0] <<
			" " << std::setw(width) << m[2][1] <<
			" " << std::setw(width) << m[2][2] <<
			" " << std::setw(width) << m[2][3] << "\n" <<

			" " << std::setw(width) << m[3][0] <<
			" " << std::setw(width) << m[3][1] <<
			" " << std::setw(width) << m[3][2] <<
			" " << std::setw(width) << m[3][3] << "]";

		s.flags(oldFlags);
		return s;
	}
};

typedef Matrix4x4<float> Matrix4x4f;