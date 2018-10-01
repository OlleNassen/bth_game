function start()
	dt = 0
	can_jump = 1
	player = {}
	player.x = 0
	player.y = 0
	player.is_dead = false
	WALK_SPEED = 200
	JUMP_FORCE = 60

end


function quit()
end


function update(a)
	--print("Wazooo")

	--print("Lua can_jump: ")
	--print(dt)

	player.x = 0
	player.y = 0

	if(a == 0)then
		--print("Up")
		
		end
	if(a == 1)then
	--	print("Left")
		walk(-dt)
		end
	if(a == 2)then
		--print("Down")
		end
	if(a == 3)then
		--print("Right")
		walk(dt)
		end
	if(a == 4)then
		jump(can_jump)
		--print("Jump")
	end
	
	return player.x, player.y
end

function walk(dt)
	--print(player.x)
	player.x = (dt/1000) * WALK_SPEED -- + fysik
end

function jump(can_jump)

	if can_jump == 1 then
		--player.can_jump = 0
		player.y = JUMP_FORCE 
	end
end