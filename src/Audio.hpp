#pragma once


#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <sndfile.h>
#include <string>
#include <vector>


class Audio
{
	public:
		Audio(std::string &path)
		{
			loadAudio(path);
		}
		void deleteBuffer();
		ALuint AudioFromFile(const char *path, const std::string &directory);
		ALuint mSource;
	private:
		void loadAudio(const std::string &path);
		ALuint mBuffer;

};

bool InitOpenAL();
void ShutdownOpenAL();
