

function spawnShits()
	local x = math.random(-10, 10)
	local y = math.random(0, 10)
	local z = math.random(-10, 10)

	local entity = _createEntity()
	local entity_transform = _addComponent(entity, TRANSFORM)
	_setPosition(entity_transform, x, y, z)
	_setRotation(entity_transform, 1, 0, 0, 0)
	_setScale(entity_transform, 1, 1, 1)
	_setModel(_addComponent(entity, MODEL_RENDERER), "Resources/Models/SphereSmooth.glb")
	local entity_rigidBody = _addComponent(entity, RIGID_BODY)
	_setRigidBody(entity_rigidBody, 1, SPHERE, 1)
end


function onMessage(entity, type, message)
	if type == KEY_PRESSED then
		keyCode = _getInt(message, 1)
		if keyCode == KEY_F then
			spawnShits()
		end
	end

end


function update(delta, entity)
	
end