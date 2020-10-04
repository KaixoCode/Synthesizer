//#include "Audio.hpp"
//
//void FillBuffer(Buffer& buffer, Channel& chain)
//{
//	for (int i = 0; i < CHANNELS * BUFFER_SIZE;) 
//	{ 
//		Stereo w = chain();
//		buffer[i++] = w.left;
//		buffer[i++] = w.right;
//	}
//}
//
//void FillBuffer(Buffer& buffer, MonoChannel& chain)
//{
//	for (int i = 0; i < CHANNELS * BUFFER_SIZE;) 
//	{ 
//		Sample w = chain();
//		buffer[i++] = w; // Left channel 
//		buffer[i++] = w; // Right channel
//	}
//}
//
//#ifdef __linux__
//#include <alsa/asoundlib.h>
//#include <thread>
//
//namespace Audio 
//{
//	void Handle();
//
//	const char* device = "default";
//	Buffer buffer;
//	std::array<float, (CHANNELS* BUFFER_SIZE) / OVERSAMPLING> downsample;
//	std::function<void(Buffer&)> Callback;
//	std::thread handleThread;
//	bool active = true;
//	int err;
//	snd_pcm_t* handle;
//	snd_pcm_sframes_t frames;
//
//	int Start() 
//	{
//		if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC)) < 0) 
//		{
//			printf("Playback open error: %s\n", snd_strerror(err));
//			exit(EXIT_FAILURE);
//		}
//
//		if ((err = snd_pcm_set_params(handle,
//			SND_PCM_FORMAT_FLOAT_LE,        // Float values (-1, 1)
//			SND_PCM_ACCESS_RW_INTERLEAVED,  // Interleaved
//			CHANNELS,                       // Amount of channels
//			SAMPLE_RATE/OVERSAMPLING,       // Sample rate
//			1,                              // Soft resampling
//			50000)) < 0)                    // Latency in ns
//		{
//			printf("Playback open error: %s\n", snd_strerror(err));
//			exit(EXIT_FAILURE);
//		}
//
//		handleThread = std::thread(Handle);
//		return 0;
//	}
//
//	void Downsample(std::array<float, CHANNELS* BUFFER_SIZE / OVERSAMPLING>& d, Buffer& b)
//	{
//		int index = 0;
//		for (int i = 0; i < CHANNELS * BUFFER_SIZE / OVERSAMPLING; i++)
//		{
//			float avg = 0;
//			for (int j = 0; j < OVERSAMPLING; j++)
//			{
//				avg += b[index];
//				index++;
//			}
//			avg /= OVERSAMPLING;
//			d[i] = avg;
//		}
//	}
//
//	void Handle() 
//	{
//		while (active)
//		{
//			Callback(buffer);
//			Downsample(downsample, buffer);
//			frames = snd_pcm_writei(handle, &downsample[0], BUFFER_SIZE/OVERSAMPLING);
//			if (frames < 0) frames = snd_pcm_recover(handle, frames, 0);
//		}
//	}
//
//	void Clean() 
//	{
//		active = false;
//		handleThread.join();
//		err = snd_pcm_drain(handle);
//		if (err < 0)
//			printf("snd_pcm_drain failed: %s\n", snd_strerror(err));
//		snd_pcm_close(handle);
//	}
//
//	void SetCallback(BufferCallback func) 
//	{
//		Callback = func;
//	}
//}
//
//#endif
//#ifdef _WIN32
//#include <portaudio.h>
//#include <complex>
//#include <valarray>
//
//namespace Audio 
//{
//	static int playCallback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);
//	static Buffer buffer;
//	std::function<void(Buffer&)> Callback;
//	PaError err;
//	PaStream* stream;
//
//	int Start() 
//	{
//		err = Pa_Initialize();
//		if (err != paNoError) return -1;
//
//		PaStreamParameters outputParameters;
//		outputParameters.device = Pa_GetDefaultOutputDevice();
//		if (outputParameters.device == paNoDevice)
//			return -1;
//		outputParameters.channelCount = CHANNELS;  // stereo output 
//		outputParameters.sampleFormat = paFloat32; // 32 bit floating point output 
//		outputParameters.suggestedLatency = 0.050; // Force low latency
//		outputParameters.hostApiSpecificStreamInfo = NULL;
//		
//		err = Pa_OpenStream(&stream, NULL, &outputParameters, (double) SAMPLE_RATE, BUFFER_SIZE, paClipOff, playCallback, 0);
//		if (err != paNoError) return -1;
//
//		err = Pa_StartStream(stream);
//		if (err != paNoError) return -1;
//		return 0;
//	}
//
//	void Clean() 
//	{
//		err = Pa_StopStream(stream);
//	}
//
//	static int playCallback(const void* inputBuffer, void* outputBuffer,
//		unsigned long framesPerBuffer,
//		const PaStreamCallbackTimeInfo* timeInfo,
//		PaStreamCallbackFlags statusFlags,
//		void* userData) 
//	{
//		float* out = (float*)outputBuffer;
//		Callback(buffer);
//		for (int i = 0; i < CHANNELS * framesPerBuffer; i++)
//			*out++ = buffer[i];
//		return 0;
//	}
//
//	void SetCallback(BufferCallback func) 
//	{
//		Callback = func;
//	}
//}
//#endif