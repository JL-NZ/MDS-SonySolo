#pragma once
#include <stdlib.h>
#include <string>
#include <libsysmodule.h>
#include <scream/sce_scream.h>


class AudioManager
{
public:
	void Initialize();
	SceScreamSFXBlock2 *LoadAudioBank(std::string _filepath);
	SceScreamSoundParams &InitializeScreamParams();
	static void* example_malloc(int32_t bytes, int32_t use);
	static void example_free(void *memory);
	bool PlaySound(SceScreamSFXBlock2 *_screamBank, std::string _filepath, SceScreamSoundParams& _soundParams);

	// Singleton
public:
	static AudioManager* GetInstance();
	void Destroy();
private:
	static AudioManager* p_AudioManager;

	AudioManager();
	~AudioManager();
};

