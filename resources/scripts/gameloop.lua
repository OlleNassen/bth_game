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

	game.immune = {false, false, false, false}

	--shield
	--game.shield_triggered = {false, false, false, false}
	game.shield_triggered = {false, false, false, false}
	game.is_spike = {false, false, false, false}
	game.is_laser = {false, false, false, false}

	game.time = 1000000000000.0
	game.max_time = 90.0
	game.start_round = true
end

round = 0
death_height = { 0.0, 0.0, 0.0, 0.0 }

checkpoints = { {}, {}, {}, {} }

checkpoints[1].x = 0.0
checkpoints[1].y = 0.9

checkpoints[2].x = 0.0
checkpoints[2].y = 92.0

checkpoints[3].x = 0.0
checkpoints[3].y = 140.0

checkpoints[4].x = 0.0
checkpoints[4].y = 194.0
			
respawn_timer = { 0.0, 0.0, 0.0, 0.0 }
last_checkpoint = { 0, 0, 0, 0 }

function update(delta_seconds, game, entities, player_count)

	
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