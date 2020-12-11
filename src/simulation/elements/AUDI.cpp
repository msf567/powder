#include "simulation/ElementCommon.h"
#include "audio.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_AUDI()
{
    Identifier = "DEFAULT_PT_AUDI";
    Name = "AUDI";

    Colour = PIXPACK(0xFFFFFF);
    MenuVisible = 1;
    MenuSection = SC_SPECIAL;
    Enabled = 1;
    Advection = 0.0f;
    AirDrag = 0.00f * CFDS;
    AirLoss = 0.90f;
    Loss = 0.00f;
    Collision = 0.0f;
    Gravity = 0.0f;
    Diffusion = 0.00f;
    HotAir = 0.000f * CFDS;
    Falldown = 0;
    Flammable = 0;
    Explosive = 0;
    Meltable = 0;
    Hardness = 0;		
    Weight = 100;
    HeatConduct = 251;
    Description = "Play an audio clip";
    Properties = TYPE_SOLID | PROP_LIFE_DEC;

    LowPressure = IPL;
    LowPressureTransition = NT;
    HighPressure = IPH;
    HighPressureTransition = NT;
    LowTemperature = ITL;
    LowTemperatureTransition = NT;
    HighTemperature = ITH;
    HighTemperatureTransition = NT;

    Update = &update;
    Graphics = &graphics;
}

//#TPT-Directive ElementHeader Element_HETR static int update(UPDATE_FUNC_ARGS)
static int update(UPDATE_FUNC_ARGS)
{
    int targetParticle, searchX, searchY;
    for (searchX = -1; searchX <= 1; searchX++)
        for (searchY = -1; searchY <= 1; searchY++)
            if (BOUNDS_CHECK && (searchX || searchY))
            {
                targetParticle = pmap[y + searchY][x + searchX];
                if (!targetParticle || TYP(targetParticle) == PT_LOAD)
                    targetParticle = sim->photons[y + searchY][x + searchX];
                if (!targetParticle)
                    continue;

                if (TYP(targetParticle) == PT_SPRK)
                {
                    //sim->LoadNextSave();
                    playSound("A:\\The-Powder-Toy\\build\\door2.wav", SDL_MIX_MAXVOLUME / 2);
                }

            }
    return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
    return 1;
}

