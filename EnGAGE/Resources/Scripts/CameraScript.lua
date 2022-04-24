
local x = 0.0
local y = 0.0
local z = 0.0
local pitch = 0.0
local yaw = 0.0
local roll = 0.0
local fov = 60.0
local near = 1.0
local far = 1000.0

local flycam_speed = 5.0
local mouse_sensitivity = 0.3

local move_forward = 0

local direction = {x = 0, y = 0, z = 0}
local forward = false
local backward = false
local left = false
local right = false
local up = false
local down = false

function onMessage(entity, type, message)

	if type == CURSOR_MOVED and _isCursorLocked() then
		local x, y, dx, dy = _getFloat(message, 4)

		yaw = yaw - dx * mouse_sensitivity
		pitch = pitch - dy * mouse_sensitivity

		if pitch > 90.0 then pitch = 90.0
		elseif pitch < -90.0 then pitch = -90.0 end

		if yaw > 360.0 then yaw = -360.0
		elseif yaw < -360.0 then yaw = 360.0 end

	elseif type == KEY_PRESSED then
		local key = _getInt(message, 1)
		if key == KEY_ESCAPE then
			_sendMessage(TOGGLE_CURSOR)
		elseif key == KEY_W then
			forward = true
		elseif key == KEY_S then
			backward = true
		elseif key == KEY_D then
			right = true
		elseif key == KEY_A then
			left = true
		elseif key == KEY_SPACE then
			up = true
		elseif key == KEY_LEFT_SHIFT then
			down = true
		end
	elseif type == KEY_RELEASED then
		local key = _getInt(message, 1)

		if key == KEY_W  then
			forward = false
		elseif key == KEY_S then
			backward = false
		elseif key == KEY_D then
			right = false
		elseif key == KEY_A then
			left = false
		elseif key == KEY_SPACE then
			up = false
		elseif key == KEY_LEFT_SHIFT then
			down = false
		end
	end
end

function update(delta, entity)
	if _isCursorLocked() then
		direction = {x = 0, y = 0, z = 0}

		local sinYaw =  math.sin(math.rad(yaw))
		local sin90Yaw =  math.sin(math.rad(yaw + 90.0))
		local cosYaw =  math.cos(math.rad(yaw))
		local cos90Yaw =  math.cos(math.rad(yaw + 90.0))

		if forward then
			direction.x = direction.x - sinYaw
			direction.z = direction.z - cosYaw
		end

		if backward then
			direction.x = direction.x + sinYaw
			direction.z = direction.z + cosYaw
		end

		if right then
			direction.x = direction.x + sin90Yaw
			direction.z = direction.z + cos90Yaw
		end

		if left then
			direction.x = direction.x - sin90Yaw
			direction.z = direction.z - cos90Yaw
		end

		if up then
			direction.y = direction.y + 1
		end

		if down then
			direction.y = direction.y - 1
		end

		local direction_length_squared = 
		direction.x * direction.x + 
		direction.y * direction.y +
		direction.z * direction.z;

		if direction_length_squared ~= 0.0 then
			local direction_length = math.sqrt(direction_length_squared);
			direction.x = direction.x / direction_length;
			direction.y = direction.y / direction_length;
			direction.z = direction.z / direction_length;
		end

		x = x + direction.x * delta * flycam_speed
		y = y + direction.y * delta * flycam_speed
		z = z + direction.z * delta * flycam_speed
	
	end
	_updateCamera(x, y, z, pitch, yaw, roll, fov, near, far)
end