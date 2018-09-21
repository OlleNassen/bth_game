local t = require("resources/scripts/player_movement")
--loadfile("player_movement")


--f = assert (loadfile("resources/scripts/player_movement"))

function start()
	print(t)
	print(H)
	t.start()
	--f = dofile("test.lua")	
	--player_movement.start()
end

function update(a)
	--print("Wazooo")

	if(a == 0)then
		print("Up")
		end
	if(a == 1)then
		print("Left")
		--player_movement.walk(-delta_time)
		end
	if(a == 2)then
		print("Down")
		end
	if(a == 3)then
		print("Right")
		--player_movement.walk(delta_time)
		end
	if(a == 4)then
		print("Jump")
		end


	return a

end

function quit()
end
