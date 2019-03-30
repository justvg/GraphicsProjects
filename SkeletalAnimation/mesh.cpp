#include "mesh.h"

#define Assert(Expression) if(!(Expression)) { *(int *)0 = 0; }

using namespace core;
using namespace math;

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

GLuint LoadTexture(const char *Path)
{
	GLuint TextureID;
	glGenTextures(1, &TextureID);

	int Width, Height, nrChannels;
	unsigned char *Data = stbi_load(Path, &Width, &Height, &nrChannels, 0);
	if(Data)
	{
		GLenum Format;
		if(nrChannels == 1) Format = GL_RED;
		else if(nrChannels == 3) Format = GL_RGB;
		else if(nrChannels == 4) Format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "CAN'T LOAD A TEXTURE" << std::endl;
		TextureID = 0xFFFFFFFF;
	}
	stbi_image_free(Data);

	return (TextureID);
}

namespace core {

	mesh::vertex_attrib_bone::vertex_attrib_bone()
	{
		for(uint32_t I = 0; I < BONES_PER_VERTEX; I++)
		{
			IDs[I] = 0;
			Weights[I] = 0.0f;
		}
	}

	mesh::mesh_entry::mesh_entry() 
	{
		BaseVertex = 0;
		BaseIndex = 0;
		NumIndices  = 0;
		MaterialIndex = INVALID_MATERIAL;
	}

	mesh::mesh() 
	{
		VAO = 0;
		for(uint32_t I = 0; I < BuffersCount; I++)
		{
			Buffers[I] = 0;
		}
		BonesCount = 0;
		Scene = 0;
	}

	bool mesh::LoadMesh(const std::string &Filename)
	{
		bool Result = false;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(BuffersCount, Buffers);
		glBindVertexArray(VAO);

		Scene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
													aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

		if(Scene)
		{
			GlobalInverseTransform = Scene->mRootNode->mTransformation.Inverse();
			InitFromScene(Scene, Filename);
			Result = true;
		}
		else
		{
			std::cout << "Assimp can't parse " << Filename << " " << Importer.GetErrorString();
		}

		glBindVertexArray(0);

		return (Result);
	}

	void mesh::Render(GLuint ShaderID)
	{
		glBindVertexArray(VAO);
		glUniform1i(glGetUniformLocation(ShaderID, "Diffuse"), 0);

		for(uint32_t I = 0; I < MeshEntries.size(); I++)
		{
			GLuint Material = Textures[MeshEntries[I].MaterialIndex];
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Material);

			glDrawElementsBaseVertex(GL_TRIANGLES, MeshEntries[I].NumIndices, GL_UNSIGNED_INT, 
						   			 (void *)(sizeof(unsigned int) * MeshEntries[I].BaseIndex),
						  			 MeshEntries[I].BaseVertex);
		}

