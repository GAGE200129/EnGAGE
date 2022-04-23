#pragma once

namespace Core
{
	namespace Physics
	{

		enum class ColliderType : unsigned int
		{
			NONE,
			SPHERE,
			PLANE,
			COUNT
		};

		struct SphereCollider
		{
			float radius;
		};

		struct PlaneCollider
		{
			float x, y, z; // Normal
			float distance;
		};

		struct ColliderData
		{
			const char* name;
			unsigned int size;
		};

		void init();
		void shutdown();

		void update(float delta);

		const ColliderData& getColliderData(ColliderType type);
		void initCollider(char* colliderData, ColliderType type);

	}
}