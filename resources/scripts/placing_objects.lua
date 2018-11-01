function setup(entity)

end

rotateWasPressed = { false, false, false, false }
iterator = 1

function update(delta_seconds, entity)

	
	if entity.button.left
	then
		entity.position.x = entity.position.x + (1.0 * delta_seconds)
	elseif entity.button.right
	then
		entity.position.x = entity.position.x - (1.0 * delta_seconds)
	end
	
	if entity.button.up
	then
		entity.position.y = entity.position.y + (1.0 * delta_seconds)
	elseif entity.button.down
	then
		entity.position.y = entity.position.y - (1.0 * delta_seconds)
	end

	if iterator == 1
	then
		if entity.button.rotate and not rotateWasPressed[iterator]
		then
			print("DO ROTATION!")
		end

		rotateWasPressed[iterator] = entity.button.rotate
	end

	iterator = iterator + 1
	if iterator > 4
	then
		iterator = 1
	end
end