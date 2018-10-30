function setup(entity)
	--player_playable = true -- 1 should be thought as a playable player 
	entity.points = 0
	entity.jump_timer = 2.0
	entity.can_move = true -- instead of playable
end

function update(delta_seconds, entity)
	--print(entity.points)
	entity.velocity.x = 5
	entity.size.x = 2045
	entity.impulse.y = -13

	--entity.velocity.y = delta_seconds * 100 * entity.direction.z
end