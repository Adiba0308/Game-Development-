/*
	iMain.cpp
	Main Entry Point
*/

#include "iGraphics.hpp"
#include "GameVariables.hpp"
#include "Utilities.hpp"
#include "Audio.hpp"
#include "Menu.hpp"
#include "Slideshow.hpp"
#include "Level1.hpp"
// #include "Location2.hpp" // Removed as merged
#include "Puzzles.hpp"
#include "GameOver.hpp"
#include "GameLoop.hpp"
#include <time.h>

void iDraw() {
	iClear();

	switch (gameState) {
	case STATE_MENU:
		DrawMenu();
		break;
	case STATE_SLIDESHOW:
		DrawSlideshow();
		break;
	case STATE_PLAY:
		DrawLevel1();
		break;
	case STATE_PUZZLE:
		DrawPuzzle();
		break;
	case STATE_PUZZLE_FLOW:
		DrawFlowPuzzle();
		break;
	case STATE_INSTRUCTIONS:
		DrawInstructions();
		break;
	case STATE_SETTINGS:
		DrawSettings();
		break;
	case STATE_SCOREBOARD:
		DrawGameState("SCOREBOARD");
		break;
	case STATE_CREDITS:
		DrawCredits();
		break;
	case STATE_GAMEOVER:
		DrawGameOver();
		break;
	case STATE_LEVEL_COMPLETED:
		DrawLevelCompleted();
		break;
	case STATE_EXIT:
		SaveGame();
		exit(0);
		break;
	}

	if (transitionAlpha > 0) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		SetColor(0, 0, 0, (int)(255 * transitionAlpha));
		iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glDisable(GL_BLEND);
	}
}

void iMouseMove(int mx, int my) {
	// Not used for drag, but required by iGraphics
}

void iPassiveMouseMove(int mx, int my) {
	if (gameState == STATE_MENU) {
		for (int i = 0; i < numButtons; i++) {
			int by = menuButtons[i].y + (int)menuButtons[i].floatOffset;
			if (mx >= menuButtons[i].x && mx <= menuButtons[i].x + menuButtons[i].width &&
				my >= by && my <= by + menuButtons[i].height) {
				menuButtons[i].isHovered = true;
			}
			else {
				menuButtons[i].isHovered = false;
			}
		}
	}
	else if (gameState == STATE_SETTINGS) {
		for (int i = 0; i < 2; i++) {
			int by = settingsButtons[i].y + (int)settingsButtons[i].floatOffset;
			if (mx >= settingsButtons[i].x && mx <= settingsButtons[i].x + settingsButtons[i].width &&
				my >= by && my <= by + settingsButtons[i].height) {
				settingsButtons[i].isHovered = true;
			}
			else {
				settingsButtons[i].isHovered = false;
			}
		}
	}
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (gameState == STATE_MENU) {
			for (int i = 0; i < numButtons; i++) {
				if (menuButtons[i].isHovered) {
					if (menuButtons[i].targetState == STATE_CONTINUE) {
						if (LoadGame()) {
							// Successfully loaded, gameState is updated
							// Optional: Play sound
						} else {
							// Failed to load (file doesn't exist)
							// Do nothing or give feedback
							printf("No save file found!\n");
						}
					}
					else {
						gameState = menuButtons[i].targetState;
						if (gameState == STATE_SLIDESHOW) LoadSlideshow();
					}
				}
			}
		}
		else if (gameState == STATE_SETTINGS) {
			for (int i = 0; i < 2; i++) {
				if (settingsButtons[i].isHovered) {
					if (i == 0) { // Music On
						musicOn = true;
						PlayBackgroundMusic();
					}
					else if (i == 1) { // Music Off
						musicOn = false;
						StopMusic();
					}
				}
			}
		}
		else if (gameState == STATE_SLIDESHOW) {
			SkipSlide();
		}
		else if (gameState == STATE_LEVEL_COMPLETED) {
			gameState = STATE_MENU;
		}
		else if (gameState == STATE_PUZZLE) {
			if (puzzle.delayTimer > 0) return;
			for (int i = 0; i < 12; i++) {
				Card &c = puzzle.cards[i];
				if (!c.isMatched && !c.isFlipped && mx >= c.x && mx <= c.x + c.w && my >= c.y && my <= c.y + c.h) {
					c.isFlipped = true;
					puzzle.flippedIndices[puzzle.numFlipped++] = i;
					if (puzzle.numFlipped == 2) {
						if (puzzle.cards[puzzle.flippedIndices[0]].symbolID == puzzle.cards[puzzle.flippedIndices[1]].symbolID) {
							puzzle.cards[puzzle.flippedIndices[0]].isMatched = true;
							puzzle.cards[puzzle.flippedIndices[1]].isMatched = true;
							puzzle.numFlipped = 0;
							
							bool allMatched = true;
							for(int j=0; j<12; j++) if(!puzzle.cards[j].isMatched) allMatched = false;
							if(allMatched) { isGateUnlocked = true; gameState = STATE_PLAY; }
						} else {
							puzzle.delayTimer = 1.0f;
						}
					}
				}
			}
		} else if (gameState == STATE_PUZZLE_FLOW) {
			for (int r = 0; r < 3; r++) {
				for (int c = 0; c < 4; c++) {
					PipeSegment &p = flowPuzzle.grid[r][c];
					if (mx >= p.x && mx <= p.x + p.w && my >= p.y && my <= p.y + p.h) {
						p.rotation = (p.rotation + 90) % 360;
						CheckFlowConnectivity();
						
						// Success Check
						if (flowPuzzle.isSolved) {
							level1.vinesActive = false;
							gameState = STATE_PLAY;
						}
					}
				}
			}
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (gameState == STATE_SLIDESHOW) {
			gameState = STATE_PLAY;
			currentLocation = 1;
			player.x = 100;
			player.y = level1.groundY;
			level1.cameraX = 0;
			PlayBackgroundMusic();
		}
	}
}

void iKeyboard(unsigned char key) {
	if (key == 'm' || key == 'M') {
		if (gameState == STATE_PLAY || gameState == STATE_PUZZLE || gameState == STATE_PUZZLE_FLOW) SaveGame();
		gameState = STATE_MENU;
	}
	if (key == ' ') {
		if (gameState == STATE_SLIDESHOW) SkipSlide();
	}
	if (key == 'j' || key == 'J') {
		if (gameState == STATE_PLAY && !player.isAttacking) {
			player.isAttacking = true;
			player.attack.currentFrame = 0;
			player.attack.timer = 0;
		}
	}
}

void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_END) {
		SaveGame();
		exit(0);
	}
}

int main() {
	srand((unsigned)time(NULL)); // Seed random number generator
	InitializeButtons();
	InitializeSettingsButtons();

	// Timer for animations (approx 60 FPS update for smoothness, but simplistic)
	iSetTimer(16, AnimationUpdate);

	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Echoes of the Forgotten City");

	// Load Menu Background (Must be after iInitialize)
	menuBgID = iLoadImage("asset/background/menu.b.png");
	creditsImageID = iLoadImage("asset/background/credits.png");
	instructionsImageID = iLoadImage("asset/background/instructions.png");

	// Initialize Level 1
	LoadLevel1();
	PlayBackgroundMusic();
	iStart();

	return 0;
}