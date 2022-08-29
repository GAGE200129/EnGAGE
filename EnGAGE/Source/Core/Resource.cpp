#include "pch.hpp"
#include "Resource.hpp"

#include "Core/Data/Vertex.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>
#include <glad/glad.h>



static Scope<Core::Model> loadModel(const String& filePath);
static Core::Texture parseTexture(const tinygltf::Model& model, const tinygltf::Texture& texture);
static Core::Node parseNode(const tinygltf::Node& node);
static Core::Mesh parseMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh);
static void extractAccessor(const tinygltf::Model& model, const tinygltf::Accessor& accessor, DynArr<char>& outBuffer,
	unsigned int& outComponentType, unsigned int& outType, unsigned int& outCount);
static void extractAccessorBuffer(const tinygltf::Model& model, const tinygltf::Accessor& accessor, DynArr<char>& outBuffer);
static Scope<Core::TextureSheet> loadTextureSheet(const String& filePath);

static DynArr<Scope<Core::Model>> gModels;
static DynArr<Scope<Core::TextureSheet>> gTextureSheets;

namespace Core::Resource
{
	Model* getModel(const String& filePath)
	{
		for (auto it = gModels.cbegin(); it != gModels.cend(); it++)
		{
			if ((*it)->name == filePath)
			{
				return (*it).get();
			}
		}
		//Load and add a new model to the pool
		if (auto model = loadModel(filePath))
		{
			Model* ptr = model.get();
			gModels.push_back(std::move(model));
			return ptr;
		}
		else
		{
			return nullptr;
		}
	}

	TextureSheet* getTextureSheet(const String& texturePath)
	{
		for (auto it = gTextureSheets.cbegin(); it != gTextureSheets.cend(); it++)
		{
			if ((*it)->name == texturePath)
			{
				return (*it).get();
			}
		}
		//Load and add a new model to the pool
		if (auto sheet = loadTextureSheet(texturePath))
		{
			TextureSheet* ptr = sheet.get();
			gTextureSheets.push_back(std::move(sheet));
			return ptr;
		}
		else
		{
			return nullptr;
		}
	}


	void clear()
	{
		for (const auto& model : gModels)
		{
			for (const auto& mesh : model->meshes)
			{
				for (const auto& primitive : mesh.primitives)
				{
					glDeleteVertexArrays(1, &primitive.vao);
					glDeleteBuffers(1, &primitive.vbo);
					glDeleteBuffers(1, &primitive.ebo);
					EN_INFO("Deallocating gpu memory: vao: {}, vbo: {}, ebo: {}", primitive.vao, primitive.vbo, primitive.ebo);
				}
			}

			for (const auto& texture : model->textures)
			{
				EN_INFO("Deleting texture: {}", texture.textureID);
				glDeleteTextures(1, &texture.textureID);
			}
		}

		for (const auto& texture : gTextureSheets)
		{
			EN_INFO("Deleting texture sheet: {}", texture->id);
			glDeleteTextures(1, &texture->id);
		}

		gTextureSheets.clear();
		gModels.clear();

	}

}

