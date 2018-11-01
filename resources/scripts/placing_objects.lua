function setup(entity)
	
end

function update(delta_seconds, entity)
	
	if entity.button.left
	then
		entity.position.x = entity.position.x + (1.0 * delta_seconds)
		print(entity.position.x)
	end

	if entity.button.right
	then
		entity.position.x = entity.position.x - (1.0 * delta_seconds)
		print(entity.position.x)
	end

	if entity.button.up
	then
		entity.position.y = entity.position.y + (1.0 * delta_seconds)
		print(entity.position.y)
	end

	if entity.button.down
	then
		entity.position.y = entity.position.y - (1.0 * delta_seconds)
		print(entity.position.y)
	end

	if entity.button.rotate
	then
		print("DO ROTATION!")
	end

end