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
		Audio(std::string &path, std::string type)
		{
			loadAudio(path, type);
		}
		void deleteBuffer();
		void play();
		void stop();
		void pause();
	private:
		void loadAudio(const std::string &path, std::string type);
		ALuint mBuffer, mSource;
		ALint mStatus;
		std::string mPath;

};

bool initOpenAL();
void shutdownOpenAL();
