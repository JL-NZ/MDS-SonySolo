#include "AudioManager.h"

AudioManager* AudioManager::p_AudioManager = nullptr;

AudioManager::AudioManager()
{
}


AudioManager::~AudioManager()
{
}

void AudioManager::Initialize()
{
	sceSysmoduleLoadModule(SCE_SYSMODULE_NGS2);
	SceScreamPlatformInitEx2 screamInit;
	memset(&screamInit, 0, sizeof(SceScreamPlatformInitEx2));
	screamInit.size = sizeof(SceScreamPlatformInitEx2);

	// set default scream arugements
	sceScreamFillDefaultScreamPlatformInitArgsEx2(&screamInit);

	//specify alloc and dealloc static functions 
	screamInit.memAlloc = example_malloc; 
	screamInit.memFree = example_free; 

	// start sound system 
	sceScreamStartSoundSystemEx2(&screamInit);

	
}

SceScreamSFXBlock2 * AudioManager::LoadAudioBank(std::string _filepath)
{
	return sceScreamBankLoadEx(_filepath.c_str(), 0);
}

SceScreamSoundParams &AudioManager::InitializeScreamParams()
{
	// initialize scream params, stores sound specific parameter values
	SceScreamSoundParams m_soundParams;
	memset(&m_soundParams, 0, sizeof(SceScreamSoundParams));
	m_soundParams.size = sizeof(SceScreamSoundParams);
	m_soundParams.gain = 1.0f;
	m_soundParams.mask = SCE_SCREAM_SND_MASK_GAIN;

	return m_soundParams;
}

void * AudioManager::example_malloc(int32_t bytes, int32_t use)
{
	void* p;
	p = malloc(bytes);
	if (p) return p; 
	return NULL;
}

void AudioManager::example_free(void * memory)
{
	free(memory);
}

bool AudioManager::PlaySound(SceScreamSFXBlock2 * _screamBank, std::string _filepath, SceScreamSoundParams & _soundParams)
{
	if (!(_filepath.length() > 1))
	{
		return false;
	}
	if (_screamBank == nullptr)
	{
		return false;
	}

	sceScreamPlaySoundByNameEx(
		_screamBank, // bank name 
		"owowowow.wav", // filename 
		&_soundParams //parameters
	);
	
	return true;
}

AudioManager * AudioManager::GetInstance()
{
	if (!p_AudioManager)
		p_AudioManager = new AudioManager();
	return p_AudioManager;
}

void AudioManager::Destroy()
{
	sceScreamStopSoundSystem();
	sceSysmoduleUnloadModule(SCE_SYSMODULE_NGS2);

	if (p_AudioManager)
		delete p_AudioManager;
	p_AudioManager = nullptr;
}
