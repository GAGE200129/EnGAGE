
temp = math.random()
speed = math.random()

randomScale = math.random()

function update(delta)
	local transform = _getComponent(entity, TRANSFORM)

	temp = temp + delta

	transform["x"] = math.sin(temp * speed) * randomScale * 100
	transform["y"] = math.cos(temp * speed) * randomScale * 100
	transform["z"] = math.cos(temp * speed) * randomScale * 100


	_updateComponent(entity, TRANSFORM, transform)
	
end