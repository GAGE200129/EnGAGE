
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

local direction = { x = 0, y = 0, z = 0 }

function input(entity)
	
	direction.x = 0
	direction.y = 0
	direction.z = 0

	
	if _keyPressedOnce(KEY_ESCAPE) then
		_toggleCursor()
	end

	if _isCursorLocked() then
		local dX, dY = _getCursorPosDelta()

		yaw = yaw - dX * mouse_sensitivity
		pitch = pitch - dY * mouse_sensitivity

		if pitch > 90.0 then pitch = 90.0
		elseif pitch < -90.0 then pitch = -90.0 end

		if yaw > 360.0 then yaw = -360.0
		elseif yaw < -360.0 then yaw = 360.0 end

		if _keyPressed(KEY_W) then
			direction.x = direction.x - math.sin(math.rad(yaw))
			direction.z = direction.z - math.cos(math.rad(yaw))
		end

		if _keyPressed(KEY_S) then
			direction.x = direction.x + math.sin(math.rad(yaw))
			direction.z = direction.z + math.cos(math.rad(yaw))
		end

		if _keyPressed(KEY_D) then
			direction.x = direction.x + math.sin(math.rad(yaw + 90.0))
			direction.z = direction.z + math.cos(math.rad(yaw + 90.0))
		end

		if _keyPressed(KEY_A) then
			direction.x = direction.x - math.sin(math.rad(yaw + 90.0))
			direction.z = direction.z - math.cos(math.rad(yaw + 90.0))
		end

		if _keyPressed(KEY_SPACE) then
			direction.y = direction.y + 1
		end

		if _keyPressed(KEY_LEFT_SHIFT) then
			direction.y = direction.y - 1
		end

		local length = direction.x ^ 2 + direction.y ^ 2 + direction.z ^ 2
		if length ~= 0 then
			direction.x = direction.x / length
			direction.y = direction.y / length
			direction.z = direction.z / length
		end
	end
end

function update(delta, entity)

	x = x + direction.x * delta * flycam_speed; 
	y = y + direction.y * delta * flycam_speed; 
	z = z + direction.z * delta * flycam_speed;
	
	_updateCamera(x, y, z, pitch, yaw, roll, fov, near, far)
end