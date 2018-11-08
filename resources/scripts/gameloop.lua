function setup(game)
	game.goal = 258
	game.points = 0 -- do not change this, edit value in update loop
	game.max_points = 10
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
end

max_time = 90.0

function update(delta_seconds, game, entities)

	game.clock = game.clock + delta_seconds
	game.time = max_time - game.clock

	--should be as long as a play time
	if game.clock > max_time -- Deal out points and reset if timer is too high
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

			--spike_trap -- kvar
			if entities[i].triggered_type == 0
			then
				game.finished[i] = true
				game.died[i] = true
				entities[i].position.y = -2000
				entities[i].position.x = -2000

				game.points = game.points - 1

			end

		--	--sticky_platform
		--	if entities[i].triggered_type == 3
		--	then
		--		
		--		if entities[i].velocity.x >= game.max_velocity and entities[i].velocity.x > 0 --right
		--		then
		--			--entities[i].velocity.x = 1
		--			entities[i].forces.x = entities[i].forces.x - (entities[i].forces.x / 1.5) 
		--		
		--		elseif entities[i].velocity.x <= -game.max_velocity and entities[i].velocity.x < 0 --left
		--		then
		--			--entities[i].velocity.x = -1
		--			entities[i].forces.x = entities[i].forces.x - (entities[i].forces.x / 1.5)
		--		end
		--	end
		--
		--	----standard_platform
		--	--if entities[i].triggered_type == 6
		--	--then
		--	--
		--	--end
		--
		--	--speed_boost
		--	if entities[i].triggered_type == 7 and game.speed_boost_triggerd[i] == false
		--	then
		--		game.speed_boost_triggerd[i] = true
		--		game.speed_boost_timer[i] = 0.0
		--	end
		--end
		--
		--if	game.speed_boost_triggerd[i] == true and game.speed_boost_timer[i] <= 5.0
		--then
		--	if entities[i].velocity.x < game.max_velocity_boost and entities[i].velocity.x > -game.max_velocity_boost and entities.button.right
		--	then
		--		if entities[i].velocity.x > 0
		--		then
		--			entities[i].forces.x = game.max_speed_boost * 1.2 --right
		--		else
		--			entities[i].forces.x = game.max_speed_boost * 0.6
		--		end
		--
		--	elseif entities[i].velocity.x > -game.max_velocity_boost and entities[i].velocity.x < game.max_velocity_boost and entities.button.left
		--	then 
		--		if entities[i].velocity.x < 0
		--		then
		--			entities[i].forces.x = -game.max_speed_boost * 1.2 --left
		--		else
		--			entities[i].forces.x = -game.max_speed_boost * 0.6
		--		end
		--
		--	end
		--end
		--
		--if	game.speed_boost_timer[i] <= 5.0
		--then
		--	game.speed_boost_timer[i] = game.speed_boost_timer[i] + delta_seconds
		--
		--elseif	game.speed_boost_timer[i] >= 5.0
		--then
		--	game.speed_boost_triggerd[i] = false
		--end

	end

	for i = 1, 4, 1
	do
		if game.scores[i] > game.max_points
		then
			game.winner = true
		end
	end
end