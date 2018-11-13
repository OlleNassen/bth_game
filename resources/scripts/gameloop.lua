function setup(game)
	game.goal = 258
	game.points = 0 -- do not change this, edit value in update loop
	game.max_points = 4
	--4 playerscores:
	game.names = {"p1", "p2", "p3", "p4"}
	game.scores = { 0, 0, 0, 0 }
	game.finished = { false, false, false, false }
	game.died = {false, false, false, false}
	game.clock = 0.0
	game.winner = false

	--game.speed_boost_timer = {0.0, 0.0, 0.0, 0.0}
	--game.speed_boost_triggerd = { false, false, false, false }
	--game.max_speed = 8700
	--game.max_speed_boost = game.max_speed * 2
	--game.max_velocity = 16
	--game.max_velocity_boost = game.max_velocity * 1.3

	game.time = 0.0
	game.max_time = 90.0
end

round = 0

function update(delta_seconds, game, entities)

	game.clock = game.clock + delta_seconds
	game.time = game.max_time - game.clock

	--should be as long as a play time
	if game.clock > game.max_time -- Deal out points and reset if timer is too high
	then
		while game.points > 0
		do
			-- Find any unfinished player
			highest_index = -1
			for i = 1, 4, 1
			do
				if game.finished[i] == false
				then
					highest_index = i
				end
			end

			-- Find player closest to goal
			highest_y = entities[highest_index].position.y
			for i = 1, 4, 1
			do
				if game.finished[i] == false
				then
					if entities[i].position.y > highest_y
					then
						highest_index = i
						highest_y = entities[i].position.y
					end
				end
			end

			game.scores[highest_index] = game.scores[highest_index] + game.points
			game.finished[highest_index] = true
			game.points = game.points - 1
		end
	end

	-- Reset
	if game.points == 0
	then
		-- Start positions
		entities[1].position.x = 0
		entities[1].position.y = 1.75

		entities[2].position.x = 3
		entities[2].position.y = 1.75

		entities[3].position.x = 6
		entities[3].position.y = 1.75

		entities[4].position.x = 9
		entities[4].position.y = 1.75
		
		game.points = 4 -- Don't change unless you know what you're doing :)
		round = round + 1
		--game.clock = 0

		for i = 1, 4, 1
		do
			game.finished[i] = false
			game.died[i] = false
		end

	end

	--Check if players finished
	for i = 1, 4, 1
	do
		if entities[i].position.y > game.goal and not game.finished[i]
		then
			game.scores[i] = game.scores[i] + game.points
			game.points = game.points - 1

			game.finished[i] = true

			entities[i].position.x = -2000
			entities[i].position.y = -2000
		end
	end
		--Check if players dead
	for i = 1, 4, 1
	do

		if entities[i].triggered >= 4 and not game.finished[i]
		then

			--spike_trap -- kvar
			if entities[i].triggered_type == 0
			then
				game.finished[i] = true
				game.died[i] = true
				entities[i].position.y = -2000
				entities[i].position.x = -2000

				game.points = game.points - 1

			end
		end
	end

	for i = 1, 4, 1
	do
		if game.scores[i] > game.max_points
		then
			game.winner = true
		end
	end

	for i = 1, 4, 1
	do
		if game.died[i] == true or game.finished[i]
		then
			entities[i].impulse.y = -entities[i].velocity.y
			--print(entities[i].position.y)
		end
	end
end

function reset_time(game)
	game.time = game.max_time

	--4 playerscores:
	game.finished = { false, false, false, false }
	game.died = {false, false, false, false}
	game.clock = 0.0
end