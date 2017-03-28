#include <algorithm>

#include "Quaternion.h"

Quaternion::Quaternion(const Matrix4f& matrix)
{
	w = sqrt(std::max(0.0f, 1 + matrix[0] + matrix[5] + matrix[10])) / 2;
	x = sqrt(std::max(0.0f, 1 + matrix[0] - matrix[5] - matrix[10])) / 2;
	y = sqrt(std::max(0.0f, 1 - matrix[0] + matrix[5] - matrix[10])) / 2;
	z = sqrt(std::max(0.0f, 1 - matrix[0] - matrix[5] + matrix[10])) / 2;

	x = _copysignf(x, matrix[6] - matrix[9]);
	y = _copysignf(y, matrix[8] - matrix[2]);
	z = _copysignf(z, matrix[1] - matrix[4]);
}

Quaternion::Quaternion(const Vector3f& direction, const Vector3f& up)
	: Quaternion(Matrix4f(direction, up))
{
}

Matrix4f Quaternion::ToMatrix() const
{
	Matrix4f matrix(0.0f);

	matrix[0] = 1 - 2 * y * y - 2 * z * z;
	matrix[1] = 2 * x * y + 2 * w * z;
	matrix[2] = 2 * x * z - 2 * w * y;
	matrix[3] = 0;
	matrix[4] = 2 * x * y - 2 * w * z;
	matrix[5] = 1 - 2 * x * x - 2 * z * z;
	matrix[6] = 2 * y * z + 2 * w * x;
	matrix[7] = 0;
	matrix[8] = 2 * x * z + 2 * w * y;
	matrix[9] = 2 * y * z - 2 * w * x;
	matrix[10] = 1 - 2 * x * x - 2 * y * y;
	matrix[11] = 0;
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;

	return matrix;
}

float Quaternion::Dot(const Quaternion& other) const
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-w, -x, -y, -z);
}

Quaternion Quaternion::Slerp(const Quaternion& other, float slerpValue) const
{

	float cosOmega = Dot(other);
	Quaternion tempQ;

	if (cosOmega < 0.0f)
	{
		cosOmega = -cosOmega;
		tempQ = -other;
	}
	else
	{
		tempQ = other;
	}

	float omega = acosf(cosOmega);
	float sinOmega = sinf(omega);
	float coeffA, coeffB;

	if (sinOmega > 0.00001)
	{
		coeffA = sinf(omega * (1 - slerpValue)) / sinOmega;
		coeffB = sinf(omega * slerpValue) / sinOmega;
	}
	else
	{
		coeffA = 1 - slerpValue;
		coeffB = slerpValue;
	}

	Quaternion q;

	q.w = w * coeffA + tempQ.w * coeffB;
	q.x = x * coeffA + tempQ.x * coeffB;
	q.y = y * coeffA + tempQ.y * coeffB;
	q.z = z * coeffA + tempQ.z * coeffB;

	return q;
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	Quaternion ret;

	ret.w = other.w * w - other.x * x - other.y * y - other.z * z;
	ret.x = other.w * x + other.x * w - other.y * z + other.z * y;
	ret.y = other.w * y + other.x * z + other.y * w - other.z * x;
	ret.z = other.w * z - other.x * y + other.y * x + other.z * w;

	return ret;
}

Quaternion& Quaternion::operator*=(const Quaternion& other)
{
	Quaternion temp = *this * other;

	w = temp.w;
	x = temp.x;
	y = temp.y;
	z = temp.z;

	return *this;
}

Quaternion& Quaternion::Yaw(float angleRadian)
{
	operator*=(YawQuaternion(angleRadian));
	return *this;
}

Quaternion& Quaternion::Pitch(float angleRadian)
{
	operator*=(PitchQuaternion(angleRadian));
	return *this;
}

Quaternion& Quaternion::Roll(float angleRadian)
{
	operator*=(RollQuaternion(angleRadian));
	return *this;
}

Quaternion Quaternion::YawQuaternion(float angleRadian)
{
	return Quaternion(Vector3f::UNIT_X_POS, angleRadian);
}

Quaternion Quaternion::PitchQuaternion(float angleRadian)
{
	return Quaternion(Vector3f::UNIT_Y_POS, angleRadian);
}

Quaternion Quaternion::RollQuaternion(float angleRadian)
{
	return Quaternion(Vector3f::UNIT_Z_POS, angleRadian);
}

Vector3f Quaternion::GetForwardVector() const
{
	return (Vector3f(
		2 * x * z - 2 * w * y,
		2 * y * z + 2 * w * x,
		1 - 2 * x * x - 2 * y * y
		)).Normalize();
}

Vector3f Quaternion::GetRightVector() const
{
	return (Vector3f(
		1 - 2 * y * y - 2 * z * z,
		2 * x * y - 2 * w * z,
		2 * x * z + 2 * w * y
		)).Normalize();
}

Vector3f Quaternion::GetUpVector() const
{
	return (Vector3f(
		2 * x * y + 2 * w * z,
		1 - 2 * x * x - 2 * z * z,
		2 * y * z - 2 * w * x
		)).Normalize();
}