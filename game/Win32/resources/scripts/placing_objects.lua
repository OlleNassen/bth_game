function setup(entity)
	entity.rotate_was_pressed = false
	entity.done = false
	entity.force_done = false
	entity.clock = 0.0
	entity.time = 0.0
end

place_speed = 10
max_time = 15.0

function update(delta_seconds, entity)

	delta_place_speed = place_speed * delta_seconds
	entity.clock = entity.clock + delta_seconds
	entity.time = max_time - entity.clock

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

		if entity.button.jump
		then
			entity.done = true
		end

	end

	if entity.clock > max_time
	then
		entity.force_done = true
	end

	width = 20

	if entity.position.x < -width
	then
		entity.position.x  = -width
	elseif entity.position.x > width
	then
		entity.position.x = width
	end
	
	maxHeight = 256.0
	minHeight = 19.32

	if entity.position.y < minHeight
	then
		entity.position.y  = minHeight
	elseif entity.position.y > maxHeight
	then
		entity.position.y = maxHeight
	end

	entity.rotate_was_pressed = entity.button.rotate
end