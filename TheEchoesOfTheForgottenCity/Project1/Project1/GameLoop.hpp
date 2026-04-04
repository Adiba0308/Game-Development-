#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"
#include "Audio.hpp"
#include "Level1.hpp" // For LoadLevel1, UpdateBubbles, UpdateFirePillar, iScreenShake
#include "Slideshow.hpp" // For UpdateSlideshow, SkipSlide
#include "Puzzles.hpp" // For UpdatePuzzle, UpdateFlowPuzzle, InitPuzzle, InitFlowPuzzle
#include "SaveLoad.hpp"

void TriggerSkipLocation() {
	if (gameState != STATE_PLAY) return;

	if (currentLevel == 1) {
		if (currentLocation == 1) {
			// Skip Level 1 Loc 1 -> Level 1 Loc 2
			isGateUnlocked = true;
			level1.gateY = 600;
			player.x = 2600; 
			printf("Skipping to Level 1 Location 2...\n");
		} else {
			// Skip Level 1 Loc 2 -> Level 2
			ApplyTimeBonus();
			gameState = STATE_LEVEL_COMPLETED;
			printf("Skipping to Level 2...\n");
		}
	} else if (currentLevel == 2) {
		if (currentLocation == 1) {
			// Skip Level 2 Loc 1 -> Level 2 Loc 2
			isTransitioning = true;
			printf("Skipping to Level 2 Location 2...\n");
		} else {
			// Skip Level 2 Loc 2 -> Completed
			ApplyTimeBonus();
			gameState = STATE_LEVEL_COMPLETED;
			printf("Level 2 Completed (Skip)!\n");
		}
	} else if (currentLevel == 3) {
		if (currentLocation == 1) {
			// Skip Level 3 Loc 1 -> Level 3 Loc 2
			isGateUnlocked = true;
			eyeAlignmentPuzzle.isSolved = true;
			level3.gateY = 600;
			player.x = level3.gateX + 200;
			player.hasSword = true;
			printf("Skipping to Level 3 Location 2...\n");
		} else {
			// Skip Level 3 Loc 2 -> Completed
			ApplyTimeBonus();
			gameState = STATE_LEVEL_COMPLETED;
			printf("Level 3 Completed (Skip)!\n");
		}
	}
}

void UpdateSkipMechanism(float dt) {
	static bool sPrev = false;
	bool sDown = isKeyPressed('s') || isKeyPressed('S');

	if (sDown && !sPrev) {
		float currentTime = globalTime; // Using globalTime as a timer
		if (sPressedOnce && (currentTime - lastSPressTime < 3.0f)) {
			TriggerSkipLocation();
			sPressedOnce = false;
		} else {
			sPressedOnce = true;
			lastSPressTime = currentTime;
		}
	}

	if (sPressedOnce && (globalTime - lastSPressTime > 3.0f)) {
		sPressedOnce = false;
	}

	sPrev = sDown;
}

