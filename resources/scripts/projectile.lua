function setup(entity)
	
	--start_pos
	--pos
	--direction
	--bool active

	--entity.timer = 0


	--position
	--velocity
	--turret_position
	--turret_direction
	--turret_width
	--turret_height

	entity.a_speed = 0.5
	entity.timer_max = 5

end



function  move(delta_seconds, entity)
	
	entity.timer = entity.timer + delta_seconds

	start(delta_seconds, entity)

	if entity.active --within map (and moving)
	then
		if --entity.direction == x/y +/-
			entity.speed = entity.direction * (entity.a_speed * delta_seconds)
		end

		entity.pos = entity.pos + entity.speed
	else
		entity.pos.x = -100 --outside map
		entity.pos.y = -100
	end
end

function start(delta_seconds, entity)
	
	if entity.timer >= entity.timer_max and entity.active == false
	then
		entity.pos = entity.start_pos
		entity.active = true
	end
end