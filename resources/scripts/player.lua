function setup(entity)
	entity.can_jump = true
	entity.can_walljump = true
	entity.can_move = true -- instead of playable
	
	entity.jump_timer = 0
	entity.jump_impulse_x = 0
	entity.max_speed = 16
	entity.max_air_speed = 16
	entity.ground_acceleration = 100
	entity.deceleration = 80
	entity.air_acceleration = 100
	entity.ungrounded_time = 0
	entity.jump_forgiveness_time = 0.5

	--trigger

	--shock_trap
	entity.stun_trap_timer = 0.0
	entity.stun_trap_immune_timer = 0.0
	entity.stun_trap_triggered = false
	entity.stun_trap_immune = false
	entity.temp = 0

	--speed boost
	entity.speed_boost_timer = 0.0
	entity.speed_boost_triggered = false
	

	--double jump
	entity.double_jump_timer = 0.0
	entity.double_jump_triggered = false --triggered
	entity.have_double_jumped = false
	entity.can_double_jump = false --if have_double_jumped = false, can_double_jump = true
	entity.jump_pushed_last_frame = false --if jump button pressed last frame = pressed this frame, if released last frame, released now
	entity.now_you_can_jump = false --if was in jump last frame
	entity.in_wall_jump_state = false --in air after hanging r/l anim
	entity.set_once = false


	--glide_trap
	entity.glide_trap_triggered = false
	entity.glide_trap_timer = 0.0
	entity.friction = 0
	entity.friction_slowrate = 0

	--shield
	entity.is_stun_trap = false

	--random_buff
	entity.random_buff_triggered = false
	entity.random_assigned = false
	entity.random_last = 0
	entity.random_buff_timer = 0.0

	--triggers


	entity.shock_trap_max_timer = 3
	entity.shock_trap_immune_max_timer = 5
	
	entity.max_speed_boost = 16 * 1.5
	entity.max_speed_boost_air = 16 * 1.5
	entity.speed_boost_max_timer = 10
	
	entity.double_jump_impulse = 40
	entity.double_jump_timer_max = 10
	
	entity.glide_trap_max_timer = 10
	entity.glide_decrease = 0.0055
	
	entity.random_buff_max_timer = 10
	entity.buffs_id = { 3, 4, 5, 6 }

	entity.jump_speed = 0
	entity.gravity = 120
	entity.max_gravity = 1600 --1800

end



--[[local max_speed = 16
local max_air_speed = 16
local ground_acceleration = 100
local deceleration = 60
local   = 100
]]--


function update(delta_seconds, entity)
	
	if entity.stun_trap_triggered == false
	then
		update_controls(delta_seconds, entity)
	end

	update_triggers(delta_seconds, entity)


	--Gravity 
	if entity.anim.current == entity.anim.hanging_left and entity.velocity.y < 0 or entity.anim.current == entity.anim.hanging_right and entity.velocity.y < 0
	then
		entity.velocity.y = entity.velocity.y - entity.gravity * 0.2 * delta_seconds
		
	else
		entity.velocity.y = entity.velocity.y - entity.gravity * delta_seconds
		if entity.velocity.y < -entity.max_gravity * delta_seconds
		then
			entity.velocity.y = -entity.max_gravity * delta_seconds
		end
	end

	--triggers

	entity.jump_pushed_last_frame = entity.button.jump

	if entity.can_double_jump and entity.jump_pushed_last_frame == false and entity.now_you_can_jump == false
	then
		entity.now_you_can_jump = true

	elseif entity.can_double_jump == false
	then
		entity.now_you_can_jump = false
	end
end