void UpdatePlay(float dt) {
	if (isTransitioning) {
		transitionAlpha += 2.0f * dt;
		if (transitionAlpha >= 1.0f) {
			transitionAlpha = 1.0f;
			if (currentLocation == 1) {
				currentLocation = 2;
				player.x = level1.checkpointX;
				player.y = level1.checkpointY;
				level1.cameraX = 0;
			}
			isTransitioning = false;
		}
		return;
	}
	if (transitionAlpha > 0) transitionAlpha -= 2.0f * dt;

	if (levelTextTimer > 0) {
		levelTextTimer -= dt;
	}

	if (screenFreezeTimer > 0) {
		screenFreezeTimer -= dt;
		return;
	}

	// Movement
	float speed = 420.0f;
	player.isMoving = false;
	if (!player.isAttacking) { // Only move if not attacking
		if (isKeyPressed('d') != 0 || isKeyPressed('D') != 0 || isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
			player.x += speed * dt;
			player.isFacingRight = true;
			player.isMoving = true;
		}
		if (isKeyPressed('a') != 0 || isKeyPressed('A') != 0 || isSpecialKeyPressed(GLUT_KEY_LEFT)) {
			player.x -= speed * dt;
			player.isFacingRight = false;
			player.isMoving = true;
		}
	}

	// Health Death
	if (player.health <= 0) {
		player.health = 0;
		gameState = STATE_GAMEOVER;
		gameOverTimer = 0;
		PlayGameOverMusic();
		return;
	}

	// Jump (Double Jump supported)
	bool spacePressed = (isKeyPressed(' ') != 0) || isSpecialKeyPressed(GLUT_KEY_UP);
	if (!player.isAttacking && spacePressed && !player.spacePrev) {
		if (player.jumpCount < 2) {
			player.isJumping = true;
			player.jumpCount++;
			// Second jump is higher
			player.jumpVel = (player.jumpCount == 1) ? 550.0f : 650.0f;
			player.jump.currentFrame = 0;
			player.jump.timer = 0;
			PlayJumpSound();
		}
	}
	player.spacePrev = spacePressed;

	if (player.isJumping) {
		player.y += player.jumpVel * dt;
		player.jumpVel -= 1000.0f * dt; // Gravity

		if (player.y <= level1.groundY) {
			player.y = level1.groundY;
			player.isJumping = false;
			player.jumpVel = 0;
			player.jumpCount = 0;
		}
	}

	// Run Sound Logic with Grace Period
	static bool isRunSoundPlaying = false;
	static int runGraceFrames = 0;
	if (player.isMoving && !player.isJumping) {
		runGraceFrames = 10; // Keep it playing for 10 frames after stopping/jumping
		if (!isRunSoundPlaying) {
			PlayRunSound();
			isRunSoundPlaying = true;
		}
	} else {
		if (runGraceFrames > 0) {
			runGraceFrames--;
		} else if (isRunSoundPlaying) {
			StopRunSound();
			isRunSoundPlaying = false;
		}
	}

	// Boundaries
	if (player.x < 40) player.x = 40;
	if (player.x > level1.levelWidth - 40) player.x = level1.levelWidth - 40;

	// Camera Follow
	level1.cameraX = player.x - SCREEN_WIDTH / 2;
	if (level1.cameraX < 0) level1.cameraX = 0;
	if (level1.cameraX > level1.levelWidth - SCREEN_WIDTH) level1.cameraX = level1.levelWidth - SCREEN_WIDTH;

	// Animation Update
	UpdateAnim(player.idle, dt);
	UpdateAnim(player.runRight, dt);
	UpdateAnim(player.runLeft, dt);
	if (player.isJumping) {
		UpdateAnim(player.jump, dt);
	}
	if (player.isAttacking) {
		float prevTimer = player.attack.timer;
		UpdateAnim(player.attack, dt);
		// If it just looped (timer reset to 0), it means the duration finished
		if (player.attack.timer < prevTimer) {
			player.isAttacking = false;
		}
	}

	if (currentLocation == 1) {
		// Fire Pillar Logic
		UpdateFirePillar(dt);

		// Collision Fire
		for (int i = 0; i < 2; i++) {
			if (firePillars[i].state == FIRE_BURST) {
				// Collision width scaled: 40 * scale
				if (abs(player.x - firePillars[i].x) < 40 * firePillars[i].scale) {
					player.health -= 50 * dt;
				}
			}
		}
	} else {
		// Location 2 Hazard Logic: Blades
		for (int i = 0; i < 4; i++) {
			if (level1.traps[i].active) {
				float dist = sqrt(pow(player.x - level1.traps[i].x, 2) + pow(player.y + 40 - level1.traps[i].y, 2));
				if (dist < level1.traps[i].radius) {
					player.health -= 60 * dt; // Deadly blades
				}
			}
		}
	}

	// Health Potion Usage ('H')
	static bool hPrev = false;
	bool hDown = isKeyPressed('h') || isKeyPressed('H');
	if (hDown && !hPrev) {
		if (player.healthPotions > 0 && player.health < 100) {
			player.healthPotions--;
			player.health += 30;
			if (player.health > 100) player.health = 100;
		}
	}
	hPrev = hDown;

	// Attack Trigger ('F') - General responsiveness
	bool fPressed = (isKeyPressed('f') != 0) || (isKeyPressed('F') != 0) || isSpecialKeyPressed(GLUT_KEY_DOWN);
	if (fPressed && !player.isAttacking) {
		player.isAttacking = true;
		player.attack.currentFrame = 0;
		player.attack.timer = 0;
	}

	// Bubble Interaction Logic
	if (currentLocation == 1) {
		UpdateBubbles(dt); // Added animation update
		for (int i = 0; i < 2; i++) {
			if (bubbles[i].active) {
				float dist = sqrt(pow(player.x - bubbles[i].x, 2) + pow(player.y + 40 - bubbles[i].y, 2));
				bubbles[i].showHint = (dist < 150);
				if (player.isAttacking && dist < 50) {
					bubbles[i].active = false;
					AddScore(100);
					screenFreezeTimer = 0.5f;
					bool allPopped = true;
					for (int j = 0; j < 2; j++) if (bubbles[j].active) allPopped = false;
					if (allPopped) { InitPuzzle(); gameState = STATE_PUZZLE; player.isAttacking = false; }
					PlayBubblePopSound();
				}
			}
		}
	} else {
		// Location 2: Red Bubbles (Now Blue Animated)
		for (int i = 0; i < 3; i++) {
			if (level1.redBubbles[i].active) {
				// Animation and Motion
				level1.redBubbles[i].y = level1.redBubbles[i].baseY + sin(globalTime * 2 + i) * 15;
				UpdateAnim(level1.redBubbles[i].anim, dt);

				float dist = sqrt(pow(player.x - level1.redBubbles[i].x, 2) + pow(player.y + 40 - level1.redBubbles[i].y, 2));
				level1.redBubbles[i].showHint = (dist < 150);
				if (player.isAttacking && dist < 50) {
					level1.redBubbles[i].active = false;
					AddScore(150);
					// iScreenShake(0.5f); // Removed as requested
					screenFreezeTimer = 0.3f;
					if (level1.redBubbles[i].isKey) {
						InitFlowPuzzle(); 
						gameState = STATE_PUZZLE_FLOW;
						player.isAttacking = false;
					}
					PlayBubblePopSound();
				}
			}
		}
		
		// Vine blockage
		/*
		if (level1.vinesActive && player.x > 1950) {
			player.x = 1950;
		}
		*/

		// Gate 2 Logic
		if (flowPuzzle.isSolved && level1.gate2Y < 500) {
			level1.gate2Y += 100 * dt;
		}

		// Gate 2 Collision
		if (level1.gate2Y < 400 && player.x > level1.gate2X - 100) {
			player.x = level1.gate2X - 100;
		}

		// Level Complete Trigger
		if (player.x > level1.gate2X + 50) {
			gameState = STATE_LEVEL_COMPLETED;
		}
	}

	// Gate Logic
	if (isGateUnlocked && level1.gateY < 600) {
		level1.gateY += 100 * dt; // Faster opening
	}

	// Collision Gate (block player)
	// Block if locked OR if still opening (below 400 height)
	if (currentLocation == 1) {
		if ((!isGateUnlocked || level1.gateY < 400) && player.x > 2100) {
			player.x = 2100;
		}
		// Transition check
		if (isGateUnlocked && level1.gateY >= 500 && player.x > 2500) {
			isTransitioning = true;
		}
	}

}

