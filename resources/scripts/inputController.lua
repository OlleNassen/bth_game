--få i fråm lua_load.cpp
--function tablelength(T)
 -- local count = 0
 -- print("wazoo")
--  for _ in pairs(T) do count = count + 1 end
 -- return 100
--end
--inputTable {}

function checkInput(a)
	--print("Wazooo")

	if(a == 0)then
		print("Up")
		end
	if(a == 1)then
		print("Left")
		end
	if(a == 2)then
		print("Down")
		end
	if(a == 3)then
		print("Right")
		end
	if(a == 4)then
		print("Jump")
		end


	return a

end
