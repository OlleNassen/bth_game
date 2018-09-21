require("../resources/scripts/test")
--loadfile("player_movement")

--f = assert (loadfile("test.lua"))
--t.start()
function start()
	
	--test.start()
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
