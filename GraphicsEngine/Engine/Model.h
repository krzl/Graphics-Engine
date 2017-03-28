#pragma once

#include <unordered_map>

#include "Math\Quaternion.h"

namespace Kz
{
	class Texture2D;
	class Scene;
	class TextureCreator;
	class GraphicsDevice;
	class Mesh;
	class ShaderManager;

	enum MaterialVariableType
	{
		VARTPE_NULL,
		VARTYPE_BYTE,
		VARTYPE_INT,
		VARTYPE_FLOAT,
		VARTYPE_VECTOR2,
		VARTYPE_VECTOR3,
		VARTYPE_VECTOR4,
		VARTYPE_TEXTURE
	};

	struct BoneAssignment_t
	{
		int								m_boneIndex;
		int								m_vertexIndex;
		float							m_weight;
	};

	struct Bone_t
	{
		int								m_id;
		std::string						m_name;
		std::string						m_parentName;
		Matrix4f						m_transformMatrix;
		Matrix4f						m_staticMatrix;
		std::vector<Bone_t*>			m_childBones;
	};

	struct KeyFrame_t
	{
		float							m_time;
		Matrix4f						m_translationMatrix;
		Quaternion						m_rotationQuaternion;
	};

	struct BoneKeyFrames_t
	{
		Matrix4f						m_rotationMatrix;
		Matrix4f						m_translationMatrix;
		std::vector<KeyFrame_t*>		m_keyFrames;
	};

	struct Animation_t
	{
		float							m_length;
		std::vector<BoneKeyFrames_t*>	m_boneTracks;
	};

	struct FinalKeyFrame_t
	{
		double							m_endTime;
	};

	struct MaterialVariable
	{
		void*                   data;
		int                     dataSize;
		MaterialVariableType    type;

		MaterialVariable() :
			data(NULL),
			dataSize(0),
			type(VARTPE_NULL)
		{}

		MaterialVariable(void* _data, int _dataSize, MaterialVariableType _type) :
			data(_data),
			dataSize(_dataSize),
			type(_type)
		{}
	};

	class Model
	{
		friend class EntityManager;
		friend class SceneNode;
		friend class Entity;
		friend class Material;
		friend class AnimationSystem;

	private:

		std::string											m_meshFilepath;
		Mesh*												m_mesh;

		std::vector<Bone_t*>								m_rootBones;
		std::map<std::string, Bone_t>						m_bones;
		std::map<std::string, Animation_t*>					m_animations;

		std::map<std::string, MaterialVariable>				m_materialVariables;

		Model(const std::string& meshFilepath, GraphicsDevice& gd, TextureCreator& textureCreator);

		void												SetUpBoneHierarchy(Bone_t& bone);

	public:

		~Model();

		bool												IsRigged();
		bool												LoadAnimation(const std::string& animationName);
	};
}