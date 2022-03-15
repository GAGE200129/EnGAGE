#pragma once

#include "pch.hpp"

namespace Core
{
	struct Primitive
	{
		unsigned int vao, vbo, ebo, vertexCoumt, eboDataType;
	};

	struct Mesh
	{
		String name;
		DynArr<Primitive> primitives;
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
		DynArr<Mesh> meshes;
		DynArr<Node> nodes;
		unsigned int rootNodeIndex;
	};
}