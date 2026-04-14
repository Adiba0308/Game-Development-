#ifndef SAVELOAD_HPP
#define SAVELOAD_HPP

#include <stdio.h>
#include "GameVariables.hpp"

// Function to save the game state to a file
// Basic Save/Load System
void SaveGame() { // Serializes the complete game state (player, levels, puzzles, score, audio settings) to a binary save file for the continue feature
	FILE* fp;
	fopen_s(&fp, "savegame.dat", "wb");
	if (fp) {
		fwrite(&currentLocation, sizeof(int), 1, fp);
		fwrite(&currentLevel, sizeof(int), 1, fp);
		fwrite(&player, sizeof(Player), 1, fp);
		fwrite(&level1, sizeof(Level1), 1, fp); // Using Level1
		fwrite(&level2, sizeof(Level2), 1, fp); // Level2 data
		fwrite(&level3, sizeof(Level3), 1, fp); // Level3 data
		fwrite(&currentScore, sizeof(long), 1, fp);
		fwrite(&isGateUnlocked, sizeof(bool), 1, fp); 
		
		// Save Puzzle States
		fwrite(&puzzle.isSolved, sizeof(bool), 1, fp);
		fwrite(&scalePuzzle.isSolved, sizeof(bool), 1, fp);
		fwrite(&statuePuzzle.isSolved, sizeof(bool), 1, fp);
		fwrite(&eyeAlignmentPuzzle.isSolved, sizeof(bool), 1, fp);
		fwrite(&player.hasSword, sizeof(bool), 1, fp);
		fwrite(&moonSealPuzzle.isSolved, sizeof(bool), 1, fp);
		
		// Save Audio Settings
		fwrite(&musicOn, sizeof(bool), 1, fp);
		fwrite(&sfxOn, sizeof(bool), 1, fp);
		
		fclose(fp);
		printf("Game Saved!\n");
	}
}

bool LoadGame() { // Deserializes the saved game state from disk, restoring player position, level data, puzzles, and score; preserves current audio settings and resumes gameplay
	FILE* fp;
	fopen_s(&fp, "savegame.dat", "rb");
	if (fp) {
		// Preserve current audio settings (user may have changed them in menu)
		bool savedMusicOn = musicOn;
		bool savedSfxOn = sfxOn;

		fread(&currentLocation, sizeof(int), 1, fp);
		fread(&currentLevel, sizeof(int), 1, fp);
		fread(&player, sizeof(Player), 1, fp);
		fread(&level1, sizeof(Level1), 1, fp); // Using Level1
		fread(&level2, sizeof(Level2), 1, fp); // Level2 data
		fread(&level3, sizeof(Level3), 1, fp); // Level3 data
		fread(&currentScore, sizeof(long), 1, fp);
		fread(&isGateUnlocked, sizeof(bool), 1, fp);

		// Load Puzzle States
		fread(&puzzle.isSolved, sizeof(bool), 1, fp);
		fread(&scalePuzzle.isSolved, sizeof(bool), 1, fp);
		fread(&statuePuzzle.isSolved, sizeof(bool), 1, fp);
		fread(&eyeAlignmentPuzzle.isSolved, sizeof(bool), 1, fp);
		fread(&player.hasSword, sizeof(bool), 1, fp);
		fread(&moonSealPuzzle.isSolved, sizeof(bool), 1, fp);

		// Read saved audio settings but discard them — use current user preference
		bool dummyMusic, dummySfx;
		fread(&dummyMusic, sizeof(bool), 1, fp);
		fread(&dummySfx, sizeof(bool), 1, fp);

		fclose(fp);

		// Restore the user's current audio preference
		musicOn = savedMusicOn;
		sfxOn = savedSfxOn;

		printf("Game Loaded!\n");
		
		// Restore state logic
		gameState = STATE_PLAY;
		PlayBackgroundMusic(); 
		return true;
	}
	return false;
}

#endif // SAVELOAD_HPP
