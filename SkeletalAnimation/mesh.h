#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "GL/glew.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "math/vec.h"
#include "math/mat4.h"

using namespace core;
using namespace math;

namespace core {

	#define INVALID_MATERIAL 0xFFFFFFFF
	#define BONES_PER_VERTEX 4

	enum buffer_type
	{
		EBO,
		PosVBO,
		NormalVBO,
		TexCoordVBO,
		BoneVBO,

		BuffersCount
	};

	enum attrib_location
	{	
		PosAttrib,
		NormalAttrib,
		TexCoordAttrib,
		BoneIDAttrib,
		BoneWeightAttrib
	};

	struct vertex
	{
		vec3 Pos;
		vec3 Normal;
		vec2 TextCoords;
	};

	class mesh
	{
		public:
			mesh();

			bool LoadMesh(const std::string &Filename);
			void BoneTransform(float TimeInSeconds, std::vector<mat4> &Transforms);
			void Render(GLuint ShaderID);

		private:
			struct bone_transform
			{
				mat4 BoneOffset;
				mat4 FinalTransform;
			};

			struct vertex_attrib_bone
			{
				vertex_attrib_bone();

				uint32_t IDs[BONES_PER_VERTEX];
				float Weights[BONES_PER_VERTEX];

				void AddBoneData(uint32_t BoneID, float Weight);
			};

			void InitFromScene(const aiScene *Scene, const std::string &Filename);
			void InitMesh(uint32_t MeshEntryIndex, const aiMesh *MeshStruct, 
						  std::vector<vec3> &Positions,  
						  std::vector<vec3> &Normals,
                		  std::vector<vec2> &TexCoords,
						  std::vector<vertex_attrib_bone> &Bones,
						  std::vector<unsigned int> &Indices);
			void LoadBones(uint32_t MeshEntryIndex, const aiMesh *Mesh, std::vector<vertex_attrib_bone> &Bones);
			void InitMaterials(const aiScene *Scene, const std::string &Filename);

			void ReadNodeHeirarchy(float AnimationTime, const aiNode *Node, const mat4 &ParentTransform);
			void CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim *NodeAnim);
			void CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime, const aiNodeAnim *NodeAnim);
			void CalcInterpolatedTranslation(aiVector3D &Out, float AnimationTime, const aiNodeAnim *NodeAnim);
			uint32_t FindScaling(float AnimationTime, const aiNodeAnim *NodeAnim);
			uint32_t FindRotation(float AnimationTime, const aiNodeAnim *NodeAnim);
			uint32_t FindTranslation(float AnimationTime, const aiNodeAnim *NodeAnim);

			GLuint VAO;
			GLuint Buffers[BuffersCount];

			struct mesh_entry
			{
				mesh_entry();
				
				unsigned int BaseVertex;
				unsigned int BaseIndex;
				unsigned int NumIndices;
				unsigned int MaterialIndex; 
			};

			mat4 GlobalInverseTransform;
			uint32_t BonesCount;
			std::map<std::string, uint32_t> BoneNameToIndexInBoneTransforms; 
			std::vector<bone_transform> BoneTransforms;

			std::map<std::string, aiNodeAnim *> BoneNameToNodeAnim; // For Animation[0]

			const aiScene *Scene;
			Assimp::Importer Importer;

			std::vector<mesh_entry> MeshEntries;
			std::vector<unsigned int> Textures;
	};

}
