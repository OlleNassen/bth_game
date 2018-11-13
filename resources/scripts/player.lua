function setup(entity)
	--player_playable = true -- 1 should be thought as a playable player 
	entity.jump_timer = 1.0
	entity.can_jump = true
	entity.can_walljump = true
	entity.can_move = true -- instead of playable
	entity.maxSpeed = 8700
	entity.max_velocity = 16
	entity.max_air_speed = 14
 	entity.acceleration = 580000
	entity.jump_timer = 0

	--trigger

	--oil platform
	entity.normal_friction = 0.98

	--shock_trap
	entity.shock_trap_timer = 0.0
	entity.shock_trap_immun_timer = 0.0
	entity.shock_trap_triggerd = false
	entity.shock_trap_immun = false
	entity.temp = 0

	--speed boost
	entity.speed_boost_timer = 0.0
	entity.speed_boost_triggerd = false
	entity.max_speed_boost = entity.maxSpeed * 2
	entity.max_velocity_boost = entity.max_velocity * 1.3

	--steam boost
	entity.steam_boost_timer = 0.0
	entity.steam_boost_timer_max = 10
	entity.steam_boost_delay_timer = 0.0
	entity.steam_boost_triggerd = false
	entity.have_doubble_jumpt = false

	entity.can_dubbel_jump = false


	entity.ungrounded_time = 0
	entity.jump_forgiveness_time = 0.5
end

