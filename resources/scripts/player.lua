function setup(entity)
	--player_playable = true -- 1 should be thought as a playable player 
	entity.can_jump = true
	entity.can_walljump = true
	entity.can_move = true -- instead of playable
	entity.max_speed = 16
	entity.max_air_speed = 14
 	entity.acceleration = 580000
	entity.jump_timer = 0
	entity.jump_impulse_x = 0

	--trigger
	entity.speed_boost_timer = 0.0
	entity.speed_boost_triggerd = false
	entity.max_speed_boost = entity.max_speed * 2
	entity.max_velocity_boost = entity.max_speed * 1.3


	entity.ungrounded_time = 0
	entity.jump_forgiveness_time = 0.5
end

local jump_speed = 0
local gravity = 120
local max_gravity = 800
local wall_jump_speed = 
{
	x = 150,
	y = 20
}

function update(delta_seconds, entity)
	
	--update_anim_state(delta_seconds, entity)

	update_controls(delta_seconds, entity)


	--trigger
	if entity.triggered >= 4
	then
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

		----standard_platform
		--if entity.triggered_type == 6
		--then
		--
		--end

		--speed_boost
		if entity.triggered_type == 7 and entity.speed_boost_triggerd == false
		then
			entity.speed_boost_triggerd = true
			entity.speed_boost_timer = 0.0

			print("Sprint_boost")
		end
	end

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
	

	--Gravity 
	entity.velocity.y = entity.velocity.y - gravity * delta_seconds
	if entity.velocity.y < -max_gravity * delta_seconds
	then
		entity.velocity.y = -max_gravity * delta_seconds
	end


	--[[if entity.anim.current == entity.anim.falling 
		or entity.anim.current == entity.anim.in_jump 
		or entity.anim.current == entity.anim.hanging_left 
		or entity.anim.current == entity.anim.hanging_right
		or entity.anim.current == entity.anim.jump_from_wall
		or entity.anim.current == entity.anim.idle
		or entity.anim.current == entity.anim.running
		then
		entity.velocity.y = entity.velocity.y - gravity * delta_seconds
		if entity.velocity.y < -max_gravity * delta_seconds
		then
		entity.velocity.y = -max_gravity * delta_seconds
		end
	end

	if entity.anim.current ~= entity.anim.in_jump 
	then
		entity.velocity.y = entity.velocity.y - (entity.velocity.y * delta_seconds)
	end]]--

end


