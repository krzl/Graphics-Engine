#pragma once

namespace Kz
{
	class Mesh
	{
	public:

		virtual void BindMesh() = 0;
		virtual void Render() = 0;
	};
}