// FixedUpdate is called periodically by iGraphics for logic/physics
void fixedUpdate() {
	// Keyboard Input Handling (Polling)

	// 'M' to return to menu
	if (isKeyPressed('m') || isKeyPressed('M')) {
		if (gameState == STATE_PLAY || gameState == STATE_PUZZLE || gameState == STATE_PUZZLE_FLOW || gameState == STATE_PUZZLE_STATUE_ROTATION || gameState == STATE_PUZZLE_EYE_ALIGNMENT || gameState == STATE_PUZZLE_MOON_SEAL) SaveGame();
		if (gameState != STATE_NAME_INPUT) { // Don't intercept 'M' during name typing
			StopRunSound();
			gameState = STATE_MENU;
		}
	}

	UpdateSkipMechanism(0.016f);

	// ESC to Exit or Back to Menu
	if (isKeyPressed(27)) {
		if (gameState == STATE_MENU) exit(0);
		else if (gameState == STATE_SLIDESHOW) {
			// Skip entire slideshow
			gameState = STATE_PLAY;
			PlayBackgroundMusic();
		}
		else {
		if (gameState == STATE_PLAY || gameState == STATE_PUZZLE || gameState == STATE_PUZZLE_FLOW || gameState == STATE_PUZZLE_EYE_ALIGNMENT || gameState == STATE_PUZZLE_MOON_SEAL) SaveGame();
			StopRunSound();
			gameState = STATE_MENU;
		}
	}

	// Space or Click to skip slide (Handled in iMouse for click, here for Space)
	if (gameState == STATE_SLIDESHOW && isKeyPressed(' ')) {
		SkipSlide();
	}

	if (gameState == STATE_PLAY) {
		if (currentLevel == 1) UpdatePlay(0.016f);
		else if (currentLevel == 2) UpdateLevel2(0.016f);
		else if (currentLevel == 3) UpdateLevel3(0.016f);
		UpdateEffects(0.016f);

		// Level Timer countdown
		if (levelTimer > 0) {
			levelTimer -= 0.016f;
			if (levelTimer <= 0) {
				levelTimer = 0;
				gameState = STATE_GAMEOVER;
				gameOverTimer = 0;
				PlayGameOverMusic();
			}
		}
	}

	if (gameState == STATE_PUZZLE) {
		UpdatePuzzle(0.016f);
		if (isKeyPressed(27)) gameState = STATE_PLAY; // Cancel puzzle
		if (isKeyPressed('s') || isKeyPressed('S')) {
			puzzle.isSolved = true;
			for (int i = 0; i < 12; i++) puzzle.cards[i].isMatched = true;
			AddScore(500);
			energyWaveRadius = 1.0f;
			isGateUnlocked = true;
			gameState = STATE_PLAY;
		}
	}

	if (gameState == STATE_PUZZLE_FLOW) {
		UpdateFlowPuzzle(0.016f);
		if (isKeyPressed(27)) gameState = STATE_PLAY; // Cancel puzzle
		if (isKeyPressed('s') || isKeyPressed('S')) {
			flowPuzzle.isSolved = true;
			AddScore(500);
			level1.vinesActive = false;
			gameState = STATE_PLAY;
		}
	}

	if (gameState == STATE_PUZZLE_SCALE) {
		UpdateScalePuzzle(0.016f);
		if (isKeyPressed(27)) gameState = STATE_PLAY; // Cancel puzzle
		if (isKeyPressed('s') || isKeyPressed('S')) {
			scalePuzzle.isSolved = true;
			AddScore(1000);
			level2.gate2Y = level2.gateY;
			isGateUnlocked = true;
			gameState = STATE_PLAY;
		}
	}

	if (gameState == STATE_PUZZLE_STATUE_ROTATION) {
		UpdateStatueRotationPuzzle(0.016f);
	}

	if (gameState == STATE_PUZZLE_EYE_ALIGNMENT) {
		UpdateEyeAlignmentPuzzle(0.016f);
		if (isKeyPressed(27)) gameState = STATE_PLAY;
		if (isKeyPressed('s') || isKeyPressed('S')) {
			eyeAlignmentPuzzle.isSolved = true;
			AddScore(1500);
			isGateUnlocked = true;
			gameState = STATE_PLAY;
		}
	}

	if (gameState == STATE_PUZZLE_MOON_SEAL) {
		UpdateMoonSealPuzzle(0.016f);
		if (isKeyPressed(27)) gameState = STATE_PLAY;
		if (isKeyPressed('s') || isKeyPressed('S')) {
			moonSealPuzzle.isSolved = true;
			AddScore(2000);
			level3.finalDoorOpen = true;
			gameState = STATE_PLAY;
		}
	}

	// END to Exit
	if (isSpecialKeyPressed(GLUT_KEY_END)) {
		exit(0);
	}

	// ENTER for Next Level
	static bool enterPrev = false;
	bool enterDown = isKeyPressed(13); // 13 is Enter
	if (enterDown && !enterPrev) {
		if (gameState == STATE_LEVEL_COMPLETED) {
			if (currentLevel == 1) {
				currentLevel = 2;
				LoadLevel2();
				gameState = STATE_PLAY;
				PlayBackgroundMusic();
				printf("Transitioning to Level 2...\n");
			} else if (currentLevel == 2) {
				currentLevel = 3;
				LoadLevel3();
				gameState = STATE_PLAY;
				PlayBackgroundMusic();
				printf("Transitioning to Level 3...\n");
			} else {
				gameState = STATE_MENU;
				printf("Returning to Menu...\n");
			}
		}
	}
	enterPrev = enterDown;
}

