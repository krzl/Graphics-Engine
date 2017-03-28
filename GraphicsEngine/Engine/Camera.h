#pragma once

#include "Math\Matrix.h"

namespace Kz
{
	class Camera
	{
	private:

		Vector3f                    m_position;
		Vector3f                    m_lookAtPoint;
		float                       m_yaw;
		float                       m_pitch;

	public:

		Camera() :
			m_yaw(0),
			m_pitch(0),
			m_position(Vector3f()),
			m_lookAtPoint(Vector3f(0.0f, 0.0f, 0.0f))
		{}

		Camera(Vector3f position, Vector3f lookAtPoint) :
			m_yaw(0),
			m_pitch(0),
			m_position(position),
			m_lookAtPoint(lookAtPoint)
		{}

		Matrix4f                    GetLookAtMatrix() const;
		Vector3f                    GetPosition() const;
		Vector3f                    GetUp() const;
		Vector3f                    GetLookAt() const;
		void                        SetPosition(const Vector3f& cameraPosition);
		void                        SetLookAt(const Vector3f& lookAt);
		void                        Translate(const Vector3f& translationVector);
		void                        MoveCamera(float yaw, float pitch);
	};
}