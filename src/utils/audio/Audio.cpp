
#include "Audio.hpp"
#include <thread>



#ifdef __linux__
#include <alsa/asoundlib.h>
#include <thread>

namespace Audio {

	// Stubs
	void Handle();

	// Playback info
	const char* device = "default";            /* playback device */
	const unsigned int SAMPLE_RATE = 48000;
	const unsigned int BUFFER_SIZE = 512;
	const unsigned int CHANNELS = 1;

	// Buffers
	static float buffer[2][BUFFER_SIZE];

	// Callback method
	void (*Callback)(float*);

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
			SAMPLE_RATE,                    // Sample rate
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
	void Handle() {

		// Buffer id
		int bid = 0;
		while (busy) {

			// Swap the buffers
			bid = (bid + 1) % 2;

			// Call the callback method to request data
			if (Callback) Callback(buffer[(bid + 1) % 2]);

			// Send the buffer to ALSA
			auto b = &buffer[bid][0];
			frames = snd_pcm_writei(handle, b, BUFFER_SIZE);

			// Recover if it underran
			if (frames < 0) frames = snd_pcm_recover(handle, frames, 0);
		}
	}

	// Cleanup
	void Clean() {

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
	void SetCallback(void func(float*)) {
		Callback = func;
	}
}

#endif

#ifdef _WIN32
#include <portaudio.h>
#include <complex>
#include <valarray>

namespace Audio {

	// Playback info
	const unsigned int SAMPLE_RATE = 44100;
	const unsigned int BUFFER_SIZE = 512;
	const unsigned int CHANNELS = 1;

	//short-hand for complex<double> representing a+bi
	typedef std::complex<float> ComplexVal;

	//array of complex-valued samples
	typedef std::valarray<ComplexVal> SampleArray;

	// Phasedata for outgoing 2 channel audio stream
	typedef struct {
		float left_phase;
		float right_phase;
	} PaPhaseData;

	static int playCallback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);

	// Buffers
	static float buffer[BUFFER_SIZE];

	SampleArray samplesLeft, samplesRight;
	PaError err;
	PaStream* stream;

	// Callback method
	void (*Callback)(float*);

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
		outputParameters.channelCount = 1;       /* stereo output */
		outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
		outputParameters.suggestedLatency = 0.050; // Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;
		err = Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE, BUFFER_SIZE, paClipOff, playCallback, 0);
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
		if (Callback) Callback(buffer);

		for (i = 0; i < framesPerBuffer; i++) {
			*out++ = buffer[i];
		}
		return 0;
	}

	// Sets the callback
	void SetCallback(void func(float*)) {
		Callback = func;
	}
}
#endif
