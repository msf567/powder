
local lol = elements.allocate("PURE" , "ENERGY")
elements.element(lol, elements.element(elements.DEFAULT_PT_SING))
elements.property(lol, "Name" , "ERGY")
elements.property(lol, "Description" , "Pure Energy, Extremely Powerful")
function ENERGYUpdate(i,x,y,s,n)
    if math.random(1,3) == 1 then
        tpt.create(x + math.random(-1,1), y + math.random(-1,1), 'phot')
    elseif math.random(1,3) == 2 then
        tpt.create(x + math.random(-1,1), y + math.random(-1,1), 'neut')
    elseif math.random(1,3) == 3 then
        tpt.create(x + math.random(-1,1), y + math.random(-1,1), 'elec')
    end
    tpt.set_property("temp", math.huge, x, y)
    sim.pressure(x/4,y/4,math.huge)
end
tpt.element_func(ENERGYUpdate,lol)