		glBindVertexArray(0);		
	}

	void mesh::InitFromScene(const aiScene *Scene, const std::string &Filename)
	{
		MeshEntries.resize(Scene->mNumMeshes);
		Textures.resize(Scene->mNumMaterials);

		std::vector<vec3> Positions;
		std::vector<vec3> Normals;
		std::vector<vec2> TexCoords;
		std::vector<vertex_attrib_bone> Bones;
		std::vector<unsigned int> Indices;

		uint32_t VerticesCount = 0;
		uint32_t IndicesCount = 0;

		for(uint32_t I = 0; I < MeshEntries.size(); I++)
		{
			MeshEntries[I].MaterialIndex = Scene->mMeshes[I]->mMaterialIndex;
			MeshEntries[I].BaseVertex = VerticesCount;
			MeshEntries[I].BaseIndex = IndicesCount;
			MeshEntries[I].NumIndices = Scene->mMeshes[I]->mNumFaces * 3;

			VerticesCount += Scene->mMeshes[I]->mNumVertices;
			IndicesCount += MeshEntries[I].NumIndices;
		}		
	
		Positions.reserve(VerticesCount);
		Normals.reserve(VerticesCount);
		TexCoords.reserve(VerticesCount);
		Bones.resize(VerticesCount);
		Indices.reserve(IndicesCount);

		for(uint32_t I = 0; I < MeshEntries.size(); I++)
		{
			const aiMesh *MeshStruct = Scene->mMeshes[I];
			InitMesh(I, MeshStruct, Positions, Normals, TexCoords, Bones, Indices);
		}
		
		InitMaterials(Scene, Filename);

		// NOTE(georgy): Init NodeAnim map
		for(uint32_t I = 0; I < Scene->mAnimations[0]->mNumChannels; I++)
		{
			aiNodeAnim *NodeAnim = Scene->mAnimations[0]->mChannels[I];
			BoneNameToNodeAnim[NodeAnim->mNodeName.data] = NodeAnim;
		}

		glBindBuffer(GL_ARRAY_BUFFER, Buffers[PosVBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(PosAttrib);
		glVertexAttribPointer(PosAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glBindBuffer(GL_ARRAY_BUFFER, Buffers[NormalVBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(NormalAttrib);
		glVertexAttribPointer(NormalAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glBindBuffer(GL_ARRAY_BUFFER, Buffers[TexCoordVBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(TexCoordAttrib);
		glVertexAttribPointer(TexCoordAttrib, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glBindBuffer(GL_ARRAY_BUFFER, Buffers[BoneVBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(BoneIDAttrib);
		glVertexAttribIPointer(BoneIDAttrib, 4, GL_INT, sizeof(vertex_attrib_bone), (void *)0);
		glEnableVertexAttribArray(BoneWeightAttrib);
		glVertexAttribPointer(BoneWeightAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_attrib_bone), (void *)(4*sizeof(uint32_t)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[EBO]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
	}
	
	void mesh::InitMesh(uint32_t MeshEntryIndex, const aiMesh *MeshStruct, std::vector<vec3> &Positions, std::vector<vec3> &Normals,
                	    std::vector<vec2> &TexCoords, std::vector<vertex_attrib_bone> &Bones, std::vector<unsigned int> &Indices)
	{
		const aiVector3D Zero(0.0f, 0.0f, 0.0f);

		for(uint32_t I = 0; I < MeshStruct->mNumVertices; I++)
		{
			const aiVector3D *Pos = &(MeshStruct->mVertices[I]); 
			const aiVector3D *Normal = MeshStruct->HasNormals() ? &(MeshStruct->mNormals[I]) : &Zero; 
			const aiVector3D *TexCoordsValues = MeshStruct->HasTextureCoords(0) ? &(MeshStruct->mTextureCoords[0][I]) : &Zero;

			Positions.push_back(vec3(Pos->x, Pos->y, Pos->z)); 
			Normals.push_back(vec3(Normal->x, Normal->y, Normal->z));
			TexCoords.push_back(vec2(TexCoordsValues->x, TexCoordsValues->y));
		}

		LoadBones(MeshEntryIndex, MeshStruct, Bones);

		for(uint32_t I = 0; I < MeshStruct->mNumFaces; I++)
		{
			const aiFace *Face = &MeshStruct->mFaces[I];
			Assert(Face->mNumIndices == 3);
			Indices.push_back(Face->mIndices[0]);
			Indices.push_back(Face->mIndices[1]);
			Indices.push_back(Face->mIndices[2]);
		}
	}

	void mesh::LoadBones(uint32_t MeshEntryIndex, const aiMesh *Mesh, std::vector<vertex_attrib_bone> &Bones)
	{
		for(uint32_t I = 0; I < Mesh->mNumBones; I++)
		{
			uint32_t BoneIndex;
			std::string BoneName = Mesh->mBones[I]->mName.data;

			if(BoneNameToIndexInBoneTransforms.find(BoneName) == BoneNameToIndexInBoneTransforms.end())
			{
				BoneIndex = BonesCount;
				BonesCount++;
				BoneNameToIndexInBoneTransforms[BoneName] = BoneIndex;

				bone_transform bt;
				BoneTransforms.push_back(bt);
				BoneTransforms[BoneIndex].BoneOffset = Mesh->mBones[I]->mOffsetMatrix;
			}
			else
			{
				BoneIndex = BoneNameToIndexInBoneTransforms[BoneName];
			}

			for(uint32_t J = 0; J < Mesh->mBones[I]->mNumWeights; J++)
			{
				uint32_t VertexID = MeshEntries[MeshEntryIndex].BaseVertex + Mesh->mBones[I]->mWeights[J].mVertexId;
				float Weight = Mesh->mBones[I]->mWeights[J].mWeight; 
				Bones[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
	}
	
	void mesh::vertex_attrib_bone::AddBoneData(uint32_t BoneID, float Weight)
	{
		for(uint32_t I = 0; I < BONES_PER_VERTEX; I++)
		{
			if(Weights[I] == 0.0f)
			{
				IDs[I] = BoneID;
				Weights[I] = Weight;
				return;
			}
		}

		Assert(0);
	}
	
	void mesh::InitMaterials(const aiScene *Scene, const std::string &Filename)
	{
		// Extract the directory part from the file name
		// NOTE(georgy): We assume that textures are located in the same dir. as model
		std::string::size_type SlashIndex = Filename.find_last_of("/");
		std::string Dir;

		if (SlashIndex == std::string::npos) {
			Dir = ".";
		}
		else if (SlashIndex == 0) {
			Dir = "/";
		}
		else {
			Dir = Filename.substr(0, SlashIndex);
		}

		for(uint32_t I = 0; I < Scene->mNumMaterials; I++)
		{
			const aiMaterial *Material = Scene->mMaterials[I];

			Textures[I] = INVALID_MATERIAL;
			if(Material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString Path;

				if(Material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, 0, 0, 0, 0, 0) == AI_SUCCESS)
				{
					std::string FullPath = Dir + "/" + Path.data;
					Textures[I] = LoadTexture(FullPath.c_str());
				}
			}

			if (Textures[I] == INVALID_MATERIAL) 
			{
				Textures[I] = LoadTexture("white.jpg");
			}
		}
	}

	void mesh::ReadNodeHeirarchy(float AnimationTime, const aiNode *Node, const mat4 &ParentTransform)
	{
		std::string NodeName = Node->mName.data;

		aiNodeAnim *NodeAnim = BoneNameToNodeAnim[NodeName];

		mat4 NodeTransform;
		NodeTransform = Node->mTransformation;

		if (NodeAnim)
		{
			aiVector3D Scale;
			CalcInterpolatedScaling(Scale, AnimationTime, NodeAnim);
			mat4 Scaling = mat4::Scale(Scale.x, Scale.y, Scale.z);

			aiQuaternion Rotate;
			CalcInterpolatedRotation(Rotate, AnimationTime, NodeAnim);
			mat4 Rotation;
			Rotation = Rotate.GetMatrix();

			aiVector3D Translate;
			CalcInterpolatedTranslation(Translate, AnimationTime, NodeAnim);
			mat4 Translation = mat4::Translation(vec3(Translate.x, Translate.y, Translate.z));

			NodeTransform = Translation * Rotation * Scaling;
		}

		mat4 GlobalTransformation = ParentTransform * NodeTransform;

		if (BoneNameToIndexInBoneTransforms.find(NodeName) != BoneNameToIndexInBoneTransforms.end())
		{
			uint32_t BoneIndex = BoneNameToIndexInBoneTransforms[NodeName];
			BoneTransforms[BoneIndex].FinalTransform = GlobalInverseTransform * GlobalTransformation * BoneTransforms[BoneIndex].BoneOffset;
		}

		for (uint32_t I = 0; I < Node->mNumChildren; I++)
		{
			ReadNodeHeirarchy(AnimationTime, Node->mChildren[I], GlobalTransformation);
		}
	}

	void mesh::BoneTransform(float TimeInSeconds, std::vector<mat4> &Transforms)
	{
		mat4 Identity = mat4::Identity(1.0f);

		float TicksPerSecond = Scene->mAnimations[0]->mTicksPerSecond != 0.0f ?
							   (float)Scene->mAnimations[0]->mTicksPerSecond : 25.0f;
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float CurrentAnimationTime = fmod(TimeInTicks, (float)Scene->mAnimations[0]->mDuration);

		ReadNodeHeirarchy(CurrentAnimationTime, Scene->mRootNode, Identity);

		Transforms.resize(BonesCount);

		for(uint32_t I = 0; I < BonesCount; I++)
		{
			Transforms[I] = BoneTransforms[I].FinalTransform;
		}
	}

	uint32_t mesh::FindScaling(float AnimationTime, const aiNodeAnim *NodeAnim)
	{
		Assert(NodeAnim->mNumScalingKeys > 0);

		for(uint32_t I = 0; I < NodeAnim->mNumScalingKeys - 1; I++)
		{
			if(AnimationTime < (float)NodeAnim->mScalingKeys[I + 1].mTime)
			{
				return (I);
			}
		}

		Assert(0);
		return(0);
	}

	uint32_t mesh::FindTranslation(float AnimationTime, const aiNodeAnim *NodeAnim)
	{
		Assert(NodeAnim->mNumPositionKeys > 0);

		for(uint32_t I = 0; I < NodeAnim->mNumPositionKeys - 1; I++)
		{
			if(AnimationTime < (float)NodeAnim->mPositionKeys[I + 1].mTime)
			{
				return (I);
			}
		}

		Assert(0);
		return(0);
	}

	uint32_t mesh::FindRotation(float AnimationTime, const aiNodeAnim *NodeAnim)
	{
		Assert(NodeAnim->mNumRotationKeys > 0);

		for(uint32_t I = 0; I < NodeAnim->mNumRotationKeys - 1; I++)
		{
			if(AnimationTime < (float)NodeAnim->mRotationKeys[I + 1].mTime)
			{
				return (I);
			}
		}

		Assert(0);
		return(0);
	}

	void mesh::CalcInterpolatedTranslation(aiVector3D &Out, float AnimationTime, const aiNodeAnim *NodeAnim)
	{
		if(NodeAnim->mNumPositionKeys == 1)
		{
			Out = NodeAnim->mPositionKeys[0].mValue;
			return;
		}

		uint32_t StartIndex = FindTranslation(AnimationTime, NodeAnim);
		uint32_t NextIndex = StartIndex + 1;
		Assert(NextIndex < NodeAnim->mNumPositionKeys);

		float DeltaTime = (float)(NodeAnim->mPositionKeys[NextIndex].mTime - NodeAnim->mPositionKeys[StartIndex].mTime); 
		float Factor = (AnimationTime - (float)NodeAnim->mPositionKeys[StartIndex].mTime) / DeltaTime;
		Assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D &Start = NodeAnim->mPositionKeys[StartIndex].mValue;
		const aiVector3D &End = NodeAnim->mPositionKeys[NextIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	void mesh::CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim *NodeAnim)
	{
		if(NodeAnim->mNumScalingKeys == 1)
		{
			Out = NodeAnim->mScalingKeys[0].mValue;
			return;
		}

		uint32_t StartIndex = FindScaling(AnimationTime, NodeAnim);
		uint32_t NextIndex = StartIndex + 1;
		Assert(NextIndex < NodeAnim->mNumScalingKeys);

		float DeltaTime = (float)(NodeAnim->mScalingKeys[NextIndex].mTime - NodeAnim->mScalingKeys[StartIndex].mTime);
		float Factor = (AnimationTime - (float)NodeAnim->mScalingKeys[StartIndex].mTime) / DeltaTime;
		Assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D &Start = NodeAnim->mScalingKeys[StartIndex].mValue;
		const aiVector3D &End = NodeAnim->mScalingKeys[NextIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	void mesh::CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime, const aiNodeAnim *NodeAnim)
	{
		if(NodeAnim->mNumRotationKeys == 1)
		{
			Out = NodeAnim->mRotationKeys[0].mValue;
			return;
		}

		uint32_t StartRotIndex = FindRotation(AnimationTime, NodeAnim);
		uint32_t NextRotIndex = StartRotIndex + 1;
		Assert(NextRotIndex < NodeAnim->mNumRotationKeys);

		float DeltaTime = (float)(NodeAnim->mRotationKeys[NextRotIndex].mTime - NodeAnim->mRotationKeys[StartRotIndex].mTime);
		float Factor = (AnimationTime - (float)NodeAnim->mRotationKeys[StartRotIndex].mTime) / DeltaTime;
		Assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion &StartRotation = NodeAnim->mRotationKeys[StartRotIndex].mValue;
		const aiQuaternion &EndRotation = NodeAnim->mRotationKeys[NextRotIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotation, EndRotation, Factor);
		Out = Out.Normalize();
	}
	
}