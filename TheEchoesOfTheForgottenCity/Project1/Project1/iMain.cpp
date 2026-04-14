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
#include "Level2.hpp"
#include "Level3.hpp"
// #include "Location2.hpp" // Removed as merged
#include "Puzzles.hpp"
#include "GameOver.hpp"
#include "Leaderboard.hpp"
#include "GameLoop.hpp"
#include <time.h>
//graphics
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
		if (currentLevel == 1) DrawLevel1();
		else if (currentLevel == 2) DrawLevel2();
		else if (currentLevel == 3) DrawLevel3();
		DrawIngameHud();
		break;
	case STATE_PUZZLE:
		DrawPuzzle();
		DrawIngameHud();
		break;
	case STATE_PUZZLE_FLOW:
		DrawFlowPuzzle();
		DrawIngameHud();
		break;
	case STATE_PUZZLE_SCALE:
		DrawScalePuzzle();
		DrawIngameHud();
		break;
	case STATE_PUZZLE_STATUE_ROTATION:
		DrawStatueRotationPuzzle();
		DrawIngameHud();
		break;
	case STATE_PUZZLE_EYE_ALIGNMENT:
		DrawEyeAlignmentPuzzle();
		DrawIngameHud();
		break;
	case STATE_PUZZLE_MOON_SEAL:
		DrawMoonSealPuzzle();
		DrawIngameHud();
		break;
	case STATE_PAUSED:
		// Draw frozen game world underneath
		if (currentLevel == 1) DrawLevel1();
		else if (currentLevel == 2) DrawLevel2();
		else if (currentLevel == 3) DrawLevel3();
		DrawPauseOverlay();
		break;
	case STATE_INSTRUCTIONS:
		DrawInstructions();
		break;
	case STATE_SETTINGS:
		DrawSettings();
		break;
	case STATE_SCOREBOARD:
	case STATE_LEADERBOARD_CONFIRM:
		DrawLeaderboard();
		break;
	case STATE_NAME_INPUT:
		DrawNameInput();
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
		for (int i = 0; i < 4; i++) {
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
	else if (gameState == STATE_SCOREBOARD || gameState == STATE_LEADERBOARD_CONFIRM) {
		HandleLeaderboardMouseMove(mx, my);
	}
	// In-game HUD hover (during play and puzzle states)
	if (gameState == STATE_PLAY ||
		gameState == STATE_PUZZLE || gameState == STATE_PUZZLE_FLOW ||
		gameState == STATE_PUZZLE_SCALE || gameState == STATE_PUZZLE_STATUE_ROTATION ||
		gameState == STATE_PUZZLE_EYE_ALIGNMENT || gameState == STATE_PUZZLE_MOON_SEAL) {
		for (int i = 0; i < 2; i++) {
			ingameHudButtons[i].isHovered =
				(mx >= ingameHudButtons[i].x && mx <= ingameHudButtons[i].x + ingameHudButtons[i].width &&
				 my >= ingameHudButtons[i].y && my <= ingameHudButtons[i].y + ingameHudButtons[i].height);
		}
	}
	// Pause overlay hover
	if (gameState == STATE_PAUSED) {
		int by = ingameContinueButton.y + (int)ingameContinueButton.floatOffset;
		ingameContinueButton.isHovered =
			(mx >= ingameContinueButton.x && mx <= ingameContinueButton.x + ingameContinueButton.width &&
			 my >= by && my <= by + ingameContinueButton.height);
	}
}


