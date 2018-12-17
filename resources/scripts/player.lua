
function setup(entity)
	--Jump stuff
	entity.can_jump = true
	entity.jump_timer = 0.0
	entity.jump_impulse_x = 0
	entity.jump_impulse_y = 0
	entity.jump_forgiveness_time = 0.125
	entity.jumped_last = false

	--Walljump stuff
	entity.can_walljump = true

	entity.states = {"idle", "moving", "jumping", "falling",
	"landing", "hanging_left", "hanging_right", "sliding", "wall_jumping"}
	entity.current_state = entity.states[1]
	
	entity.max_speed = 16
	entity.max_air_speed = 16
	entity.ground_acceleration = 180
	entity.deceleration = 70
	entity.air_acceleration = 100
	entity.ungrounded_time = 0
	entity.velocity_y_last = 0

	entity.gravity = 115
	entity.max_gravity = 1800

	entity.dash_timer = 0.0
	entity.dash_control = 0.0 -- How long until you can control your character again

	--TriGGerS---------------------------------------------------------------

	--shock_trap
	entity.stun_trap_timer = 0.0
	entity.stun_trap_immune_timer = 0.0
	entity.stun_trap_triggered = false
	entity.stun_trap_immune = false
	entity.temp = 0
 	entity.shock_trap_max_timer = 3
	entity.shock_trap_immune_max_timer = 5

	--speed boost
	entity.speed_boost_timer = 0.0
	entity.speed_boost_triggered = false
	entity.max_speed_boost = entity.max_speed * 1.25
	entity.max_speed_boost_air = entity.max_air_speed * 1.25
	entity.speed_boost_acceleration = entity.ground_acceleration * 3
	entity.speed_boost_air_acceleration = entity.air_acceleration * 2
	entity.speed_boost_max_timer = 10
	

	--double jump
	entity.double_jump_timer = 0.0
	entity.double_jump_triggered = false --triggered
	entity.have_double_jumped = false
	entity.can_double_jump = false --if have_double_jumped = false, can_double_jump = true
	entity.jump_pushed_last_frame = false --if jump button pressed last frame = pressed this frame, if released last frame, released now
	entity.now_you_can_jump = false --if was in jump last frame
	entity.in_wall_jump_state = false --in air after hanging r/l anim
	entity.set_once = false
	entity.double_jump_impulse = 40
	entity.double_jump_timer_max = 10

	--glide_trap
	entity.glide_trap_triggered = false
	entity.glide_trap_timer = 0.0
	entity.friction = 0
	entity.friction_slowrate = 0
	entity.glide_trap_max_timer = 10
	entity.glide_decrease = 0.0055

	--shield
	entity.is_stun_trap = false

	--random_buff
	entity.random_buff_triggered = false
	entity.random_assigned = false
	entity.random_last = 0
	entity.random_buff_timer = 0.0
	entity.random_buff_max_timer = 10
	entity.buffs_id = { 3, 4, 5, 6 }
	entity.have_set_seed = false

	entity.buff_activ = -1;


	entity.falling_delay_timer = 0.0
	entity.falling_delay_timer_max = 0.175


end

local epsilon = 0.001

--[[
	local max_speed = 16
	local max_air_speed = 16
	local ground_acceleration = 100
	local deceleration = 60
	local   = 100
]]--


function update(delta_seconds, entity)
	
	if entity.stun_trap_triggered == false
	then
		update_control(delta_seconds, entity)
	end

	update_triggers(delta_seconds, entity)

	if entity.have_set_seed == false
	then
		math.randomseed(entity.random_value)
		entity.have_set_seed = true
	end

	entity.falling_delay_timer = entity.falling_delay_timer + delta_seconds
	
	--Gravity
	if entity.current_state == entity.states[6] and entity.velocity.y < 0
	or entity.current_state == entity.states[7] and entity.velocity.y < 0 
	then
		entity.velocity.y = entity.velocity.y -(entity.gravity * 0.2 * delta_seconds)
		
	else
		entity.velocity.y = entity.velocity.y - entity.gravity * delta_seconds
		if entity.velocity.y < -entity.max_gravity * delta_seconds
		then
			entity.velocity.y = -entity.max_gravity * delta_seconds
		end
	end

	--Dash timer
	entity.dash_timer = entity.dash_timer - delta_seconds
	entity.dash_control = entity.dash_control - delta_seconds
	
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

