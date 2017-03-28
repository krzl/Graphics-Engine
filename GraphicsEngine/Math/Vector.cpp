#ifndef VECTOR_CPP
#define VECTOR_CPP

#include "Vector.h"

template<typename T>
Vector2<T> Vector2<T>::operator-() const
{
	return Vector2<T>(-x, -y);
}

template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const
{
	return Vector2<T>(x - other.x, y - other.y);
}

template<typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const
{
	return Vector2<T>(x + other.x, y + other.y);
}

template<typename T>
Vector2<T> Vector2<T>::operator*(float scalar) const
{
	return Vector2<T>(x * scalar, y * scalar);
}

template<typename T>
Vector2<T>& Vector2<T>::operator=(const Vector2<T>& right)
{
	x = right.x;
	y = right.y;

	return *this;
}

template<typename T>
Vector3<T> Vector3<T>::UNIT_X_POS = Vector3<T>::Vector3(1, 0, 0);

template<typename T>
Vector3<T> Vector3<T>::UNIT_Y_POS = Vector3<T>::Vector3(0, 1, 0);

template<typename T>
Vector3<T> Vector3<T>::UNIT_Z_POS = Vector3<T>::Vector3(0, 0, 1);

template<typename T>
Vector3<T> Vector3<T>::UNIT_X_NEG = Vector3<T>::Vector3(-1, 0, 0);

template<typename T>
Vector3<T> Vector3<T>::UNIT_Y_NEG = Vector3<T>::Vector3(0, -1, 0);

template<typename T>
Vector3<T> Vector3<T>::UNIT_Z_NEG = Vector3<T>::Vector3(0, 0, -1);

template<typename T>
Vector3<T> Vector3<T>::Cross(const Vector3<T> other) const
{
	return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

template<typename T>
T Vector3<T>::Dot(const Vector3<T> other) const
{
	return x * other.x + y * other.y + z * other.z;
}

template<typename T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& other) const
{
	return Vector3<T>(x - other.x, y - other.y, z - other.z);
}

template<typename T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& other) const
{
	return Vector3<T>(x + other.x, y + other.y, z + other.z);
}

template<typename T>
bool Vector3<T>::operator==(const Vector3<T>& other) const
{
	return x == other.x && y == other.y && z == other.z;
}

template<typename T>
bool Vector3<T>::operator<(const Vector3<T>& other) const
{
	return x == other.x ? (y == other.y ? z < other.z : y < other.y) : x < other.x;
}

template<typename T>
Vector3<T> Vector3<T>::operator*(float scalar) const
{
	return Vector3<T>(x * scalar, y * scalar, z * scalar);
}

template<typename T>
Vector3<T>& Vector3<T>::Normalize()
{
	T vecLength = Length();

	x /= vecLength;
	y /= vecLength;
	z /= vecLength;

	return *this;
}

template<typename T>
T Vector3<T>::Length()
{
	return (T)sqrt(Dot(*this));
}

template<typename T>
T Vector3<T>::operator[](int index) const
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}
	return 0;
}

template<typename T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& right)
{
	x = right.x;
	y = right.y;
	z = right.z;

	return *this;
}

template<typename T>
T& Vector3<T>::operator[](int index)
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}
	return 0;
}

template<typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

template<typename T>
Vector3<T> Vector3<T>::operator-() const
{
	return Vector3<T>(-x, -y, -z);
}

template<typename T>
Vector3<T>& Vector3<T>::operator*=(const Vector3<T>& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

template<typename T>
Vector4<T>::operator Vector3<T>() const
{
	return Vector3<T>(x, y, z);
}

template<typename T>
T Vector4<T>::operator[](int index) const
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}
	return 0;
}

template<typename T>
T& Vector4<T>::operator[](int index)
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}
	return 0;
}

#endif