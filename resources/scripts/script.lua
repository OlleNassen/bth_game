function setup()
	player_playable = false
	

end

function update(delta_seconds, entity)
	entity.velocity.x = delta_seconds * 100 * entity.direction.x
	entity.velocity.y = delta_seconds * 100 * entity.direction.z	
	
	return entity.velocity.x, entity.velocity.y
end