function update_controls(delta_seconds, entity)
	

	--Running
	if entity.anim.current == entity.anim.running
	then
		entity.anim.current = entity.anim.idle
		if entity.button.right 
		then
			entity.velocity.x = entity.max_speed
		end
		if entity.button.left
		then
			entity.velocity.x = -entity.max_speed
		end
		if entity.button.jump
		then
			entity.anim.current = entity.anim.start_jump
		end
		if entity.velocity.y < -0.3
		then 
			entity.anim.current = entity.anim.falling
		end
	end

	--Idle
	if entity.anim.current == entity.anim.idle
	then
		if entity.button.right 
		then
			entity.velocity.x = entity.max_speed
			entity.anim.current = entity.anim.running
		end
		if entity.button.left 
		then
			entity.velocity.x = -entity.max_speed
			entity.anim.current = entity.anim.running
		end
		if entity.button.jump
		then
			entity.anim.current = entity.anim.start_jump
		end
		if entity.velocity.y < -0.3
		then 
			entity.anim.current = entity.anim.falling
		end
	end

	--[[Start jump
	if entity.anim.current == entity.anim.start_jump
	then
		entity.jump_timer = 0

		--[[if entity.can_jump and entity.button.jump == true and entity.jump_timer < 0.17
			then
			jump_speed = jump_speed + 5
			entity.jump_timer = entity.jump_timer + delta_seconds
			elseif (jump_speed > 5 and entity.button.jump == false) or (entity.jump_timer > 0.08 and jump_speed > 5) 
			then
			entity.anim.current = entity.anim.in_jump
		end
	]]--

	--In Jump
	if entity.anim.current == entity.anim.in_jump
	then 

	--Start here after lunch
		if entity.can_jump == true and entity.button.jump == true and entity.jump_timer < 0.17
		then
			jump_speed = jump_speed + 5
			entity.jump_timer = entity.jump_timer + delta_seconds

		elseif (entity.can_jump == true and jump_speed > 5 and entity.button.jump == false) or (entity.can_jump == true and entity.jump_timer > 0.08 and jump_speed > 5) 
		then
			entity.impulse.y = jump_speed
			entity.can_jump = false
			jump_speed = 0
		end	


		--[[if entity.can_jump 
		then
			entity.impulse.y = jump_speed
			entity.can_jump = false
		end]]--

		if entity.button.right 
		then
			entity.velocity.x = 0
			entity.velocity.x = entity.max_air_speed
			--entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.button.left
		then
			entity.velocity.x = 0
			entity.velocity.x = -entity.max_air_speed
			--entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end

		if entity.velocity.y < -0.3
		then 
			entity.anim.current = entity.anim.falling
		end
	end

	--Landing
	if entity.anim.current == entity.anim.landing
	then
		jump_speed = 0
		entity.ungrounded_time = 0	
		entity.jump_timer = 0
		entity.can_jump = true
		entity.can_walljump = true

		
		if entity.button.right
		then
			entity.velocity.x = 0
			entity.velocity.x = entity.max_air_speed
			--entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.button.left
		then
			entity.velocity.x = 0
			entity.velocity.x = -entity.max_air_speed
			--entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
	end

	--Falling
	if entity.anim.current == entity.anim.falling
	then
	

		if entity.button.right 
		then
			entity.velocity.x = 0
			entity.velocity.x =  entity.max_air_speed
			--entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.button.left
		then
			entity.velocity.x = 0
			entity.velocity.x = -entity.max_air_speed
			--entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end

		
		--[[if entity.velocity.y <= -0.1 and entity.velocity.y > -1.0
		then 
			print("asdau")
			entity.velocity.y = 0
			entity.anim.current = entity.anim.idle
		end]]--


		--print(entity.velocity.x)
		--[[Test of jumpforgiveness
		if entity.can_jump == false
		then
		entity.ungrounded_time = entity.ungrounded_time + delta_seconds
			if entity.button.jump and entity.ungrounded_time <= 0.5
			then
			entity.can_jump = true
			entity.anim.current = entity.anim.start_jump
			--entity.impulse.y = 35
			end
		end]]--
	end

	--Hanging_Right
	if entity.anim.current == entity.anim.hanging_right
	then
		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds
		

		if entity.button.jump and entity.can_walljump and entity.jump_timer > 0.2
		then
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0


			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 40
			entity.impulse.x = -26
			entity.can_walljump = true
			entity.jump_timer = 0
		end
	
		--entity.forces.y = entity.forces.y + (delta_seconds * entity.maxSpeed * 40) 
	end

	--Hanging_Left
	if entity.anim.current == entity.anim.hanging_left
	then

		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds
		
		if entity.button.jump and entity.can_walljump and entity.jump_timer > 0.2
		then
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0
				

			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 40
			entity.impulse.x = 26
			entity.can_walljump = true
			entity.jump_timer = 0
		end
		
		--entity.forces.y = entity.forces.y + (delta_seconds * entity.maxSpeed * 40) 
	end


	--[[if entity.velocity.y <= -0.0001
		then 
			--print("asdau")
			entity.anim.current = entity.anim.landing
	end]]--

	if entity.anim.current == entity.anim.falling
	then
		print("falling")
	end
	 
	--Cap the velocity/entity.forces.x
	--[[if entity.forces.x > entity.maxSpeed
		then
		entity.forces.x = entity.maxSpeed
	elseif entity.forces.x < -entity.maxSpeed
	then 
		entity.forces.x = -entity.maxSpeed
	end]]--
end