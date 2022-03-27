
function update()
	local rigidBody = _getComponent(_entity, RIGID_BODY)

	local x, y, z = _getVelocity(rigidBody)
	local lengthSquared = x ^ 2 + y ^ 2 + z ^ 2
	if lengthSquared < 0.01 then
		_markForRemove(_entity)
	end
	
end