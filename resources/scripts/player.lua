
local  start_jump
local  in_jump
local  falling
local  landing
local  hanging_wall
local  connect_wall
local  jump_from_wall
local  idle
local  running
local  turning
local  sliding
local  hanging_left
local  hanging_right
local  independent

function setup(entity)
	entity.jump_timer = 0.0
	entity.update = idle
end

local gravity = 10000

function update(delta_seconds, entity)
	entity.update(delta_seconds, entity)
	entity.forces.y = entity.forces.y - gravity
end

local move = function(entity, force) 
	
	local result = 0
	local vel = 5
	
	if entity.button.left then
		result = result - force
		if entity.velocity.x > -vel  then
			result = result * 2
		end
	end
	
	if entity.button.right then
		result = result + force
		if entity.velocity.x < vel  then
			result = result * 2
		end
	end

	entity.forces.x = result
end

local jump = function(entity, impulse) 
	if entity.button.jump then
		entity.update = start_jump
		entity.anim.current = entity.anim.start_jump
		entity.impulse.y = impulse
	end
end


start_jump = function(delta_seconds, entity) 
	move(entity, 7000 * 0.8)
	entity.update = in_jump
	entity.anim.current = entity.anim.in_jump
end

in_jump = function(delta_seconds, entity) 
	move(entity, 7000 * 0.8)

	if entity.velocity.y < 5.0 then
		entity.update = falling
		entity.anim.current = entity.anim.falling
	end
end

falling = function(delta_seconds, entity) 	
	move(entity, 7000 * 0.8)

	print("here")
	if entity.anim.current == entity.anim.landing then
		entity.update = idle
		entity.anim.current = entity.anim.idle
	end
end

landing = function(delta_seconds, entity) 
	move(entity, 7000 * 0.8)

end

hanging_wall = function(delta_seconds, entity) 

end

connect_wall = function(delta_seconds, entity) 

end

jump_from_wall = function(delta_seconds, entity) 

end

idle = function(delta_seconds, entity) 
	move(entity, 7000)
	jump(entity, 15000)
	
end

running = function(delta_seconds, entity) 
	jump(entity, 15000)
end

turning = function(delta_seconds, entity) 
	jump(entity, 15000)
end

sliding = function(delta_seconds, entity) 

end

hanging_left = function(delta_seconds, entity) 

end

hanging_right = function(delta_seconds, entity) 

end

independent = function(delta_seconds, entity) 

end