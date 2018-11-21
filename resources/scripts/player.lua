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
	entity.wall_jump_timer = 0.0
	entity.double_jump_triggered = false --triggered
	entity.have_double_jumped = false
	entity.can_double_jump = false --if have_double_jumped = false, can_double_jump = true
	entity.jump_was_push = false --if jump button pressed last frame = pressed this frame, if released last frame, released now
	entity.have_wall_jumped = false --not using atm
	entity.now_you_can_jump = false --if was in jump last frame

	entity.in_wall_jump = false --in air after hanging r/l anim


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

	--projectile


end

--triggers

--shock_trap
local shock_trap_max_timer = 3
local shock_trap_immune_max_timer = 5

--speed boost
local max_speed_boost = 16 * 1.5
local speed_boost_max_timer = 20

--double jump
local double_jump_impulse = 50
local double_jump_timer_max = 20
local wall_jump_timer_max = 0.175

--glide_trap
local glide_trap_max_timer = 20

--random_buff
local random_buff_max_timer = 5
local buffs_id = { 3, 4, 5, 6 }


local jump_speed = 0
local gravity = 120
local max_gravity = 1600
local wall_jump_speed = 
{
	x = 150,
	y = 20
}

function update(delta_seconds, entity)
	
	math.randomseed( os.time() )

	--update_anim_state(delta_seconds, entity)

	have_wall_jumped = true

	if entity.stun_trap_triggered == false
	then
		update_controls(delta_seconds, entity)
	end

	update_triggers(delta_seconds, entity)


	--Gravity 
	entity.velocity.y = entity.velocity.y - gravity * delta_seconds
	if entity.velocity.y < -max_gravity * delta_seconds
	then
		entity.velocity.y = -max_gravity * delta_seconds
	end

	entity.jump_was_push = entity.button.jump

 
	if entity.can_double_jump and entity.jump_was_push == false and entity.now_you_can_jump == false
	then
		entity.now_you_can_jump = true
	elseif entity.can_double_jump == false
	then
		entity.now_you_can_jump = false
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

	--Start jump
	if entity.anim.current == entity.anim.start_jump
	then
		if entity.can_jump and entity.button.jump == true and entity.jump_timer < 0.17
		then
			jump_speed = jump_speed + 5
			entity.jump_timer = entity.jump_timer + delta_seconds

		elseif jump_speed > 0 and entity.button.jump == false and entity.jump_timer > 0.016 or entity.jump_timer > 0.17  and entity.jump_timer > 0.016
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

		if entity.double_jump_triggered and entity.can_double_jump == false --and entity.have_double_jumped == false --trigger
		then
			entity.can_double_jump = true
		end



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
			entity.have_wall_jumped = false
		end

		
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

		if entity.velocity.y == 0
		then 
			entity.anim.current = entity.anim.idle
		end


		if entity.have_double_jumped == false --trigger
		then
			entity.can_double_jump = true
		end

	end


	--jump_from_wall
	if entity.anim.current == entity.anim.jump_from_wall --and entity.wall_jump_timer >= wall_jump_timer_max
	then
		entity.in_wall_jump = false
		if entity.have_double_jumped == false
		then
			entity.can_double_jump = true
		end
	end



	--Hanging_Right
	if entity.anim.current == entity.anim.hanging_right
	then

		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds
		
		entity.in_wall_jump = true
		entity.wall_jump_timer = 0.0

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

		entity.in_wall_jump = true
		entity.wall_jump_timer = 0.0

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




	--Cap the velocity/entity.forces.x
	--[[if entity.forces.x > entity.maxSpeed
		then
		entity.forces.x = entity.maxSpeed
	elseif entity.forces.x < -entity.maxSpeed
	then 
		entity.forces.x = -entity.maxSpeed
	end]]--
end


function update_triggers(delta_seconds, entity)
	--trigger
	if entity.triggered >= 4
	then
		

		--stun_trap
		if entity.triggered_type == 4 and entity.shield_active == true 
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

			--print("Sprint_boost")
		end

		--double_jump
		if entity.triggered_type == 8 and entity.double_jump_triggered == false
		then
			entity.double_jump_triggered = true
			entity.can_double_jump = false
			entity.double_jump_timer = 0.0
			entity.steam_boost_delay_timer = 0.0

			if entity.speed_boost_triggered or entity.glide_trap_triggered or entity.shield_active
			then
				entity.speed_boost_triggered = false
				entity.glide_trap_triggered = false
				entity.shield_active = false
			end

			print("steam_boost")
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

		end

		--random_buff
		if entity.triggered_type == 2 and entity.random_buff_triggered == false
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
			entity.friction_slowrate = entity.velocity.x * 0.0055
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

		if entity.button.right
		then
			entity.velocity.x = max_speed_boost--right
		
		elseif entity.button.left
		then 
			entity.velocity.x = -max_speed_boost --left
		end

	elseif entity.speed_boost_triggered == true
	then
		entity.speed_boost_triggered = false
	end


	--double_jump
	if entity.double_jump_triggered == true and entity.can_double_jump and entity.double_jump_timer <= double_jump_timer_max
	then 
		entity.double_jump_timer = entity.double_jump_timer + delta_seconds
		entity.wall_jump_timer = entity.wall_jump_timer + delta_seconds

		--if entity.button.jump
		--then
		--	print("1: jump		", entity.button.jump )
		--	print("2: now you can jump", entity.now_you_can_jump)
		--	print("3: have double jumped", entity.have_double_jumped)
		--	print("4: in wall jump ",entity.in_wall_jump)
		--	print("5: ", entity.wall_jump_timer)
		--	print("")
		--end

		if entity.button.jump and entity.now_you_can_jump and entity.have_double_jumped == false and entity.in_wall_jump == false and entity.wall_jump_timer >= wall_jump_timer_max
		then
			--print("WOOOOOOOOOW")
			--print(entity.wall_jump_timer)
			entity.velocity.y = double_jump_impulse
			entity.can_double_jump = false
			entity.have_double_jumped = true

			entity.now_you_can_jump = false

		end
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
				print("glide trap")
				entity.glide_trap_triggered = true
				entity.glide_trap_timer = 0.0

			elseif id == buffs_id[2] --speed_boost
			then
				print("speed boost")
				entity.speed_boost_triggered = true
				entity.speed_boost_timer = 0.0

			elseif id == buffs_id[3] --double_jump
			then
				print("double jump")
				entity.double_jump_triggered = true
				entity.double_jump_timer = 0.0
				entity.steam_boost_delay_timer = 0.0

			elseif id == buffs_id[4] --shield?
			then
				print("shield player")
				entity.shield_active = true;
			end
		end
	else
		entity.random_buff_triggered = false
		entity.random_assigned = false
	end
