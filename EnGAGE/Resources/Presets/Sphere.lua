
function create()
	entity = _createEntity()
	entity_transform = _addComponent(entity, TRANSFORM)
	_setPosition(entity_transform, 17.6934, 1.4, 9.29726)
	_setRotation(entity_transform, 0.462145, 0.392665, 0.188358, 0.772501)
	_setScale(entity_transform, 1, 1, 1)
	_setModel(_addComponent(entity, MODEL_RENDERER), "Resources/Models/Sphere.glb")
	entity_rigidBody = _addComponent(entity, RIGID_BODY)
	_setRigidBody(entity_rigidBody, 1)
	_rigidBodyColShapeSphere(entity_rigidBody, 1)
end