function update_controls(delta_seconds, entity)
	

	--Running
	if entity.anim.current == entity.anim.running
	then
		entity.anim.current = entity.anim.idle
		if entity.button.right 
		then
			accelerate(delta_seconds, entity, entity.max_speed, entity.ground_acceleration)

		end
		if entity.button.left
		then
			accelerate(delta_seconds, entity, -entity.max_speed, entity.ground_acceleration)

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
			accelerate(delta_seconds, entity, entity.max_speed, entity.ground_acceleration)
			entity.anim.current = entity.anim.running
		end
		if entity.button.left 
		then
			accelerate(delta_seconds, entity, -entity.max_speed, entity.ground_acceleration)
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
			entity.jump_speed = entity.jump_speed + 1.1
			entity.jump_timer = entity.jump_timer + delta_seconds
			
			if entity.jump_speed < 2.2
			then
				entity.impulse.y = 29
			elseif entity.jump_speed > 2.2 and entity.jump_speed < 6.6
			then
				
				entity.impulse.y = entity.jump_speed
			end

		elseif entity.jump_speed > 0 and entity.button.jump == false or entity.jump_timer > 0.17 
		then
			entity.anim.current = entity.anim.in_jump
		end
	end

	--In Jump
	if entity.anim.current == entity.anim.in_jump
	then 
		if entity.can_jump
		then
			entity.impulse.y = entity.jump_speed
			entity.can_jump = false
		end

		if entity.double_jump_triggered and entity.can_double_jump == false --trigger
		then
			entity.can_double_jump = true
		end

		if entity.button.right 
		then
			
			accelerate(delta_seconds, entity,  entity.max_air_speed, entity.air_acceleration)
		elseif entity.button.left
		then
			accelerate(delta_seconds, entity, - entity.max_air_speed, entity.air_acceleration)
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
		entity.jump_speed = 0
		entity.ungrounded_time = 0	
		entity.jump_timer = 0
		entity.can_jump = true
		entity.can_walljump = true


		if entity.double_jump_triggered --trigger
		then
			entity.have_double_jumped = false
			entity.can_double_jump = false
			entity.in_wall_jump_state = false
			entity.set_once = false
		end

		
		if entity.button.right
		then
			entity.velocity.x = 0
			entity.velocity.x =  entity.max_air_speed
		end
		if entity.button.left
		then
			entity.velocity.x = 0
			entity.velocity.x = - entity.max_air_speed
		end
	end

	--Falling

	if entity.anim.current == entity.anim.falling
	then
		--print("X: ", entity.velocity.x)
		--print("Y: ", entity.velocity.y)

		if entity.velocity.y >= -1.0934910278593e-3 and entity.velocity.y < 0 
		then
			entity.velocity.x = -5
		elseif entity.velocity.y >= -1.0934910278593e-3 and entity.velocity.y < 0 and entity.button.right == true
		then	
			entity.velocity.x = 5
		end


		
		if entity.button.left
		then
			if math.abs(entity.velocity.x) > 0.001 or entity.velocity.x == 0
			then
				accelerate(delta_seconds, entity, - entity.max_air_speed, entity.air_acceleration)
			end
		elseif entity.button.right 
		then
			if math.abs(entity.velocity.x) > 0.001 or entity.velocity.x == 0
			then
				accelerate(delta_seconds, entity,  entity.max_air_speed, entity.air_acceleration)
			end
		else
			decelerate(delta_seconds, entity)
		end

		if entity.velocity.y > -0.01 and entity.velocity.y < 0 
		then 
			entity.velocity.y = 0
			entity.anim.current = entity.anim.idle
		end


		if entity.have_double_jumped == false --trigger
		then
			entity.can_double_jump = true
		end

	end


	--jump_from_wall
	if entity.anim.current == entity.anim.jump_from_wall
	then
		if entity.set_once == false
		then
			entity.now_you_can_jump = false
			entity.in_wall_jump_state = false
			entity.set_once = true
		end
	end



	--Hanging_Right
	if entity.anim.current == entity.anim.hanging_right
	then

		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds
		
		entity.in_wall_jump_state = true

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
		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds

		entity.in_wall_jump_state = true

		if entity.button.jump and entity.button.right and entity.can_walljump and entity.jump_timer > 0.2
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
		if entity.velocity.x < top_speed	
		then
			entity.velocity.x = entity.velocity.x +(acceleration*delta_seconds )
			--entity.anim.current = entity.anim.running
		else
			entity.velocity.x = top_speed
		end
	elseif top_speed < 0
	then
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
			entity.velocity.x = entity.velocity.x -(entity.deceleration*delta_seconds )
		elseif entity.velocity.x < -1 
		then
			entity.velocity.x = entity.velocity.x +(entity.deceleration*delta_seconds )
		else	
			entity.velocity.x = 0
		end
