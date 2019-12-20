#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <SDL2/SDL.h>

struct AudioData{
    Uint8* position;
    Uint32 length;
};
class Audio{
    public:
        Audio();
        ~Audio();
        bool Initialize();
        bool Start();
        void Update(bool pause);
        friend void MyAudioCallback(void* userData, Uint8* stream, int streamLength);
    private:
        SDL_AudioSpec wavSpec;
        Uint8* wavStart;
        Uint32 wavLength;
        const char* filePath;
        SDL_AudioDeviceID audioDevice;
        AudioData adata;
};

#endif //AUDIO_H