function setup(game)
	game.goal = 100
	game.points = 0 -- do not change this, edit value in update loop
	--4 playerscores:
	game.scores = { 0, 0, 0, 0 }

end

function update(delta_seconds, game, entities)

	if game.points <= 0
	then
		entities[1].position.x = 0
		entities[1].position.y = 0

		entities[2].position.x = 5
		entities[2].position.y = 0

		entities[3].position.x = 10
		entities[3].position.y = 0

		entities[4].position.x = 3
		entities[4].position.y = 0
		game.points = 5
	end

	for i = 1, 4, 1
	do
		if entities[i].position.y > game.goal
		then
			game.scores[i] = game.scores[i] + game.points
			game.points = game.points - 1
		end
	end

	--print(game.scores[1])

end