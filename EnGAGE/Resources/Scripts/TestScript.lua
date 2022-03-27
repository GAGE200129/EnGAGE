

entities = {}

function generateEntity()
	local entityID = _createEntity()
	local transform = _addComponent(entityID, TRANSFORM)
	local model = _addComponent(entityID, MODEL_RENDERER)
	local rigidbody = _addComponent(entityID, RIGID_BODY)
	local script = _addComponent(entityID, SCRIPT)

	_setPosition(transform, math.random(-1, 1), 0, math.random(-1, 1))
	_setModel(model, "Resources\\Models\\Sphere.glb")
	_setScript(script, "Resources\\Scripts\\RemoveScript.lua")
	_setRigidBody(rigidbody, math.random(-5, 5), math.random(10, 30), math.random(-5, 5), 0.0, 0.0, 0.0, 2.0)
	_setCollisionShapeSphere(rigidbody, 1.0)
end

function generateEntities(n)

	for i, e in pairs(entities) do
			_markForRemove(e)
			entities[i] = nil
	end

	for i = 0, n, 1 do
		local entityID = _createEntity()
		entities[i] = entityID
		transform = _addComponent(entityID, TRANSFORM)
		model = _addComponent(entityID, MODEL_RENDERER)
		rigidbody = _addComponent(entityID, RIGID_BODY)

		_setPosition(transform, math.random(-5, 5), math.random(10, 30), math.random(-5, 5))
		_setModel(model, "Resources\\Models\\Sphere.glb")
		_setRigidBody(rigidbody, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0)
		_setCollisionShapeSphere(rigidbody, 1.0)
	end
end

function input()
	if _keyPressedOnce(KEY_1) then
		generateEntities(12)
	elseif _keyPressedOnce(KEY_2) then
		generateEntities(24)
	elseif _keyPressedOnce(KEY_3) then
		generateEntities(36)
	elseif _keyPressedOnce(KEY_4) then
		generateEntities(36)
	elseif _keyPressedOnce(KEY_5) then
		generateEntities(48)
	elseif _keyPressedOnce(KEY_6) then
		generateEntities(56)
	end

	if _keyPressed(KEY_F) then
		generateEntity()
	end
end

function update(delta)
	
end