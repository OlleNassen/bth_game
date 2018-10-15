function setup()
	player_playable = true -- 1 should be thought as a playable player 
	player_points = 0
end

function update(delta_seconds, entity)
	entity.velocity.x = delta_seconds * 100 * entity.direction.x
	entity.velocity.y = delta_seconds * 100 * entity.direction.z	
	
	return entity.velocity.x, entity.velocity.y
end

function reached_goal(value)
	--print("You reached the goal")
	player_playable = value
end

function get_playable()
	return player_playable
end