end


function update_anim_state(delta_seconds, entity)

	--entity.jump_impulse_x = entity.jump_impulse_x * 0.5
	if entity.jump_impulse_x > 0.1 or entity.jump_impulse_x < -0.1
	then
		entity.jump_impulse_x = entity.jump_impulse_x * 0.5
		entity.velocity.x = entity.jump_impulse_x * delta_seconds
	else
		entity.velocity.x = 0
	end
	

	local speed = entity.max_speed * delta_seconds
	
	--if entity.anim.current == entity.anim.in_jump
	--or entity.anim.current == entity.anim.falling 
	--then
	--	speed = entity.max_air_speed * delta_seconds
	--end

	

	--In Running state
	if entity.anim.current == entity.anim.running
	then	

		--Running
		if entity.button.left
		then
			entity.velocity.x =  -speed --entity.velocity.x
		end
		if entity.button.right
		then
			entity.velocity.x =  speed --entity.velocity.x +
		end

		if entity.button.jump
		then
			entity.anim.current = entity.anim.start_jump
		end
		if entity.velocity.y < -0.4
		then 
			entity.anim.current = entity.anim.falling
		end

		entity.anim.current = entity.anim.idle
	end

	--In Idle state
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

	--Just started Jump state
	if entity.anim.current == entity.anim.start_jump
	then
		if entity.can_jump
		then		
			entity.velocity.y = jump_speed * delta_seconds
			entity.can_jump = false
		end	
	end
	
	--In Jump state
	if entity.anim.current == entity.anim.in_jump
	then
		if entity.can_jump
		then		
			entity.velocity.y = jump_speed * delta_seconds
			entity.can_jump = false
		end	
		
		--Glide in air
		if entity.button.left
		then
			entity.velocity.x = -speed --entity.velocity.x 
 		end
		if entity.button.right
		then
			entity.velocity.x = speed --entity.velocity.x + 
		end

		if entity.velocity.y < -0.4
		then 
			entity.anim.current = entity.anim.falling
		end
	end

	--In landing state
	if entity.anim.current == entity.anim.landing
	then
		entity.ungrounded_time = 0
		entity.can_jump = true
		entity.can_walljump = true

		--Glide in air
		if entity.button.left
		then
			entity.velocity.x = -speed --entity.velocity.x 
		end
		if entity.button.right
		then
			entity.velocity.x = speed --entity.velocity.x + 
		end
	end	
	
	if entity.anim.current == entity.anim.falling
	then
		--Glide in air
		if entity.button.left
		then
			entity.velocity.x = -speed --entity.velocity.x - 
		end
		if entity.button.right
		then
			entity.velocity.x =  speed --entity.velocity.x +
		end
	end


	if entity.anim.current == entity.anim.hanging_right
	then
		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds
		--and entity.button.left 
		if entity.button.jump and entity.can_walljump and entity.jump_timer > 0.2
		then	
			entity.anim.current = entity.anim.jump_from_wall
			entity.velocity.y = wall_jump_speed.y
			entity.jump_impulse_x = -wall_jump_speed.x
			entity.velocity.x = -wall_jump_speed.x
			entity.can_walljump = true  --false
		end
	end

	if entity.anim.current == entity.anim.hanging_left
	then
		entity.ungrounded_time = 0
		entity.jump_timer = entity.jump_timer + delta_seconds
		--and entity.button.right
		if entity.button.jump  and entity.can_walljump and entity.jump_timer > 0.2
		then
			entity.anim.current = entity.anim.jump_from_wall
			entity.velocity.y = wall_jump_speed.y
			entity.jump_impulse_x = wall_jump_speed.x
			entity.velocity.x = wall_jump_speed.x
			entity.can_walljump = true --false
		end
	end

	if entity.anim.current == entity.anim.jump_from_wall
	then	
		--entity.jump_impulse_x = wall_jump_speed.x * delta_seconds
	end
end
