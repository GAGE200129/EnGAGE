local entity = _createEntity()
_setName(_addComponent(entity, NAME), "Sun")
_setDirectionalLight(_addComponent(entity, DIRECTIONAL_LIGHT), -1, -0.25, -1, 1, 1, 1, 0.5)
local entity = _createEntity()
_setName(_addComponent(entity, NAME), "Fox")
entity_transform = _addComponent(entity, TRANSFORM)
_setPosition(entity_transform, -10.2517, 1.03001, 0.846834)
_setRotation(entity_transform, 0.889355, -0.0551693, -0.453823, 0.00703328)
_setScale(entity_transform, 0.0999991, 0.100001, 0.0999981)
_setModel(_addComponent(entity, MODEL_RENDERER), "Resources/Models/Fox_fixed.glb")
_setPointLight(_addComponent(entity, POINT_LIGHT), 1, 0, 0, 1, 1, 1, 1)
local entity = _createEntity()
_setName(_addComponent(entity, NAME), "Player")
entity_transform = _addComponent(entity, TRANSFORM)
_setPosition(entity_transform, -1.00456, 2.03, 0.138873)
_setRotation(entity_transform, 1, 0, 0, 0)
_setScale(entity_transform, 1, 0.999998, 1)
_setModel(_addComponent(entity, MODEL_RENDERER), "Resources/Models/Sphere.glb")
_setScript(_addComponent(entity, SCRIPT), "Resources/Scripts/Player.lua")
_addComponent(entity, KINEMATIC_BODY)
_setPointLight(_addComponent(entity, POINT_LIGHT), 0, 1, 0, 1, 1, 0.27, 0.608)
local entity = _createEntity()
_setName(_addComponent(entity, NAME), "Sun2")
_setDirectionalLight(_addComponent(entity, DIRECTIONAL_LIGHT), 0.72, -1, 0.73, 1, 1, 1, 0.5)
_setTextureSheet("Resources/TextureSheets/PEEPEE.png", 4, 4)
_addFace(-14.4, 0, 5.1, 16.6, 0, 4.5, 17, 0, -5, -14.5, 0, -4.1, 2, 3)
_addFace(-14.5, 7.2, -4.1, 17, 7.2, -5, 17, 0, -5, -14.5, 0, -4.1, 1, 1)
_addFace(-4.5, 5, 18.6, -9, 6.2, 18.5, -5.7, 0, 5, 1.2, 0.1, 4.5, 0, 0)
