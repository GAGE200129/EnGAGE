entity0 = _createEntity()
_setScript(_addComponent(entity0, SCRIPT), "Resources/Scripts/CameraScript.lua")


entity2 = _createEntity()
entity2_transform = _addComponent(entity2, TRANSFORM)
_setPosition(entity2_transform, 0, 0, 0)
_setRotation(entity2_transform, 1, 0, 0, 0)
_setScale(entity2_transform, 1, 1, 1)
_setModel(_addComponent(entity2, MODEL_RENDERER), "Resources/Models/Plane.glb")
entity2_rigidBody = _addComponent(entity2, RIGID_BODY)
_setRigidBody(entity2_rigidBody, 0, 0, 0, 0, 0, 0, 0)
_setCollisionShapePlane(entity2_rigidBody, 0, 1, 0, 0)



entity75 = _createEntity()
entity75_transform = _addComponent(entity75, TRANSFORM)
_setPosition(entity75_transform, 4.82356, 0.976233, 13.3001)
_setRotation(entity75_transform, 1, 0, 0, 0)
_setScale(entity75_transform, 1, 1, 1)
_setModel(_addComponent(entity75, MODEL_RENDERER), "Resources/Models/Sphere.glb")
entity75_rigidBody = _addComponent(entity75, RIGID_BODY)
_setRigidBody(entity75_rigidBody, 0, 0.00314102, 0, 0, 0, 0, 2)
_setCollisionShapeSphere(entity75_rigidBody, 1)
