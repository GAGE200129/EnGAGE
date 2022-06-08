local distance = 5.0
local offset = {x = 0, y = 1.5, z = 0}
local forward = {x = -1, y = 0, z = -1}

local pitch = 0.0
local yaw = 0.0

local mouseSensitivity = 0.01


function onMessage(entity, type, message)
	if type == CURSOR_MOVED and _isCursorLocked() then
		local _x, _y, dx, dy = _getFloat(message, 4)

		pitch = pitch - dy * mouseSensitivity
		yaw = yaw - dx * mouseSensitivity

		if pitch > 90.0 then pitch = 90.0 
		elseif pitch < -90.0 then pitch = 90.0 end

		if yaw > 360.0 then yaw = -360.0 
		elseif yaw < -360.0 then yaw = 360.0 end

	elseif type == KEY_PRESSED then
		local key = _getInt(message, 1)
		if key == KEY_ESCAPE then
			_toggleCursor()
		end
	end
end


function update(delta, entity)
	x, y, z, rw, rx, ry, rz, sx, sy, sz = _getTransform(_getComponent(entity, TRANSFORM))

	local pitchRadians = -math.rad(pitch)
	local yawRadians = -math.rad(yaw + 90)

	local pitchY = math.sin(pitchRadians) * distance
	local pitchX = math.cos(pitchRadians) * distance

	local offsetX = math.cos(yawRadians) * distance * math.cos(pitchRadians)
	local offsetZ = math.sin(yawRadians) * distance * math.cos(pitchRadians)

	_updateCameraPerspective(x - offsetX + offset.x, y + pitchY + offset.y, z - offsetZ + offset.z, pitch, yaw, 0, 60, 1.1, 100.0)
end