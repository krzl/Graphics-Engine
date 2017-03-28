#include "stdafx.h"
#include "Camera.h"

namespace Kz
{
	Matrix4f Camera::GetLookAtMatrix() const
	{
		Vector3f zAxis = Vector3f(m_lookAtPoint - m_position).Normalize();
		Vector3f xAxis = (Vector3f(0.0f, 1.0f, 0.0f).Cross(zAxis)).Normalize();
		Vector3f yAxis = zAxis.Cross(xAxis);

		Matrix4f leftMatrix;

		leftMatrix[3]	= -m_position.x;
		leftMatrix[7]	= -m_position.y;
		leftMatrix[11]	= -m_position.z;

		Matrix4f rightMatrix;
		rightMatrix.Insert(Vector4f(xAxis, 0.0f), 0);
		rightMatrix.Insert(Vector4f(yAxis, 0.0f), 1);
		rightMatrix.Insert(Vector4f(zAxis, 0.0f), 2);

		Matrix4f ret = (leftMatrix * rightMatrix).Transpose();
		return ret;
	}

	Vector3f Camera::GetPosition() const
	{
		return m_position;
	}

	Vector3f Camera::GetLookAt() const
	{
		return m_lookAtPoint;
	}

	Vector3f Camera::GetUp() const
	{
		Vector3f zAxis = Vector3f(m_lookAtPoint - m_position).Normalize();
		Vector3f xAxis = (Vector3f(0.0f, 1.0f, 0.0f).Cross(zAxis)).Normalize();
		Vector3f yAxis = zAxis.Cross(xAxis);
		return yAxis;
	}

	void Camera::SetLookAt(const Vector3f& lookAt)
	{
		m_lookAtPoint = lookAt;
	}

	void Camera::SetPosition(const Vector3f& newPosition)
	{
		m_position = newPosition;
	}

	void Camera::MoveCamera(float yaw, float pitch)
	{
		m_pitch	-= pitch;
		m_yaw	+= yaw;

		m_pitch = fmin(m_pitch, 1.56f);
		m_pitch = fmax(m_pitch, -1.56f);

		Vector3f newFront;
		newFront.x = cosf(m_yaw) * cosf(m_pitch);
		newFront.y = sinf(m_pitch);
		newFront.z = sinf(m_yaw) * cosf(m_pitch);
		m_lookAtPoint - m_position = newFront.Normalize();
	}

	void Camera::Translate(const Vector3f& translation)
	{
		Vector3f zAxis = Vector3f(m_lookAtPoint - m_position).Normalize();
		Vector3f xAxis = (Vector3f(0.0f, 1.0f, 0.0f).Cross(zAxis)).Normalize();
		Vector3f yAxis = zAxis.Cross(xAxis);

		Vector3f positionoff = xAxis * translation.x + yAxis * translation.y - zAxis * translation.z;
		positionoff = -positionoff;
		m_position += positionoff;
	}
}