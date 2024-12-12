#pragma once

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

#include <iostream>
#include <string>
#include <vector>


class MusicManager
{
public:
	MusicManager();
	~MusicManager();

	void loadMusic(std::string filePath);
	void playMusic();
	void update();

	int initSpectrum(int maxBars);
	void getFrequency(float* spectrum);

private:
	FMOD::System* fmodSystem;
	FMOD::Channel* channel;
	FMOD::Sound* sound;
	FMOD::DSP* dsp;

	std::vector<int> numSamplesPerBar;
	int windowSize;
	void init();
};