#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"

class Quaternion;

template<typename T>
class Matrix3
{
private:

	T							data[9];

public:

	Matrix3();
	Matrix3(T val);
	Matrix3(T* data);
	Matrix3(const Matrix3<T>& source);

	Matrix3<T>&					operator+=(const Matrix3<T>& other);
	Matrix3<T>					operator*(const Matrix3<T>& other) const;
	Vector3<T>					operator*(const Vector3<T>& other) const;
	Matrix3<T>					operator*(T val) const;
	T&							operator[](int index);
	T							operator[](int index) const;
	T*							GetData();
};

template<typename T>
class Matrix4
{
	friend class Quaternion;

private:

	T							data[16];

public:

	Matrix4();
	Matrix4(T val);
	Matrix4(const T* data);
	Matrix4(const Matrix4<T>& source);
	Matrix4(const Vector3<T>& dir, const Vector3<T>& up);

	inline T					operator[](int index) const { return data[index]; }
	inline T&					operator[](int index) { return data[index]; }
	Matrix4<T>					operator*(Matrix4<T>& other);
	Vector4<T>					operator*(const Vector4<T>& vector) const;
	Matrix4<T>					operator*(T val) const;
	Matrix4<T>&					operator*=(T val);
	Matrix4<T>&					operator+=(const Matrix4<T>& other);
	T*							GetData();
	const T*					GetData() const;
	T							GetDeterminant() const;
	void						Insert(const Vector4<T>& vector, int columnIndex);
	Matrix4<T>&					Transpose();
	Matrix4<T>					Lerp(Matrix4<T>& other, float lerpFactor);
	static Matrix4<T>			OrthogonalMatrix(float xNeg, float xPos, float yNeg, float yPos, float zNeg, float zPos);
	static Matrix4<T>			ProjectionMatrix(float ar, float hFov, float zFar, float zNear);
	static Matrix4<T>			TranslationMatrix(const Vector3<T>& other);
	static Matrix4<T>			YawMatrix(float angleRadian);
	static Matrix4<T>			PitchMatrix(float angleRadian);
	static Matrix4<T>			RollMatrix(float angleRadian);
	static Matrix4<T>			RotationMatrix(const Quaternion& rotation);
	static Matrix4<T>			ScaleMatrix(const Vector3<T>& scaleVector);
	explicit					operator Matrix3<T>() const;
};

typedef Matrix3<float> Matrix3f;
typedef Matrix4<float> Matrix4f;

#include "Matrix.cpp"

#endif