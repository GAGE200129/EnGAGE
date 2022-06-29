function create()
local entity = _createEntity()
_setDirectionalLight(_addComponent(entity, DIRECTIONAL_LIGHT), 0.33, -1, -1, 1, 1, 1, 1)
end
