#include "pch.hpp"
#include "Physics.hpp"

#include "ECS.hpp"

struct CollisionPoints
{
	Core::ECS::RigidBodyComponent* a;
	Core::ECS::TransformComponent* aT;

	Core::ECS::RigidBodyComponent* b;
	Core::ECS::TransformComponent* bT;

	glm::vec3 normal;
	float distance;
};

static glm::vec3 gGravity = { 0, -9.8f, 0 };
//static float gTerminalSpeed = ;

static void solveSpherePlaneCollision(DynArr<CollisionPoints>& points, Core::ECS::RigidBodyComponent* sphereRB, Core::ECS::TransformComponent* sphereT,
	Core::ECS::RigidBodyComponent* planeRB, Core::ECS::TransformComponent* planeT);
static void solveSphereSphereCollision(DynArr<CollisionPoints>& points, Core::ECS::RigidBodyComponent* sphereARB, Core::ECS::TransformComponent* sphereAT,
	Core::ECS::RigidBodyComponent* sphereBRB, Core::ECS::TransformComponent* sphereBT);
static void solveCollision(DynArr<CollisionPoints>& points, Core::ECS::RigidBodyComponent* aRB, Core::ECS::TransformComponent* aT,
	Core::ECS::RigidBodyComponent* bRB, Core::ECS::TransformComponent* bT);

static void collisionSolve(DynArr<CollisionPoints>& points);

void Core::Physics::init()
{
}

void Core::Physics::shutdown()
{
}

void Core::Physics::update(float delta)
{
	using namespace ECS;
	const System& system = getSystem(SystemType::PHYSICS);

	static DynArr<CollisionPoints> collisionPoints;

	//Collisions
	const auto& entities = system.entities;
	for (auto a = entities.begin(); a != entities.end(); a++)
	{
		for (auto b = std::next(a); b != entities.end(); b++)
		{
			RigidBodyComponent* aRB = (RigidBodyComponent*)getComponent(*a, ComponentType::RIGID_BODY);
			TransformComponent* aT = (TransformComponent*)getComponent(*a, ComponentType::TRANSFORM);
			RigidBodyComponent* bRB = (RigidBodyComponent*)getComponent(*b, ComponentType::RIGID_BODY);
			TransformComponent* bT = (TransformComponent*)getComponent(*b, ComponentType::TRANSFORM);
			solveCollision(collisionPoints, aRB, aT, bRB, bT);
		}
	}

	//Dynamics
	for (const auto& entity : system.entities)
	{
		TransformComponent* transform = (TransformComponent*)getComponent(entity, ComponentType::TRANSFORM);
		RigidBodyComponent* rigidBody = (RigidBodyComponent*)getComponent(entity, ComponentType::RIGID_BODY);
		if (rigidBody->mass == 0.0f) continue;

		rigidBody->force += rigidBody->mass * gGravity;
		rigidBody->velocity += rigidBody->force / rigidBody->mass * delta;
		transform->x += rigidBody->velocity.x * delta;
		transform->y += rigidBody->velocity.y * delta;
		transform->z += rigidBody->velocity.z * delta;
		rigidBody->force = { 0, 0, 0 };
	}
	collisionSolve(collisionPoints);
	collisionPoints.clear();
}

void solveSpherePlaneCollision(
	DynArr<CollisionPoints>& points,
	Core::ECS::RigidBodyComponent* sphereRB,
	Core::ECS::TransformComponent* sphereT,
	Core::ECS::RigidBodyComponent* planeRB,
	Core::ECS::TransformComponent* planeT)
{
	Core::ECS::SphereCollider* sphereCollider = (Core::ECS::SphereCollider*)sphereRB->colliderData;
	Core::ECS::PlaneCollider* planeCollider = (Core::ECS::PlaneCollider*)planeRB->colliderData;
	float dot = sphereT->x * planeCollider->x + sphereT->y * planeCollider->y + sphereT->z * planeCollider->z;
	float distance = dot - planeCollider->distance - sphereCollider->radius;
	if (distance < 0.0f) {
		glm::vec3 normal = { planeCollider->x, planeCollider->y, planeCollider->z };
		glm::vec3 spherePos = { sphereT->x, sphereT->y, sphereT->z };
		points.push_back(
			CollisionPoints{
				sphereRB, sphereT,
				planeRB, planeT ,

				normal,
				glm::abs(distance)
			}
		);
	}
}

