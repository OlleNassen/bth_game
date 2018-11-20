
local  start_jump
local  in_jump
local  falling
local  landing
local  idle
local  running
local  sliding
local  hanging_left
local  hanging_right

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

local jump = function(entity, x, y) 
	if entity.button.jump then
		entity.update = start_jump
		entity.anim.current = entity.anim.start_jump				
		entity.impulse.x = x
		entity.impulse.y = y
	end
end


start_jump = function(delta_seconds, entity) 
	move(entity, 8000 * 0.8)
	entity.update = in_jump
	entity.anim.current = entity.anim.in_jump
end

in_jump = function(delta_seconds, entity) 
	move(entity, 8000 * 0.8)

	if entity.velocity.y < 5.0 then
		entity.update = falling
		entity.anim.current = entity.anim.falling
	end
end

falling = function(delta_seconds, entity) 	
	move(entity, 8000 * 0.8)
	
	if entity.anim.current == entity.anim.hanging_left then
		entity.update = hanging_left
		entity.anim.current = entity.anim.hanging_left
	end

	if entity.anim.current == entity.anim.hanging_right then
		entity.update = hanging_right
		entity.anim.current = entity.anim.hanging_right
	end
	
	if entity.anim.current == entity.anim.landing then
		entity.update = idle
		entity.anim.current = entity.anim.idle
	end
end

landing = function(delta_seconds, entity) 
	move(entity, 8000 * 0.8)
end

idle = function(delta_seconds, entity) 
	move(entity, 8000)
	jump(entity, 0, 15000)
end

running = function(delta_seconds, entity) 
	jump(entity, 0, 15000)
end

sliding = function(delta_seconds, entity) 

end

hanging_left = function(delta_seconds, entity) 
	jump(entity, 2000, 2000)
end

hanging_right = function(delta_seconds, entity) 
	jump(entity, -2000, 2000)
end
