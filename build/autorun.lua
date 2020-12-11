
audio = require("audio.audio")
audio.init()    
fwee = audio.load("bigClick.wav")  
fwee2 = audio.load("door2.wav")  

--audio.play(fwee)

sim.loadSaveFile("Saves\\start.cps")
tpt.set_pause(0)

--load audio particle
local lol = elements.allocate("AUDI" , "AUDIO")
elements.element(lol, elements.element(elements.DEFAULT_PT_PSCN))
elements.property(lol, "Name" , "AUPI")
elements.property(lol, "Description" , "Audio Block")
elements.property(lol, "Colour", 0xFFFF00)
elements.property(lol, "Properties", elem.PROP_LIFE_DEC)

function AUDIOUpdate(i,x,y,s,n)
    local life = sim.partProperty(i, "life")

    for ry=-1,1,1 do 
      for rx=-1,1,1 do
        if life == 0 and tpt.get_property("type", x+rx, y+ry) == 15 then
          local temp = sim.partProperty(i, "tmp")
            if temp == 16 then
              audio.play(fwee)
            else
              audio.play(fwee2)
            end
          sim.partProperty(i, "life", 10)
        end
      end
    end
end
tpt.element_func(AUDIOUpdate,lol)
