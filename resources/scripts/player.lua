function setup(entity)
	--player_playable = true -- 1 should be thought as a playable player 
	entity.jump_timer = 1.0
	entity.can_move = true -- instead of playable
end

function update(delta_seconds, entity)

	if entity.button.left 
	then
		entity.forces.x = -4000
	end

	if entity.button.right 
	then
		entity.forces.x = 4000
	end

	if entity.button.jump and entity.jump_timer < 0
	then
		entity.jump_timer = 1.0
		entity.impulse.y = 25
	end

	entity.jump_timer = entity.jump_timer - delta_seconds
	--entity.forces.x = -1000
	--entity.size.x = 2045
	--entity.impulse.y =  50

	--entity.velocity.y = delta_seconds * 100 * entity.direction.z
end