function update_control(delta_seconds, entity)
	

	if entity.lw then
		entity.current_state = entity.states[6]
		entity.anim.current = entity.anim.hanging_left
	end

	if entity.rw then
		entity.current_state = entity.states[7]
		entity.anim.current = entity.anim.hanging_right
	end

	--Movement
	if entity.current_state == entity.states[1]
	or entity.current_state == entity.states[2]
	then
		entity.current_state = entity.states[1]
		entity.anim.current = entity.anim.idle
		entity.can_jump = true
		if entity.button.right 
		then
			accelerate(delta_seconds, entity, entity.max_speed, entity.ground_acceleration)
			entity.current_state = entity.states[2]
			entity.anim.current = entity.anim.running
		end

		if entity.button.left
		then
			accelerate(delta_seconds, entity, -entity.max_speed, entity.ground_acceleration)
			entity.current_state = entity.states[2]
			entity.anim.current = entity.anim.running
		end
		
		if entity.velocity.y < -1.0 -- if falling
		then

			entity.current_state = entity.states[4]

			if entity.falling_delay_timer > entity.falling_delay_timer_max
			then
				entity.anim.current = entity.anim.falling
			end
		end
		dash(delta_seconds, entity)
	end

	--Jump
	if entity.current_state == entity.states[1]
	or entity.current_state == entity.states[2]
	then

		if entity.button.jump and entity.can_jump and entity.jumped_last == false
		then
			entity.current_state = entity.states[3]
			entity.anim.current = entity.anim.start_jump

			entity.impulse.y = 46
			entity.can_jump = false

		end
		
	end

	--In Jump
	if entity.current_state == entity.states[3]
	then

		--double jump boost
		if entity.double_jump_triggered and entity.can_double_jump == false --trigger
		then
			entity.can_double_jump = true
		end

		if entity.button.right 
		then
			if math.abs(entity.velocity.x) < epsilon and entity.velocity.x ~= 0
			and entity.velocity.y < 20
			then
				entity.velocity.y = entity.velocity_y_last - entity.gravity * delta_seconds
			end
			accelerate(delta_seconds, entity, entity.max_air_speed, entity.air_acceleration)
		end
		
		if entity.button.left
		then
			if math.abs(entity.velocity.x) < epsilon and entity.velocity.x ~= 0
			and entity.velocity.y < 20
			then
				entity.velocity.y = entity.velocity_y_last - entity.gravity * delta_seconds
			end
			accelerate(delta_seconds, entity, -entity.max_air_speed, entity.air_acceleration)
		end

		if entity.button.right == false and entity.button.left == false
		then
			decelerate(delta_seconds, entity)
		end
	
		if entity.velocity.y < -0.1 -- switch to falling
		then
			entity.current_state = entity.states[4]
			entity.anim.current = entity.anim.falling
		end
		
		if entity.velocity.y > 0.0
		then
			entity.velocity_y_last = entity.velocity.y
		end

		dash(delta_seconds, entity)
	end

	--Falling
	if entity.current_state == entity.states[4]
	then
	 
		--jump_forgivenes
		entity.jump_timer = entity.jump_timer + delta_seconds

		if entity.jump_forgiveness_time > entity.jump_timer and entity.button.jump and entity.can_jump and entity.jumped_last == false
		then
			entity.impulse.y = 48
			entity.can_jump = false
		end

		if entity.button.left
		then
			if math.abs(entity.velocity.x) > epsilon or entity.velocity.x == 0
			then
				accelerate(delta_seconds, entity, - entity.max_air_speed, entity.air_acceleration)
			
			end
		end

		if entity.button.right 
		then
			if math.abs(entity.velocity.x) > epsilon or entity.velocity.x == 0
			then
				accelerate(delta_seconds, entity,  entity.max_air_speed, entity.air_acceleration)
			
			end
		end

		if entity.button.right == false and entity.button.left == false
		then
			decelerate(delta_seconds, entity)
		end
		
		if entity.velocity.y > -0.1 and entity.velocity.y <= 0 
		then 
			entity.velocity.y = 0
			entity.current_state = entity.states[5]

			if entity.falling_delay_timer > entity.falling_delay_timer_max
			then
				entity.anim.current = entity.anim.landing
				entity.falling_delay_timer = 0.0
			end

		end

		dash(delta_seconds, entity)
	end

	--Landing
	if entity.current_state == entity.states[5]
	then

		entity.can_jump = true
		entity.jump_timer = 0.0
		entity.current_state = entity.states[2]


		if entity.double_jump_triggered --trigger
		then
			entity.have_double_jumped = false
			entity.can_double_jump = false
			entity.in_wall_jump_state = false
			entity.set_once = false
		end


	end

	--Hanging left
	if entity.current_state == entity.states[6]
	then
		if entity.velocity.y > -epsilon and entity.velocity.y <= 0
		then
			entity.current_state = entity.states[1]
		end

		if entity.button.jump
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

			entity.current_state = entity.states[9]
		end

	end

	--Hanging right
	if entity.current_state == entity.states[7]
	then
		if entity.velocity.y > -epsilon and entity.velocity.y <= 0
		then
			entity.current_state = entity.states[1]
		end
		
		if entity.button.jump
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
			entity.current_state = entity.states[9]

		end
	end

	--Dash cooldown
	if entity.current_state == entity.states[8]
	then
		if entity.dash_control < 0.0
		then
			entity.current_state = entity.states[2]
		else
			entity.velocity.y = 0
		end
	end


	--Wall jumping
	if entity.current_state == entity.states[9]
	then
		if entity.velocity.y < -0.0
		then
			entity.current_state = entity.states[4]
		end
	end

	if entity.button.jump 
		then
			entity.jumped_last = true
		else
			entity.jumped_last = false
	end


	--New if-statement for standing on a platform
	if entity.triggered_type == 8 
	then
		 entity.falling_delay_timer = 0.0
	end
