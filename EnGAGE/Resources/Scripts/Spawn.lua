
function onMessage(entity, type, message)
	if type == KEY_PRESSED then
		keyCode = _getInt(message, 1)
		if keyCode == KEY_F then
			entity = _request(NEW_ENTITY)
		end
	end

end


function update(delta, entity)
	
end