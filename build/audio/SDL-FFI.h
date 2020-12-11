/*/
 *  SDL-FFI.h
 *  v0.8
 *
 *  .h file to use for FFI's cdef
 *  Gathered from SDL2/SDL_mixer header files. (SDL 2.0.7, SDL_mixer 2.0.2)
 *
 *  -- Lyrthras#7199 / Draco712 --
/*/


/*/
 *  Version
/*/
typedef struct SDL_version { uint8_t major; uint8_t minor; uint8_t patch; } SDL_version;
void  SDL_GetVersion(SDL_version *ver);
const char *SDL_GetRevision(void);
int   SDL_GetRevisionNumber(void);


/*/
 *  SDL Init
/*/
enum {
  SDL_INIT_TIMER  = 0x00000001,
  SDL_INIT_AUDIO  = 0x00000010,
  SDL_INIT_VIDEO  = 0x00000020,
  SDL_INIT_JOYSTICK = 0x00000200,
  SDL_INIT_HAPTIC   = 0x00001000,
  SDL_INIT_GAMECONTROLLER = 0x00002000,
  SDL_INIT_EVENTS      = 0x00004000,
  SDL_INIT_NOPARACHUTE = 0x00100000,
  SDL_INIT_EVERYTHING  = ( SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER)
};

int  SDL_Init(uint32_t flags);
int  SDL_InitSubSystem(uint32_t flags);
void SDL_QuitSubSystem(uint32_t flags);

uint32_t SDL_WasInit(uint32_t flags);
void SDL_Quit(void);


/*/
 *  R/W Ops
/*/
typedef struct _FILE FILE;
typedef enum { SDL_FALSE = 0, SDL_TRUE = 1 } SDL_bool;
typedef struct SDL_RWops {
  int64_t (*size)  (struct SDL_RWops *context);
  int64_t (*seek)  (struct SDL_RWops *context, int64_t offset, int whence);
  size_t  (*read)  (struct SDL_RWops *context, void *ptr, size_t size, size_t maxnum);
  size_t  (*write) (struct SDL_RWops *context, const void *ptr, size_t size, size_t num);
  int     (*close) (struct SDL_RWops *context);
  uint32_t  type;
  union {
    struct {
      SDL_bool autoclose;
      FILE *fp;
    } stdio;
    struct {
      uint8_t *base;
      uint8_t *here;
      uint8_t *stop;
    } mem;
    struct {
      void *data1;
      void *data2;
    } unknown;
  } hidden;
} SDL_RWops;
SDL_RWops *SDL_RWFromFile(const char *file, const char *mode);
SDL_RWops *SDL_RWFromFP(FILE *fp, SDL_bool autoclose);
SDL_RWops *SDL_RWFromMem(void *mem, int size);
SDL_RWops *SDL_RWFromConstMem(const void *mem, int size);
SDL_RWops *SDL_AllocRW(void);


/*/
 *  Audio
/*/

// Audio flags
enum {
  SDL_AUDIO_MASK_BITSIZE  = (0xFF),
  SDL_AUDIO_MASK_DATATYPE = (1<<8),
  SDL_AUDIO_MASK_ENDIAN   = (1<<12),
  SDL_AUDIO_MASK_SIGNED   = (1<<15)
};