void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		// --- In-game HUD buttons (pause / settings / quit) ---
		if (gameState == STATE_PLAY ||
			gameState == STATE_PUZZLE || gameState == STATE_PUZZLE_FLOW ||
			gameState == STATE_PUZZLE_SCALE || gameState == STATE_PUZZLE_STATUE_ROTATION ||
			gameState == STATE_PUZZLE_EYE_ALIGNMENT || gameState == STATE_PUZZLE_MOON_SEAL) {
			if (ingameHudButtons[0].isHovered) { // PAUSE -> Menu
				StopRunSound();
				SaveGame();
				gameState = STATE_MENU;
				PlayMenuMusic();
				return;
			}
			if (ingameHudButtons[1].isHovered) { // QUIT -> Leaderboard
				StopRunSound();
				if (!leaderboardScoreSaved) {
					SaveLeaderboardEntry(playerName, currentScore);
					leaderboardScoreSaved = true;
				}
				leaderboard.loaded = false;
				gameState = STATE_SCOREBOARD;
				PlayMenuMusic();
				return;
			}
		}

		// --- Pause overlay: Continue ---
		if (gameState == STATE_PAUSED) {
			if (ingameContinueButton.isHovered) {
				gameState = prePauseState;
				return;
			}
		}

		// --- Main menu buttons ---
		if (gameState == STATE_MENU) {
			for (int i = 0; i < numButtons; i++) {
				if (menuButtons[i].isHovered) {
					if (menuButtons[i].targetState == STATE_CONTINUE) {
						if (LoadGame()) {
							// loaded
						} else {
							printf("No save file found!\n");
						}
					}
					else if (menuButtons[i].targetState == STATE_SCOREBOARD) {
						gameState = STATE_SCOREBOARD;
						LoadLeaderboard();
						SortLeaderboard();
					}
					else {
						gameState = menuButtons[i].targetState;
						if (gameState == STATE_SLIDESHOW) {
							LoadSlideshow();
						}
					}
				}
			}
		}
		else if (gameState == STATE_SCOREBOARD || gameState == STATE_LEADERBOARD_CONFIRM) {
			HandleLeaderboardClick(mx, my);
		}
		else if (gameState == STATE_SETTINGS) {
			for (int i = 0; i < 4; i++) {
				if (settingsButtons[i].isHovered) {
					if (i == 0) { // Music On
						musicOn = true;
						PlayMenuMusic();
					}
					else if (i == 1) { // Music Off
						musicOn = false;
						StopMusic();
					}
					else if (i == 2) { sfxOn = true; }
					else if (i == 3) { sfxOn = false; }
				}
			}
		}
		else if (gameState == STATE_SLIDESHOW) {
			SkipSlide();
		}
		else if (gameState == STATE_LEVEL_COMPLETED) {
			if (currentLevel == 1) {
				currentLevel = 2;
				LoadLevel2();
				gameState = STATE_PLAY;
				PlayBackgroundMusic();
			} else if (currentLevel == 2) {
				currentLevel = 3;
				LoadLevel3();
				gameState = STATE_PLAY;
				PlayBackgroundMusic();
			} else {
				if (!leaderboardScoreSaved && currentScore > 0) {
					SaveLeaderboardEntry(playerName, currentScore);
					leaderboardScoreSaved = true;
				}
				gameState = STATE_MENU;
				PlayMenuMusic();
			}
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
						if (flowPuzzle.isSolved) {
							level1.vinesActive = false;
							gameState = STATE_PLAY;
						}
					}
				}
			}
		} else if (gameState == STATE_PUZZLE_SCALE) {
			if (mx >= SCREEN_WIDTH - 200 && mx <= SCREEN_WIDTH - 50 && my >= 50 && my <= 100) {
				if (scalePuzzle.leftWeight == scalePuzzle.rightWeight && scalePuzzle.leftWeight > 0) {
					scalePuzzle.isSolved = true;
					AddScore(1000);
				}
			}
			float invStartX = SCREEN_WIDTH / 2 - (level2.numCollectibles * 80) / 2;
			float invStartY = 50;
			for (int i = 0; i < level2.numCollectibles; i++) {
				if (level2.collectibles[i].collected) {
					float slotX = invStartX + i * 80;
					float slotY = invStartY;
					if (mx >= slotX && mx <= slotX + 60 && my >= slotY && my <= slotY + 60) {
						if (!level2.collectibles[i].placedOnScale) {
							level2.collectibles[i].placedOnScale = true;
							level2.collectibles[i].scaleSide = 1;
							scalePuzzle.leftWeight += level2.collectibles[i].weight;
						} else {
							if (level2.collectibles[i].scaleSide == 1) scalePuzzle.leftWeight -= level2.collectibles[i].weight;
							else scalePuzzle.rightWeight -= level2.collectibles[i].weight;
							level2.collectibles[i].placedOnScale = false;
							level2.collectibles[i].scaleSide = 0;
						}
					}
				}
			}
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (gameState == STATE_SLIDESHOW) {
			gameState = STATE_NAME_INPUT;
			nameInputBuffer[0] = '\0';
			nameInputLen = 0;
			nameInputCursorBlink = 0.0f;
		} else if (gameState == STATE_PUZZLE_SCALE) {
			float invStartX = SCREEN_WIDTH / 2 - (level2.numCollectibles * 80) / 2;
			float invStartY = 50;
			for (int i = 0; i < level2.numCollectibles; i++) {
				if (level2.collectibles[i].collected && !level2.collectibles[i].placedOnScale) {
					float slotX = invStartX + i * 80;
					float slotY = invStartY;
					if (mx >= slotX && mx <= slotX + 60 && my >= slotY && my <= slotY + 60) {
						level2.collectibles[i].placedOnScale = true;
						level2.collectibles[i].scaleSide = 2;
						scalePuzzle.rightWeight += level2.collectibles[i].weight;
					}
				}
			}
		}
	}
}


	
void iKeyboard(unsigned char key) {

	// Route to name input handler if in name input state
	if (gameState == STATE_NAME_INPUT) {
		HandleNameInputKey(key);
		return;
	}
	if (key == 13) { // Enter
		if (gameState == STATE_LEVEL_COMPLETED) {
			if (currentLevel == 1) {
				currentLevel = 2;
				LoadLevel2();
				gameState = STATE_PLAY;
				PlayBackgroundMusic();
			} else if (currentLevel == 2) {
				currentLevel = 3;
				LoadLevel3();
				gameState = STATE_PLAY;
				PlayBackgroundMusic();
			} else {
				if (!leaderboardScoreSaved && currentScore > 0) {
					SaveLeaderboardEntry(playerName, currentScore);
					leaderboardScoreSaved = true;
				}
				gameState = STATE_MENU;
				PlayMenuMusic();
			}
		}
	}
	if (key == 'p' || key == 'P') {
		bool isPlaying = (gameState == STATE_PLAY ||
			gameState == STATE_PUZZLE || gameState == STATE_PUZZLE_FLOW ||
			gameState == STATE_PUZZLE_SCALE || gameState == STATE_PUZZLE_STATUE_ROTATION ||
			gameState == STATE_PUZZLE_EYE_ALIGNMENT || gameState == STATE_PUZZLE_MOON_SEAL);
		if (isPlaying) {
			StopRunSound();
			SaveGame();
			gameState = STATE_MENU;
			PlayMenuMusic();
		}
	}
	if (key == 'm' || key == 'M') {
		if (gameState == STATE_PLAY || gameState == STATE_PUZZLE || gameState == STATE_PUZZLE_FLOW || gameState == STATE_PUZZLE_SCALE || gameState == STATE_PUZZLE_STATUE_ROTATION || gameState == STATE_PUZZLE_EYE_ALIGNMENT || gameState == STATE_PUZZLE_MOON_SEAL) SaveGame();
		gameState = STATE_MENU;
		PlayMenuMusic();
	}
	if (key == ' ') {
		if (gameState == STATE_SLIDESHOW) SkipSlide();
	}
	if (key == 'j' || key == 'J') {
		if (gameState == STATE_PLAY && !player.isAttacking && !player.isSwordAttacking) {
			if (currentLevel == 3 && currentLocation == 2 && player.hasSword) {
				player.isSwordAttacking = true;
				player.swordAttackRight.currentFrame = 0;
				player.swordAttackRight.timer = 0;
				player.swordAttackLeft.currentFrame = 0;
				player.swordAttackLeft.timer = 0;
			} else {
				player.isAttacking = true;
				player.attack.currentFrame = 0;
				player.attack.timer = 0;
			}
		}
	}
	if (key == 's' || key == 'S') {
		if (gameState == STATE_PUZZLE) {
			puzzle.isSolved = true;
			for (int i = 0; i < 12; i++) puzzle.cards[i].isMatched = true;
			AddScore(500);
			energyWaveRadius = 1.0f;
			isGateUnlocked = true;
			gameState = STATE_PLAY;
		} else if (gameState == STATE_PUZZLE_FLOW) {
			flowPuzzle.isSolved = true;
			AddScore(500);
			level1.vinesActive = false;
			gameState = STATE_PLAY;
		} else if (gameState == STATE_PUZZLE_STATUE_ROTATION) {
			statuePuzzle.isSolved = true;
			AddScore(1000);
			isGateUnlocked = true;
			gameState = STATE_PLAY;
		} else if (gameState == STATE_PUZZLE_EYE_ALIGNMENT) {
			eyeAlignmentPuzzle.isSolved = true;
			AddScore(1500);
			isGateUnlocked = true;
			gameState = STATE_PLAY;
		} else if (gameState == STATE_PUZZLE_MOON_SEAL) {
			moonSealPuzzle.isSolved = true;
			AddScore(2000);
			level3.finalDoorOpen = true;
			gameState = STATE_PLAY;
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
	InitLeaderboardButtons();
	InitIngameHudButtons();

	// Timer for animations (approx 60 FPS update for smoothness, but simplistic)
	iSetTimer(16, AnimationUpdate);

	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Echoes of the Forgotten City");

	// Load Menu Background (Must be after iInitialize)
	menuBgID = iLoadImage("asset/background/menu.b.png");
	creditsImageID = iLoadImage("asset/background/credits.png");
	instructionsImageID = iLoadImage("asset/background/instructions.png");

	// Initialize Level 1
	LoadLevel1();
	PlayMenuMusic();
	iStart();

	return 0;
}
