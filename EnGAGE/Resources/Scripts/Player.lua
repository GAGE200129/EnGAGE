local distance = 5.0
local offset = {x = 0, y = 1.5, z = 0}
local forward = {x = -1, y = 0, z = -1}
local camPos = {x = 0, y = 0, z = 0}
local camPitch = 0.0
local camYaw = 0.0

local pitch = 0.0
local yaw = 0.0

local mouseSensitivity = 0.01
local moveSpeed = 1000
local moveForward = false
local moveBackward = false
local moveLeft = false
local moveRight = false
local force = {x = 0, y = 0, z = 0}

function onMessage(entity, type, message)
	if type == CURSOR_MOVED and _isCursorLocked() then
		local _x, _y, dx, dy = _getFloat(message, 4)

		pitch = pitch - dy * mouseSensitivity
		yaw = yaw - dx * mouseSensitivity

		if pitch > 90.0 then
			pitch = 90.0
			camPitch = 90.0
		elseif pitch < -90.0 then 
			pitch = -90.0
			camPitch = -90.0
		end

		if yaw > 360.0 then 
			yaw = -360.0 
			camYaw = -360.0
		elseif yaw < -360.0 then
			yaw = 360.0 
			camYaw = 360.0
		end

	elseif type == KEY_PRESSED then
		local key = _getInt(message, 1)
		if key == KEY_ESCAPE then
			_toggleCursor()
		end

		if key == KEY_W then
			moveForward = true
		end

		if key == KEY_S then
			moveBackward = true
		end

		if key == KEY_A then
			moveLeft = true
		end

		if key == KEY_D then
			moveRight = true
		end

		if key == KEY_SPACE then
			_kinematicBodyJump(_getComponent(entity, KINEMATIC_BODY), 0, 0, 0)
		end

	elseif type == KEY_RELEASED then
		local key = _getInt(message, 1)
		

		if key == KEY_W then
			moveForward = false
		end

		if key == KEY_S then
			moveBackward = false
		end

		if key == KEY_A then
			moveLeft = false
		end

		if key == KEY_D then
			moveRight = false
		end
	end
end

function normalize(v)
	local lengthSquared = v.x ^ 2 + v.y ^ 2 + v.z ^ 2
	if lengthSquared ~= 0.0 then
		local l = math.sqrt(lengthSquared)
		v.x = v.x / l
		v.y = v.y / l
		v.z = v.z / l
	end
end
function lerp(a, b, t)
	return a + (b - a) * t
end

function update(delta, entity)
	local transform = _getComponent(entity, TRANSFORM)
	local x, y, z, rw, rx, ry, rz, sx, sy, sz = _getTransform(transform)

	force.x = 0
	force.y = 0
	force.z = 0
	if moveForward then
		force.x = force.x + math.cos(math.rad(yaw + 90.0))
		force.z = force.z - math.sin(math.rad(yaw + 90.0))
	end
	if moveBackward then
		force.x = force.x - math.cos(math.rad(yaw + 90.0))
		force.z = force.z + math.sin(math.rad(yaw + 90.0))
	end
	if moveLeft then
		force.x = force.x - math.cos(math.rad(yaw))
		force.z = force.z + math.sin(math.rad(yaw))
	end
	if moveRight then
		force.x = force.x + math.cos(math.rad(yaw))
		force.z = force.z - math.sin(math.rad(yaw))
	end
	normalize(force)
	_kinematicBodySetVelocity(_getComponent(entity, KINEMATIC_BODY), force.x * moveSpeed * delta, force.y * moveSpeed * delta, force.z * moveSpeed * delta)



	
	local pitchRadians = -math.rad(pitch)
	local yawRadians = -math.rad(yaw + 90)

	local pitchY = math.sin(pitchRadians) * distance
	local pitchX = math.cos(pitchRadians) * distance

	local offsetX = math.cos(yawRadians) * distance * math.cos(pitchRadians)
	local offsetZ = math.sin(yawRadians) * distance * math.cos(pitchRadians)


	camPos.x = lerp(camPos.x, x - offsetX + offset.x, delta * 10)
	camPos.y = lerp(camPos.y, y + pitchY + offset.y, delta * 10)
	camPos.z = lerp(camPos.z, z - offsetZ + offset.z, delta * 10)
	camPitch = lerp(camPitch, pitch, delta * 50)
	camYaw = lerp(camYaw, yaw, delta * 50)
	_updateCameraPerspective(camPos.x, camPos.y, camPos.z, camPitch, camYaw, 0, 60, 1.1, 100.0)
end