--[[--
  ||  TPT Audio Library
  ||  v0.8
  ||
  ||  Finally, SOUND! Possible since 92.2 (build 333), which uses LuaJIT.
  ||  See the TPT forums for more info.
  ||
  ||  -- Lyrthras#7199 / Draco712 --
--]]--

--[[--
  ||  TODO:
  ||
  ||  * Thinking of separating it into another instance so it doesn't
  ||    use TPT's CPU allocation. (if SDL even uses that much CPU).
  ||  * OGG/MP4/etc. support.
  ||  * load_music()?
  ||  * OOP structures?
--]]--


-- ohmygodwhatisthistableisthisaudiointptforrealsthistime?
audio = {}


-- Config + Defaults --
local sample_rate = SAMPLE_RATE or 22050    -- Higher = better but more CPU
local channels    = N_CHANNELS  or 2        -- 2 = stereo, 1 = mono
local sample_size = SAMPLE_SIZE or 4096     -- Lower = more effect calls.
local mixer_chans = MIXER_CHANS or 8       -- How many channels can play at once.

local DEBUG       = DEBUG or true           -- Debug just prints additional info.


-- Debug --
local function dprint(...) 
  if DEBUG then print(...) end
end


-- The main functions --
ffi = require('ffi')  -- global
sdl, mix = nil, nil   -- global too?

-- Initializes the SDL libraries.
-- Returns true if all is well.
function audio.init()
  assert(ffi,"FFI is not available. Are you sure you are running the right TPT version?")
  
  local cdef = io.open('audio/SDL-FFI.h', 'r'):read('*a')
  assert(cdef,"SDL-FFI.h file not found. Make sure you correctly installed this library.")
  ffi.cdef(cdef)
  
  xpcall(function()
      if jit.os == "Windows" then
        sdl = ffi.load('SDL2')
        mix = ffi.load('SDL2_mixer')
      elseif jit.os == "Linux" or jit.os == "POSIX" or jit.os == "BSD" then
        if not pcall(function()
          sdl = ffi.load("SDL2-2.0.so.0")
          mix = ffi.load("SDL2_mixer-2.0.so.0") end) then
          if not pcall(function()
            sdl = ffi.load("SDL2-2.0.so")
            mix = ffi.load("SDL2_mixer-2.0.so") end) then
            if not pcall(function()
              sdl = ffi.load("SDL2-2.0")
              mix = ffi.load("SDL2_mixer-2.0") end) then
              if not pcall(function()     --- such a mess...
                sdl = ffi.load("SDL2")
                mix = ffi.load("SDL2_mixer") end) then
                  error("Failed to find Linux libraries.")
              end
            end
          end
        end
      elseif jit.os == "OSX" then
        if not pcall(function()
          sdl = ffi.load("SDL2.framework")
          mix = ffi.load("SDL2_mixer.framework") end) then
          if not pcall(function()
            sdl = ffi.load("SDL2")
            mix = ffi.load("SDL2_mixer") end) then
              error("Failed to find OS X libraries.")
          end
        end
      else
        error("Unsupported OS.")
      end
    end,
    function(e) error("Error: audio.lua failed to load SDL libraries.\n"..e) end)
  assert(sdl.SDL_Init(sdl.SDL_INIT_AUDIO) == 0, "SDL failed to initialize.")
  assert(mix.Mix_Init(0) == 0, "SDL_mixer failed to start.")
  assert(mix.Mix_OpenAudio(sample_rate, mix.MIX_DEFAULT_FORMAT, channels, sample_size ) == 0,
      "Failed to init mixer API.")
  
  dprint("Audio library loaded, allocating "..
      mix.Mix_AllocateChannels(mixer_chans)..
      " channels.")
  
  return true
end

-- Cleaning up, when you're done with everything. Maybe needed?
function audio.quit()
  if (not mix) or (not sdl) then return end
  mix.Mix_CloseAudio()
  mix.Mix_Quit()
  sdl.SDL_Quit()
end
  
-- Loads a WAV file.
--- filename: Path string that points to the file. Can be absolute/relative.
-- Returns an Audio object on success.
function audio.load(filename)
  local audio = mix.Mix_LoadWAV_RW(sdl.SDL_RWFromFile(filename, "rb"), 1)
  return audio
end

-- Plays audio.
--- audio: the Audio object.
--- channel: What channel to play it on, default(-1) for next available ch.
--- time: Time in ms when to stop playing. Default(-1) plays until file end.
--- loops: Times to repeat playing the audio, def(0). -1 plays forever
-- Returns the channel the audio is played on.
function audio.play(audio, channel, time, loops)
  return mix.Mix_PlayChannelTimed( channel or -1, audio, loops or 0 , time or -1)
end

-- Plays audio, but with fading in.
--- audio, channel, time, loops: [same as above function]
--- fade_ms: How long (in ms) shall the fade-in effect last.
-- Returns the channel the audio is played on.
function audio.playFadeIn(audio, channel, time, loops, fade_ms)
  return mix.Mix_FadeInChannelTimed( channel or -1, audio, loops or 0, fade_ms or 300, time or -1)
end

-- Sets channel fade-out effect, starting right after call.
--- channel: Channel(s) to apply fade-out, -1 for all channels
--- fade_ms: How long shall the fade-out effect last.
function audio.setFadeOut(channel, fade_ms)
  mix.Mix_FadeOutChannel(channel, fade_ms)
end

-- Set the separate left and right volume of a channel.
--- channel: You know the drill.
--- left, right: Volume for each of the respective sides, from 0 to 255
-- Returns 0 on error.
function audio.setPanning(channel, left, right)
  return mix.Mix_SetPanning(channel, left, right)
end

-- Sets the volume for the channel.
--- channel: Channel.
--- volume: A value within 0 to 128. Defaults to 128 if no volume is set.
-- Returns current channel volume after set. If channel=-1, avg. volume.
function audio.setVolume(channel, volume)
  return mix.Mix_Volume(channel, volume or 128)  -- 128 is the max volume.
end

-- * TODO: Angular sound position to simulate 3D audio? But we already have panning...

-- Pause all channels.
function audio.pauseAll()  mix.Mix_Pause(-1)  end

-- Resumes all channels from a paused state.
function audio.resumeAll() mix.Mix_Resume(-1) end

-- Returns true if the specified channel is paused.
function audio.isPaused(channel) return mix.Mix_Paused(channel)==1 end


return audio
