function setup(entity)
	entity.can_jump = true
	entity.can_walljump = true
	entity.can_move = true -- instead of playable
	
	entity.jump_timer = 0
	entity.jump_impulse_x = 0

	entity.ungrounded_time = 0
	entity.jump_forgiveness_time = 0.5

	--trigger

	--oil platform
	entity.friction = 0
	entity.friction_slowrate = 0

	--sticky_platform
	entity.slow_speed = 16/ 2

	--shock_trap
	entity.shock_trap_timer = 0.0
	entity.shock_trap_immun_timer = 0.0
	entity.shock_trap_triggerd = false
	entity.shock_trap_immun = false
	entity.temp = 0

	--treadmill
	entity.treadmill_speed = 16/ 2

	--speed boost
	entity.speed_boost_timer = 0.0
	entity.speed_boost_triggerd = false
	entity.max_speed_boost = 16 * 1.5

	--steam boost
	entity.steam_boost_timer = 0.0
	entity.steam_boost_timer_max = 5
	entity.steam_boost_delay_timer = 0.0
	entity.steam_boost_triggerd = false
	entity.have_doubble_jumpt = false
	entity.can_dubbel_jump = false
	entity.steam_boost_jump_impulse = 50
	entity.jump_was_push = false
	entity.have_wall_jumpt = false

	--trampolin
	entity.trampolin_jump = false
	entity.trampolin_iterator = 0
	entity.trampolin_jump_impulse = 22.5

end

local jump_speed = 0
local gravity = 120
local max_gravity = 1800
local wall_jump_speed = 
{
	x = 150,
	y = 20
}

local max_speed = 16
local max_air_speed = 16
local ground_acceleration = 100
local deceleration = 60
local air_acceleration = 80


function update(delta_seconds, entity)
	
	have_wall_jumpt = true

	if entity.shock_trap_triggerd == false
	then
		update_controls(delta_seconds, entity)
	end

	update_triggers(delta_seconds, entity)


	--Gravity 
	if entity.anim.current == entity.anim.hanging_left and entity.velocity.y < 0 or entity.anim.current == entity.anim.hanging_right and entity.velocity.y < 0
	then
		entity.velocity.y = entity.velocity.y - gravity * 0.2 * delta_seconds
		
	else
		entity.velocity.y = entity.velocity.y - gravity * delta_seconds
		if entity.velocity.y < -max_gravity * delta_seconds
		then
			entity.velocity.y = -max_gravity * delta_seconds
		end
	end

	entity.jump_was_push = entity.button.jump

end


