local M = {}

local function start()
	--[[player = {}
	player.in_air = false
	player.is_moving = false
	player.is_dead = false
	player.force_up = 0
	player.position.x = 0
	player.position.y = 0
	
	]]
	print("Kaffe")
end
M.start = start
--[[
function walk(dt)
	player.position.x = player.position.x + (dt * 5)
	return player.position.x
end


function jump()
	if player.in_air == false then--not in mid air
		--player.force_up =+ 10
	end
end

function update(dt)
	if player.in_air == true then
		--player.force_up =-3
	end
end


function quit()
end
]]
return M
