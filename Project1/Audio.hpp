#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "GameVariables.hpp"

// --- Audio Controller ---
void StopMusic() {
	mciSendString("stop all", NULL, 0, NULL);
	mciSendString("close all", NULL, 0, NULL);
}

void PlayBackgroundMusic() {
	StopMusic();
	if (musicOn) {
		mciSendString("open \"Audios/background.mp3\" type mpegvideo alias bgm", NULL, 0, NULL);
		mciSendString("play bgm repeat", NULL, 0, NULL);
	}
}

void PlayGameOverMusic() {
	StopMusic();
	if (musicOn) {
		mciSendString("open \"Audios/gameover.mp3\" type mpegvideo alias gom", NULL, 0, NULL);
		mciSendString("play gom", NULL, 0, NULL);
	}
}

#endif // AUDIO_HPP