// Audio fmt flags
enum {
  SDL_AUDIO_U8     = 0x0008,
  SDL_AUDIO_S8     = 0x8008,
  SDL_AUDIO_U16LSB = 0x0010,
  SDL_AUDIO_S16LSB = 0x8010,
  SDL_AUDIO_U16MSB = 0x1010,
  SDL_AUDIO_S16MSB = 0x9010,
  SDL_AUDIO_U16    = SDL_AUDIO_U16LSB,
  SDL_AUDIO_S16    = SDL_AUDIO_S16LSB,
  // int32
  SDL_AUDIO_S32LSB = 0x8020,
  SDL_AUDIO_S32MSB = 0x9020,
  SDL_AUDIO_S32    = SDL_AUDIO_S32LSB,
  //float32
  SDL_AUDIO_F32LSB = 0x8120,
  SDL_AUDIO_F32MSB = 0x9120,
  SDL_AUDIO_F32    = SDL_AUDIO_F32LSB
};
// compat
enum {
  AUDIO_U8     = SDL_AUDIO_U8    ,
  AUDIO_S8     = SDL_AUDIO_S8    ,
  AUDIO_U16LSB = SDL_AUDIO_U16LSB,
  AUDIO_S16LSB = SDL_AUDIO_S16LSB,
  AUDIO_U16MSB = SDL_AUDIO_U16MSB,
  AUDIO_S16MSB = SDL_AUDIO_S16MSB,
  AUDIO_U16    = SDL_AUDIO_U16   ,
  AUDIO_S16    = SDL_AUDIO_S16   ,
  // int32
  AUDIO_S32LSB = SDL_AUDIO_S32LSB,
  AUDIO_S32MSB = SDL_AUDIO_S32MSB,
  AUDIO_S32    = SDL_AUDIO_S32   ,   
  //float32
  AUDIO_F32LSB = SDL_AUDIO_F32LSB,
  AUDIO_F32MSB = SDL_AUDIO_F32MSB,
  AUDIO_F32    = SDL_AUDIO_F32   
};

// Allow change flags
enum {
  SDL_AUDIO_ALLOW_FREQUENCY_CHANGE    = 0x00000001,
  SDL_AUDIO_ALLOW_FORMAT_CHANGE       = 0x00000002,
  SDL_AUDIO_ALLOW_CHANNELS_CHANGE     = 0x00000004,
  SDL_AUDIO_ALLOW_ANY_CHANGE          = 0x00000007
};

typedef uint16_t SDL_AudioFormat;
typedef void (*SDL_AudioCallback) (void *userdata, uint8_t *stream, int len);
typedef struct SDL_AudioSpec {
  int freq;
  SDL_AudioFormat format;
  uint8_t  channels;
  uint8_t  silence;
  uint16_t samples;
  uint16_t padding;
  uint32_t size;
  SDL_AudioCallback callback;
  void *userdata;
} SDL_AudioSpec;

typedef uint32_t SDL_AudioDeviceID;
int   SDL_AudioInit(const char *driver_name);
void  SDL_AudioQuit(void);
int   SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained);
int   SDL_GetNumAudioDrivers(void);
int   SDL_GetNumAudioDevices(int iscapture);
char * SDL_AudioDriverName(char *namebuf, int maxlen); // compat
const char * SDL_GetAudioDriver(int index);
const char * SDL_GetCurrentAudioDriver(void);
const char * SDL_GetAudioDeviceName(int index, int iscapture);

SDL_AudioDeviceID SDL_OpenAudioDevice(
  const char *device,
  int   iscapture,
  const SDL_AudioSpec *desired,
  SDL_AudioSpec *obtained,
  int   allowed_changes
);

typedef enum {
  SDL_AUDIO_STOPPED = 0,
  SDL_AUDIO_PLAYING,
  SDL_AUDIO_PAUSED
} SDL_AudioStatus;

SDL_AudioStatus SDL_GetAudioStatus(void);
SDL_AudioStatus SDL_GetAudioDeviceStatus(SDL_AudioDeviceID dev);
void SDL_PauseAudio(int pause_on);
void SDL_PauseAudioDevice(SDL_AudioDeviceID dev, int pause_on);

void SDL_Delay(uint32_t ms);

// usage: SDL_LoadWAV_RW(SDL_RWFromFile("sample.wav", "rb"), 1, ...);
SDL_AudioSpec *SDL_LoadWAV_RW(
  SDL_RWops *src,
  int freesrc,
  SDL_AudioSpec *spec,
  uint8_t **audio_buf,
  uint32_t *audio_len
);
void SDL_FreeWAV(uint8_t *audio_buf);
  
enum {SDL_MIX_MAXVOLUME = 128};
void SDL_MixAudio(uint8_t *dst, const uint8_t *src, uint32_t len, int volume);
void SDL_MixAudioFormat(uint8_t *dst, const uint8_t *src, SDL_AudioFormat format, uint32_t len, int volume);

