#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "GameVariables.hpp"

// --- Audio Controller For Background---
void StopRunSound();

void StopMusic() {
	mciSendString("stop bgm", NULL, 0, NULL);
	mciSendString("close bgm", NULL, 0, NULL);
	mciSendString("stop mm", NULL, 0, NULL);
	mciSendString("close mm", NULL, 0, NULL);
	mciSendString("stop gom", NULL, 0, NULL);
	mciSendString("close gom", NULL, 0, NULL);
	StopRunSound();
}

void PlayBackgroundMusic() {
	StopMusic();
	if (musicOn) {
		mciSendString("open \"Audios/background.mp3\" type mpegvideo alias bgm", NULL, 0, NULL);
		mciSendString("setaudio bgm volume to 600", NULL, 0, NULL);
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

void PlayJumpSound() {
	if (sfxOn) {
		mciSendString("close jsfx", NULL, 0, NULL);
		mciSendString("open \"Audios/jump.mp3\" type mpegvideo alias jsfx", NULL, 0, NULL);
		mciSendString("play jsfx", NULL, 0, NULL);
	}
}

void PlayBubblePopSound() {
	if (sfxOn) {
		mciSendString("close bsfx", NULL, 0, NULL);
		mciSendString("open \"Audios/bubble.mp3\" type mpegvideo alias bsfx", NULL, 0, NULL);
		mciSendString("play bsfx", NULL, 0, NULL);
	}
}

void PlayCollectSound() {
	if (sfxOn) {
		mciSendString("close csfx", NULL, 0, NULL);
		mciSendString("open \"Audios/collect.mp3\" type mpegvideo alias csfx", NULL, 0, NULL);
		mciSendString("play csfx", NULL, 0, NULL);
	}
}

void PlayMenuMusic() {
	StopMusic();
	if (musicOn) {
		mciSendString("open \"Audios/menu.mp3\" type mpegvideo alias mm", NULL, 0, NULL);
		mciSendString("play mm repeat", NULL, 0, NULL);
	}
}

void PlayRunSound() {
	if (sfxOn) {
		// Open if not already open
		mciSendString("open \"Audios/run.mp3\" type mpegvideo alias rsfx", NULL, 0, NULL);
		// Resume if paused, otherwise play repeat
		mciSendString("resume rsfx", NULL, 0, NULL);
		mciSendString("play rsfx repeat", NULL, 0, NULL);
	}
}

void PlayWolfHowlSound() {
	if (sfxOn) {
		mciSendString("close wsfx", NULL, 0, NULL);
		mciSendString("open \"Audios/collect.mp3\" type mpegvideo alias wsfx", NULL, 0, NULL);
		mciSendString("play wsfx", NULL, 0, NULL);
	}
}

void StopRunSound() {
	// Use pause instead of stop or seek to minimize gap when resuming
	mciSendString("pause rsfx", NULL, 0, NULL);
}

#endif // AUDIO_HPP