function update_controls(delta_seconds, entity)
	

	--Running
	if entity.anim.current == entity.anim.running
	then
		entity.anim.current = entity.anim.idle
		if entity.button.right 
		then
			accelerate(delta_seconds, entity, max_speed, ground_acceleration)

		end
		if entity.button.left
		then
			accelerate(delta_seconds, entity, -max_speed, ground_acceleration)

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
			accelerate(delta_seconds, entity, max_speed, ground_acceleration)
			entity.anim.current = entity.anim.running
		end
		if entity.button.left 
		then
			accelerate(delta_seconds, entity, -max_speed, ground_acceleration)
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

	--Start jump
	if entity.anim.current == entity.anim.start_jump
	then
		if entity.can_jump and entity.button.jump == true
		then
			jump_speed = jump_speed + 1.1
			entity.jump_timer = entity.jump_timer + delta_seconds
			
			if jump_speed < 2.2
			then
				entity.impulse.y = 29
			elseif jump_speed > 2.2 and jump_speed < 6.6
			then
				
				entity.impulse.y = jump_speed
			end

		elseif jump_speed > 0 and entity.button.jump == false or entity.jump_timer > 0.17 
		then
			entity.anim.current = entity.anim.in_jump
		end

		if entity.steam_boost_triggerd and entity.can_dubbel_jump == false and entity.have_doubble_jumpt == false --trigger
		then
			entity.can_dubbel_jump = true
			entity.steam_boost_delay_timer = 0.0
		end
	end

	--In Jump
	if entity.anim.current == entity.anim.in_jump
	then 
		if entity.can_jump
		then
			entity.impulse.y = jump_speed
			entity.can_jump = false
		end

		if entity.button.right 
		then
			
			accelerate(delta_seconds, entity,  max_air_speed, air_acceleration)
		elseif entity.button.left
		then
			accelerate(delta_seconds, entity, - max_air_speed, air_acceleration)
		else
			decelerate(delta_seconds, entity)
		end

		if entity.velocity.y < -0.0
		then 
			entity.anim.current = entity.anim.falling
		end

		if entity.velocity.y == 0
		then 
			entity.anim.current = entity.anim.idle
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


		if entity.steam_boost_triggerd --trigger
		then
			entity.have_doubble_jumpt = false
			entity.can_dubbel_jump = false
			entity.have_wall_jumpt = false
		end

		
		if entity.button.right
		then
			entity.velocity.x = 0
			entity.velocity.x =  max_air_speed
		end
		if entity.button.left
		then
			entity.velocity.x = 0
			entity.velocity.x = - max_air_speed
		end
	end

	--Falling

	if entity.anim.current == entity.anim.falling
	then
		if entity.velocity.y >= -1.0934910278593e-3 and entity.velocity.y < 0 and entity.button.left == true
		then
			entity.velocity.x = -5
		elseif entity.velocity.y >= -1.0934910278593e-3 and entity.velocity.y < 0 and entity.button.right == true
		then	
			entity.velocity.x = 5
		end


		
		if entity.button.left
		then
			if entity.velocity.x > 0.001 or entity.velocity.x < -0.001 and entity.velocity.y < -1 or  entity.velocity.x == 0
			then
				accelerate(delta_seconds, entity, - max_air_speed, air_acceleration)
			end
		elseif entity.button.right 
		then
			if entity.velocity.x > 0.001 or entity.velocity.x < -0.001 and entity.velocity.y < -1 or entity.velocity.x == 0
			then
				accelerate(delta_seconds, entity,  max_air_speed, air_acceleration)
			end
		else
			decelerate(delta_seconds, entity)
		end

		if entity.velocity.y > -0.01 and entity.velocity.y < 0 
		then 
			entity.velocity.y = 0
			entity.anim.current = entity.anim.idle
		end


		if entity.have_doubble_jumpt == false --trigger
		then
			entity.can_dubbel_jump = true
		end

	end

	--Hanging_Right
	if entity.anim.current == entity.anim.hanging_right
	then
		
		if entity.steam_boost_triggerd--trigger
		then
			entity.have_wall_jumpt = true
		end

		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds
		

		if entity.button.jump and entity.button.left and entity.can_walljump and entity.jump_timer > 0.2
		then
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0


			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 45
			entity.impulse.x = -26
			entity.can_walljump = true
			entity.jump_timer = 0
		end
	
	end

	--Hanging_Left
	if entity.anim.current == entity.anim.hanging_left
	then

		if entity.steam_boost_triggerd --trigger
		then
			entity.have_wall_jumpt = true
		end

		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds
		
		if entity.button.jump and  entity.button.right and entity.can_walljump and entity.jump_timer > 0.2
		then
			entity.forces.x = 0
			entity.forces.y = 0
			entity.velocity.x = 0
			entity.velocity.y = 0
			entity.impulse.y = 0
			entity.impulse.x = 0
				

			entity.anim.current = entity.anim.jump_from_wall
			entity.impulse.y = 45
			entity.impulse.x = 26
			entity.can_walljump = true
			entity.jump_timer = 0
		end

	end

end

function accelerate(delta_seconds, entity, top_speed, acceleration)

	if top_speed > 0
	then
		if entity.velocity.x > 0 and entity.velocity.x < 1
		then 
			entity.velocity.x = 2
		end
		if entity.velocity.x < top_speed	
		then
			entity.velocity.x = entity.velocity.x +(acceleration*delta_seconds )
		else
			entity.velocity.x = top_speed
		end
	elseif top_speed < 0
	then
		if entity.velocity.x > -1 and entity.velocity.x < 0
		then 
			entity.velocity.x = -2
		end
		if entity.velocity.x > top_speed
		then
			entity.velocity.x = entity.velocity.x -(acceleration*delta_seconds )
		else	
			entity.velocity.x = top_speed
		end
	end
