function setup(entity)
	--player_playable = true -- 1 should be thought as a playable player 
	entity.jump_timer = 1.0
	entity.can_jump = true
	entity.can_walljump = true
	entity.can_move = true -- instead of playable
	entity.max_speed = 1100
	entity.max_air_speed = 600
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

local jump_speed = 2200
local gravity = 120
local max_gravity = 500
local wall_jump_speed = 
{
	x = 600,
	y = 15
}

function update(delta_seconds, entity)
	
	update_anim_state(delta_seconds, entity)

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
	

	if entity.anim.current == entity.anim.falling 
	or entity.anim.current == entity.anim.in_jump 
	or entity.anim.current == entity.anim.hanging_left 
	or entity.anim.current == entity.anim.hanging_right
	or entity.anim.current == entity.anim.idle
	or entity.anim.current == entity.anim.running
	then
		entity.velocity.y = entity.velocity.y - gravity * delta_seconds
		print("Adam")
		if entity.velocity.y < -max_gravity * delta_seconds
		then
			entity.velocity.y = -max_gravity * delta_seconds
		end
	end
	--[[elseif entity.anim.current ~= entity.anim.in_jump 	
	then
		entity.velocity.y = entity.velocity.y - (entity.velocity.y * delta_seconds)
		print("Lisa")
	end]]--
end

function update_anim_state(delta_seconds, entity)

	entity.velocity.x = 0
	
	--[[entity.jump_impulse_x = entity.jump_impulse_x * 0.5
	if entity.jump_impulse_x < 0.1 and entity.jump_impulse_x < -0.1
	then
		entity.velocity.x = entity.jump_impulse_x * delta_seconds
	end]]--

	local speed = entity.max_speed * delta_seconds
	
	if entity.anim.current == entity.anim.in_jump
	or entity.anim.current == entity.anim.falling 
	then
		speed = entity.max_air_speed * delta_seconds
	end

	if entity.button.left
	then
		entity.velocity.x = entity.velocity.x - speed
	end

	if entity.button.right
	then
		entity.velocity.x = entity.velocity.x + speed
	end

	
	if entity.anim.current == entity.anim.running
	then	
		entity.anim.current = entity.anim.idle

		if entity.button.jump
		then
			entity.anim.current = entity.anim.start_jump
		end
		if entity.velocity.y < -0.4
		then 
			entity.anim.current = entity.anim.falling
		end
	end

	if entity.anim.current == entity.anim.idle
	then
		if entity.button.right 
		then
			entity.anim.current = entity.anim.running
		end
		
		if entity.button.left 
		then
			entity.anim.current = entity.anim.running
		end
		
		if entity.button.jump
		then
			entity.anim.current = entity.anim.start_jump
		end
	end

	if entity.anim.current == entity.anim.start_jump
	then
		if entity.can_jump
		then		
			entity.velocity.y = jump_speed * delta_seconds
			entity.can_jump = false
		end	

	end
	
	if entity.anim.current == entity.anim.in_jump
	then
		if entity.velocity.y < -0.4
		then 
			entity.anim.current = entity.anim.falling
		end
	end

	if entity.anim.current == entity.anim.landing
	then
		entity.ungrounded_time = 0
		entity.can_jump = true
		entity.can_walljump = true
	end	
	
	if entity.anim.current == entity.anim.falling
	then

	end


	if entity.anim.current == entity.anim.hanging_right
	then
		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds
		
		if entity.button.jump and not entity.button.right and entity.can_walljump and entity.jump_timer > 0.2
		then			
			entity.anim.current = entity.anim.jump_from_wall
			entity.velocity.y = wall_jump_speed.y
			entity.jump_impulse_x = -wall_jump_speed.x
			entity.can_walljump = true  --false
		end
	end

	if entity.anim.current == entity.anim.hanging_left
	then
		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds
		
		if entity.button.jump and not entity.button.left and entity.can_walljump and entity.jump_timer > 0.2
		then
			entity.anim.current = entity.anim.jump_from_wall
			entity.velocity.y = wall_jump_speed.y
			entity.jump_impulse_x = wall_jump_speed.x
			entity.can_walljump = true --false
		end
	end

	--if entity.anim.current == entity.anim.jump_from_wall
	--then	
		--entity.jump_impulse_x = wall_jump_speed.x * delta_seconds
	--end
end
