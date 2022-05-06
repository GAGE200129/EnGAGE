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
		glm::vec3 min, max;
	};

	struct Texture
	{
		unsigned int textureID;
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
	};

	struct Model
	{
		String name;
		DynArr<Mesh> meshes;
		DynArr<Node> nodes;
		DynArr<Texture> textures;
		DynArr<Material> materials;
		unsigned int rootNodeIndex;
	};
}