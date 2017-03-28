#pragma once

#include "Matrix.h"

class Quaternion
{
public:

	float                       x;
	float                       y;
	float                       z;
	float                       w;

	Quaternion(Vector3f axis, float angleRadian) :
		x(axis.x * sinf(angleRadian / 2)),
		y(axis.y * sinf(angleRadian / 2)),
		z(axis.z * sinf(angleRadian / 2)),
		w(cosf(angleRadian / 2)) {}
	Quaternion(float w, float x, float y, float z) :
		x(x), y(y), z(z), w(w) {}
	Quaternion() :
		x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion(const Vector3f& direction, const Vector3f& up);
	Quaternion(const Matrix4f& sourceMatrix);

	Quaternion                  operator-() const;
	Quaternion                  operator*(const Quaternion& other) const;
	Quaternion&                 operator*=(const Quaternion& other);
	float                       Dot(const Quaternion& other) const;
	Quaternion                  Slerp(const Quaternion& other, float slerpFactor) const;
	Matrix4f                    ToMatrix() const;
	Quaternion&                 Yaw(float angleRadian);
	Quaternion&                 Pitch(float angleRadian);
	Quaternion&                 Roll(float angleRadian);
	Vector3f                    GetForwardVector() const;
	Vector3f                    GetRightVector() const;
	Vector3f                    GetUpVector() const;
	static Quaternion           YawQuaternion(float angleRadian);
	static Quaternion           PitchQuaternion(float angleRadian);
	static Quaternion           RollQuaternion(float angleRadian);
};