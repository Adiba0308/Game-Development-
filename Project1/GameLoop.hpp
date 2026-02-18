#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"
#include "Audio.hpp"
#include "Level1.hpp" // For LoadLevel1, UpdateBubbles, UpdateFirePillar, iScreenShake
#include "Slideshow.hpp" // For UpdateSlideshow, SkipSlide
#include "Puzzles.hpp" // For UpdatePuzzle, UpdateFlowPuzzle, InitPuzzle, InitFlowPuzzle
#include "SaveLoad.hpp"

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
	float speed = 300.0f;
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

	// Automatic Reset moved to animation update block for reliability

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
				}
			}
		}
		
		// Vine blockage
		if (level1.vinesActive && player.x > 1950) {
			player.x = 1950;
		}

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
		if (gameState == STATE_PLAY || gameState == STATE_PUZZLE || gameState == STATE_PUZZLE_FLOW) SaveGame();
		gameState = STATE_MENU;
	}

	// ESC to Exit or Back to Menu
	if (isKeyPressed(27)) {
		if (gameState == STATE_MENU) exit(0);
		else if (gameState == STATE_SLIDESHOW) {
			// Skip entire slideshow
			gameState = STATE_PLAY;
		}
		else {
			if (gameState == STATE_PLAY || gameState == STATE_PUZZLE || gameState == STATE_PUZZLE_FLOW) SaveGame();
			gameState = STATE_MENU;
		}
	}

	// Space or Click to skip slide (Handled in iMouse for click, here for Space)
	if (gameState == STATE_SLIDESHOW && isKeyPressed(' ')) {
		SkipSlide();
	}

	if (gameState == STATE_PLAY) {
		UpdatePlay(0.016f);
		UpdateEffects(0.016f);
	}

	if (gameState == STATE_PUZZLE) {
		UpdatePuzzle(0.016f);
		if (isKeyPressed(27)) gameState = STATE_PLAY; // Cancel puzzle
	}

	if (gameState == STATE_PUZZLE_FLOW) {
		UpdateFlowPuzzle(0.016f);
		if (isKeyPressed(27)) gameState = STATE_PLAY; // Cancel puzzle
	}

	// END to Exit
	if (isSpecialKeyPressed(GLUT_KEY_END)) {
		exit(0);
	}
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
		for (int i = 0; i < 2; i++) {
			settingsButtons[i].floatOffset = sin(globalTime + i) * 5.0f;
		}
	}

	if (gameState == STATE_SLIDESHOW) {
		UpdateSlideshow();
	}

	if (gameState == STATE_GAMEOVER) {
		float dt = 0.016f; 
		gameOverTimer += dt;
		if (gameOverTimer >= 3.0f) {
			LoadLevel1();

			gameState = STATE_PLAY;
			PlayBackgroundMusic();
		}
	}
}

#endif // GAMELOOP_HPP