end

function dash(delta_seconds, entity)

	if entity.button.rotate and entity.dash_timer < 0.0 and math.abs(entity.velocity.x) > epsilon
	then
	
		local length = math.sqrt(entity.velocity.x * entity.velocity.x + entity.velocity.y * entity.velocity.y)

		local dir_x = entity.velocity.x / length
		local dir_y = entity.velocity.y / length

		local dash_speed = 100

		entity.velocity.x = dash_speed * dir_x
		entity.velocity.y = 0

		entity.dash_timer = 5.0
		entity.dash_control = 0.12

		entity.anim.current = entity.anim.dash

		entity.current_state = entity.states[8]
	end
end

function accelerate(delta_seconds, entity, top_speed, acceleration)

	if top_speed > 0
	then		
		if entity.velocity.x < top_speed	
		then
			entity.velocity.x = entity.velocity.x +(acceleration*delta_seconds )
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

		end

		--speed_boost
		if entity.triggered_type == 4 and entity.speed_boost_triggered == false
		then
			entity.speed_boost_triggered = true
			entity.speed_boost_timer = 0.0
			entity.buff_activ = 4

			if entity.double_jump_triggered or entity.glide_trap_triggered or entity.shield_active
			then
				entity.double_jump_triggered = false
				entity.glide_trap_triggered = false
				entity.shield_active = false
			end
		end

		--double_jump
		if entity.triggered_type == 5 and entity.double_jump_triggered == false
		then
			entity.double_jump_triggered = true
			entity.can_double_jump = false
			entity.double_jump_timer = 0.0
			entity.buff_activ = 5

			if entity.speed_boost_triggered or entity.glide_trap_triggered or entity.shield_active
			then
				entity.speed_boost_triggered = false
				entity.glide_trap_triggered = false
				entity.shield_active = false
			end
		end

		--glide_trap
		if entity.triggered_type == 3 and entity.glide_trap_triggered == false
		then
			entity.glide_trap_triggered = true
			entity.glide_trap_timer = 0.0
			entity.buff_activ = 3

			if entity.speed_boost_triggered or entity.double_jump_triggered or entity.shield_active
			then
				entity.speed_boost_triggered = false
				entity.double_jump_triggered = false
				entity.shield_active = false
			end

		end

		--random_buff
		if entity.triggered_type == 7 and entity.random_buff_triggered == false
		then
			entity.random_buff_triggered = true
			entity.random_buff_timer = 0.0
				
			if entity.speed_boost_triggered or entity.glide_trap_triggered or entity.double_jump_triggered or entity.shield_active
			then
				entity.speed_boost_triggered = false
				entity.glide_trap_triggered = false
				entity.double_jump_triggered = false
				entity.shield_active = false
			end

			random_assignment(entity)
		end

		--shield
		if entity.shield_active == true
		then
			if entity.speed_boost_triggered or entity.double_jump_triggered or entity.glide_trap_triggered
			then
				entity.speed_boost_triggered = false
				entity.double_jump_triggered = false
				entity.glide_trap_triggered = false
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
		entity.anim.current = entity.anim.stun

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



		if entity.button.right or entity.button.left or entity.current_state == entity.states[4]
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

		if entity.current_state ~= entity.states[9] and entity.current_state ~= entity.states[6] and entity.current_state ~= entity.states[7] and entity.stun_trap_triggered == false
		then
			if entity.current_state == entity.states[4]
			then
				if entity.button.right
				then
					accelerate(delta_seconds, entity, entity.max_speed_boost_air, entity.speed_boost_air_acceleration) --right

				elseif entity.button.left
				then 
					accelerate(delta_seconds, entity, -entity.max_speed_boost_air, entity.speed_boost_air_acceleration) --left
				end
			else
				if entity.button.right
				then
					accelerate(delta_seconds, entity, entity.max_speed_boost, entity.speed_boost_acceleration) --right

				elseif entity.button.left
				then 
					accelerate(delta_seconds, entity, -entity.max_speed_boost, entity.speed_boost_acceleration) --left
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
	else
		entity.random_buff_triggered = false
		entity.random_assigned = false
	end
end

function random_assignment(entity)

	if entity.random_assigned == false
	then
		entity.random_assigned = true

		id = entity.buffs_id[math.random(4)]

		entity.buff_activ = id
		entity.random_last = id

		if id == entity.buffs_id[1] --glide_trap
		then
			entity.glide_trap_triggered = true
			entity.glide_trap_timer = 0.0

		elseif id == entity.buffs_id[2] --speed_boost
		then
			entity.speed_boost_triggered = true
			entity.speed_boost_timer = 0.0

		elseif id == entity.buffs_id[3] --double_jump
		then
			entity.double_jump_triggered = true
			entity.double_jump_timer = 0.0
			entity.can_double_jump = false

		elseif id == entity.buffs_id[4] --shield
		then
			entity.shield_active = true;

		end
	end
end