static Scope<Core::Model> loadModel(const String& filePath)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;
	Scope<Core::Model> result = nullptr;

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

	result = createScope<Core::Model>();
	result->name = filePath;

	for (const auto& gltfMesh : model.meshes)
	{
		result->meshes.push_back(parseMesh(model, gltfMesh));
	}

	for (const auto& gltfNode : model.nodes)
	{
		result->nodes.push_back(parseNode(gltfNode));
	}

	for (const auto& gltfTexture : model.textures)
	{
		result->textures.push_back(parseTexture(model, gltfTexture));
	}

	for (const auto& gltfMaterial : model.materials)
	{
		Core::Material material;

		material.baseColorIndex = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;

		result->materials.push_back(material);
	}

	for (const auto& gltfAnimation : model.animations)
	{
		Core::AnimationData animation;
		animation.name = gltfAnimation.name;
		for (const auto& gltfSampler : gltfAnimation.samplers)
		{
			Core::Sampler sampler;
			std::vector<char> buffer;
			unsigned int componentType, type, count;


			//Extract sampler input
			extractAccessor(model, model.accessors[gltfSampler.input], buffer, componentType, type, count);
			sampler.input.resize(count);
			memcpy(sampler.input.data(), buffer.data(), count * sizeof(float));
			
			//Extract sampler output
			extractAccessor(model, model.accessors[gltfSampler.output], buffer, componentType, type, count);
			int componentTypeCount = -1;
			switch (type)
			{
			case TINYGLTF_TYPE_VEC3:
			{
				componentTypeCount = 3;
				//sampler.outputType = Core::Sampler::OutputType::Vec3;
				break;
			}
			case TINYGLTF_TYPE_VEC4:
			{
				componentTypeCount = 4;
				//sampler.outputType = Core::Sampler::OutputType::Quaternion;
				break;
			}
			}
			EN_ASSERT(componentTypeCount != -1, "Unknown component type count !");
			sampler.output.resize(componentTypeCount * count);
			memcpy(sampler.output.data(), buffer.data(), count * sizeof(float) * componentTypeCount);

			animation.samplers.push_back(sampler);
		}
		for (const auto& gltfChannel : gltfAnimation.channels)
		{
			Core::Channel channel;

			channel.node = gltfChannel.target_node;

			if (gltfChannel.target_path == "translation")
			{
				channel.path = Core::Channel::Path::TRANSLATION;
			}
			else if (gltfChannel.target_path == "rotation")
			{
				channel.path = Core::Channel::Path::ROTATION;
			}
			else if (gltfChannel.target_path == "scale")
			{
				channel.path = Core::Channel::Path::SCALE;
			}
			else
			{
				EN_ASSERT(false, "Unknown target path");
			}
			channel.samplerIndex = gltfChannel.sampler;
			

			animation.channels.push_back(channel);
		}
		
		result->animations.push_back(animation);
		
	}

	for (const auto& gltfSkin : model.skins)
	{
		std::vector<char> buffer;
		unsigned int componentType, type, count;
		extractAccessor(model, model.accessors[gltfSkin.inverseBindMatrices], buffer, componentType, type, count);

		for (int i = 0; i < count; ++i)
		{
			memcpy(&result->nodes[gltfSkin.joints[i]].inverseBindTransform[0][0], buffer.data() + sizeof(float) * 16 * i, sizeof(float) * 16);
		}
		

	}

	result->rootNodeIndex = model.scenes[0].nodes[0];

	return result;
}

