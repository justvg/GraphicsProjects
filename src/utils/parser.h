#include <sstream>
#include <map>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define Assert(Expression) if(!(Expression)) { *(int *)0 = 0; }

namespace core {

	struct vertex
	{
		vec3 Position;
		vec3 Normal;
		vec2 TexCoords;
	};

	struct indexed_primitive
	{
		uint32_t PosIndex;
		uint32_t NormalIndex;
		uint32_t UVIndex;

		bool operator<(const indexed_primitive& Other) const
		{
			return memcmp(this, &Other, sizeof(indexed_primitive)) > 0;
		}
	};

	struct mesh
	{
		uint32_t IndexOffset;
		uint32_t IndexCount;

		GLuint VAO, EBO;

		GLuint TextureID;

		void SetupMesh(GLuint VBO, std::vector<uint32_t> &IndexBuffer);
		void Draw();
	};

	void mesh::SetupMesh(GLuint VBO, std::vector<uint32_t> &IndexBuffer)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(uint32_t), &IndexBuffer.front() + IndexOffset, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)(3*sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)(6*sizeof(float)));

		glBindVertexArray(0);
	}

	void mesh::Draw()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID);
	
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
		
	struct model
	{
		std::vector<mesh> Meshes;

		GLuint VBO;
	};

	void InitializeSceneObjects(char *Filename, model &Model, std::vector<vertex> &VertexBuffer, std::vector<uint32_t> &IndexBuffer)
	{
		tinyobj::attrib_t Attribs;
		std::vector<tinyobj::shape_t> Shapes;
		std::vector<tinyobj::material_t> Materials;
		std::string Warn = "";
		std::string Err = "";

		std::map<std::string, GLuint> Textures;

		bool Loaded = tinyobj::LoadObj(&Attribs, &Shapes, &Materials, &Warn, &Err, Filename, "assets/", true);
		if(Loaded)
		{
			for(uint32_t I = 0; I < Materials.size(); I++)
			{
				tinyobj::material_t &Material = Materials[I];
				std::string DiffuseTextName = Material.diffuse_texname;

				if(Textures.find(DiffuseTextName) == Textures.end())
				{
					GLuint TextureID;
					glGenTextures(1, &TextureID);

					int Width, Height, nrChannels;
					unsigned char *Data = stbi_load(("assets/" + DiffuseTextName).c_str(), &Width, &Height, &nrChannels, 0);
					if(Data)
					{
						GLenum Format;
						if(nrChannels == 1) Format = GL_RED;
						else if(nrChannels == 3) Format = GL_RGB;
						else if(nrChannels == 4) Format = GL_RGBA;

						glBindTexture(GL_TEXTURE_2D, TextureID);
						glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
						glGenerateMipmap(GL_TEXTURE_2D);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					}
					else
					{
						std::cout << "CAN'T LOAD A TEXTURE" << std::endl;
					}
					stbi_image_free(Data);

					Textures[DiffuseTextName] = TextureID;
				}
			}	

			std::map<indexed_primitive, uint32_t> IndexedPrimitives;
			for(uint32_t ShapeIndex = 0; ShapeIndex < Shapes.size(); ShapeIndex++)
			{
				tinyobj::shape_t Shape = Shapes[ShapeIndex];

				uint32_t IndexOffset = IndexBuffer.size();
				for(uint32_t I = 0; I < Shape.mesh.indices.size(); I++)
				{
					tinyobj::index_t Index = Shape.mesh.indices[I];

					indexed_primitive Prim;
					Prim.PosIndex = Index.vertex_index;
					Assert(Index.vertex_index != -1);

					bool HasNormals = Index.normal_index != -1;
					bool HasUV = Index.texcoord_index != -1;

					Prim.NormalIndex =  HasNormals ? Index.normal_index : UINT32_MAX;
					Prim.UVIndex = HasUV ? Index.texcoord_index : UINT32_MAX;

					auto FoundPrim = IndexedPrimitives.find(Prim);
					if(FoundPrim != IndexedPrimitives.end())
					{
						IndexBuffer.push_back(FoundPrim->second);
					}
					else
					{
						uint32_t NewIndex = VertexBuffer.size();
						IndexedPrimitives[Prim] = NewIndex;
						
						vertex NewVertex;
						NewVertex.Position.x = Attribs.vertices[3 * Prim.PosIndex];
						NewVertex.Position.y = Attribs.vertices[3 * Prim.PosIndex + 1];
						NewVertex.Position.z = Attribs.vertices[3 * Prim.PosIndex + 2];

						NewVertex.Normal = vec3(0, 0, 0);
						if(HasNormals)
						{
							NewVertex.Normal.x = Attribs.normals[3 * Prim.NormalIndex];
							NewVertex.Normal.y = Attribs.normals[3 * Prim.NormalIndex + 1];
							NewVertex.Normal.z = Attribs.normals[3 * Prim.NormalIndex + 2];
						}

						NewVertex.TexCoords = vec2(0, 0);
						if(HasUV)
						{
							NewVertex.TexCoords.x = Attribs.texcoords[2 * Prim.UVIndex];
							NewVertex.TexCoords.y = Attribs.texcoords[2 * Prim.UVIndex + 1];
						}
						VertexBuffer.push_back(NewVertex);

						IndexBuffer.push_back(NewIndex);
					}
				}

				mesh Mesh;
				Mesh.IndexOffset = IndexOffset;
				Mesh.IndexCount = Shape.mesh.indices.size();

				Assert(Shape.mesh.material_ids[0] != -1);
				std::string DiffuseTextureName = Materials[Shape.mesh.material_ids[0]].diffuse_texname;
				Mesh.TextureID = Textures[DiffuseTextureName];

				Model.Meshes.push_back(Mesh);
			}

			glGenBuffers(1, &Model.VBO);
			glBindBuffer(GL_ARRAY_BUFFER, Model.VBO);
			glBufferData(GL_ARRAY_BUFFER, VertexBuffer.size() * sizeof(vertex), &VertexBuffer.front(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			for(uint32_t MeshIndex = 0; MeshIndex < Model.Meshes.size(); MeshIndex++)
			{
				Model.Meshes[MeshIndex].SetupMesh(Model.VBO, IndexBuffer);
			}
		}
		else
		{
			std::cout << "Can't load .OBJ file, check file path!" << std::endl;
		}
	}
	

	/*
		NOTE(georgy): Version without using library. Just for simple practice. 
					  Handle .obj files with only vertices and normals. Don't parse textures.
	*/
	void LoadObject(char *Filename, std::vector<vertex> &VertexBuffer, std::vector<uint32_t> &IndexBuffer)
	{
		std::ifstream In(Filename);
		if(!In.is_open())
		{	
			std::cout << "Cannot open " << Filename << std::endl;
			return;
		}

		std::vector<vec3> Vertices;
		std::vector<vec3> Normals;
		std::map<indexed_primitive, uint32_t> IndexedPrimitives; // NOTE(georgy): To check if a vertex is already defined in VertexBuffer
		std::string Line;
		while(getline(In, Line))
		{
			if(Line.substr(0, 2) == "v ")
			{
				std::istringstream Str(Line.substr(2));
				vec3 Vertex;
				Str >> Vertex.x; Str >> Vertex.y; Str >> Vertex.z;
				Vertices.push_back(Vertex);
			}
			else if(Line.substr(0,2 ) == "vn")
			{
				std::istringstream Str(Line.substr(2));
				vec3 Normal;
				Str >> Normal.x; Str >> Normal.y; Str >> Normal.z;
				Normals.push_back(Normal); 
			}
			else if(Line.substr(0, 2) == "f ")
			{
				std::istringstream Str(Line.substr(2));
				indexed_primitive Prim;

				for(int I = 0; I < 3; I++)
				{
					Str >> Prim.PosIndex;
					Str.seekg(2, std::ios_base::cur);
					Str >> Prim.NormalIndex;
					Prim.PosIndex--;
					Prim.NormalIndex--;

					auto FoundPrim = IndexedPrimitives.find(Prim);
					if(FoundPrim != IndexedPrimitives.end())
					{
						IndexBuffer.push_back(FoundPrim->second);
					}
					else
					{
						uint32_t NewIndex = VertexBuffer.size();
						IndexedPrimitives[Prim] = NewIndex;
						
						vertex NewVertex;
						NewVertex.Position = Vertices[Prim.PosIndex];
						NewVertex.Normal = Normals[Prim.NormalIndex];
						VertexBuffer.push_back(NewVertex);

						IndexBuffer.push_back(NewIndex);
					}
				}
			}
		}
	}

}