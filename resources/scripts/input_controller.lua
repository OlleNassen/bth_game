function start()
	dt = 0
	player = {}
	player.x = 0
	player.y = 0
	player.is_in_air = false
	player.is_dead = false
	WALK_SPEED = 15
	JUMP_FORCE = 1	

end


function quit()
end


function update(a)
	--print("Wazooo")

	if(a == 0)then
		print("Up")
		
		end
	if(a == 1)then
		print("Left")
		walk(-dt)
		end
	if(a == 2)then
		print("Down")
		end
	if(a == 3)then
		print("Right")
		walk(dt)
		end
	if(a == 4)then
		jump()
		print("Jump")
		
		end

	return a, player.x, player.y

end

function walk(dt)
	--print(player.x)
	player.x = (dt/1000) * WALK_SPEED -- + fysik
end

function jump()
	if  player.is_in_air == false then
		print("Lisa was here")
		player.y = JUMP_FORCE 	
		player.is_in_air = true
	end
end