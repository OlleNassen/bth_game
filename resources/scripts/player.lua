function setup(entity)
	--player_playable = true -- 1 should be thought as a playable player 
	entity.points = 3
	entity.lol = "hello"
end

function update(delta_seconds, entity)
	--print(entity.points)
	entity.velocity.x = delta_seconds * 100 * entity.direction.x
	entity.velocity.y = delta_seconds * 100 * entity.direction.z

	return entity.velocity.x, entity.velocity.y
end