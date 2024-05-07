#include "game/audio.h"
#include "bass.h"
#include "glfw/glfw3.h"
#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include "glfw/glfw3native.h"
#include <vector>
#include <filesystem>
bool AudioManager::Init() {
	return BASS_Init(-1, 44100, 0, glfwGetWin32Window(glfwGetCurrentContext()), NULL);
};
void AudioManager::loadStreams(std::vector<std::pair<std::string,int>>& paths) {
	for (auto& path : paths) {
		AudioManager::loadedStreams.push_back({ BASS_StreamCreateFile(false, path.first.c_str(), 0, 0, 0),path.second });
	}
}
void AudioManager::unloadStreams() {
	for (auto& stream : AudioManager::loadedStreams) {
		AudioManager::StopPlayback(stream.first);
		BASS_StreamFree(stream.first);
	}
	AudioManager::loadedStreams.clear();
}
double AudioManager::GetMusicTimePlayed(unsigned int handle) {
	return BASS_ChannelBytes2Seconds(handle, BASS_StreamGetFilePosition(handle, BASS_FILEPOS_CURRENT));
}
double AudioManager::GetMusicTimeLength(unsigned int handle) {
	return BASS_ChannelBytes2Seconds(handle, BASS_StreamGetFilePosition(handle, BASS_FILEPOS_SIZE));
}

void AudioManager::SetAudioStreamVolume(unsigned int handle, float volume) {
	BASS_ChannelSetAttribute(handle, BASS_ATTRIB_VOL, volume);
}

void AudioManager::UpdateMusicStream(unsigned int handle) {
	BASS_ChannelUpdate(handle, 0);
}

void AudioManager::BeginPlayback(unsigned int handle) {
	BASS_ChannelStart(handle);
}
void AudioManager::StopPlayback(unsigned int handle) {
	BASS_ChannelStop(handle);
}