void solveSphereSphereCollision(DynArr<CollisionPoints>& points, Core::ECS::RigidBodyComponent* sphereARB, Core::ECS::TransformComponent* sphereAT, Core::ECS::RigidBodyComponent* sphereBRB, Core::ECS::TransformComponent* sphereBT)
{
	Core::ECS::SphereCollider* sphereACollider = (Core::ECS::SphereCollider*)sphereARB->colliderData;
	Core::ECS::SphereCollider* sphereBCollider = (Core::ECS::SphereCollider*)sphereBRB->colliderData;

	glm::vec3 sphereAPos = { sphereAT->x, sphereAT->y, sphereAT->z };
	glm::vec3 sphereBPos = { sphereBT->x, sphereBT->y, sphereBT->z };
	glm::vec3 normal = sphereAPos - sphereBPos;
	float distance = glm::length(normal);
	normal /= distance;

	if (distance < (sphereACollider->radius + sphereBCollider->radius))
	{
		points.push_back(
			CollisionPoints{
				sphereARB, sphereAT,
				sphereBRB, sphereBT ,

				normal,
				(sphereACollider->radius + sphereBCollider->radius) - distance
			}
		);
	}
}

static void solveCollision(
	DynArr<CollisionPoints>& points,
	Core::ECS::RigidBodyComponent* aRB,
	Core::ECS::TransformComponent* aT,
	Core::ECS::RigidBodyComponent* bRB,
	Core::ECS::TransformComponent* bT)
{
	using namespace Core::ECS;
	if (aRB->colliderType == ColliderType::SPHERE && bRB->colliderType == ColliderType::PLANE)
	{
		//A is sphere, B is plane
		solveSpherePlaneCollision(points, aRB, aT, bRB, bT);
	}
	else if (bRB->colliderType == ColliderType::SPHERE && aRB->colliderType == ColliderType::PLANE)
	{
		//A is Plane, B is Sphere
		solveSpherePlaneCollision(points, bRB, bT, aRB, aT);
	}
	else if (aRB->colliderType == ColliderType::SPHERE && bRB->colliderType == ColliderType::SPHERE)
	{
		//A is sphere, B is sphere
		solveSphereSphereCollision(points, aRB, aT, bRB, bT);
	}
}

static void collisionSolve(DynArr<CollisionPoints>& points)
{
	constexpr float percent = 0.8f;
	constexpr float slop = 0.01f;

	constexpr float aBounciness = 0.2f, bBounciness = 0.2f;
	constexpr float aStaticFriction = 0.96f, bStaticFriction = 0.96f;
	constexpr float aDynamicFriction = 0.96f, bDynamicFriction = 0.96f;


	auto positionSolve = [&](CollisionPoints& point)
	{
		float aInvMass = point.a->mass != 0.0f ? 1.0f / point.a->mass : 0.0f;
		float bInvMass = point.b->mass != 0.0f ? 1.0f / point.b->mass : 0.0f;
		glm::vec3 correction = point.normal * point.distance * glm::max(point.distance - slop, 0.0f) / (aInvMass + bInvMass);

		if (point.a->mass != 0.0f)
		{
			point.aT->x += correction.x;
			point.aT->y += correction.y;
			point.aT->z += correction.z;
		}

		if (point.b->mass != 0.0f)
		{
			point.bT->x -= correction.x;
			point.bT->y -= correction.y;
			point.bT->z -= correction.z;
		}
	};

	auto impulseSolve = [&](CollisionPoints& point)
	{
		glm::vec3 relativeVel = point.a->velocity - point.b->velocity;
		float normalSpeed = glm::dot(relativeVel, point.normal);

		float aInvMass = point.a->mass != 0.0f ? 1.0f / point.a->mass : 0.0f;
		float bInvMass = point.b->mass != 0.0f ? 1.0f / point.b->mass : 0.0f;

		if (normalSpeed >= 0)
			return;
		float e = aBounciness * bBounciness;
		float j = -(1.0f + e) * normalSpeed / (aInvMass + bInvMass);
		glm::vec3 impulse = j * point.normal;


		glm::vec3 tangent = relativeVel - normalSpeed * point.normal;

		if (glm::length2(tangent) != 0.0f)
		{
			tangent = glm::normalize(tangent);
		}

		float fVel = glm::dot(relativeVel, tangent);
		float f = -fVel / (aInvMass + bInvMass);
		float mu = glm::length(glm::vec2(aStaticFriction, bStaticFriction));

		glm::vec3 friction;
		if (glm::abs(f) < j * mu)
		{
			friction = f * tangent;
		}
		else
		{
			mu = glm::length(glm::vec2(aDynamicFriction, bDynamicFriction));
			friction = -j * tangent * mu;
		}

		point.a->velocity += (impulse + friction) * aInvMass;
		point.b->velocity -= (impulse - friction) * bInvMass;
	};

	for (auto& point : points)
	{
		positionSolve(point);
		impulseSolve(point);
	}
}
