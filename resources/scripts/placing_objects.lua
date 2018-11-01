function setup(entity)
	entity.rotate_was_pressed = false
end

place_speed = 9.82

function update(delta_seconds, entity)

	delta_place_speed = place_speed * delta_seconds

	if entity.button.left
	then
		entity.position.x = entity.position.x - delta_place_speed
	end
	
	if entity.button.right
	then
		entity.position.x = entity.position.x + delta_place_speed
	end

	if entity.button.up
	then
		entity.position.y = entity.position.y + delta_place_speed
	end
	
	if entity.button.down
	then
		entity.position.y = entity.position.y - delta_place_speed
	end

	if entity.button.rotate and not entity.rotate_was_pressed
	then
		print("DO ROTATION!")
	end

	entity.rotate_was_pressed = entity.button.rotate
end