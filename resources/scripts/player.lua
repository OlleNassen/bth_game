
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
	
	if entity.button.left then
		entity.forces.x = entity.forces.x - 10000 * delta_seconds
	end
	
	if entity.button.right then
		entity.forces.x = entity.forces.x + 10000 * delta_seconds
	end

	print (entity.forces.x)
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