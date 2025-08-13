#include <alsa/asoundlib.h>
#include <iostream>
#include <types.hpp>

const char *device = "default";

void alsaoutrun() {
    snd_pcm_t *handle;
    auto open_result = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0);

    snd_pcm_set_params(
        handle,
        SND_PCM_FORMAT_S16_LE,         // Signed 16-bit little-endian format
        SND_PCM_ACCESS_RW_INTERLEAVED, // Interleaved access
        2,                             // Two channels (stereo)
        44100,                         // 44.1 kHz sampling rate
        1,                             // Enable resampling
        500000);                       // 0.5 sec latency
    if (handle == nullptr) {
        std::cerr << "Error opening ALSA device: "
                  << snd_strerror(open_result)
                  << std::endl;
        return;
    }

    // Here you would typically write audio data to the ALSA device
    // For demonstration purposes, we will just close the handle
    snd_pcm_close(handle);
}
