function setup(elevator_number, entity)

if elevator_number == 0
then
	entity.moving_timer = 3
	entity.up_timer = 3
	entity.down_timer = entity.up_timer * 2
	entity.travel_distance = 12
	entity.travel_distance_per_seconds = entity.travel_distance / entity.up_timer

elseif elevator_number == 1
then
	entity.moving_timer = 1.25
	entity.up_timer = 2.5
	entity.down_timer = entity.up_timer * 2
	entity.travel_distance = 18
	entity.travel_distance_per_seconds = entity.travel_distance / entity.up_timer

elseif elevator_number == 2
then
	entity.moving_timer = 0
	entity.up_timer = 3
	entity.down_timer = entity.up_timer * 2
	entity.travel_distance = 12
	entity.travel_distance_per_seconds = entity.travel_distance / entity.up_timer
end

end

function update(delta_seconds, entity)

	if entity.moving_timer < entity.up_timer
	then
		entity.position.y = entity.position.y + (entity.travel_distance_per_seconds * delta_seconds)

	elseif entity.moving_timer < entity.down_timer
	then
		entity.position.y = entity.position.y - (entity.travel_distance_per_seconds * delta_seconds)
	else
		entity.moving_timer = 0
	end

	entity.moving_timer = entity.moving_timer + delta_seconds

end