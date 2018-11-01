function setup(entity)

end

rotate_was_pressed = { false, false, false, false }
iterator = 1
place_speed = 1.0;

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
		print("NOW")
		entity.position.y = entity.position.y + delta_place_speed
	end
	
	if entity.button.down
	then
		entity.position.y = entity.position.y - delta_place_speed
	end

	if iterator == 1
	then
		if entity.button.rotate and not rotate_was_pressed[iterator]
		then
			print("DO ROTATION!")
		end

		rotate_was_pressed[iterator] = entity.button.rotate
	end

	iterator = iterator + 1
	if iterator > 4
	then
		iterator = 1
	end
end