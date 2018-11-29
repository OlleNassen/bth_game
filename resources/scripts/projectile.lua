function setup(entity)
	
	--start_pos
	--pos
	--direction
	--bool active

	--entity.timer = 0
end

local a_speed = 0.5
local timer_max = 5

function  move(entity)
	
	start(entity)

	if entity.active --within map (and moving)
	then
		if --entity.direction == x/y +/-
			entity.speed = direction * a_speed			
		end

	entity.pos = entity.pos + entity.speed

	else
		entity.pos = --outside map
	end
end

function start(entity)
	
	if entity.timer >= timer_max and entity.active == false
	then
		entity.pos = entity.start_pos
		entity.active = true
	end
end
