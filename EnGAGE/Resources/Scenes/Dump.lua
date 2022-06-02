entity = _createEntity()
_setScript(_addComponent(entity, SCRIPT), "Resources/Scripts/CameraScript.lua")

entity = _createEntity()
entity_transform = _addComponent(entity, TRANSFORM)
_setPosition(entity_transform, 16.2193, 1.39997, -0.396475)
_setRotation(entity_transform, 0.990448, -0.125052, 0.00301979, -0.058012)
_setScale(entity_transform, 1, 1, 1)
_setModel(_addComponent(entity, MODEL_RENDERER), "Resources/Models/Sphere.glb")
entity_rigidBody = _addComponent(entity, RIGID_BODY)
_setRigidBody(entity_rigidBody, 1)
_setColShapeSphere(entity_rigidBody, 1)

entity = _createEntity()
_setDirectionalLight(_addComponent(entity, DIRECTIONAL_LIGHT), 0.6, -1, 1, 1, 0, 0, 0.3)

entity = _createEntity()
entity_transform = _addComponent(entity, TRANSFORM)
_setPosition(entity_transform, 14.2231, 1.39995, -0.542616)
_setRotation(entity_transform, 0.999965, -0.000545386, 2.05756e-07, 0.0083115)
_setScale(entity_transform, 1, 1, 1)
_setModel(_addComponent(entity, MODEL_RENDERER), "Resources/Models/Sphere.glb")
entity_rigidBody = _addComponent(entity, RIGID_BODY)
_setRigidBody(entity_rigidBody, 1)
_setColShapeSphere(entity_rigidBody, 1)

entity = _createEntity()
_setDirectionalLight(_addComponent(entity, DIRECTIONAL_LIGHT), -1, -1, -1, 1, 1, 1, 1)

_setTextureSheet("Resources/TextureSheets/PEEPEE.png", 2, 2)
_addWall(-2.6, 0, -7.1, -2.6, 0, 4.4, 24.5, 0, 4.4, 24.5, 0, -7.1, -2.6, 0.4, -7.1, -2.6, 0.4, 4.4, 24.5, 0.4, 4.4, 24.5, 0.4, -7.1)
_addWall(-2.6, 0, 4.4, -2.6, 0, 4.8, 24.5, 0, 4.8, 24.5, 0, 4.4, -2.6, 16, 4.4, -2.6, 16, 4.8, 24.5, 16, 4.8, 24.5, 16, 4.4)
_addWall(-2.6, 0, -7.6, -2.6, 0, -7.1, 24.5, 0, -7.1, 24.5, 0, -7.6, -2.6, 10.8, -7.6, -2.6, 10.8, -7.1, 24.5, 10.8, -7.1, 24.5, 10.8, -7.6)

