#ifndef MATRIX_CPP
#define MATRIX_CPP

#include <emmintrin.h>
#include <cstring>

#include "Matrix.h"

template<typename T>
Matrix3<T>::Matrix3()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			data[i * 3 + j] = (T)(i == j ? 1 : 0);
		}
	}
}

template<typename T>
Matrix3<T>::Matrix3(T val)
{
	for (int i = 0; i < 9; i++)
	{
		data[i] = val;
	}
}

template<typename T>
Matrix3<T>::Matrix3(T* data)
{
	for (int i = 0; i < 9; i++)
	{
		this->data[i] = data[i];
	}
}

template<typename T>
Matrix3<T>::Matrix3(const Matrix3<T>& otherM)
{
	for (int i = 0; i < 9; i++)
	{
		this->data[i] = otherM[i];
	}
}

template<typename T>
T Matrix3<T>::operator[](int index) const
{
	return data[index];
}

template<typename T>
T& Matrix3<T>::operator[](int index)
{
	return data[index];
}

template<typename T>
T* Matrix3<T>::GetData()
{
	return data;
}

template<typename T>
Matrix3<T>& Matrix3<T>::operator+=(const Matrix3<T>& otherM)
{
	for (int i = 0; i < 9; i++)
	{
		data[i] += otherM[i];
	}
	return *this;
}

template<typename T>
Matrix3<T> Matrix3<T>::operator*(const Matrix3<T>& otherM) const
{
	T newData[9];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newData[i * 3 + j] = 0;
			for (int k = 0; k < 3; k++)
			{
				newData[i * 3 + j] += data[k * 3 + j] * otherM[i * 3 + k];
			}
		}
	}

	return Matrix3<T>(newData);
}

template<typename T>
Vector3<T> Matrix3<T>::operator*(const Vector3<T>& vec) const
{
	T newData[3];

	for (int i = 0; i < 3; i++)
	{
		newData[i] = 0;
		for (int j = 0; j < 3; j++)
		{
			newData[i] += data[j * 3 + i] * vec[j];
		}
	}

	return Vector3<T>(newData[0], newData[1], newData[2]);
}

template<typename T>
Matrix3<T> Matrix3<T>::operator*(T val) const
{
	T newData[9];

	for (int i = 0; i < 9; i++)
	{
		newData[i] = data[i] * val;
	}

	return Matrix3<T>(newData);
}

template<typename T>
Matrix4<T>::Matrix4()
{
	memset(data, 0, sizeof(T) * 15);
	data[0] = data[5] = data[10] = data[15] = (T)1;
}

template<typename T>
Matrix4<T>::Matrix4(T val)
{
	for (int i = 0; i < 16; i++)
	{
		data[i] = val;
	}
}

template<typename T>
Matrix4<T>::Matrix4(const T* data)
{
	__m128 a_line;

	for (int i = 0; i < 16; i += 4)
	{
		a_line = _mm_loadu_ps(&data[i]);
		_mm_storeu_ps(&(this->data[i]), a_line);
	}
}

template<typename T>
Matrix4<T>::Matrix4(const Matrix4<T>& otherM)
{
	__m128 a_line;

	for (int i = 0; i < 16; i += 4)
	{
		a_line = _mm_loadu_ps(&otherM.data[i]);
		_mm_storeu_ps(&(data[i]), a_line);
	}
}

template<typename T>
Matrix4<T>::Matrix4(const Vector3<T>& dir, const Vector3<T>& up)
	: Matrix4<T>(static_cast<T>(0))
{
	Vector3f zAxis = dir;
	zAxis.Normalize();
	Vector3f xAxis = (up.Cross(zAxis)).Normalize();
	Vector3f yAxis = zAxis.Cross(xAxis);



	Matrix4f leftMatrix;

	Matrix4f rightMatrix;
	rightMatrix.Insert(Vector4f(xAxis, 0.0f), 0);
	rightMatrix.Insert(Vector4f(yAxis, 0.0f), 1);
	rightMatrix.Insert(Vector4f(zAxis, 0.0f), 2);

	operator+=(rightMatrix.Transpose());
}

