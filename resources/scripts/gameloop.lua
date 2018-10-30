function setup(game)
	game.goal = 20
	game.points = 0 -- do not change this, edit value in update loop
	--4 playerscores:
	game.scores = {p1, p2, p3, p4}
	game.scores.p1 = 0
	game.scores.p2 = 0
	game.scores.p3 = 0
	game.scores.p4 = 0
end

function update(delta_seconds, game, entities)

	if game.points == 0
	then
		for i = 1, 5, 1
		do
		entities[i].position.x = 0
		entities[i].position.y = 0
		game.points = 5
		end
	end

	for i = 1, 5, 1
	do
		if entities[i].position.y > game.goal
		then
			game.scores[i] = game.scores[i] + game.points
			game.points = game.points - 1
		end
	end

	print(game.scores[1])

end