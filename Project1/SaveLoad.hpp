#ifndef SAVELOAD_HPP
#define SAVELOAD_HPP

#include <stdio.h>
#include "GameVariables.hpp"

// Function to save the game state to a file
// Basic Save/Load System
void SaveGame() {
	FILE* fp;
	fopen_s(&fp, "savegame.dat", "wb");
	if (fp) {
		fwrite(&currentLocation, sizeof(int), 1, fp);
		fwrite(&player, sizeof(Player), 1, fp);
		fwrite(&level1, sizeof(Level1), 1, fp); // Using Level1
		fwrite(&currentScore, sizeof(long), 1, fp);
		fwrite(&isGateUnlocked, sizeof(bool), 1, fp); 
		
		// Save Puzzle State
		fwrite(&puzzle.isSolved, sizeof(bool), 1, fp);
		
		fclose(fp);
		printf("Game Saved!\n");
	}
}

bool LoadGame() {
	FILE* fp;
	fopen_s(&fp, "savegame.dat", "rb");
	if (fp) {
		fread(&currentLocation, sizeof(int), 1, fp);
		fread(&player, sizeof(Player), 1, fp);
		fread(&level1, sizeof(Level1), 1, fp); // Using Level1
		fread(&currentScore, sizeof(long), 1, fp);
		fread(&isGateUnlocked, sizeof(bool), 1, fp);

		// Load Puzzle State
		fread(&puzzle.isSolved, sizeof(bool), 1, fp);

		fclose(fp);
		printf("Game Loaded!\n");
		
		// Restore state logic
		gameState = STATE_PLAY;
		if (currentLocation == 1) {
			// Restore music or specific location settings if needed
			PlayBackgroundMusic(); 
		}
		return true;
	}
	return false;
}

#endif // SAVELOAD_HPP