template<typename T>
Matrix4<T> Matrix4<T>::operator*(Matrix4<T>& otherM)
{
	Matrix4f matrix((T)0);

	/*
	__m128 a_line, b_line, r_line;
	int i, j;
	for (i = 0; i < 16; i += 4)
	{
		a_line = _mm_loadu_ps(&data[0]);
		b_line = _mm_set1_ps(otherM[i]);
		r_line = _mm_mul_ps(a_line, b_line);
		for (j = 1; j < 4; j++) 
		{
			a_line = _mm_loadu_ps(&data[j * 4]);
			b_line = _mm_set1_ps(otherM[i + j]);
			r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
		}
		_mm_storeu_ps(&matrix[i], r_line);
	}
	*/

	__m128 a_line, b_line, r_line;

	a_line = _mm_loadu_ps(&data[0]);
	b_line = _mm_set1_ps(otherM[0]);
	r_line = _mm_mul_ps(a_line, b_line);
	a_line = _mm_loadu_ps(&data[4]);
	b_line = _mm_set1_ps(otherM[1]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	a_line = _mm_loadu_ps(&data[8]);
	b_line = _mm_set1_ps(otherM[2]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	a_line = _mm_loadu_ps(&data[12]);
	b_line = _mm_set1_ps(otherM[3]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	_mm_storeu_ps(&matrix[0], r_line);

	a_line = _mm_loadu_ps(&data[0]);
	b_line = _mm_set1_ps(otherM[4]);
	r_line = _mm_mul_ps(a_line, b_line);
	a_line = _mm_loadu_ps(&data[4]);
	b_line = _mm_set1_ps(otherM[5]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	a_line = _mm_loadu_ps(&data[8]);
	b_line = _mm_set1_ps(otherM[6]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	a_line = _mm_loadu_ps(&data[12]);
	b_line = _mm_set1_ps(otherM[7]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	_mm_storeu_ps(&matrix[4], r_line);

	a_line = _mm_loadu_ps(&data[0]);
	b_line = _mm_set1_ps(otherM[8]);
	r_line = _mm_mul_ps(a_line, b_line);
	a_line = _mm_loadu_ps(&data[4]);
	b_line = _mm_set1_ps(otherM[9]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	a_line = _mm_loadu_ps(&data[8]);
	b_line = _mm_set1_ps(otherM[10]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	a_line = _mm_loadu_ps(&data[12]);
	b_line = _mm_set1_ps(otherM[11]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	_mm_storeu_ps(&matrix[8], r_line);

	a_line = _mm_loadu_ps(&data[0]);
	b_line = _mm_set1_ps(otherM[12]);
	r_line = _mm_mul_ps(a_line, b_line);
	a_line = _mm_loadu_ps(&data[4]);
	b_line = _mm_set1_ps(otherM[13]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	a_line = _mm_loadu_ps(&data[8]);
	b_line = _mm_set1_ps(otherM[14]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	a_line = _mm_loadu_ps(&data[12]);
	b_line = _mm_set1_ps(otherM[15]);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	_mm_storeu_ps(&matrix[12], r_line);

	return matrix;
}

template<typename T>
Vector4<T> Matrix4<T>::operator*(const Vector4<T>& vec) const
{
	/*T newData[4];
	for (int i = 0; i < 4; i++)
	{
		newData[i] = 0;
		for (int j = 0; j < 4; j++)
		{
		newData[i] += data[j * 4 + i] * vec[j];
		}
	}
	return Vector4<T>(newData[0], newData[1], newData[2], newData[3]);*/

	Vector4<T> newVec;

	__m128 a_line, b_line, r_line;

	a_line = _mm_loadu_ps(&data[0]);
	b_line = _mm_set1_ps(vec.x);
	r_line = _mm_mul_ps(a_line, b_line);
	a_line = _mm_loadu_ps(&data[4]);
	b_line = _mm_set1_ps(vec.y);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	a_line = _mm_loadu_ps(&data[8]);
	b_line = _mm_set1_ps(vec.z);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	a_line = _mm_loadu_ps(&data[12]);
	b_line = _mm_set1_ps(vec.w);
	r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
	_mm_storeu_ps(&newVec.x, r_line);

	return newVec;
}

template<typename T>
Matrix4<T> Matrix4<T>::operator*(T val) const
{
	T newData[16];

	for (int i = 0; i < 16; i++)
	{
		newData[i] = data[i] * val;
	}

	return Matrix4<T>(newData);
}

template<typename T>
Matrix4<T>& Matrix4<T>::operator*=(T val)
{
	for (int i = 0; i < 16; i++)
	{
		data[i] *= val;
	}
	return *this;
}

template<typename T>
Matrix4<T>& Matrix4<T>::operator+=(const Matrix4<T>& otherM)
{
	for (int i = 0; i < 16; i++)
	{
		data[i] += otherM[i];
	}
	return *this;
}

template<typename T>
void Matrix4<T>::Insert(const Vector4<T>& vec, int columnIndex)
{
	for (int i = 0; i < 4; i++)
	{
		data[columnIndex * 4 + i] = vec[i];
	}
}

template<typename T>
Matrix4<T>& Matrix4<T>::Transpose()
{
	T newData[16];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			newData[i * 4 + j] = data[j * 4 + i];
		}
	}

	for (int i = 0; i < 16; i++)
	{
		data[i] = newData[i];
	}

	return *this;
}

template<typename T>
T Matrix4<T>::GetDeterminant() const
{
	return
		data[3] * data[6] * data[9]  * data[12] - data[2] * data[7] * data[9]  * data[12] -
		data[3] * data[5] * data[10] * data[12] + data[1] * data[7] * data[10] * data[12] +
		data[2] * data[5] * data[11] * data[12] - data[1] * data[6] * data[11] * data[12] -
		data[3] * data[6] * data[8]  * data[13] + data[2] * data[7] * data[8]  * data[13] +
		data[3] * data[4] * data[10] * data[13] - data[0] * data[7] * data[10] * data[13] -
		data[2] * data[4] * data[11] * data[13] + data[0] * data[6] * data[11] * data[13] +
		data[3] * data[5] * data[8]  * data[14] - data[1] * data[7] * data[8]  * data[14] -
		data[3] * data[4] * data[9]  * data[14] + data[0] * data[7] * data[9]  * data[14] +
		data[1] * data[4] * data[11] * data[14] - data[0] * data[5] * data[11] * data[14] -
		data[2] * data[5] * data[8]  * data[15] + data[1] * data[6] * data[8]  * data[15] +
		data[2] * data[4] * data[9]  * data[15] - data[0] * data[6] * data[9]  * data[15] -
		data[1] * data[4] * data[10] * data[15] + data[0] * data[5] * data[10] * data[15];
}

template<typename T>
T* Matrix4<T>::GetData()
{
	return data;
}

template<typename T>
const T* Matrix4<T>::GetData() const
{
	return data;
}

template<typename T>
Matrix4<T> Matrix4<T>::OrthogonalMatrix(float xNeg, float xPos, float yNeg, float yPos, float zNeg, float zPos)
{
	Matrix4f matrix;

	matrix.data[0] = 2 / (xPos - xNeg);
	matrix.data[5] = 2 / (yPos - yNeg);
	matrix.data[10] = 2 / (zPos - zNeg);
	matrix.data[12] = -(xPos + xNeg) / (xPos - xNeg);
	matrix.data[12] = -(yPos + yNeg) / (yPos - yNeg);
	matrix.data[12] = -(zPos + zNeg) / (zPos - zNeg);

	return matrix;
}

template<typename T>
Matrix4<T> Matrix4<T>::ProjectionMatrix(float ar, float hFov, float zFar, float zNear)
{
	T tanFov = tanf(hFov / 2.0f * 3.141592653589793f / 180.0f);
	T data[16];

	for (int i = 0; i < 16; i++)
	{
		data[i] = 0;
	}

	data[0] = 1.0f / tanFov;
	data[5] = ar / tanFov;
	data[10] = -zFar / (zFar - zNear);
	data[11] = -1.0f;
	data[14] = -zFar * zNear / (zFar - zNear);

	/*
	For depth <-1, 1>
	data[10] = -(zFar + zNear) / (zFar - zNear);
	data[14] = -2.0f * zFar * zNear / (zFar - zNear);
	*/
	
	Matrix4f projectionMatrix(data);

	return projectionMatrix * Matrix4f::ScaleMatrix(Vector3f(-1.0f, 1.0f, -1.0f));
}

template<typename T>
Matrix4<T> Matrix4<T>::Lerp(Matrix4<T>& otherM, float t)
{
	Matrix4<T> matrix((T)0);

	for (int i = 0; i < 16; i++)
	{
		matrix[i] = data[i] * (1.0f - t) + otherM[i] * t;
	}

	return matrix;
}

template<typename T>
Matrix4<T> Matrix4<T>::TranslationMatrix(const Vector3<T>& translationVector)
{
	Matrix4<T> matrix;

	for (int i = 0; i < 3; i++)
	{
		matrix[12 + i] = translationVector[i];
	}

	return matrix;
}

template<typename T>
Matrix4<T> Matrix4<T>::YawMatrix(float angleRadian)
{
	Matrix4<T> matrix;

	matrix[0] = cosf(angleRadian);
	matrix[1] = sinf(angleRadian);
	matrix[4] = -sinf(angleRadian);
	matrix[5] = cosf(angleRadian);

	return matrix;
}

template<typename T>
Matrix4<T> Matrix4<T>::PitchMatrix(float angleRadian)
{
	Matrix4<T> matrix;

	matrix[0] = cosf(angleRadian);
	matrix[2] = -sinf(angleRadian);
	matrix[8] = sinf(angleRadian);
	matrix[10] = cosf(angleRadian);

	return matrix;
}

template<typename T>
Matrix4<T> Matrix4<T>::RollMatrix(float angleRadian)
{
	Matrix4<T> matrix;

	matrix[5] = cosf(angleRadian);
	matrix[6] = sinf(angleRadian);
	matrix[9] = -sinf(angleRadian);
	matrix[10] = cosf(angleRadian);

	return matrix;
}

template<typename T>
Matrix4<T> Matrix4<T>::RotationMatrix(const Quaternion& quaternion)
{
	return quaternion.ToMatrix();
}

template<typename T>
Matrix4<T> Matrix4<T>::ScaleMatrix(const Vector3<T>& scaleVector)
{
	Matrix4<T> matrix;

	for (int i = 0; i < 3; i++)
	{
		matrix[i * 4 + i] = scaleVector[i];
	}

	return matrix;
}

#endif