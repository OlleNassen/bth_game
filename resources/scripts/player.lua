function setup(entity)
	--player_playable = true -- 1 should be thought as a playable player 
	entity.can_jump = true
	entity.can_walljump = true
	entity.can_move = true -- instead of playable
	--max_speed = 16
	-- max_air_speed = 16
 	--ground_acceleration = 100
	--air_acceleration = 80
	--deceleration = 60
	entity.jump_timer = 0
	entity.jump_impulse_x = 0

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
end

local jump_speed = 0
local gravity = 120
local max_gravity = 1600 --1800
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

--triggers

--shock_trap
local shock_trap_max_timer = 3
local shock_trap_immune_max_timer = 5

--speed boost
local max_speed_boost = max_speed * 5
local max_speed_boost_air = max_air_speed * 2
local speed_boost_ground_acceleration = ground_acceleration * 5
local speed_boost_air_acceleration = air_acceleration * 2
local speed_boost_max_timer = 10

--double jump
local double_jump_impulse = 40
local double_jump_timer_max = 10

--glide_trap
local glide_trap_max_timer = 10
local glide_decrease = 0.0055

--random_buff
local random_buff_max_timer = 10
local buffs_id = { 3, 4, 5, 6 }


function update(delta_seconds, entity)
	
	math.randomseed( os.time() )

	if entity.stun_trap_triggered == false
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
			--entity.velocity.x = max_speed
			accelerate(delta_seconds, entity, max_speed, ground_acceleration)

		end
		if entity.button.left
		then
			--entity.velocity.x = -max_speed
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
	end

	--In Jump
	if entity.anim.current == entity.anim.in_jump
	then 
		if entity.can_jump
		then
			entity.impulse.y = jump_speed
			entity.can_jump = false
		end

		if entity.double_jump_triggered and entity.can_double_jump == false --trigger
		then
			entity.can_double_jump = true
		end

		if entity.button.right 
		then
			
			accelerate(delta_seconds, entity,  max_air_speed, air_acceleration)
			--entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		elseif entity.button.left
		then
			accelerate(delta_seconds, entity, - max_air_speed, air_acceleration)
			--entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
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
			entity.velocity.x =  max_air_speed
			--entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
		if entity.button.left
		then
			entity.velocity.x = 0
			entity.velocity.x = - max_air_speed
			--entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
		end
	end

	--Falling

	if entity.anim.current == entity.anim.falling
	then
		--print("X: ", entity.velocity.x)
		--print("Y: ", entity.velocity.y)

		--[[if entity.velocity.y >= -1.0934910278593e-3 and entity.velocity.y < 0 
		then
		print("Lisa")
			entity.velocity.y = 10
		end]]--


		if entity.button.right 
		then
			if entity.velocity.x > 0.001 or entity.velocity.x < -0.001 and entity.velocity.y < -1 or entity.velocity.x == 0
			then
			--entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
				accelerate(delta_seconds, entity,  max_air_speed, air_acceleration)
			end
		elseif entity.button.left
		then
			if entity.velocity.x > 0.001 or entity.velocity.x < -0.001 and entity.velocity.y < -1 or  entity.velocity.x == 0
			then
			--entity.forces.x = entity.forces.x + (entity.maxSpeed*entity.acceleration*delta_seconds) / 3
				accelerate(delta_seconds, entity, - max_air_speed, air_acceleration)
			end
			--entity.forces.x = entity.forces.x + (-entity.maxSpeed*entity.acceleration*delta_seconds) / 3
			--accelerate(delta_seconds, entity, - max_air_speed, air_acceleration)
		else
			decelerate(delta_seconds, entity)
		end

		

		if entity.velocity.y > -0.01 and entity.velocity.y < 0 
		then 
			--print("adam")
			entity.velocity.y = 0
			entity.anim.current = entity.anim.jump_from_wall
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
		

		--stun_trap
		if entity.triggered_type == 2 and entity.shield_active == true 
		then
			entity.is_stun_trap = true

		elseif entity.triggered_type == 4 and entity.stun_trap_triggered == false and entity.stun_trap_immune == false
		then
			entity.stun_trap_triggered = true
			entity.is_stund = true
			entity.stun_trap_timer = 0.0

			--print("stun_trap")
		end

		--speed_boost
		if entity.triggered_type == 7 and entity.speed_boost_triggered == false
		then
			entity.speed_boost_triggered = true
			entity.speed_boost_timer = 0.0

			if entity.double_jump_triggered or entity.glide_trap_triggered or entity.shield_active
			then
				entity.double_jump_triggered = false
				entity.glide_trap_triggered = false
				entity.shield_active = false
			end

			print("Sprint_boost")
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
		if entity.triggered_type == 4 and entity.random_buff_triggered == false
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
	if	entity.stun_trap_triggered == true and entity.stun_trap_timer <= shock_trap_max_timer --stun timer
	then
		entity.stun_trap_timer = entity.stun_trap_timer + delta_seconds

	elseif	entity.stun_trap_triggered == true
	then
		entity.stun_trap_triggered = false
		entity.is_stund = false

		entity.stun_trap_immune_timer = 0.0
		entity.stun_trap_immune = true
	end

	if	entity.stun_trap_immune_timer <= shock_trap_immune_max_timer and entity.stun_trap_immune == true --immune timer
	then
		entity.stun_trap_immune_timer = entity.stun_trap_immune_timer + delta_seconds

	elseif entity.stun_trap_immune == true
	then
		entity.stun_trap_immune = false
	end


	--glide_trap
	if entity.glide_trap_triggered == true and entity.glide_trap_timer <= glide_trap_max_timer
	then
		entity.glide_trap_timer = entity.glide_trap_timer + delta_seconds



		if entity.button.right or entity.button.left or entity.anim.current == entity.anim.falling
		then
			entity.friction = entity.velocity.x
			entity.friction_slowrate = entity.velocity.x * glide_decrease
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
	if entity.speed_boost_triggered == true and entity.speed_boost_timer <= speed_boost_max_timer
	then
		entity.speed_boost_timer = entity.speed_boost_timer + delta_seconds

		if entity.anim.current ~= entity.anim.jump_from_wall and entity.anim.current ~= entity.anim.hanging_left and entity.anim.current ~= entity.anim.hanging_right and entity.stun_trap_triggered == false
		then
			if entity.anim.current == entity.anim.falling
			then
				if entity.button.right
				then
					--entity.velocity.x = max_speed_boost_air--right
					accelerate(delta_seconds, entity, max_speed_boost_air, speed_boost_air_acceleration)
				
				elseif entity.button.left
				then 
					--entity.velocity.x = -max_speed_boost_air --left
					accelerate(delta_seconds, entity, -max_speed_boost_air, speed_boost_air_acceleration)
				end
			else
				if entity.button.right
				then
					--entity.velocity.x = max_speed_boost--right
					accelerate(delta_seconds, entity, max_speed_boost, speed_boost_ground_acceleration)
				elseif entity.button.left
				then 
					--entity.velocity.x = -max_speed_boost --left
					accelerate(delta_seconds, entity, -max_speed_boost, speed_boost_ground_acceleration)
				end
			end
		end

	elseif entity.speed_boost_triggered == true
	then
		entity.speed_boost_triggered = false
	end


	--double_jump
	if entity.double_jump_triggered == true and entity.double_jump_timer <= double_jump_timer_max
	then 

		entity.double_jump_timer = entity.double_jump_timer + delta_seconds

		if entity.can_double_jump
		then
			if entity.button.jump and entity.now_you_can_jump and entity.have_double_jumped == false and entity.in_wall_jump_state == false 
			then
				entity.velocity.y = double_jump_impulse
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
	if entity.random_buff_triggered == true and entity.random_buff_timer <= random_buff_max_timer
	then

		entity.random_buff_timer = entity.random_buff_timer + delta_seconds

		if entity.random_assigned == false
		then
			entity.random_assigned = true


			rand = math.random(1, 4)
			id = buffs_id[rand]

			while id == entity.random_last
			do
				rand = math.random(1, 4)
				id = buffs_id[rand]
			end

			entity.random_last = id

			if id == buffs_id[1] --glide_trap
			then
				--print("glide trap")
				entity.glide_trap_triggered = true
				entity.glide_trap_timer = 0.0

			elseif id == buffs_id[2] --speed_boost
			then
				--print("speed boost")
				entity.speed_boost_triggered = true
				entity.speed_boost_timer = 0.0

			elseif id == buffs_id[3] --double_jump
			then
				--print("double jump")
				entity.double_jump_triggered = true
				entity.double_jump_timer = 0.0
				entity.can_double_jump = false

			elseif id == buffs_id[4] --shield
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