// Update animations
void AnimationUpdate() {
	globalTime += 0.1f;
	titleGlow += titleGlowSpeed;

	if (gameState == STATE_MENU) {
		for (int i = 0; i < numButtons; i++) {
			// Floating effect: Sine wave based on time and index
			menuButtons[i].floatOffset = sin(globalTime + i) * 5.0f;
		}
	}
	
	if (gameState == STATE_SETTINGS) {
		for (int i = 0; i < 4; i++) {
			settingsButtons[i].floatOffset = sin(globalTime + i) * 5.0f;
		}
	}

	if (gameState == STATE_SCOREBOARD || gameState == STATE_LEADERBOARD_CONFIRM) {
		for (int i = 0; i < 2; i++) {
			lbButtons[i].floatOffset = sin(globalTime + i) * 3.0f;
			confirmButtons[i].floatOffset = sin(globalTime + i + 2) * 3.0f;
		}
	}

	if (gameState == STATE_SLIDESHOW) {
		UpdateSlideshow();
	}

	// === PAUSED — skip all game logic ===
	if (gameState == STATE_PAUSED) {
		return;
	}

	if (gameState == STATE_GAMEOVER) {
		float dt = 0.016f; 
		gameOverTimer += dt;
		if (gameOverTimer >= 3.0f) {
			player.lives--;
			if (player.lives > 0) {
				if (currentLevel == 1) LoadLevel1();
				else if (currentLevel == 2) LoadLevel2();
				else if (currentLevel == 3) LoadLevel3();

				gameState = STATE_PLAY;
				PlayBackgroundMusic();
			} else {
				if (!leaderboardScoreSaved) {
					SaveLeaderboardEntry(playerName, currentScore);
					leaderboardScoreSaved = true;
				}
				leaderboard.loaded = false;
				gameState = STATE_SCOREBOARD;
			}
		}
	}
}


#endif // GAMELOOP_HPP
