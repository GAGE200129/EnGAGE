#include "pch.hpp"
#include "Resource.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>
#include <glad/glad.h>

struct Vertex
{
	float x, y, z;
	float nX, nY, nZ;
	float tU, tV;
};


static Ref<Core::Model> loadModel(const String& filePath);
static Core::Node parseNode(const tinygltf::Node& node);
static Core::Mesh parseMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh);
static void extractAccessor(const tinygltf::Model& model, const tinygltf::Accessor& accessor, DynArr<char>& outBuffer,
	unsigned int& outComponentType, unsigned int& outType, unsigned int& outCount);
static void extractAccessorBuffer(const tinygltf::Model& model, const tinygltf::Accessor& accessor, DynArr<char>& outBuffer);

static UnorderedMap<String, Ref<Core::Model>> gModels;

namespace Core
{
	namespace Resource
	{

		Ptr<Model> getModel(const String& filePath)
		{
			if (gModels.find(filePath) != gModels.end())
			{
				return gModels.at(filePath);
			}
			else
			{
				//Load and add a new model to the pool
				Ref<Model> model = loadModel(filePath);
				gModels.emplace(filePath, model);
				return model;
			}
		}

		void cleanup()
		{
			for (const auto& pair : gModels)
			{
				const auto& model = pair.second;
				for (const auto& mesh : model->meshes)
				{
					for (const auto& primitive : mesh.primitives)
					{
						glDeleteVertexArrays(1, &primitive.vao);
						glDeleteBuffers(1, &primitive.vbo);
						glDeleteBuffers(1, &primitive.ebo);
					}
				}
			}
			gModels.clear();
		}
	}
}

static Ref<Core::Model> loadModel(const String& filePath)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;
	Ref<Core::Model> result = createRef<Core::Model>();

	bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filePath);

	if (!warn.empty()) {
		EN_WARN("Warn: {}", warn.c_str());
	}

	if (!err.empty()) {
		EN_ERROR("Err: {}", err.c_str());
	}

	if (!ret) {
		EN_ERROR("Failed to parse glTF\n");
		return nullptr;
	}

	for (const auto& gltfMesh : model.meshes)
	{
		result->meshes.push_back(parseMesh(model, gltfMesh));
	}

	for (const auto& gltfNode : model.nodes)
	{
		result->nodes.push_back(parseNode(gltfNode));
	}

	result->rootNodeIndex = model.scenes[0].nodes[0];

	return result;
}

static Core::Node parseNode(const tinygltf::Node& node)
{
	Core::Node result;
	result.name = node.name;
	
	result.meshIndex = node.mesh;
	result.children = node.children;

	result.position = { 0, 0, 0 };
	result.rotation = { 1, 0, 0, 0 };
	result.scale = { 1, 1, 1 };

	if (node.translation.size() != 0)
	{
		result.position.x = node.translation[0];
		result.position.y = node.translation[1];
		result.position.z = node.translation[2];
	}

	if (node.scale.size() != 0)
	{
		result.scale.x = node.scale[0];
		result.scale.y = node.scale[1];
		result.scale.z = node.scale[2];
	}

	if (node.rotation.size() != 0)
	{
		result.rotation.w = node.rotation[0];
		result.rotation.x = node.rotation[1];
		result.rotation.y = node.rotation[2];
		result.rotation.z = node.rotation[3];
	}

	if (node.matrix.size() != 0)
	{
		EN_ASSERT(false, "TODO");
	}



	return result;
}

static Core::Mesh parseMesh(const tinygltf::Model& model, const tinygltf::Mesh& gltfMesh)
{
	Core::Mesh result;
	result.name = gltfMesh.name;

	for (const auto& gltfPrimitive : gltfMesh.primitives)
	{
		Core::Primitive primitive;

		DynArr<Vertex> vertices;
		DynArr<char> positionBuffer, normalBuffer, textureBuffer, indicesBuffer;
		unsigned int componentType, type, count;
		extractAccessor(model, model.accessors[gltfPrimitive.attributes.at("POSITION")], positionBuffer, componentType, type, count);
		extractAccessorBuffer(model, model.accessors[gltfPrimitive.attributes.at("NORMAL")], normalBuffer);
		extractAccessorBuffer(model, model.accessors[gltfPrimitive.attributes.at("TEXCOORD_0")], textureBuffer);
		EN_ASSERT(
			componentType == TINYGLTF_COMPONENT_TYPE_FLOAT &&
			type == TINYGLTF_TYPE_VEC3,
			"Invalid vertex format !");

		vertices.reserve(count);
		for (unsigned int i = 0; i < count; i++)
		{
			Vertex v;
			unsigned int positonSize = sizeof(float) * 3;
			unsigned int normalSize = sizeof(float) * 3;
			unsigned int texCoordSize = sizeof(float) * 2;
			memcpy(&v.x, positionBuffer.data() + positonSize * i, positonSize);
			memcpy(&v.nX, normalBuffer.data() + normalSize * i, normalSize);
			memcpy(&v.tU, textureBuffer.data() + texCoordSize * i, texCoordSize);
			vertices.push_back(v);
		}

		extractAccessor(model, model.accessors[gltfPrimitive.indices], indicesBuffer, componentType, type, count);
		primitive.eboDataType = componentType;
		primitive.vertexCoumt = count;
		glGenVertexArrays(1, &primitive.vao);
		glGenBuffers(1, &primitive.vbo);
		glGenBuffers(1, &primitive.ebo);

		glBindVertexArray(primitive.vao);
		glBindBuffer(GL_ARRAY_BUFFER, primitive.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 6));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitive.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer.size(), indicesBuffer.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		result.primitives.push_back(primitive);
	}


	return result;
}

static void extractAccessor(const tinygltf::Model& model, const tinygltf::Accessor& accessor, DynArr<char>& outBuffer, unsigned int& outComponentType, unsigned int& outType, unsigned int& outCount)
{
	const auto& bufferView = model.bufferViews[accessor.bufferView];
	const auto& buffer = model.buffers[bufferView.buffer].data;
	outBuffer = DynArr<char>(buffer.begin() + bufferView.byteOffset, buffer.begin() + bufferView.byteOffset + bufferView.byteLength);
	outType = accessor.type;
	outComponentType = accessor.componentType;
	outCount = accessor.count;
}

void extractAccessorBuffer(const tinygltf::Model& model, const tinygltf::Accessor& accessor, DynArr<char>& outBuffer)
{
	const auto& bufferView = model.bufferViews[accessor.bufferView];
	const auto& buffer = model.buffers[bufferView.buffer].data;
	outBuffer = DynArr<char>(buffer.begin() + bufferView.byteOffset, buffer.begin() + bufferView.byteOffset + bufferView.byteLength);
}