Core::Texture parseTexture(const tinygltf::Model& model, const tinygltf::Texture& gltfTexture)
{
	Core::Texture texture;
	const auto& sampler = model.samplers[gltfTexture.sampler];
	const auto& image = model.images[gltfTexture.source];

	glGenTextures(1, &texture.textureID);
	EN_INFO("Generating texture: {}", texture.textureID);


	glBindTexture(GL_TEXTURE_2D, texture.textureID);
	if (image.mimeType == "image/jpeg")
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image.width, image.height, 0, GL_RGB, image.pixel_type, image.image.data());
	}
	else if (image.mimeType == "image/png")
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width, image.height, 0, GL_RGBA, image.pixel_type, image.image.data());
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapS);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
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
		result.position.x = (float)node.translation[0];
		result.position.y = (float)node.translation[1];
		result.position.z = (float)node.translation[2];
	}

	if (node.scale.size() != 0)
	{
		result.scale.x = (float)node.scale[0];
		result.scale.y = (float)node.scale[1];
		result.scale.z = (float)node.scale[2];
	}

	if (node.rotation.size() != 0)
	{
		result.rotation.x = (float)node.rotation[0];
		result.rotation.y = (float)node.rotation[1];
		result.rotation.z = (float)node.rotation[2];
		result.rotation.w = (float)node.rotation[3];
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
	result.skinned = false;
	float maxDistance = 0.0f;

	for (const auto& gltfPrimitive : gltfMesh.primitives)
	{
		Core::Primitive primitive;

		DynArr<Core::Vertex> vertices;
		DynArr<char> positionBuffer, normalBuffer, textureBuffer, indicesBuffer, boneIDBuffer, boneWeightBuffer;
		unsigned int componentType, type, count;
		unsigned int skinnedComponentType, skinnedType, skinnedCount;
		extractAccessor(model, model.accessors[gltfPrimitive.attributes.at("POSITION")], positionBuffer, componentType, type, count);
		extractAccessorBuffer(model, model.accessors[gltfPrimitive.attributes.at("NORMAL")], normalBuffer);
		extractAccessorBuffer(model, model.accessors[gltfPrimitive.attributes.at("TEXCOORD_0")], textureBuffer);

		if (gltfPrimitive.attributes.find("JOINTS_0") != gltfPrimitive.attributes.end() ||
			gltfPrimitive.attributes.find("WEIGHTS_0") != gltfPrimitive.attributes.end())
		{
			result.skinned = true;
			extractAccessor(model, model.accessors[gltfPrimitive.attributes.at("JOINTS_0")], boneIDBuffer, skinnedComponentType, skinnedType, skinnedCount);
			extractAccessorBuffer(model, model.accessors[gltfPrimitive.attributes.at("WEIGHTS_0")], boneWeightBuffer);
		}

		EN_ASSERT(
			componentType == TINYGLTF_COMPONENT_TYPE_FLOAT &&
			type == TINYGLTF_TYPE_VEC3,
			"Invalid vertex format !");

		vertices.reserve(count);
		for (unsigned int i = 0; i < count; i++)
		{
			Core::Vertex v;
			unsigned int positonSize = sizeof(float) * 3;
			unsigned int normalSize = sizeof(float) * 3;
			unsigned int texCoordSize = sizeof(float) * 2;
			
			memcpy(&v.x, positionBuffer.data() + positonSize * i, positonSize);
			memcpy(&v.nX, normalBuffer.data() + normalSize * i, normalSize);
			memcpy(&v.tU, textureBuffer.data() + texCoordSize * i, texCoordSize);
			//Skin data
			v.boneIDs[0] = -1; v.boneIDs[1] = -1; v.boneIDs[2] = -1; v.boneIDs[3] = -1;
			v.boneWeights[0] = 0; v.boneWeights[1] = 0; v.boneWeights[2] = 0; v.boneWeights[3] = 0;

			if (result.skinned)
			{
				switch (skinnedComponentType)
				{
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
				case TINYGLTF_COMPONENT_TYPE_BYTE:
				{
					unsigned int boneIDSize = 1 * 4; // one byte
					unsigned char ids[4];
					memcpy(&ids, boneIDBuffer.data() + boneIDSize * i, boneIDSize);
					v.boneIDs[0] = ids[0]; v.boneIDs[1] = ids[1]; v.boneIDs[2] = ids[2]; v.boneIDs[3] = ids[3];
					break;
				}

				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
				case TINYGLTF_COMPONENT_TYPE_SHORT:
				{
					unsigned int boneIDSize = 2 * 4; // two bytes
					unsigned short ids[4];
					memcpy(&ids, boneIDBuffer.data() + boneIDSize * i, boneIDSize);
					v.boneIDs[0] = ids[0]; v.boneIDs[1] = ids[1]; v.boneIDs[2] = ids[2]; v.boneIDs[3] = ids[3];
					break;
				}

				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
				case TINYGLTF_COMPONENT_TYPE_INT:
				{
					unsigned int boneIDSize = 4 * 4; // four bytes
					unsigned int ids[4];
					memcpy(&ids, boneIDBuffer.data() + boneIDSize * i, boneIDSize);
					v.boneIDs[0] = ids[0]; v.boneIDs[1] = ids[1]; v.boneIDs[2] = ids[2]; v.boneIDs[3] = ids[3];
					break;
				}

				default:
					EN_ASSERT(false, "Unknown bone id component type");
					break;
				}

				unsigned int boneWeightSize = sizeof(float) * 4;
				memcpy(&v.boneWeights, boneWeightBuffer.data() + boneWeightSize * i, boneWeightSize);
			}

			maxDistance = glm::max(maxDistance, glm::length(Vec3{ v.x, v.y, v.z }));
			vertices.push_back(v);
		}

		extractAccessor(model, model.accessors[gltfPrimitive.indices], indicesBuffer, componentType, type, count);
		primitive.eboDataType = componentType;
		primitive.vertexCoumt = count;
		glGenVertexArrays(1, &primitive.vao);
		glGenBuffers(1, &primitive.vbo);
		glGenBuffers(1, &primitive.ebo);

		EN_INFO("Allocating gpu memory: vao: {}, vbo: {}, ebo: {}", primitive.vao, primitive.vbo, primitive.ebo);


		glBindVertexArray(primitive.vao);
		glBindBuffer(GL_ARRAY_BUFFER, primitive.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Core::Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (const void*)(sizeof(float) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (const void*)(sizeof(float) * 6));

		glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(Core::Vertex), (const void*)(sizeof(unsigned int) * 8));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (const void*)(sizeof(float) * 8 + sizeof(unsigned int) * 8));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitive.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer.size(), indicesBuffer.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		primitive.materialIndex = gltfPrimitive.material;

		result.primitives.push_back(primitive);
	}

	result.boundingSphereRadius = maxDistance;
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

static Scope<Core::TextureSheet> loadTextureSheet(const String& filePath)
{
	Scope<Core::TextureSheet> result = createScope<Core::TextureSheet>();

	int width, height, bpp;
	stbi_set_flip_vertically_on_load(0);
	stbi_uc* image = stbi_load(filePath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);

	if (image == nullptr)
	{
		EN_ERROR("Failed to load texture: {}", filePath);
		return nullptr;
	}

	glGenTextures(1, &result->id);
	EN_INFO("Generating texture sheet: {}", result->id);


	glBindTexture(GL_TEXTURE_2D, result->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);


	stbi_image_free(image);
	result->name = filePath;
	result->width = width;
	result->height = height;
	result->div = { 1, 1 };
	return result;
}
