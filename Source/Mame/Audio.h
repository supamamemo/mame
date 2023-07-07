#pragma once

#include <xaudio2.h>
#include <mmreg.h>

#include "../misc.h"

class Audio
{
private:
    WAVEFORMATEXTENSIBLE wfx         = { 0 };
    XAUDIO2_BUFFER       buffer      = { 0 };

    IXAudio2SourceVoice* sourceVoice = nullptr;

public:
    Audio(IXAudio2* xaudio2, const wchar_t* filename);
    virtual ~Audio();

    void Play(const bool isLoop = false);
    void Stop(bool playTails = true, size_t afterSamplesPlayed = 0);
    void Volume(float volume);
    bool Queuing();
};

