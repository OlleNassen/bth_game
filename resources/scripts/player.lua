
local  idle
local  start_jump
local  in_jump
local  falling
local  landing
local  hanging_left
local  hanging_right
local  sliding

function setup(entity)
	entity.update = idle
end

local gravity = 10000

function update(delta_seconds, entity)
	entity.update(delta_seconds, entity)
	entity.forces.y = entity.forces.y - gravity
end

-- helpers --

local move = function(entity, force) 
		
	local result = 0
	local vel = 5
	
	if entity.button.left then
		result = result - force
	end
	
	if entity.button.right then
		result = result + force	
	end
	
	if entity.velocity.x < vel or entity.velocity.x > -vel  then
		result = result * 2
	end

	entity.forces.x = result
	if result < -0.1 or result > 0.1 then
		if entity.anim.current == entity.anim.idle then
			entity.anim.current = entity.anim.running
		end
	elseif entity.anim.current == entity.anim.running then
		entity.anim.current = entity.anim.idle
	end
end

local jump = function(entity, x, y, anim) 
	if entity.button.jump then
		entity.update = start_jump
		entity.anim.current = anim				
		entity.impulse.x = x
		entity.impulse.y = y
	end
end

local fall  = function(entity) 
	if entity.velocity.y < -2.0 then
		entity.update = falling
		entity.anim.current = entity.anim.falling
	end
end

-- states --
local speed = 8000

idle = function(delta_seconds, entity) 
	move(entity, speed)
	jump(entity, 0, 15000, entity.anim.start_jump)
	fall(entity)
end

start_jump = function(delta_seconds, entity) 
	move(entity, speed * 0.8)
	entity.update = in_jump
	entity.anim.current = entity.anim.in_jump
end

in_jump = function(delta_seconds, entity) 
	move(entity, speed * 0.8)
	fall(entity)
end

falling = function(delta_seconds, entity) 	
	move(entity, speed * 0.8)
	
	if entity.anim.current == entity.anim.hanging_left then
		entity.update = hanging_left
	end

	if entity.anim.current == entity.anim.hanging_right then
		entity.update = hanging_right
	end
	
	if entity.anim.current == entity.anim.landing then
		entity.update = idle
		entity.anim.current = entity.anim.idle
	end
end

landing = function(delta_seconds, entity) 
	move(entity, speed * 0.8)
end

hanging_left = function(delta_seconds, entity) 
	jump(entity, 2000, 2000, entity.anim.jump_from_wall)

	if entity.anim.current == entity.anim.idle then
		entity.update = idle
	end
end

hanging_right = function(delta_seconds, entity) 
	jump(entity, -2000, 2000, entity.anim.jump_from_wall)

	if entity.anim.current == entity.anim.idle then
		entity.update = idle
	end
end

sliding = function(delta_seconds, entity) 

end
