#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

template<typename T>
class Vector2
{
public:

	T							x;
	T							y;

	Vector2() :
		x(0), y(0) {};
	Vector2(T val) :
		x(val), y(val) {};
	Vector2(T x, T y) :
		x(x), y(y) {};

	Vector2<T>					operator-() const;
	Vector2<T>					operator-(const Vector2<T>& other) const;
	Vector2<T>					operator+(const Vector2<T>& other) const;
	Vector2<T>					operator*(float value) const;
	Vector2<T>&					operator=(const Vector2<T>& other);
};

template<typename T>
class Vector3
{
public:

	T							x;
	T							y;
	T							z;

	static Vector3<T>			UNIT_X_POS;
	static Vector3<T>			UNIT_Y_POS;
	static Vector3<T>			UNIT_Z_POS;
	static Vector3<T>			UNIT_X_NEG;
	static Vector3<T>			UNIT_Y_NEG;
	static Vector3<T>			UNIT_Z_NEG;

	Vector3() :
		x(0), y(0), z(0) {};
	Vector3(T val) :
		x(val), y(val), z(val) {};
	Vector3(T x, T y, T z) :
		x(x), y(y), z(z) {};

	Vector3<T>					operator-() const;
	Vector3<T>					operator-(const Vector3<T>& other) const;
	Vector3<T>					operator+(const Vector3<T>& other) const;
	Vector3<T>					operator*(float multiplier) const;
	Vector3<T>&					operator=(const Vector3<T>& other);
	Vector3<T>&					operator+=(const Vector3<T>& other);
	Vector3<T>&					operator*=(const Vector3<T>& other);
	bool						operator==(const Vector3<T>& other) const;
	bool						operator<(const Vector3<T>& other) const;
	T							operator[](int index) const;
	T&							operator[](int index);
	Vector3<T>					Cross(const Vector3<T> other) const;
	T							Dot(const Vector3<T> other) const;
	Vector3<T>&					Normalize();
	T							Length();
};

template<typename T>
class Vector4
{

public:

	T							x;
	T							y;
	T							z;
	T							w;

	Vector4(Vector3<T> vec) :
		x(vec.x), y(vec.y), z(vec.z), w(1) {}
	Vector4(Vector3<T> vec, T w) :
		x(vec.x), y(vec.y), z(vec.z), w(w) {}
	Vector4() :
		x(0), y(0), z(0), w(0) {};
	Vector4(T val) :
		x(val), y(val), z(val), w(val) {};
	Vector4(T x, T y, T z, T w) :
		x(x), y(y), z(z), w(w) {};

	T							operator[](int index) const;
	T&							operator[](int index);
	explicit					operator Vector3<T>() const;
};

typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;

#include "Vector.cpp"

#endif