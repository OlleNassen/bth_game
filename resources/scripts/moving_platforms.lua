function setup(entity)
	entity.moving_timer = 0
	entity.up_timer = 3
	entity.down_timer = entity.up_timer * 2
	entit.travel_distance = 12
	entit.travel_distance_per_seconds = entit.travel_distance / entity.up_timer


end

function update(delta_seconds, entity)

	if entity.moving_timer < entity.up_timer
	then
		entit.position.y = entit.position.y + (entit.travel_distance_per_seconds * delta_seconds)

	elseif entity.moving_timer < entity.down_timer
	then
		entit.position.y = entit.position.y - (entit.travel_distance_per_seconds * delta_seconds)
	else
		entity.moving_timer = 0
	end

	entity.moving_timer = entity.moving_timer + delta_seconds

end