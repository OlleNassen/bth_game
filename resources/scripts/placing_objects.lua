function setup(entity)
	entity.rotate_was_pressed = false
	entity.done = false
	entity.clock = 0.0
end

place_speed = 10

function update(delta_seconds, entity)

	delta_place_speed = place_speed * delta_seconds
	entity.clock = entity.clock + delta_seconds

	if entity.done == false
	then

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

		if entity.button.jump
		then
			entity.done = true
		end

	end

	if entity.clock > 10.0
	then
		entity.done = true
	end


	entity.rotate_was_pressed = entity.button.rotate
end