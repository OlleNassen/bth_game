require ("/Resources/Scripts/Setup/Input")

setResolution(true) -- fullscreen true / false
setFramerate(60)

bindKeyboard("moveUp", keyboard.S, keyboard.W)
bindKeyboard("moveRight", keyboard.D, keyboard.A)
bindKeyboard("jump", keyboard.Space, keyboard.None)
bindKeyboard("dash", keyboard.E, keyboard.None)
bindKeyboard("quit", keyboard.Escape, keyboard.None)
bindKeyboard("selectGame", keyboard.Return, keyboard.None)
bindKeyboard("selectEditor", keyboard.RShift, keyboard.None)
bindMouse("mouseLeft", mouse.Left, mouse.None)
bindMouse("mouseRight", mouse.Right, mouse.None)
bindKeyboard("next", keyboard.E, keyboard.Q)
bindKeyboard("laser", keyboard.F, mouse.None)

bindKeyboard("save", keyboard.K, keyboard.None)
bindKeyboard("load", keyboard.L, keyboard.None)

--newState("Resources/Scripts/LuaStates/Map/MenuState.lua")