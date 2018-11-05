function setup(entity)
	--player_playable = true -- 1 should be thought as a playable player 
	entity.jump_timer = 1.0
	entity.can_jump = true
	entity.can_walljump = true
	entity.can_move = true -- instead of playable
	entity.maxSpeed = 5000
	entity.acceleration = 580000
	entity.jump_timer = 0
end

function update(delta_seconds, entity)

	
	if entity.anim.current == entity.anim.running
	then
	
		entity.anim.current = entity.anim.idle
		if entity.button.right 
		then
			if entity.velocity.x < 6.0
			then
				entity.forces.x = (entity.forces.x + 5000 + (entity.maxSpeed*entity.acceleration*delta_seconds))
				print(entity.velocity.x)
			elseif  entity.velocity.x > 6.0
			then
				print(entity.velocity.x)
				entity.forces.x = (entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds))
			--entity.forces.x = 4000
			end
		end
		if entity.button.left
		then
			if entity.velocity.x > -6.0
			then
				entity.forces.x = (entity.forces.x + -5000 + (-entity.maxSpeed*entity.acceleration*delta_seconds))
				print(entity.velocity.x)
			elseif  entity.velocity.x < -6.0
			then
				print(entity.velocity.x)
				entity.forces.x = (entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds))
			--entity.forces.x = 4000
			end
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

	if entity.anim.current == entity.anim.start_jump
	then
		if entity.can_jump
		then
			print("jump")
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0
			

			entity.impulse.y = 41
			entity.can_jump = false
		end	

	end

	if entity.anim.current == entity.anim.in_jump
	then
		
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
		entity.jump_timer = entity.jump_timer + delta_seconds
		if entity.button.jump and entity.can_walljump and entity.jump_timer > 0.2
		then
			print("Walljump")
			entity.jump_timer = 0
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0
			
			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 25
			entity.impulse.x = -25
			entity.can_walljump = false
		end
		entity.forces.y = entity.forces.y + (delta_seconds * entity.maxSpeed * 40) 
	end

	if entity.anim.current == entity.anim.hanging_left
	then
		entity.jump_timer = entity.jump_timer + delta_seconds
		if entity.button.jump and entity.can_walljump and entity.jump_timer > 0.2
		then
			print("Walljump")
			entity.jump_timer = 0
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0
			
			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 25
			entity.impulse.x = 25
			entity.can_walljump = false
		end
		entity.forces.y = entity.forces.y + (delta_seconds * entity.maxSpeed * 40) 
	end

	--Cap the velocity/entity.forces.x
	if entity.anim.current ~= entity.anim.hanging_left or entity.anim.current ~= entity.anim.hanging_right or entity.anim.current ~= entity.anim.jump_from_wall 
	then
		if entity.forces.x > entity.maxSpeed
		then
			entity.forces.x = entity.maxSpeed
		elseif entity.forces.x < -entity.maxSpeed
		then 
			entity.forces.x = -entity.maxSpeed
		end
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

	entity.forces.y = entity.forces.y - 6400
end