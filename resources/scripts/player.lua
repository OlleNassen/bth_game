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
	entity.double_jump_delay_timer = 0.0
	entity.double_jump_triggered = false
	entity.have_doubble_jumpt = false
	entity.can_dubbel_jump = false
	entity.jump_was_push = false
	entity.have_wall_jumpt = false


	--glide_trap
	entity.glide_trap_triggered = false
	entity.glide_trap_timer = 0.0
	entity.friction = 0
	entity.friction_slowrate = 0

	--random_buff
	entity.random_buff_triggered = false
	entity.random_buff_timer = 0.0


end

--triggers

--shock_trap
local shock_trap_max_timer = 3
local shock_trap_immune_max_timer = 5

--speed boost
local max_speed_boost = 16 * 1.5
local speed_boost_max_timer = 5

--double jump
local double_jump_impulse = 50
local double_jump_timer_max = 5
local double_jump_delay_max_timer = 0.2

--glide_trap
local glide_trap_max_timer = 5

--random_buff
local random_buff_max_timer = 5


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

	have_wall_jumpt = true

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

		if entity.double_jump_triggered and entity.can_dubbel_jump == false and entity.have_doubble_jumpt == false --trigger
		then
			entity.can_dubbel_jump = true
			entity.double_jump_delay_timer = 0.0
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
			entity.have_doubble_jumpt = false
			entity.can_dubbel_jump = false
			entity.have_wall_jumpt = false
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


		if entity.have_doubble_jumpt == false --trigger
		then
			entity.can_dubbel_jump = true
		end

	end

	--Hanging_Right
	if entity.anim.current == entity.anim.hanging_right
	then
		
		if entity.double_jump_triggered--trigger
		then
			entity.have_wall_jumpt = true
		end

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

		if entity.double_jump_triggered --trigger
		then
			entity.have_wall_jumpt = true
		end

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
		--stun trap
		if entity.triggered_type == 4 and entity.stun_trap_triggered == false and entity.stun_trap_immune == false
		then
			entity.stun_trap_triggered = true
			entity.is_stund = true
			entity.stun_trap_timer = 0.0

			if entity.speed_boost_triggered or entity.double_jump_triggered or entity.glide_trap_triggered
			then
				entity.speed_boost_triggered = false
				entity.double_jump_triggered = false
				entity.glide_trap_triggered = false
			end

			--print("shock_trap")
		end

		--speed_boost
		if entity.triggered_type == 7 and entity.speed_boost_triggered == false
		then
			entity.speed_boost_triggered = true
			entity.speed_boost_timer = 0.0

			if entity.stun_trap_triggered or entity.double_jump_triggered or entity.glide_trap_triggered
			then
				entity.stun_trap_triggered = false
				entity.double_jump_triggered = false
				entity.glide_trap_triggered = false
			end

			--print("Sprint_boost")
		end

		--double_jump
		if entity.triggered_type == 8 and entity.double_jump_triggered == false
		then
			entity.double_jump_triggered = true
			entity.double_jump_timer = 0.0
			entity.steam_boost_delay_timer = 0.0

			if entity.stun_trap_triggered or entity.speed_boost_triggered or entity.glide_trap_triggered
			then
				entity.stun_trap_triggered = false
				entity.speed_boost_triggered = false
				entity.glide_trap_triggered = false
			end

			--print("steam_boost")
		end

		--glide_trap
		if entity.triggered_type == 3 and entity.glide_trap_triggered == false
		then
			entity.glide_trap_triggered = true
			entity.glide_trap_timer = 0.0

			if entity.stun_trap_triggered or entity.speed_boost_triggered or entity.double_jump_triggered
			then
				entity.stun_trap_triggered = false
				entity.speed_boost_triggered = false
				entity.double_jump_triggered = false				
			end

		end

		--random_buff
		if entity.triggered_type == 2 and entity.random_buff_triggered == false
		then
			entity.random_buff_triggered = true

		end
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
			entity.friction_slowrate = entity.velocity.x * 0.005
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
			print(max_speed_boost)

		elseif entity.button.left
		then 
			entity.velocity.x = -max_speed_boost --left
			print(max_speed_boost)
		end

	elseif entity.speed_boost_triggered == true
	then
		entity.speed_boost_triggered = false
	end


	--double_jump
	if entity.double_jump_triggered == true and entity.can_dubbel_jump and entity.double_jump_timer <= double_jump_timer_max
	then
		entity.double_jump_timer = entity.double_jump_timer + delta_seconds
		entity.double_jump_delay_timer = entity.double_jump_delay_timer + delta_seconds

		print("1")

		if entity.button.jump and not entity.jump_was_push and entity.have_wall_jumpt == false and entity.double_jump_delay_timer > double_jump_delay_max_timer
		then
			
			entity.velocity.y = double_jump_impulse

			entity.can_dubbel_jump = false
			entity.have_doubble_jumpt = true

		end
	elseif entity.double_jump_triggered == true
	then
		entity.double_jump_triggered = false
	end


	--random_buff
	if entity.random_buff_triggered == true and entity.random_buff_timer <= random_buff_max_timer
	then
		entity.random_buff_timer = entity.random_buff_timer + delta_seconds

	elseif entity.random_buff_triggered == true
	then
		entity.random_buff_triggered = false
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