int  SDL_QueueAudio(SDL_AudioDeviceID dev, const void *data, uint32_t len);
uint32_t SDL_DequeueAudio(SDL_AudioDeviceID dev, void *data, uint32_t len);
uint32_t SDL_GetQueuedAudioSize(SDL_AudioDeviceID dev);
void SDL_ClearQueuedAudio(SDL_AudioDeviceID dev);

void SDL_LockAudio(void);
void SDL_LockAudioDevice(SDL_AudioDeviceID dev);
void SDL_UnlockAudio(void);
void SDL_UnlockAudioDevice(SDL_AudioDeviceID dev);
void SDL_CloseAudio(void);
void SDL_CloseAudioDevice(SDL_AudioDeviceID dev);
  

/*/
 *  SDL_Mixer
/*/

enum {
  SDL_MIXER_MAJOR_VERSION = 2,
  SDL_MIXER_MINOR_VERSION = 0,
  SDL_MIXER_PATCHLEVEL    = 2
};
//more compat 
enum {
  MIX_MAJOR_VERSION  = SDL_MIXER_MAJOR_VERSION,
  MIX_MINOR_VERSION  = SDL_MIXER_MINOR_VERSION,
  MIX_PATCHLEVEL     = SDL_MIXER_PATCHLEVEL
};

const SDL_version * Mix_Linked_Version(void);

typedef enum
{
  MIX_INIT_FLAC   = 0x00000001,
  MIX_INIT_MOD    = 0x00000002,
  MIX_INIT_MP3    = 0x00000008,
  MIX_INIT_OGG    = 0x00000010,
  MIX_INIT_MID    = 0x00000020
} MIX_InitFlags;

int  Mix_Init(int flags);
void Mix_Quit(void);

// defaults
enum {
  MIX_CHANNELS          = 8,
  MIX_DEFAULT_FREQUENCY = 22050,
  MIX_DEFAULT_FORMAT    = AUDIO_S16LSB, // Using little endian here
  MIX_DEFAULT_CHANNELS  = 2,
  MIX_MAX_VOLUME        = SDL_MIX_MAXVOLUME
};

typedef struct Mix_Chunk {
    int allocated;
    uint8_t *abuf;
    uint32_t alen;
    uint8_t volume;       /* Per-sample volume, 0-128 */
} Mix_Chunk;

typedef enum {
    MIX_NO_FADING,
    MIX_FADING_OUT,
    MIX_FADING_IN
} Mix_Fading;

typedef enum {
    MUS_NONE,
    MUS_CMD,
    MUS_WAV,
    MUS_MOD,
    MUS_MID,
    MUS_OGG,
    MUS_MP3,
    MUS_MP3_MAD_UNUSED,
    MUS_FLAC,
    MUS_MODPLUG_UNUSED
} Mix_MusicType;
typedef struct _Mix_Music Mix_Music;

int  Mix_OpenAudio(int frequency, uint16_t format, int channels, int chunksize);
int  Mix_OpenAudioDevice(int frequency, uint16_t format, int channels, int chunksize, const char* device, int allowed_changes);

int Mix_AllocateChannels(int numchans);
int Mix_QuerySpec(int *frequency, uint16_t *format, int *channels);
// Mix_Chunk * Mix_LoadWAV(const char *file);
//// use Mix_LoadWAV_RW(SDL_RWFromFile("file.wav", "rb"), 1) instead
Mix_Chunk * Mix_LoadWAV_RW(SDL_RWops *src, int freesrc);
Mix_Chunk * Mix_QuickLoad_WAV(uint8_t *mem);
Mix_Chunk * Mix_QuickLoad_RAW(uint8_t *mem, uint32_t len);
Mix_Music * Mix_LoadMUS_RW(SDL_RWops *src, int freesrc);
Mix_Music * Mix_LoadMUS(const char *file);
Mix_Music * Mix_LoadMUSType_RW(SDL_RWops *src, Mix_MusicType type, int freesrc);

void Mix_FreeChunk(Mix_Chunk *chunk);
void Mix_FreeMusic(Mix_Music *music);

int  Mix_GetNumChunkDecoders(void);
int  Mix_GetNumMusicDecoders(void);
const char * Mix_GetChunkDecoder(int index);
const char * Mix_GetMusicDecoder(int index);
SDL_bool Mix_HasChunkDecoder(const char *name);
SDL_bool Mix_HasMusicDecoder(const char *name);