end

function decelerate(delta_seconds, entity)
	
		if entity.velocity.x > 1 
		then
			entity.velocity.x = entity.velocity.x -(deceleration*delta_seconds )
		elseif entity.velocity.x < -1 
		then
			entity.velocity.x = entity.velocity.x +(deceleration*delta_seconds )
		else	
			entity.velocity.x = 0
		end
end

function update_triggers(delta_seconds, entity)
	--trigger
	if entity.triggered >= 4
	then
		--platform_oil
		if entity.triggered_type == 2
		then

			if entity.button.right or entity.button.left or entity.anim.current == entity.anim.falling
			then
				entity.friction = entity.velocity.x
				entity.friction_slowrate = entity.velocity.x * 0.0125
			end

			if entity.button.right == false and entity.button.left == false
			then
				if entity.velocity.x > 0.1
				then
					entity.friction = entity.friction - entity.friction_slowrate
					entity.velocity.x = entity.friction

				elseif entity.velocity.x < -0.1
				then
					entity.friction = entity.friction - entity.friction_slowrate
					entity.velocity.x = entity.friction
				
				elseif entity.velocity.x > -0.1 and entity.velocity.x < 0.1
				then
					entity.velocity.x = 0
					entity.friction = 0
				end
			end
			--print("platform_oil")
		else
			entity.friction = 0
			entity.friction_slowrate = 0
		end



		--sticky_platform
		if entity.triggered_type == 3
		then
			
			if entity.button.right --right
			then
				--entity.velocity.x = 1
				entity.velocity.x = entity.slow_speed
			
			elseif entity.button.left --left
			then
				--entity.velocity.x = -1
				entity.velocity.x = -entity.slow_speed
			end

			if entity.button.jump
			then
				entity.impulse.y = entity.impulse.y * 0.675
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
			if entity.button.right or entity.button.left
			then
				entity.velocity.x = entity.velocity.x + entity.treadmill_speed
			
			else
				entity.velocity.x = entity.treadmill_speed
			end

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

			--print("steam_boost")
		end

		--trampolin
		if entity.triggered_type == 9 --and entity.velocity.y <= -10
		then
			entity.trampolin_jump = true
			entity.anim.current = entity.anim.falling
			entity.can_jump = false
			--print("trampolin")
		end
	end

	--shock_trap

	if	entity.shock_trap_timer <= 3.0 and entity.shock_trap_triggerd == true
	then
		entity.shock_trap_timer = entity.shock_trap_timer + delta_seconds
		if entity.anim.current ~= entity.anim.Falling and entity.anim.current ~= entity.anim.in_jump
		then
			--entity.velocity.x = 0
			--entity.velocity.y = 0
		end

	elseif	entity.shock_trap_timer >= 3.0 and entity.shock_trap_triggerd == true
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
		if entity.button.right
		then
			entity.velocity.x = entity.max_speed_boost--right

		elseif entity.button.left
		then 
			entity.velocity.x = -entity.max_speed_boost --left
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
		if entity.button.jump and not entity.jump_was_push and entity.have_wall_jumpt == false and entity.steam_boost_delay_timer > 0.2
		then
			
			entity.velocity.y = entity.steam_boost_jump_impulse

			entity.can_dubbel_jump = false
			entity.have_doubble_jumpt = true

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

	--trampolin
	if entity.trampolin_jump
	then
		entity.can_jump = false
		if entity.trampolin_iterator < 3
		then
			if entity.velocity.y <= -4
			then
				entity.impulse.y = (entity.velocity.y * -1)
			end
			
			entity.impulse.y = entity.impulse.y + entity.trampolin_jump_impulse;
			entity.trampolin_iterator = entity.trampolin_iterator + 1
		else
			entity.trampolin_jump = false
			entity.trampolin_iterator = 0
		end
	end
end



