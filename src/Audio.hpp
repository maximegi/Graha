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
		void play(int type);
		void stop();
		void pause();
	private:
		void loadAudio(const std::string &path);
		ALuint mBuffer, mSource;
		ALint mStatus;
		std::string mPath;

};

bool initOpenAL();
void shutdownOpenAL();
