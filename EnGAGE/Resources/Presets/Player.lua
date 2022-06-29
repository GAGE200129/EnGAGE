
function create()
	entity = _createEntity()
	entity_transform = _addComponent(entity, TRANSFORM)
	_setPosition(entity_transform, 17.4502, 1.4, -3.3488)
	_setRotation(entity_transform, -0.300518, 0.850361, -0.064038, -0.427169)
	_setScale(entity_transform, 1.00002, 0.999998, 1.00002)
	_setModel(_addComponent(entity, MODEL_RENDERER), "Resources/Models/Sphere.glb")
	_setScript(_addComponent(entity, SCRIPT), "Resources/Scripts/Player.lua")
	entity_rigidBody = _addComponent(entity, RIGID_BODY)
	_setRigidBody(entity_rigidBody, 1)
	_rigidBodyColShapeSphere(entity_rigidBody, 1)
end