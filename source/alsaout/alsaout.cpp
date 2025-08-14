#include <alsa/asoundlib.h>
#include <iostream>
#include <types.hpp>

const char *device = "default";

#include <vector>
#include <cmath>

std::vector<short> generate_sine_wave(int sample_rate, int duration_seconds, int frequency) {
    const int num_channels = 2;
    const int num_samples = sample_rate * duration_seconds * num_channels;
    std::vector<short> pcm_data(num_samples);
    const short max_amplitude = 32767;

    for (int i = 0; i < sample_rate * duration_seconds; ++i) {
        double time = static_cast<double>(i) / sample_rate;
        short sample_value = static_cast<short>(max_amplitude * sin(2.0 * M_PI * frequency * time));
        
        // Interleaved stereo samples
        pcm_data[i * 2] = sample_value;       // Left channel
        pcm_data[i * 2 + 1] = sample_value;   // Right channel
    }
    return pcm_data;
}


void alsaoutrun() {
    snd_pcm_t *handle;
    auto open_result = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0);
    if (open_result < 0) {
        std::cerr << "Error opening ALSA device: "
                  << snd_strerror(open_result)
                  << std::endl;
        return;
    }

    snd_pcm_set_params(
        handle,
        SND_PCM_FORMAT_S16_LE,         // Signed 16-bit little-endian format
        SND_PCM_ACCESS_RW_INTERLEAVED, // Interleaved access
        2,                             // Two channels (stereo)
        44100,                         // 44.1 kHz sampling rate
        1,                             // Enable resampling
        500000);                       // 0.5 sec latency
    if (handle == nullptr) {
        std::cerr << "Error opening ALSA device2: "
                  << snd_strerror(open_result)
                  << std::endl;
        return;
    }
    auto pcm_data = generate_sine_wave(44100, 1, 440); // Generate 5 seconds of 440 Hz sine wave
    auto res = snd_pcm_writei(handle, pcm_data.data(), 44100 * 1);
    if (res < 0) {
        std::cerr << "Error writing to ALSA device: "
                  << snd_strerror(res)
                  << std::endl;
    } else {
        std::cout << "Successfully wrote " << res << " frames to ALSA device." << std::endl;
    }
    snd_pcm_drain(handle); // Wait until all data is played
    // Here you would typically write audio data to the ALSA device
    // For demonstration purposes, we will just close the handle
    snd_pcm_close(handle);
}