end

function update_triggers(delta_seconds, entity)
	--trigger
	if entity.triggered >= 4
	then
		

		--stun_trap
		if entity.triggered_type == 2 and entity.shield_active == true 
		then
			entity.is_stun_trap = true

		elseif entity.triggered_type == 2 and entity.stun_trap_triggered == false and entity.stun_trap_immune == false
		then
			entity.stun_trap_triggered = true
			entity.is_stund = true
			entity.stun_trap_timer = 0.0

			--print("stun_trap")
		end

		--speed_boost
		if entity.triggered_type == 4 and entity.speed_boost_triggered == false
		then
			entity.speed_boost_triggered = true
			entity.speed_boost_timer = 0.0

			if entity.double_jump_triggered or entity.glide_trap_triggered or entity.shield_active
			then
				entity.double_jump_triggered = false
				entity.glide_trap_triggered = false
				entity.shield_active = false
			end

			--print("Sprint_boost")
		end

		--double_jump
		if entity.triggered_type == 5 and entity.double_jump_triggered == false
		then
			entity.double_jump_triggered = true
			entity.can_double_jump = false
			entity.double_jump_timer = 0.0

			if entity.speed_boost_triggered or entity.glide_trap_triggered or entity.shield_active
			then
				entity.speed_boost_triggered = false
				entity.glide_trap_triggered = false
				entity.shield_active = false
			end

			--print("double_jump")
		end

		--glide_trap
		if entity.triggered_type == 3 and entity.glide_trap_triggered == false
		then
			entity.glide_trap_triggered = true
			entity.glide_trap_timer = 0.0

			if entity.speed_boost_triggered or entity.double_jump_triggered or entity.shield_active
			then
				entity.speed_boost_triggered = false
				entity.double_jump_triggered = false
				entity.shield_active = false
			end

			--print("glide_trap")

		end

		--random_buff
		if entity.triggered_type == 7 and entity.random_buff_triggered == false
		then
			entity.random_buff_triggered = true
			entity.random_buff_timer = 0.0


			entity.shield_active = true;
				
			if entity.speed_boost_triggered or entity.glide_trap_triggered or entity.double_jump_triggered or entity.shield_active
			then
				entity.speed_boost_triggered = false
				entity.glide_trap_triggered = false
				entity.double_jump_triggered = false
				entity.shield_active = false
			end

			--print("random_buff")
		end

		--shield
		if entity.shield_active == true
		then
			if entity.speed_boost_triggered or entity.double_jump_triggered or entity.glide_trap_triggered
			then
				entity.speed_boost_triggered = false
				entity.double_jump_triggered = false
				entity.glide_trap_triggered = false
				--entity.random_buff_triggered = false
			end
		end
	elseif entity.shield_active == true and entity.is_stun_trap == true
	then
		entity.shield_active = false
		entity.is_stun_trap = false
	end

	--trap and boost "functions"

	--stun trap
	if	entity.stun_trap_triggered == true and entity.stun_trap_timer <= entity.shock_trap_max_timer --stun timer
	then
		entity.stun_trap_timer = entity.stun_trap_timer + delta_seconds

	elseif	entity.stun_trap_triggered == true
	then
		entity.stun_trap_triggered = false
		entity.is_stund = false

		entity.stun_trap_immune_timer = 0.0
		entity.stun_trap_immune = true
	end

	if	entity.stun_trap_immune_timer <= entity.shock_trap_immune_max_timer and entity.stun_trap_immune == true --immune timer
	then
		entity.stun_trap_immune_timer = entity.stun_trap_immune_timer + delta_seconds

	elseif entity.stun_trap_immune == true
	then
		entity.stun_trap_immune = false
	end


	--glide_trap
	if entity.glide_trap_triggered == true and entity.glide_trap_timer <= entity.glide_trap_max_timer
	then
		entity.glide_trap_timer = entity.glide_trap_timer + delta_seconds



		if entity.button.right or entity.button.left or entity.anim.current == entity.anim.falling
		then
			entity.friction = entity.velocity.x
			entity.friction_slowrate = entity.velocity.x * entity.glide_decrease
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
	elseif entity.glide_trap_triggered == true
	then
		entity.glide_trap_triggered = false
	end


	--speed_boost
	if entity.speed_boost_triggered == true and entity.speed_boost_timer <= entity.speed_boost_max_timer
	then
		entity.speed_boost_timer = entity.speed_boost_timer + delta_seconds

		if entity.anim.current ~= entity.anim.jump_from_wall and entity.anim.current ~= entity.anim.hanging_left and entity.anim.current ~= entity.anim.hanging_right and entity.stun_trap_triggered == false
		then
			if entity.anim.current == entity.anim.falling
			then
				if entity.button.right
				then
					entity.velocity.x = entity.max_speed_boost_air--right
				
				elseif entity.button.left
				then 
					entity.velocity.x = -entity.max_speed_boost_air --left
				end
			else
				if entity.button.right
				then
					entity.velocity.x = entity.max_speed_boost--right
				
				elseif entity.button.left
				then 
					entity.velocity.x = -entity.max_speed_boost --left
				end
			end
		end

	elseif entity.speed_boost_triggered == true
	then
		entity.speed_boost_triggered = false
	end


	--double_jump
	if entity.double_jump_triggered == true and entity.double_jump_timer <= entity.double_jump_timer_max
	then 

		entity.double_jump_timer = entity.double_jump_timer + delta_seconds

		if entity.can_double_jump
		then
			if entity.button.jump and entity.now_you_can_jump and entity.have_double_jumped == false and entity.in_wall_jump_state == false 
			then
				entity.velocity.y = entity.double_jump_impulse
				entity.can_double_jump = false
				entity.have_double_jumped = true

				entity.now_you_can_jump = false

			end

			--if entity.button.jump and entity.anim.current == entity.anim.jump_from_wall
			--then
			--
			--	if entity.now_you_can_jump
			--	then
			--		entity.velocity.y = double_jump_impulse
			--		entity.can_double_jump = false
			--		entity.have_double_jumped = true
			--
			--		entity.now_you_can_jump = false
			--	end
			--end
		end
	elseif entity.double_jump_triggered == true
	then
		entity.double_jump_triggered = false
	end

	--random_buff
	if entity.random_buff_triggered == true and entity.random_buff_timer <= entity.random_buff_max_timer
	then

		entity.random_buff_timer = entity.random_buff_timer + delta_seconds

		if entity.random_assigned == false
		then
			entity.random_assigned = true

			id = entity.buffs_id[1]

			entity.random_last = id

			if id == entity.buffs_id[1] --glide_trap
			then
				--print("glide trap")
				entity.glide_trap_triggered = true
				entity.glide_trap_timer = 0.0

			elseif id == entity.buffs_id[2] --speed_boost
			then
				--print("speed boost")
				entity.speed_boost_triggered = true
				entity.speed_boost_timer = 0.0

			elseif id == entity.buffs_id[3] --double_jump
			then
				--print("double jump")
				entity.double_jump_triggered = true
				entity.double_jump_timer = 0.0
				entity.can_double_jump = false

			elseif id == entity.buffs_id[4] --shield
			then
				--print("shield player")
				entity.shield_active = true;
			end
		end
	else
		entity.random_buff_triggered = false
		entity.random_assigned = false
	end
end