Mix_MusicType Mix_GetMusicType(const Mix_Music *music);

// some func templates
typedef void (*mix_func) (void *udata, uint8_t *stream, int len);
typedef void (*music_finished)   (void);
typedef void (*channel_finished) (int channel);
void Mix_SetPostMix(mix_func f, void *arg);
void Mix_HookMusic (mix_func f, void *arg);
void Mix_HookMusicFinished(music_finished f);
void * Mix_GetMusicHookData(void);
void Mix_ChannelFinished(channel_finished f);
enum {MIX_CHANNEL_POST = -2};

typedef void (*Mix_EffectFunc_t) (int chan, void *stream, int len, void *udata);
typedef void (*Mix_EffectDone_t) (int chan, void *udata);
int  Mix_RegisterEffect(int chan, Mix_EffectFunc_t f, Mix_EffectDone_t d, void *arg);
int  Mix_UnregisterEffect(int channel, Mix_EffectFunc_t f);
int  Mix_UnregisterAllEffects(int channel);

char MIX_EFFECTSMAXSPEED;  // = "MIX_EFFECTSMAXSPEED"}
int  Mix_SetPanning(int channel, uint8_t left, uint8_t right);
int  Mix_SetPosition(int channel, int16_t angle, uint8_t distance);
int  Mix_SetDistance(int channel, uint8_t distance);
//   int  Mix_SetReverb(int channel, uint8_t echo);  // Unimplemented
int  Mix_SetReverseStereo(int channel, int flip);

int  Mix_ReserveChannels(int num);
int  Mix_GroupChannel(int which, int tag);
int  Mix_GroupChannels(int from, int to, int tag);
int  Mix_GroupAvailable(int tag);
int  Mix_GroupCount(int tag);
int  Mix_GroupOldest(int tag);
int  Mix_GroupNewer(int tag);

// int  Mix_PlayChannel(int channel, Mix_Chunk *chunk, int loops);
//// Use func below instead, ticks = -1
int  Mix_PlayChannelTimed(int channel, Mix_Chunk *chunk, int loops, int ticks);
int  Mix_PlayMusic(Mix_Music *music, int loops);
int  Mix_FadeInMusic(Mix_Music *music, int loops, int ms);
int  Mix_FadeInMusicPos(Mix_Music *music, int loops, int ms, double position);
// int  Mix_FadeInChannel(int channel, Mix_Chunk *chunk, int loops, int ms);
//// Use func below instead, ticks = -1
int  Mix_FadeInChannelTimed(int channel, Mix_Chunk *chunk, int loops, int ms, int ticks);

int  Mix_Volume(int channel, int volume);
int  Mix_VolumeChunk(Mix_Chunk *chunk, int volume);
int  Mix_VolumeMusic(int volume);

int  Mix_HaltChannel(int channel);
int  Mix_HaltGroup(int tag);
int  Mix_HaltMusic(void);

int  Mix_ExpireChannel(int channel, int ticks);
int  Mix_FadeOutChannel(int which, int ms);
int  Mix_FadeOutGroup(int tag, int ms);
int  Mix_FadeOutMusic(int ms);
Mix_Fading Mix_FadingMusic(void);
Mix_Fading Mix_FadingChannel(int which);

void Mix_Pause(int channel);
void Mix_Resume(int channel);
int  Mix_Paused(int channel);
void Mix_PauseMusic(void);
void Mix_ResumeMusic(void);
void Mix_RewindMusic(void);
int  Mix_PausedMusic(void);

int  Mix_SetMusicPosition(double position);
int  Mix_Playing(int channel);
int  Mix_PlayingMusic(void);
int  Mix_SetMusicCMD(const char *command);
int  Mix_SetSynchroValue(int value);
int  Mix_GetSynchroValue(void);
int  Mix_SetSoundFonts(const char *paths);
const char* Mix_GetSoundFonts(void);
int  Mix_EachSoundFont(int ( *function)(const char*, void*), void *data);

Mix_Chunk * Mix_GetChunk(int channel);

void Mix_CloseAudio(void);

