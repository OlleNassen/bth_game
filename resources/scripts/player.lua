
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
	entity.update = idle
end

function update(delta_seconds, entity)
	entity.update(delta_seconds, entity)
end

local move = function(entity, force) 
	local result = 0

	if entity.button.left then
		result = result - force
	end
	
	if entity.button.right then
		result = result + force
	end

	if entity.velocity.x < 0 then
		result = result * 2
	end
	
	if entity.velocity.x > 0 then
		result = result * 2
	end


	entity.forces.x = result
end


start_jump = function(delta_seconds, entity) 

end

in_jump = function(delta_seconds, entity) 

end

falling = function(delta_seconds, entity) 

end

landing = function(delta_seconds, entity) 

end

hanging_wall = function(delta_seconds, entity) 

end

connect_wall = function(delta_seconds, entity) 

end

jump_from_wall = function(delta_seconds, entity) 

end

idle = function(delta_seconds, entity) 
	
	move(entity, 300)
end

running = function(delta_seconds, entity) 

end

turning = function(delta_seconds, entity) 

end

sliding = function(delta_seconds, entity) 

end

hanging_left = function(delta_seconds, entity) 

end

hanging_right = function(delta_seconds, entity) 

end

independent = function(delta_seconds, entity) 

end