#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"

void DrawGameOver() {
	iClear();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetColor(0, 0, 0, 255);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// Blood Vignette
	float pulse = (sin(globalTime * 2) + 1) / 2.0f;
	for (int i = 0; i < 20; i++) {
		SetColor(150, 0, 0, (int)(10 + pulse * 20));
		iRectangle(i * 10, i * 10, SCREEN_WIDTH - i * 20, SCREEN_HEIGHT - i * 20);
	}

	float glow = (sin(globalTime * 8) + 1) / 2.0f;
	
	// Bold Shadowed Text
	SetColor(50, 0, 0, 200);
	// iText(SCREEN_WIDTH / 2 - 148, SCREEN_HEIGHT / 2 - 2, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
	DrawStrokeText(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 20, 0, "GAME OVER", 0.5f, 5.0f);
	
	SetColor(255, (int)(50 + glow * 50), 50, (int)(200 + glow * 55));
	// iText(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
	DrawStrokeText(SCREEN_WIDTH / 2 - 255, SCREEN_HEIGHT / 2 - 15, 0, "GAME OVER", 0.5f, 3.0f);
	
	SetColor(200, 200, 200, 180);
	iText(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 60, "The ruins reclaim your soul...", GLUT_BITMAP_HELVETICA_18);
	iText(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 90, "Respawning soon...", GLUT_BITMAP_HELVETICA_12);

	// Display Final Score
	char finalScoreStr[50];
	sprintf_s(finalScoreStr, "FINAL SCORE: %ld", currentScore);
	SetColor(255, 215, 0, 255);
	iText(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 130, finalScoreStr, GLUT_BITMAP_TIMES_ROMAN_24);

	// Save to leaderboard (once per game over)
	if (!leaderboardScoreSaved && currentScore > 0) {
		SaveLeaderboardEntry(playerName, currentScore);
		leaderboardScoreSaved = true;
	}

	glDisable(GL_BLEND);
}

void DrawLevelCompleted() {
	iClear();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetColor(0, 0, 0, 255);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// Golden Glow
	float glow = (sin(globalTime * 3) + 1) / 2.0f;
	
	// Success Vignette
	for (int i = 0; i < 20; i++) {
		SetColor(50, 200, 255, (int)(10 + glow * 10));
		iRectangle(i * 10, i * 10, SCREEN_WIDTH - i * 20, SCREEN_HEIGHT - i * 20);
	}

	SetColor(255, 215, 0, 255);
	DrawStrokeText(SCREEN_WIDTH / 2 - 280, SCREEN_HEIGHT / 2 + 80, 0, "CONGRATULATIONS!", 0.5f, 4.0f);
	
	char levelStr[50];
	sprintf_s(levelStr, "You have cleared Level %d", currentLevel);
	iText(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 20, levelStr, GLUT_BITMAP_TIMES_ROMAN_24);

	// Display Time Taken
	float elapsed = 180.0f - levelTimer;
	int mins = (int)(elapsed / 60);
	int secs = (int)(elapsed) % 60;
	char timeStr[50];
	sprintf_s(timeStr, "TIME: %d:%02d", mins, secs);
	SetColor(200, 200, 255, 255);
	iText(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 20, timeStr, GLUT_BITMAP_TIMES_ROMAN_24);

	// Display Time Bonus
	char bonusStr[50];
	sprintf_s(bonusStr, "TIME BONUS: +%ld", levelTimeBonus);
	SetColor(0, 255, 200, 255);
	iText(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 55, bonusStr, GLUT_BITMAP_TIMES_ROMAN_24);

	// Display Final Score
	char finalScoreStr[50];
	sprintf_s(finalScoreStr, "FINAL SCORE: %ld", currentScore);
	SetColor(255, 215, 0, 255);
	iText(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 90, finalScoreStr, GLUT_BITMAP_TIMES_ROMAN_24);

	SetColor(200, 200, 200, 180);
	iText(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 140, "PRESS ENTER TO GO TO THE NEXT LEVEL", GLUT_BITMAP_HELVETICA_18);
	iText(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 170, "Press END to Exit", GLUT_BITMAP_HELVETICA_12);

	glDisable(GL_BLEND);
}

#endif // GAMEOVER_HPP
