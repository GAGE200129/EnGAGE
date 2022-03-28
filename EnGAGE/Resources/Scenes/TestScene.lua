
_setScript(_addComponent(_createEntity(), SCRIPT), "Resources\\Scripts\\CameraScript.lua")
_setScript(_addComponent(_createEntity(), SCRIPT), "Resources\\Scripts\\TestScript.lua")

local plane = _createEntity();
_addComponent(plane, TRANSFORM);
_setModel(_addComponent(plane, MODEL_RENDERER), "Resources\\Models\\Plane.glb")
local planeRigidBody = _addComponent(plane, RIGID_BODY)
_setRigidBody(planeRigidBody, 0, 0, 0, 0, 0, 0, 0)
_setCollisionShapePlane(planeRigidBody, 0, 1, 0, 0)
