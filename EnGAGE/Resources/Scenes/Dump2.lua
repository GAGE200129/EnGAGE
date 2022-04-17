entity0 = _createEntity()
_setScript(_addComponent(entity0, SCRIPT), "Resources/Scripts/CameraScript.lua")

entity1 = _createEntity()
entity1_transform = _addComponent(entity1, TRANSFORM)
_setPosition(entity1_transform, 0, 0, 0)
_setRotation(entity1_transform, 1, 0, 0, 0)
_setScale(entity1_transform, 1, 1, 1)
_setModel(_addComponent(entity1, MODEL_RENDERER), "Resources/Models/Plane.glb")
entity1_rigidBody = _addComponent(entity1, RIGID_BODY)
_setRigidBody(entity1_rigidBody, 0, 0, 0, 0, 0, 0, 0)
_setCollisionShapePlane(entity1_rigidBody, 0, 1, 0, 0)

entity2 = _createEntity()
entity2_transform = _addComponent(entity2, TRANSFORM)
_setPosition(entity2_transform, 4.82356, 0.980405, 13.3001)
_setRotation(entity2_transform, 1, 0, 0, 0)
_setScale(entity2_transform, 1, 1, 1)
_setModel(_addComponent(entity2, MODEL_RENDERER), "Resources/Models/Sphere.glb")
entity2_rigidBody = _addComponent(entity2, RIGID_BODY)
_setRigidBody(entity2_rigidBody, 0, 0.036547, 0, 0, 0, 0, 2)
_setCollisionShapeSphere(entity2_rigidBody, 1)

entity3 = _createEntity()
entity3_transform = _addComponent(entity3, TRANSFORM)
_setPosition(entity3_transform, 0, 3.769, 0)
_setRotation(entity3_transform, 1, 0.4, 1, 0)
_setScale(entity3_transform, 1, 1, 1)
_setModel(_addComponent(entity3, MODEL_RENDERER), "Resources/Models/NodeTest.glb")
