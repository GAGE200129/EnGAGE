entity = _createEntity()
_setScript(_addComponent(entity, SCRIPT), "Resources/Scripts/CameraScript.lua")


entity = _createEntity()
entity_transform = _addComponent(entity, TRANSFORM)
_setPosition(entity_transform, 15.4765, 0.975888, -0.15961)
_setRotation(entity_transform, 1, 0, 0, 0)
_setScale(entity_transform, 1, 1, 1)
_setModel(_addComponent(entity, MODEL_RENDERER), "Resources/Models/Sphere.glb")
entity_rigidBody = _addComponent(entity, RIGID_BODY)
_setRigidBody(entity_rigidBody, 1)
_setColShapeSphere(entity_rigidBody, 1)

entity = _createEntity()
_setDirectionalLight(_addComponent(entity, DIRECTIONAL_LIGHT), 0.6, -1, 1, 1, 0, 0, 0.3)

entity = _createEntity()
entity_transform = _addComponent(entity, TRANSFORM)
_setPosition(entity_transform, 0, 0.96342, 0)
_setRotation(entity_transform, 1, 0, 0, 0)
_setScale(entity_transform, 1, 1, 1)
_setModel(_addComponent(entity, MODEL_RENDERER), "Resources/Models/Sphere.glb")
entity_rigidBody = _addComponent(entity, RIGID_BODY)
_setRigidBody(entity_rigidBody, 1)
_setColShapeSphere(entity_rigidBody, 1)

entity = _createEntity()
_setDirectionalLight(_addComponent(entity, DIRECTIONAL_LIGHT), -1, -1, -1, 1, 1, 1, 1.0)

