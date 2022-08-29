#pragma once

#include "pch.hpp"

namespace Core
{
	struct Primitive
	{
		unsigned int vao, vbo, ebo, vertexCoumt, eboDataType, materialIndex;
	};

	struct Mesh
	{
		String name;
		DynArr<Primitive> primitives;
		float boundingSphereRadius;
		bool skinned;
	};

	struct Texture
	{
		String name;
		UInt32 textureID;
	};

	struct Material
	{
		int baseColorIndex;
	};

	struct Node
	{
		String name;
		int meshIndex;
		glm::vec3 position;
		glm::vec3 scale;
		glm::quat rotation;
		DynArr<int> children;
		glm::mat4 inverseBindTransform;//For animation
	};


	struct Sampler
	{
		//enum class OutputType
		//{
		//	Vec3,
		//	Quaternion
		//};
		//OutputType outputType;
		std::vector<float> input;
		std::vector<float> output;
	};

	struct Channel
	{
		enum class Path
		{
			TRANSLATION,
			ROTATION,
			SCALE
		};
		size_t samplerIndex;
		size_t node;
		Path path;
	};
	struct AnimationData
	{
		std::string name;
		std::vector<Sampler> samplers;
		std::vector<Channel> channels;
	};

	struct Model
	{
		String name;
		DynArr<Mesh> meshes;
		DynArr<Node> nodes;
		DynArr<Texture> textures;
		DynArr<Material> materials;
		std::vector<AnimationData> animations;
		unsigned int rootNodeIndex;
	};
}