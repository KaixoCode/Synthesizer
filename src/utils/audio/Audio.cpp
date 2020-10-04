
#include "Audio.hpp"

void FillBuffer(Buffer& buffer, Channel& chain)
{
	for (int i = 0; i < CHANNELS * BUFFER_SIZE;) 
	{ 
		Stereo w = chain();
		buffer[i++] = w.left;
		buffer[i++] = w.right;
	}
}

void FillBuffer(Buffer& buffer, MonoChannel& chain)
{
	for (int i = 0; i < CHANNELS * BUFFER_SIZE;) 
	{ 
		Sample w = chain();
		buffer[i++] = w; 
		buffer[i++] = w; 
	}
}

#ifdef __linux__
#include <alsa/asoundlib.h>
#include <thread>

namespace Audio 
{
	// Stubs
	void Handle();

	// Playback info
	const char* device = "default";            /* playback device */

	// Buffers
	static Buffer buffer;

	// Callback method
	std::function<void(Buffer&)> Callback;

	// Handle thread
	std::thread handleThread;

	// Init all ALSA stuff
	int err;
	snd_pcm_t* handle;
	snd_pcm_sframes_t frames;
	int Start() {

		// Open device
		if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC)) < 0) {
			printf("Playback open error: %s\n", snd_strerror(err));
			exit(EXIT_FAILURE);
		}

		// Set params
		if ((err = snd_pcm_set_params(handle,
			SND_PCM_FORMAT_FLOAT_LE,        // Float values (-1, 1)
			SND_PCM_ACCESS_RW_INTERLEAVED,  // Interleaved
			CHANNELS,                       // Amount of channels
			SAMPLE_RATE/OVERSAMPLING,       // Sample rate
			1,                              // Soft resampling
			50000)) < 0) {                  // Latency
			printf("Playback open error: %s\n", snd_strerror(err));
			exit(EXIT_FAILURE);
		}

		// Open the Handle thread
		handleThread = std::thread(Handle);
		return 0;
	}

	// Thread that handles the output of audio buffers

	bool busy = true;
	std::array<float, (CHANNELS* BUFFER_SIZE) / OVERSAMPLING> downsample;
	void Handle() 
	{

		// Buffer id
		int bid = 0;
		while (busy) {

			// Call the callback method to request data
			Callback(buffer);

			Downsample(downsample, buffer);

			// Send the buffer to ALSA
			auto b = &downsample[0];
			frames = snd_pcm_writei(handle, b, BUFFER_SIZE/OVERSAMPLING);

			// Recover if it underran
			if (frames < 0) frames = snd_pcm_recover(handle, frames, 0);
		}
	}

	// Cleanup
	void Clean() 
	{

		// Stop the handle thread
		busy = false;
		handleThread.join();

		// ALSA cleanage
		err = snd_pcm_drain(handle);
		if (err < 0)
			printf("snd_pcm_drain failed: %s\n", snd_strerror(err));
		snd_pcm_close(handle);
	}

	// Sets the callback
	void SetCallback(BufferCallback func) 
	{
		Callback = func;
	}

	void Downsample(std::array<float, CHANNELS * BUFFER_SIZE / OVERSAMPLING>& d, Buffer& b)
	{
		int index = 0;
		for (int i = 0; i < CHANNELS * BUFFER_SIZE / OVERSAMPLING; i++) 
		{
			float avg = 0;
			for (int j = 0; j < OVERSAMPLING; j++) 
			{
				avg += b[index];
				index++;
			}
			avg /= OVERSAMPLING;
			d[i] = avg;
		}
	}
}

#endif

#ifdef _WIN32
#include <portaudio.h>
#include <complex>
#include <valarray>


namespace Audio {

	static int playCallback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);

	// Buffers
	static Buffer buffer;

	PaError err;
	PaStream* stream;

	// Callback method
	std::function<void(Buffer&)> Callback;

	// Init portaudio
	int Start() {
		err = Pa_Initialize();
		if (err != paNoError) return -1;

		// Set input parameters
		PaStreamParameters outputParameters;
		outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
		if (outputParameters.device == paNoDevice) {
			return -1;
		}
		outputParameters.channelCount = CHANNELS;       /* stereo output */
		outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
		outputParameters.suggestedLatency = 0.050; // Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;
		err = Pa_OpenStream(&stream, NULL, &outputParameters, (double) SAMPLE_RATE, BUFFER_SIZE, paClipOff, playCallback, 0);
		if (err != paNoError) return -1;

		// Start
		err = Pa_StartStream(stream);
		if (err != paNoError) return -1;
		return 0;
	}

	// Cleanup
	void Clean() {
		err = Pa_StopStream(stream);
	}

	// Callback Portaudio outgoing audio
	static int playCallback(const void* inputBuffer, void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData) {

		// Put all the data in the output buffer
		float* out = (float*)outputBuffer;
		unsigned int i;

		// Call the callback method to request data
		Callback(buffer);

		for (i = 0; i < CHANNELS*framesPerBuffer; i++) {
			*out++ = buffer[i];
		}
		return 0;
	}

	// Sets the callback
	void SetCallback(BufferCallback func) {
		Callback = func;
	}
}
#endif
