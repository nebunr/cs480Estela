#include "audio.h"

void MyAudioCallback(void* userData, Uint8* stream, int streamLength);
Audio::Audio(){}
Audio::~Audio(){}

//For things that need to be ran once
bool Audio::Initialize()
{
    //return true; //Remove comment to disable
    
    if(SDL_Init(SDL_INIT_AUDIO) < 0){
        printf("SDL failed to initialize: %s\n", SDL_GetError());
		return false; 
    }
    
    //File path for audio track
    filePath = "../assets/audio/backgroundMusic.wav";

    if(!Start())
    {
        printf("Audio failed to initialize: %s\n", SDL_GetError());
		return false; 
    }

    return true;
}

bool Audio::Start()
{
    if(SDL_LoadWAV(filePath, &wavSpec, &wavStart, &wavLength) == NULL)
    {
        std::cerr << "Wrong audio file path." << std::endl;
        return false;
    }
    adata.position = wavStart;
    adata.length = wavLength;
    wavSpec.callback = MyAudioCallback;
    wavSpec.userdata = &adata;
    audioDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if(audioDevice == 0)
    {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //Starts track
    SDL_PauseAudioDevice(audioDevice, 0);

    //If track has 0 length, end it.
    if(adata.length == 0)
    {
        SDL_CloseAudioDevice(audioDevice);
        SDL_FreeWAV(wavStart);
    }
    return true;
}

void Audio::Update(bool pause)
{
    //Allows us to loop tracks
    if(adata.length == 0)
    {
        SDL_CloseAudioDevice(audioDevice);
        SDL_FreeWAV(wavStart);
        //printf("Looping Track\n"); //Feel free to remove this line
        Start();
    }
    //Whenever the game is paused, the audio pauses with it
    if(pause)
    {
        SDL_PauseAudioDevice(audioDevice, 1);
    }
    else
    {
        SDL_PauseAudioDevice(audioDevice, 0);
    }
}

void MyAudioCallback(void* userData, Uint8* stream, int streamLength)
{
    AudioData* audioTemp = (AudioData*)userData;

    if (audioTemp->length == 0)
    {
        return;
    }

    Uint32 length = (Uint32)streamLength;
    length = (length > audioTemp->length ? audioTemp->length : length);

    SDL_memcpy(stream, audioTemp->position, length);

    audioTemp->position += length;
    audioTemp->length -= length;
}
