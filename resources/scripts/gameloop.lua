function setup(game)
	game.goal = 171
	game.points = 0 -- do not change this, edit value in update loop
	game.max_points = 10
	--4 playerscores:
	game.names = {"ole", "ed", "pet", "osby"}
	game.scores = { 0, 0, 0, 0 }
	game.finished = { false, false, false, false }
	game.died = {false, false, false, false}
	game.clock = 0.0
	game.winner = false
end

function update(delta_seconds, game, entities)

	game.clock = game.clock + delta_seconds

	if game.clock > 120.0 -- Deal out points and reset if timer is too high
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
		entities[1].position.y = 4

		entities[2].position.x = 3
		entities[2].position.y = 4

		entities[3].position.x = 5
		entities[3].position.y = 4

		entities[4].position.x = 8
		entities[4].position.y = 4

		game.points = 4 -- Don't change unless you know what you're doing :)
		game.clock = 0

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
			game.points = game.points - 1

			game.finished[i] = true
			game.died[i] = true
			entities[i].position.x = -2000
			entities[i].position.y = -2000
		end
	end

	for i = 1, 4, 1
	do
		if game.scores[i] > game.max_points
		then
			game.winner = true
		end
	end

end