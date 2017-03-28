#include "stdafx.h"
#include "Model.h"
#include "Texture.h"
#include "SceneNode.h"
#include "GraphicsDevice.h"
#include "Mesh.h"
#include "TextureCreator.h"
#include "Shaders/ShaderManager.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace Kz
{
	struct Edge
	{
		int a, b;
		Edge(int _a, int _b)
		{
			a = _a < _b ? _a : _b;
			b = _a < _b ? _b : _a;
		}

		bool operator==(const Edge& other)
		{
			return (other.a == a && other.b == b) || (other.a == b && other.b == a);
		}

		bool operator<(const Edge& other) const
		{
			return a == other.a ? b < other.b : a < other.a;
		}
	};

	Model::Model(const std::string& meshFilepath, GraphicsDevice& gd, TextureCreator& textureCreator)
	{
		m_meshFilepath = meshFilepath;
		std::string materialFilename = meshFilepath.substr(0, meshFilepath.size() - 8) + "material";
		std::ifstream materialFile(materialFilename);
		int vertexCount;
		float* vertexData;
		std::string line;

		Buffer* indexBuffer = NULL;
		Buffer* vertexBuffer = NULL;

		if (materialFile.is_open())
		{
			while (std::getline(materialFile, line))
			{
				std::vector<std::string> stringSplitted;
				std::stringstream stream(line);
				std::string item;

				while (std::getline(stream, item, ' '))
				{
					stringSplitted.push_back(item);
				}

				if (stringSplitted[0] == "byte")
				{
					char* value = new char(std::stoi(stringSplitted[2]));
					m_materialVariables[stringSplitted[1]] 
						= MaterialVariable(value, sizeof(char), VARTYPE_BYTE);
				}
				else if (stringSplitted[0] == "int")
				{
					int* value = new int(std::stoi(stringSplitted[2]));
					m_materialVariables[stringSplitted[1]] 
						= MaterialVariable(value, sizeof(int), VARTYPE_INT);
				}
				else if (stringSplitted[0] == "float")
				{
					float* value = new float(std::stof(stringSplitted[2]));
					m_materialVariables[stringSplitted[1]] 
						= MaterialVariable(value, sizeof(float), VARTYPE_FLOAT);
				}
				else if (stringSplitted[0] == "vec2")
				{
					Vector2f* value = new Vector2f(std::stof(stringSplitted[2]), std::stof(stringSplitted[3]));
					m_materialVariables[stringSplitted[1]] 
						= MaterialVariable(value, sizeof(Vector2f), VARTYPE_VECTOR2);
				}
				else if (stringSplitted[0] == "vec3")
				{
					Vector3f* value = new Vector3f(std::stof(stringSplitted[2]),
						std::stof(stringSplitted[3]), std::stof(stringSplitted[4]));
					m_materialVariables[stringSplitted[1]] 
						= MaterialVariable(value, sizeof(Vector3f), VARTYPE_VECTOR3);
				}
				else if (stringSplitted[0] == "vec4")
				{
					Vector4f* value = new Vector4f(std::stof(stringSplitted[2]), std::stof(stringSplitted[3]),
						std::stof(stringSplitted[4]), std::stof(stringSplitted[5]));
					m_materialVariables[stringSplitted[1]] 
						= MaterialVariable(value, sizeof(Vector4f), VARTYPE_VECTOR4);
				}
				else if (stringSplitted[0] == "tex")
				{
					int last = 0;

					int pos = m_meshFilepath.find("\\");

					while (pos != std::string::npos)
					{
						last = pos;
						pos = m_meshFilepath.find("\\", pos + 1);
					}

					std::string textureFilapath = m_meshFilepath.substr(0, last) + "\\" + stringSplitted[2];
					Texture2D* texture = textureCreator.CreateTexture2D(textureFilapath);
					m_materialVariables[stringSplitted[1]] 
						= MaterialVariable(reinterpret_cast<void*>(texture), 0, VARTYPE_TEXTURE);
				}
			}

			materialFile.close();
		}

		rapidxml::file<> meshFile(m_meshFilepath.c_str());
		rapidxml::xml_document<> meshXML;
		try
		{
			meshXML.parse<0>(meshFile.data());
		}
		catch (rapidxml::parse_error p)
		{
			p.what();
		}

		rapidxml::xml_node<>* meshNode = meshXML.first_node();
		rapidxml::xml_node<>* sharedgeometryNode = meshNode->first_node("sharedgeometry");

		vertexCount = std::stoi(sharedgeometryNode->first_attribute("vertexcount")->value());

		vertexData = new float[vertexCount * 16];

		memset(vertexData, 0, sizeof(float) * vertexCount * 16);

		std::set<Vector3f> vertexSet;
		std::vector<int> vertexUnique;

		for (rapidxml::xml_node<>* vertexBufferNode = sharedgeometryNode->first_node(); 
			vertexBufferNode; vertexBufferNode = vertexBufferNode->next_sibling())
		{
			bool position = vertexBufferNode->first_attribute("positions") && 
				std::string(vertexBufferNode->first_attribute("positions")->value()) == "true";

			bool normal = vertexBufferNode->first_attribute("normals") && 
				std::string(vertexBufferNode->first_attribute("normals")->value()) == "true";

			bool texcoord = vertexBufferNode->first_attribute("texture_coords") && 
				std::string(vertexBufferNode->first_attribute("texture_coords")->value()) == "1";

			int i = 0;

			for (rapidxml::xml_node<>* vertexNode = vertexBufferNode->first_node(); 
				vertexNode; vertexNode = vertexNode->next_sibling())
			{
				if (position)
				{
					rapidxml::xml_node<>* position = vertexNode->first_node("position");

					float x = std::stof(position->first_attribute("x")->value());
					float y = std::stof(position->first_attribute("y")->value());
					float z = std::stof(position->first_attribute("z")->value());

					vertexData[i * 16 + 0] = x;
					vertexData[i * 16 + 1] = y;
					vertexData[i * 16 + 2] = z;

					vertexSet.emplace(x, y, z);
				}

				if (normal)
				{
					rapidxml::xml_node<>* normal = vertexNode->first_node("normal");

					float x = std::stof(normal->first_attribute("x")->value());
					float y = std::stof(normal->first_attribute("y")->value());
					float z = std::stof(normal->first_attribute("z")->value());

					vertexData[i * 16 + 3] = x;
					vertexData[i * 16 + 4] = y;
					vertexData[i * 16 + 5] = z;
				}

				if (texcoord)
				{
					rapidxml::xml_node<>* texcoord = vertexNode->first_node("texcoord");

					float u = std::stof(texcoord->first_attribute("u")->value());
					float v = std::stof(texcoord->first_attribute("v")->value());

					vertexData[i * 16 + 6] = u;
					vertexData[i * 16 + 7] = 1 - v;
				}

				i++;
			}

			for (int j = 0; j < vertexCount; j++)
			{
				Vector3f vertex(vertexData[j * 16 + 0], vertexData[j * 16 + 1], vertexData[j * 16 + 2]);
				std::set<Vector3f>::iterator it = vertexSet.find(vertex);
				vertexUnique.push_back(std::distance(vertexSet.begin(), it));
			}
		}

		rapidxml::xml_node<>* submeshesNode = meshNode->first_node("submeshes");

		for (rapidxml::xml_node<>* submeshNode = submeshesNode->first_node(); 
			submeshNode; submeshNode = submeshNode->next_sibling())
		{
			rapidxml::xml_node<>* facesNode = submeshNode->first_node("faces");

			std::string materialName = submeshNode->first_attribute("material")->value();
			int facesCount = std::stoi(facesNode->first_attribute("count")->value());
			int* indexData = new int[facesCount * 6];

			std::map<Edge, int> edgeMap;

			int i = 0;

			for (rapidxml::xml_node<>* faceNode = facesNode->first_node(); 
				faceNode; faceNode = faceNode->next_sibling())
			{
				unsigned int v1 = std::stoi(faceNode->first_attribute("v1")->value());
				unsigned int v2 = std::stoi(faceNode->first_attribute("v2")->value());
				unsigned int v3 = std::stoi(faceNode->first_attribute("v3")->value());

				indexData[i * 6 + 0] = v1;
				indexData[i * 6 + 1] = v1;
				indexData[i * 6 + 2] = v2;
				indexData[i * 6 + 3] = v2;
				indexData[i * 6 + 4] = v3;
				indexData[i * 6 + 5] = v3;

				int uniqueVertexA = vertexUnique[v1];
				int uniqueVertexB = vertexUnique[v2];
				int uniqueVertexC = vertexUnique[v3];

				std::pair<std::map<Edge, int>::iterator, bool> insertRetA 
					= edgeMap.emplace(std::pair<Edge, int>(Edge(uniqueVertexA, uniqueVertexB), i * 6 + 1));
				std::pair<std::map<Edge, int>::iterator, bool> insertRetB 
					= edgeMap.emplace(std::pair<Edge, int>(Edge(uniqueVertexB, uniqueVertexC), i * 6 + 3));
				std::pair<std::map<Edge, int>::iterator, bool> insertRetC 
					= edgeMap.emplace(std::pair<Edge, int>(Edge(uniqueVertexC, uniqueVertexA), i * 6 + 5));

				if (!insertRetA.second)
				{
					int otherIndex = insertRetA.first->second;
					indexData[i * 6 + 1] = indexData[(otherIndex + 3) % 6 + otherIndex - (otherIndex % 6)];
					indexData[otherIndex] = indexData[i * 6 + 4];
				}
				if (!insertRetB.second)
				{
					int otherIndex = insertRetB.first->second;
					indexData[i * 6 + 3] = indexData[(otherIndex + 3) % 6 + otherIndex - (otherIndex % 6)];
					indexData[otherIndex] = indexData[i * 6 + 0];
				}
				if (!insertRetC.second)
				{
					int otherIndex = insertRetC.first->second;
					indexData[i * 6 + 5] = indexData[(otherIndex + 3) % 6 + otherIndex - (otherIndex % 6)];
					indexData[otherIndex] = indexData[i * 6 + 2];
				}

				i++;
			}

			indexBuffer = gd.CreateBuffer(BUFFER_TYPE_INDEX, sizeof(int) * 6 * facesCount, 
				indexData, BUFFER_UPDATE_STATIC, facesCount * 6);

			delete[] indexData;
		}

		rapidxml::xml_node<>* boneAssignmentsNode = meshNode->first_node("boneassignments");

		if (boneAssignmentsNode != NULL)
		{

			int* boneAssignmentCount = new int[vertexCount];
			memset(boneAssignmentCount, 0, sizeof(int) * vertexCount);

			for (rapidxml::xml_node<>* boneAssignmentNode = boneAssignmentsNode->first_node(); 
				boneAssignmentNode; boneAssignmentNode = boneAssignmentNode->next_sibling())
			{
				int boneIndex = std::stoi(boneAssignmentNode->first_attribute("boneindex")->value());
				int vertexIndex = std::stoi(boneAssignmentNode->first_attribute("vertexindex")->value());
				float weight = std::stof(boneAssignmentNode->first_attribute("weight")->value());

				if (boneAssignmentCount[vertexIndex] < 4)
				{
					vertexData[vertexIndex * 16 + boneAssignmentCount[vertexIndex] + 8]  = (float)boneIndex;
					vertexData[vertexIndex * 16 + boneAssignmentCount[vertexIndex] + 12] = weight;
				}
				boneAssignmentCount[vertexIndex]++;
			}

			delete[] boneAssignmentCount;
		}


		rapidxml::xml_node<>* skeletonLinkNode = meshNode->first_node("skeletonlink");
		if (skeletonLinkNode != NULL)
		{
			std::string skeletonFilename = meshFilepath.substr(0, meshFilepath.size() - 8) + "skeleton.xml";
			meshXML.clear();
			rapidxml::file<> skeletonFile(skeletonFilename.c_str());
			rapidxml::xml_document<> skeletonXML;
			try
			{
				skeletonXML.parse < 0 >(skeletonFile.data());

				rapidxml::xml_node<>* skeletonNode = skeletonXML.first_node();
				rapidxml::xml_node<>* bonesNode = skeletonNode->first_node("bones");

				for (rapidxml::xml_node<>* boneNode = bonesNode->first_node(); 
					boneNode; boneNode = boneNode->next_sibling())
				{
					std::string boneName = boneNode->first_attribute("name")->value();

					m_bones[boneName] = Bone_t();

					Matrix4f staticRotationMatrix;
					Matrix4f staticTranslationMatrix;

					Matrix4f invRotationMatrix;
					Matrix4f invTranslationMatrix;

					rapidxml::xml_node<>* positionNode = boneNode->first_node("position");
					rapidxml::xml_node<>* rotationNode = boneNode->first_node("rotation");

					if (positionNode)
					{
						float x = std::stof(positionNode->first_attribute("x")->value());
						float y = std::stof(positionNode->first_attribute("y")->value());
						float z = std::stof(positionNode->first_attribute("z")->value());

						invTranslationMatrix[12] = -x;
						invTranslationMatrix[13] = -y;
						invTranslationMatrix[14] = -z;

						staticTranslationMatrix[12] = x;
						staticTranslationMatrix[13] = y;
						staticTranslationMatrix[14] = z;
					}

					if (rotationNode)
					{
						rapidxml::xml_node<>* axisNode = rotationNode->first_node("axis");

						float angle = std::stof(rotationNode->first_attribute("angle")->value());
						float x = std::stof(axisNode->first_attribute("x")->value());
						float y = std::stof(axisNode->first_attribute("y")->value());
						float z = std::stof(axisNode->first_attribute("z")->value());

						invRotationMatrix = Quaternion(Vector3f(-x, -y, -z), angle).ToMatrix();

						staticRotationMatrix = Quaternion(Vector3f(x, y, z), angle).ToMatrix();
					}

					std::string name = boneNode->first_attribute("name")->value();
					m_bones[boneName].m_id = std::stoi(boneNode->first_attribute("id")->value());
					m_bones[boneName].m_staticMatrix = staticTranslationMatrix * staticRotationMatrix;
					m_bones[boneName].m_transformMatrix = invRotationMatrix * invTranslationMatrix;
					m_bones[boneName].m_parentName = "";
					m_bones[boneName].m_name = boneName;
				}

				rapidxml::xml_node<>* bonehierarchyNode = skeletonNode->first_node("bonehierarchy");

				for (rapidxml::xml_node<>* boneparentNode = bonehierarchyNode->first_node(); 
					boneparentNode; boneparentNode = boneparentNode->next_sibling())
				{
					std::string childName = boneparentNode->first_attribute("bone")->value();
					std::string parentName = boneparentNode->first_attribute("parent")->value();

					m_bones[childName].m_parentName = parentName;
					m_bones[parentName].m_childBones.push_back(&m_bones[childName]);
				}

				int i = 0;
				std::map<std::string, Bone_t>::iterator it = m_bones.begin();

				while (it != m_bones.end())
				{
					if (it->second.m_parentName == "")
					{
						m_rootBones.push_back(&(it->second));
					}
					it++;
					i++;
				}

				for (size_t i = 0; i < m_rootBones.size(); i++)
				{
					SetUpBoneHierarchy(*m_rootBones[i]);
				}

				skeletonXML.clear();
			}
			catch (rapidxml::parse_error p)
			{
				p.what();
			}
		}

		vertexBuffer = gd.CreateBuffer(BUFFER_TYPE_VERTEX, sizeof(float) * 16 * vertexCount, 
			vertexData, BUFFER_UPDATE_STATIC, vertexCount);
		m_mesh = gd.CreateMesh(*vertexBuffer, *indexBuffer);

		delete[] vertexData;
	}

	void Model::SetUpBoneHierarchy(Bone_t& bone)
	{
		for (size_t i = 0; i < bone.m_childBones.size(); i++)
		{
			bone.m_childBones[i]->m_transformMatrix = bone.m_childBones[i]->m_transformMatrix * bone.m_transformMatrix;
			SetUpBoneHierarchy(*bone.m_childBones[i]);
		}
	}

	bool Model::LoadAnimation(const std::string& animationName)
	{
		int last = 0;

		int pos = m_meshFilepath.find("\\");

		while (pos != std::string::npos)
		{
			last = pos;
			pos = m_meshFilepath.find("\\", pos + 1);
		}

		std::string animationFilename 
			= m_meshFilepath.substr(0, last) + "\\" + animationName + ".skeleton.xml";

		std::map<std::string, Animation_t*>::iterator it = m_animations.find(animationFilename);
		if (it == m_animations.end())
		{
			rapidxml::file<> animationFile(animationFilename.c_str());
			rapidxml::xml_document<> animationXML;
			try
			{
				animationXML.parse < 0 >(animationFile.data());
			}
			catch (rapidxml::parse_error p)
			{}

			rapidxml::xml_node<>* skeletonNode = animationXML.first_node();
			rapidxml::xml_node<>* bonesNode = skeletonNode->first_node("bones");

			std::unordered_map<std::string, std::tuple<Matrix4f, Matrix4f>> boneMatrices;

			Animation_t* animation = new Animation_t;

			for (rapidxml::xml_node<>* boneNode = bonesNode->first_node(); 
				boneNode; boneNode = boneNode->next_sibling())
			{
				BoneKeyFrames_t* boneKeyFrames = new BoneKeyFrames_t;

				Quaternion rotationQuaternion;
				Matrix4f translationMatrix;

				rapidxml::xml_node<>* positionNode = boneNode->first_node("position");
				rapidxml::xml_node<>* rotationNode = boneNode->first_node("rotation");

				if (positionNode)
				{
					float x = std::stof(positionNode->first_attribute("x")->value());
					float y = std::stof(positionNode->first_attribute("y")->value());
					float z = std::stof(positionNode->first_attribute("z")->value());

					translationMatrix[12] = x;
					translationMatrix[13] = y;
					translationMatrix[14] = z;
				}

				if (rotationNode)
				{
					rapidxml::xml_node<>* axisNode = rotationNode->first_node("axis");

					float angle = std::stof(rotationNode->first_attribute("angle")->value());
					float x = std::stof(axisNode->first_attribute("x")->value());
					float y = std::stof(axisNode->first_attribute("y")->value());
					float z = std::stof(axisNode->first_attribute("z")->value());

					rotationQuaternion = Quaternion(Vector3f(x, y, z), angle);
				}

				int id = std::stoi(boneNode->first_attribute("id")->value());

				boneKeyFrames->m_translationMatrix = translationMatrix;
				boneKeyFrames->m_rotationMatrix = rotationQuaternion.ToMatrix();

				animation->m_boneTracks.push_back(boneKeyFrames);
			}

			rapidxml::xml_node<>* animationsNode = skeletonNode->first_node("animations");
			rapidxml::xml_node<>* animationNode = animationsNode->first_node();
			rapidxml::xml_node<>* tracksNode = animationNode->first_node("tracks");

			animation->m_length = std::stof(animationNode->first_attribute("length")->value());

			m_animations[animationNode->first_attribute("name")->value()] = animation;

			for (rapidxml::xml_node<>* trackNode = tracksNode->first_node(); 
				trackNode; trackNode = trackNode->next_sibling())
			{
				rapidxml::xml_node<>* keyframesNode = trackNode->first_node("keyframes");

				Bone_t bone = m_bones[trackNode->first_attribute("bone")->value()];

				BoneKeyFrames_t* boneKeyframe = animation->m_boneTracks[bone.m_id];

				for (rapidxml::xml_node<>* keyframeNode = keyframesNode->first_node(); 
					keyframeNode; keyframeNode = keyframeNode->next_sibling())
				{
					rapidxml::xml_node<>* positionNode = keyframeNode->first_node("translate");
					rapidxml::xml_node<>* rotationNode = keyframeNode->first_node("rotate");

					KeyFrame_t* keyframe = new KeyFrame_t;

					keyframe->m_time = std::stof(keyframeNode->first_attribute("time")->value());

					Matrix4f translationMatrix;
					Quaternion rotationQuaternion;

					if (positionNode)
					{
						float x = std::stof(positionNode->first_attribute("x")->value());
						float y = std::stof(positionNode->first_attribute("y")->value());
						float z = std::stof(positionNode->first_attribute("z")->value());

						translationMatrix[12] = x;
						translationMatrix[13] = y;
						translationMatrix[14] = z;
					}

					if (rotationNode)
					{
						rapidxml::xml_node<>* axisNode = rotationNode->first_node("axis");

						float angle = std::stof(rotationNode->first_attribute("angle")->value());
						float x = std::stof(axisNode->first_attribute("x")->value());
						float y = std::stof(axisNode->first_attribute("y")->value());
						float z = std::stof(axisNode->first_attribute("z")->value());

						rotationQuaternion = Quaternion(Vector3f(x, y, z), angle);
					}

					keyframe->m_translationMatrix = translationMatrix;
					keyframe->m_rotationQuaternion = rotationQuaternion;
					boneKeyframe->m_keyFrames.push_back(keyframe);
				}
			}
		}

		return true;
	}

	bool Model::IsRigged()
	{
		return m_rootBones.size() > 0;
	}

	Model::~Model()
	{
		delete m_mesh;
		std::map<std::string, MaterialVariable>::iterator it = m_materialVariables.begin();
		while (it != m_materialVariables.end())
		{
			if (it->second.type != VARTYPE_TEXTURE)
				delete it->second.data;
			it++;
		}
	}
}