function setup(entity)
	--player_playable = true -- 1 should be thought as a playable player 
	entity.jump_timer = 1.0
	entity.can_jump = true
	entity.can_walljump = true
	entity.can_move = true -- instead of playable
	entity.maxSpeed = 7000
	entity.acceleration = 5500
	entity.jump_timer = 0
end

function update(delta_seconds, entity)

	
	if entity.anim.current == entity.anim.running
	then
	
		entity.anim.current = entity.anim.idle
		if entity.button.right 
		then
			entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds)
			--entity.forces.x = 4000
		end
		if entity.button.left
		then
			entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds)
			--entity.forces.x = -4000
		end
		if entity.button.jump
		then
			entity.anim.current = entity.anim.start_jump
		end
		if entity.velocity.y < -1.4
		then 
			entity.anim.current = entity.anim.falling
		end
	end

	if entity.anim.current == entity.anim.idle
	then
		if entity.button.right 
		then
			entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds)
			--entity.forces.x = 4000
			entity.anim.current = entity.anim.running
		end
		if entity.button.left 
		then
			entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds)
			--entity.forces.x = -4000
			entity.anim.current = entity.anim.running
		end
		if entity.button.jump
		then
			entity.anim.current = entity.anim.start_jump
		end
		--if entity.velocity.y < -0.4
		--then 
		--	entity.anim.current = entity.anim.falling
		--end
	end

	if entity.anim.current == entity.anim.in_jump
	then
		if entity.can_jump
		then
			entity.impulse.y = 41
			entity.can_jump = false
		end
		if entity.button.right 
		then
			entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.button.left
		then
			entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.velocity.y < -0.8
		then 
			entity.anim.current = entity.anim.falling
		end
	end

	if entity.anim.current == entity.anim.landing
	then
		entity.can_jump = true
		entity.can_walljump = true
		if entity.button.right
		then
			entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.button.left
		then
			entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end

	end

	if entity.anim.current == entity.anim.falling
	then
		if entity.button.right 
		then
			entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.button.left
		then
			entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
	end

	if entity.anim.current == entity.anim.hanging_right
	then
		if entity.button.jump and entity.can_walljump 
		then
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0


			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 35
			entity.impulse.x = -25
			entity.can_walljump = false
		end
		entity.forces.y = entity.forces.y + (delta_seconds * entity.maxSpeed * 40) 
	end

	if entity.anim.current == entity.anim.hanging_left
	then
		if entity.button.jump and entity.can_walljump
		then

			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0

			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 35
			entity.impulse.x = 25
			entity.can_walljump = false
		end
		entity.forces.y = entity.forces.y + (delta_seconds * entity.maxSpeed * 40) 
	end


	if entity.anim.current == entity.anim.connect_wall
		then
		if entity.button.jump and entity.button.left and entity.can_walljump
		then
			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 18
			entity.impulse.x = -8
			entity.can_walljump = false
		end
		if entity.button.jump and entity.button.right and entity.can_walljump
		then
			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 18
			entity.impulse.x = 8
			entity.can_walljump = false
		end
	end

	--Cap the velocity/entity.forces.x
	if entity.forces.x > entity.maxSpeed
	then
		entity.forces.x = entity.maxSpeed
	elseif entity.forces.x < -entity.maxSpeed
	then 
		entity.forces.x = -entity.maxSpeed
	end

	--if entity.anim.current == entity.anim.jump_from_wall
	--then
	--	if 
	--	then
	--
	--	end
	--end

	
	--entity.jump_timer = entity.jump_timer - delta_seconds
	--entity.forces.x = -1000
	--entity.size.x = 2045
	--entity.impulse.y =  50

	--entity.velocity.y = delta_seconds * 100 * entity.direction.z
end