function update(delta_seconds, entity)

	--if entity.anim.current == entity.anim.idle and entity.first_jump_done == true  --trigger
	--then
	--	entity.first_jump_done = false
	--	print("set")
	--end

	if entity.anim.current == entity.anim.running
	then
	
		entity.anim.current = entity.anim.idle
		if entity.button.right 
		then
			if entity.velocity.x < entity.max_velocity
			then
				--entity.forces.x = 8900
				entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds)
			elseif  entity.velocity.x > entity.max_velocity
			then
				entity.forces.x = entity.forces.x
			end
		end
		
		if entity.button.left
		then
			if entity.velocity.x > -entity.max_velocity
			then
				--entity.forces.x = -8900
				entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds)
			elseif  entity.velocity.x < -entity.max_velocity
			then
				entity.forces.x = entity.forces.x
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
			if entity.velocity.x < entity.max_velocity
			then
				--entity.forces.x = 8900
				entity.forces.x = entity.forces.x + 6500 + (entity.maxSpeed*entity.acceleration*delta_seconds)
			elseif  entity.velocity.x > entity.max_velocity
			then
				entity.forces.x = entity.forces.x 
			end	
			entity.anim.current = entity.anim.running
		end
		if entity.button.left 
		then
			if entity.velocity.x > -entity.max_velocity
			then
				--entity.forces.x = -8900
				entity.forces.x = entity.forces.x + -6500 + (-entity.maxSpeed*entity.acceleration*delta_seconds)
			elseif  entity.velocity.x < -entity.max_velocity
			then
				entity.forces.x = entity.forces.x
			end
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
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0
			

			entity.impulse.y = 40
			entity.can_jump = false


			if entity.steam_boost_triggerd and entity.can_dubbel_jump == false and entity.have_doubble_jumpt == false --trigger
			then
				entity.can_dubbel_jump = true
				entity.steam_boost_delay_timer = 0.0
			end


		end	
	end

	if entity.anim.current == entity.anim.in_jump
	then
		if entity.can_jump
		then
			entity.impulse.y = 40
			entity.can_jump = false
		end
		if entity.button.right 
		then
			if entity.velocity.x < entity.max_air_speed
			then
				entity.forces.x = (entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) )/ 3
			elseif entity.velocity.x > entity.max_air_speed
			then
				entity.forces.x = entity.forces.x /3
			end
		end

		if entity.button.left
		then
			if entity.velocity.x > -entity.max_air_speed
			then
				entity.forces.x = (entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) )/3
			elseif  entity.velocity.x < -entity.max_air_speed
			then
				entity.forces.x = entity.forces.x/3
			end
			
			--entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.velocity.y < -0.8
		then 
			entity.anim.current = entity.anim.falling
		end
	end

	if entity.anim.current == entity.anim.landing
	then
		entity.ungrounded_time = 0
		entity.can_jump = true
		entity.can_walljump = true


		if entity.have_doubble_jumpt --trigger
		then
			entity.have_doubble_jumpt = false
		end


		if entity.button.right
		then
			if entity.velocity.x < entity.max_air_speed
			then
				entity.forces.x = (entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds)) / 3
			elseif entity.velocity.x > entity.max_air_speed
			then
				entity.forces.x = entity.forces.x /3
			end
			
			--entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.button.left
		then
			if entity.velocity.x > -entity.max_air_speed
			then
				entity.forces.x = (entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) )/3
			elseif  entity.velocity.x < -entity.max_air_speed
			then
				entity.forces.x = entity.forces.x /3
			end

			--entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
	end


	--Test for jumpforgiveness Doesn't work at the moment
	--[[if entity.ungrounded_time > entity.jump_forgiveness_time and entity.anim.current == entity.anim.falling and  entity.can_jump == false
	then
			entity.can_jump = false
	else
			entity.ungrounded_time = entity.ungrounded_time + delta_seconds
			entity.can_jump = true
	end]]--
	





	if entity.anim.current == entity.anim.falling 
	then

		if entity.have_doubble_jumpt == false --trigger
		then
			entity.can_dubbel_jump = true
		end

		if entity.button.right 
		then
			if entity.velocity.x < entity.max_air_speed
			then
				entity.forces.x = (entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds)) / 3
			elseif entity.velocity.x > entity.max_air_speed
			then
				entity.forces.x = entity.forces.x 
			end

			--entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.button.left
		then
			if entity.velocity.x > -entity.max_air_speed
			then
				entity.forces.x = (entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) )/ 3
			elseif entity.velocity.x < -entity.max_air_speed
			then
				entity.forces.x = entity.forces.x /3
			end
			--entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
	end

	if entity.anim.current == entity.anim.hanging_right
	then
		--Test for jumpforgiveness
		entity.ungrounded_time = 0

		entity.jump_timer = entity.jump_timer + delta_seconds
		if entity.button.jump and entity.can_walljump and entity.jump_timer > 0.2
		then
			entity.jump_timer = 0
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0
			
			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 37
			entity.impulse.x = -25
			entity.can_walljump = true --false
		end
		entity.forces.y = entity.forces.y + (delta_seconds * entity.maxSpeed * 18) 
	end

	if entity.anim.current == entity.anim.hanging_left
	then
		--Test for jumpforgiveness
		entity.ungrounded_time = 0

		entity.jump_timer = entity.jump_timer + delta_seconds
		if entity.button.jump and entity.can_walljump and entity.jump_timer > 0.2
		then
			entity.jump_timer = 0
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0
			
			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 37
			entity.impulse.x = 25
			entity.can_walljump = true --false
		end
		entity.forces.y = entity.forces.y + (delta_seconds * entity.maxSpeed * 18) 
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
	--entity.anim.current = entity.anim.running
	--entity.jump_timer = entity.jump_timer - delta_seconds
	--entity.forces.x = -1000
	--entity.size.x = 2045
	--entity.impulse.y =  50

	--entity.velocity.y = delta_seconds * 100 * entity.direction.z




	entity.friction = entity.normal_friction
	--trigger
	if entity.triggered >= 4
	then
		--platform_oil
		if entity.triggered_type == 2
		then
			entity.friction = 0.05
			--print("platform_oil")
		end

		--sticky_platform
		if entity.triggered_type == 3
		then
			
			if entity.velocity.x >= entity.max_velocity and entity.velocity.x > 0 --right
			then
				--entity.velocity.x = 1
				entity.forces.x = entity.forces.x - (entity.forces.x / 1.5) 
			
			elseif entity.velocity.x <= -entity.max_velocity and entity.velocity.x < 0 --left
			then
				--entity.velocity.x = -1
				entity.forces.x = entity.forces.x - (entity.forces.x / 1.5)
			end
		end

		--shock_trap
		if entity.triggered_type == 4 and entity.shock_trap_triggerd == false and entity.shock_trap_immun == false
		then
			entity.shock_trap_triggerd = true
			entity.is_stund = true
			entity.shock_trap_timer = 0.0

			--print("shock_trap")
		end

		--treadmill
		if entity.triggered_type == 5
		then
			entity.forces.x = entity.forces.x + -entity.maxSpeed / 2 --Need fixing
			--print("treadmill")
		end

		--speed_boost
		if entity.triggered_type == 7 and entity.speed_boost_triggerd == false
		then
			entity.speed_boost_triggerd = true
			entity.speed_boost_timer = 0.0

			--print("Sprint_boost")
		end

		--steam_boost
		if entity.triggered_type == 8 and entity.steam_boost_triggerd == false
		then
			entity.steam_boost_triggerd = true
			entity.steam_boost_timer = 0.0
			entity.steam_boost_delay_timer = 0.0

			print("steam_boost")
		end

		--trampolin
		if entity.triggered_type == 9
		then

			--print("trampolin")
		end
	end

	--shock_trap

	if	entity.shock_trap_timer <= 5.0 and entity.shock_trap_triggerd == true
	then
		entity.shock_trap_timer = entity.shock_trap_timer + delta_seconds
		entity.forces.x = 0
		entity.forces.y = 0

	elseif	entity.shock_trap_timer >= 5.0 and entity.shock_trap_triggerd == true
	then
		entity.shock_trap_triggerd = false
		entity.is_stund = false

		entity.shock_trap_immun_timer = 0
		entity.shock_trap_immun = true
	end

	if	entity.shock_trap_immun_timer <= 5.0 and entity.shock_trap_immun == true
	then
		entity.shock_trap_immun_timer = entity.shock_trap_immun_timer + delta_seconds

	elseif	entity.shock_trap_immun_timer >= 5.0 and entity.shock_trap_immun == true
	then
		entity.shock_trap_immun = false
	end


	--speed_boost
	if	entity.speed_boost_triggerd == true and entity.speed_boost_timer <= 5.0
	then
		if entity.velocity.x < entity.max_velocity_boost and entity.velocity.x > -entity.max_velocity_boost and entity.button.right
		then
			if entity.velocity.x > 0
			then
				entity.forces.x = entity.max_speed_boost * 1.2 --right
			else
				entity.forces.x = entity.max_speed_boost * 0.6
			end

		elseif entity.velocity.x > -entity.max_velocity_boost and entity.velocity.x < entity.max_velocity_boost and entity.button.left
		then 
			if entity.velocity.x < 0
			then
				entity.forces.x = -entity.max_speed_boost * 1.2 --left
			else
				entity.forces.x = -entity.max_speed_boost * 0.6
			end

		end
	end

	if	entity.speed_boost_timer <= 5.0
	then
		entity.speed_boost_timer = entity.speed_boost_timer + delta_seconds

	elseif	entity.speed_boost_timer >= 5.0
	then
		entity.speed_boost_triggerd = false
	end


	--steam_boost
	if	entity.steam_boost_triggerd and entity.can_dubbel_jump and entity.steam_boost_timer <= entity.steam_boost_timer_max
	then
		--print("steam_boost2")
		if entity.button.jump and entity.steam_boost_delay_timer > 0.2
		then
			entity.impulse.y = 40
			entity.can_dubbel_jump = false
			entity.have_doubble_jumpt = true
			print("steam_boost3")

		end
	end

	if	entity.steam_boost_timer <= entity.steam_boost_timer_max
	then
		entity.steam_boost_timer = entity.steam_boost_timer + delta_seconds
		entity.steam_boost_delay_timer = entity.steam_boost_delay_timer + delta_seconds

	elseif	entity.steam_boost_timer >= entity.steam_boost_timer_max
	then
		entity.steam_boost_triggerd = false
	end





	entity.forces.y = entity.forces.y - 6400 --gravity
end
