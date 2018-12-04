function setup(game)
	game.goal = 258
	game.points = 0 -- do not change this, edit value in update loop
	game.max_points = 10
	--4 playerscores:
	game.names = {"p1", "p2", "p3", "p4"}
	game.scores = { 0, 0, 0, 0 }
	game.finished = { false, false, false, false }
	game.died = { false, false, false, false }
	game.triggered_type = { -1, -1, -1, -1 }
	game.clock = 0.0
	game.winner = false


	--shield
	--game.shield_triggered = {false, false, false, false}
	game.shield_triggered = {false, false, false, false}
	game.is_spike = {false, false, false, false}
	

	game.time = 0.0
	game.max_time = 900000.0
	game.start_round = true
end

round = 0
death_height = { 0.0, 0.0, 0.0, 0.0 }
total_players = 0;

function update(delta_seconds, game, entities, player_count)

	if total_players == 0 
	then
		total_players = player_count
	end

	game.clock = game.clock + delta_seconds
	game.time = game.max_time - game.clock

	if game.start_round
	then
		game.start_round = false

		-- Start positions
		for i = 1, 4, 1
		do
			entities[i].position.x = 3 * (i - 1)
			entities[i].position.y = 0.9

			entities[i].impulse.x = 0
			entities[i].impulse.y = 0

			entities[i].velocity.x = 0
			entities[i].velocity.y = 0

			game.finished[i] = false
			game.died[i] = false
		end

		--game.max_points = player_count * 3
		game.points = total_players -- Don't change unless you know what you're doing :)
		round = round + 1
		--game.clock = 0

		for i = 1, 4, 1
		do
			game.finished[i] = false
			game.died[i] = false
		end
	end

	--[[
	-- Reset
	if game.points == 0
	then
		print("In Game.points == 0")
		-- Start positions
		entities[1].position.x = 0
		entities[1].position.y = 250

		entities[2].position.x = 3
		entities[2].position.y = 250

		entities[3].position.x = 6
		entities[3].position.y = 1.75

		entities[4].position.x = 9
		entities[4].position.y = 1.75
		
		game.points = player_count -- Don't change unless you know what you're doing :)
		round = round + 1
		--game.clock = 0

		for i = 1, 4, 1
		do
			game.finished[i] = false
			game.died[i] = false
		end
	end
	--]]

	--should be as long as a play time
	if game.clock > game.max_time -- Deal out points and reset if timer is too high
	then
		--new round
		game.points = 0 
	end

	--Check if players finished
	for i = 1, player_count, 1
	do
		if entities[i].position.y >= game.goal and not game.finished[i] and not game.died[i] and game.points ~= 0
		then
			game.scores[i] = game.scores[i] + game.points
			game.points = game.points - 1

			game.finished[i] = true

			entities[i].position.y = 270
			entities[i].position.x = 0

			entities[i].impulse.x = 0
			entities[i].impulse.y = 0

			entities[i].velocity.x = 0
			entities[i].velocity.y = 0
		end
	end
	
	--Check if players dead
	for i = 1, player_count, 1
	do
		if entities[i].triggered >= 4 and not game.finished[i]
		then
			if entities[i].triggered_type == 2 or entities[i].triggered_type == 6
			then
				game.shield_triggered[i] = entities[i].shield_active
			end

			--spike_trap
			if entities[i].triggered_type == 0 and game.shield_triggered[i] == false and not game.died[i] and game.spike_frame <= 50
			then
				game.finished[i] = true
				game.died[i] = true
				
				death_height[i] = entities[i].position.y;

				--entities[i].position.y = entities[i].position.y
				entities[i].position.x = -40

				entities[i].impulse.x = 0
				entities[i].impulse.y = 0

				entities[i].velocity.x = 0
				entities[i].velocity.y = 0

			elseif entities[i].triggered_type == 0 and game.shield_triggered[i] == true
			then
				game.is_spike[i] = true
				--print("protected")
			end



			--shield
			if entities[i].triggered_type == 6 and game.shield_triggered[i] == false
			then
				game.shield_triggered[i] = true
				entities[i].shield_active = true
			end

		elseif game.shield_triggered[i] == true and game.is_spike[i] == true
		then
			game.shield_triggered[i] = false
			game.is_spike[i] = false
			entities[i].shield_active = false
			--print("removed")

		end
	end

	--If player finished hold him up high
	for i = 1, 4, 1
	do
		if game.finished[i] and not game.died[i]
		then
			entities[i].position.y = 270
			entities[i].position.x = 0

			entities[i].impulse.x = 0
			entities[i].impulse.y = 0

			entities[i].velocity.x = 0
			entities[i].velocity.y = 0
		end
	end

	for i = 1, 4, 1
	do
		if game.died[i] and game.finished[i]
		then
			entities[i].position.y = death_height[i]
			entities[i].position.x = -40

			entities[i].impulse.x = 0
			entities[i].impulse.y = 0

			entities[i].velocity.x = 0
			entities[i].velocity.y = 0
		end
	end

	for i = 1, player_count, 1
	do
		if game.scores[i] >= game.max_points
		then
			game.winner = true
		end
	end

	for i = 1, player_count, 1
	do
		game.triggered_type[i] = entities[i].triggered_type
	end

	for i = 1, 4, 1
	do

		local left = entities[i]
		if left.dash_active and left.triggered < 4
		then
			local right = entities[left.triggered + 1]
			right.velocity.x = left.velocity.x

			--right.position.x = -10000
		end

	end

	--for i = 1, 4, 1
	--do
	--	if game.died[i] == true or game.finished[i]
	--	then
	--
	--		--entities[i].impulse.y = -entities[i].velocity.y
	--		--print(entities[i].position.y)
	--	end
	--end
end

function reset_time(game)
	game.start_round = true
	game.time = game.max_time
	--4 playerscores:
	game.finished = { false, false, false, false }
	game.died = {false, false, false, false}
	game.triggered_type = { -1, -1, -1, -1 }
	game.clock = 0.0
end