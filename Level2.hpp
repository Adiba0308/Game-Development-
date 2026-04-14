#ifndef LEVEL2_HPP
#define LEVEL2_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"

// Forward declaration from Level1/iMain
extern void ILOADIMAGE(char*);

void LoadLevel2() {
	// Backgrounds (Location 1 and Location 2)
	level2.bgImgs[0] = iLoadImage("asset/background/level.2/b1.1.png");
	level2.bgImgs[1] = iLoadImage("asset/background/level.2/b1.2.png");
	level2.bgImgs[2] = iLoadImage("asset/background/level.2/b1.3.png");
	level2.bgImgs[3] = iLoadImage("asset/background/level.2/b1.4.png");

	StopRunSound();

	level2.parallaxSpeeds[0] = 0.3f; // Far
	level2.parallaxSpeeds[1] = 1.0f; // Fore (Ground speed)

	level2.cameraX = 0;
	level2.levelWidth = SCREEN_WIDTH * 2; // Location 1 is 2 screens wide
	level2.groundY = 100;
	level2.gateY = level2.groundY;
	level2.gateTexture = iLoadImage("asset/gate/g.1.png");
	levelTextTimer = 1.0f; 
	levelTimer = 180.0f;
	currentLocation = 1; // Restart at Location 1 of Level 2
	
	// Reset Player
	player.x = 200;
	player.y = level2.groundY;
	player.width = 80;
	player.height = 80;
	player.isFacingRight = true;
	player.isMoving = false;
	player.health = 100;
	player.isAttacking = false;
	player.isJumping = false;
	player.jumpVel = 0;
	player.jumpCount = 0;
	player.spacePrev = false;

	// Load Spike Traps (Retractable)
	level2.numSpikes = 5;
	float spikeXs[5] = { 600, 1000, 1400, 1800, 2100 };
	for (int i = 0; i < level2.numSpikes; i++) {
		level2.spikeTraps[i].x = spikeXs[i];
		level2.spikeTraps[i].y = level2.groundY;
		level2.spikeTraps[i].timer = (i % 2) * 1.5f; // Offset timers
		level2.spikeTraps[i].extended = false;
		// Wait, LoadAnim takes folder and prefix and builds the string. "asset/spike", "s", 4.
		// Wait, the Level 1 code just loaded 4 images into `level1.spikeAnimTex`. We can do the same to save memory or use LoadAnim.
		// LoadAnim("spike traps", "s", 4, 0.1f) works directly.
		LoadAnim(level2.spikeTraps[i].anim, (char*)"spike traps", (char*)"s", 4, 0.15f, true);
	}

	// Load Collectibles
	level2.numCollectibles = 5;
	// 0=Gold, 1=Stone, 2=Skull, 3=Diamond, 4=Truth
	int types[5] = { 0, 1, 2, 3, 4 };
	int weights[5] = { 4, 3, 2, 2, 5 }; // Based on Truth (5) > Diamond (2)
	float colXs[5] = { 500, 800, 1200, 1600, 2000 };
	
	unsigned int texs[5];
	texs[0] = iLoadImage((char*)"asset/weights/gold relic.png");
	texs[1] = iLoadImage((char*)"asset/weights/stone weight.png");
	texs[2] = iLoadImage((char*)"asset/weights/skull surface.png");
	texs[3] = iLoadImage((char*)"asset/weights/diamond.png");
	texs[4] = iLoadImage((char*)"asset/weights/truth tablet.png");

	for (int i = 0; i < level2.numCollectibles; i++) {
		level2.collectibles[i].type = types[i];
		level2.collectibles[i].weight = weights[i];
		level2.collectibles[i].x = colXs[i];
		level2.collectibles[i].y = level2.groundY + 40; // Hover slightly above ground
		level2.collectibles[i].collected = false;
		level2.collectibles[i].placedOnScale = false;
		level2.collectibles[i].scaleSide = 0;
		level2.collectibles[i].showHint = false;
		level2.collectibles[i].texture = texs[i];
	}

	// Bubbles
	level2.numBubbles = 3;
	float bubbleXs[3] = { 700, 1300, 1900 };
	float bubbleYs[3] = { 300, 400, 350 };
	for (int i = 0; i < level2.numBubbles; i++) {
		level2.bubbles[i].x = bubbleXs[i];
		level2.bubbles[i].y = bubbleYs[i];
		level2.bubbles[i].baseY = bubbleYs[i];
		level2.bubbles[i].active = true;
		level2.bubbles[i].showHint = false;
		LoadAnim(level2.bubbles[i].anim, "bubble", "bb", 4, 0.15f);
	}

	level2.gate2X = 3200; // End of Loc 2 roughly
	level2.gate2Y = level2.groundY;
	statuePuzzle.isSolved = false;
	isGateUnlocked = false; // Lock Level 1 Gate on entering Level 2

	// Load Wrecking Balls (Location 2 only)
	level2.numWreckingBalls = 6;
	for (int i = 0; i < level2.numWreckingBalls; i++) {
		level2.wreckingBalls[i].x = -1000; // Hidden initially
		level2.wreckingBalls[i].y = level2.groundY + 450; // Ceiling anchor
		level2.wreckingBalls[i].timer = i * 1.5f;
		level2.wreckingBalls[i].chainLength = 300.0f;
		level2.wreckingBalls[i].swingRange = 1.0f + (i % 2) * 0.3f; // Max angle (~60-75 degrees)
		level2.wreckingBalls[i].swingSpeed = 2.0f + (i % 3) * 0.5f;
		level2.wreckingBalls[i].radius = 45.0f;
		level2.wreckingBalls[i].texture = iLoadImage((char*)"asset/trap/wrecking_ball.png");
	}

	// Load Chests
	level2.numChests = 3;
	for (int i = 0; i < level2.numChests; i++) {
		level2.chests[i].x = -1000;
		level2.chests[i].y = level2.groundY;
		level2.chests[i].opened = false;
		level2.chests[i].showHint = false;
		level2.chests[i].texClosed = iLoadImage((char*)"asset/treasure chest/t.1.png");
		level2.chests[i].texOpen = iLoadImage((char*)"asset/treasure chest/t.2.png");
	}
}

void DrawSpikeTraps() {
	for (int i = 0; i < level2.numSpikes; i++) {
		if (currentLocation == 1 && level2.spikeTraps[i].x > level2.levelWidth) continue;
		
		float sx = level2.spikeTraps[i].x - level2.cameraX;
		float sy = level2.spikeTraps[i].y;

		// Draw Spikes
		if (level2.spikeTraps[i].extended) {
			unsigned int currentTex = level2.spikeTraps[i].anim.textures[level2.spikeTraps[i].anim.currentFrame];
			// Drawn 100% larger (160x160 instead of 80x80)
			iShowImage(sx - 80, sy, 160, 160, currentTex);
		} else {
			// Draw just a small crack or base when retracted so the player knows where it is
			SetColor(30, 30, 35, 255);
			iFilledRectangle(sx - 60, sy - 5, 120, 10);
		}
	}
}

void DrawWreckingBalls() {
	if (currentLocation != 2) return;
	for (int i = 0; i < level2.numWreckingBalls; i++) {
		if (level2.wreckingBalls[i].x < 0) continue; // Disabled

		float sx = level2.wreckingBalls[i].x - level2.cameraX;

		float sy = level2.wreckingBalls[i].y;
		float angle = level2.wreckingBalls[i].angle;
		float length = level2.wreckingBalls[i].chainLength;
		
		float bx = sx + sin(angle) * length;
		float by = sy - cos(angle) * length;

		// Draw Chain
		SetColor(50, 50, 50, 255);
		glLineWidth(4.0f);
		iLine(sx, sy, bx, by);
		glLineWidth(1.0f);

		// Anchor
		SetColor(80, 80, 80, 255);
		iFilledRectangle(sx - 15, sy - 5, 30, 10);

		// Wrecking Ball
		if (level2.wreckingBalls[i].texture != 0) {
			glPushMatrix();
			glTranslatef(bx, by, 0);
			glRotatef(-angle * 180.0f / 3.14159f, 0, 0, 1);
			iShowImage(-level2.wreckingBalls[i].radius, -level2.wreckingBalls[i].radius, level2.wreckingBalls[i].radius*2, level2.wreckingBalls[i].radius*2, level2.wreckingBalls[i].texture);
			glPopMatrix();
		} else {
			// Fallback placeholder
			SetColor(80, 80, 80, 255);
			iFilledCircle(bx, by, level2.wreckingBalls[i].radius);
			SetColor(40, 40, 40, 255);
			for(int a=0; a<8; a++) {
				float px = bx + cos(a * 3.14159f / 4) * (level2.wreckingBalls[i].radius + 5);
				float py = by + sin(a * 3.14159f / 4) * (level2.wreckingBalls[i].radius + 5);
				iLine(bx, by, px, py);
			}
		}
	}
}

void DrawChests() {
	if (currentLocation != 2) return;
	for (int i = 0; i < level2.numChests; i++) {
		if (level2.chests[i].x < 0) continue;

		float cx = level2.chests[i].x - level2.cameraX;
		float cy = level2.chests[i].y;
		
		unsigned int tex = level2.chests[i].opened ? level2.chests[i].texOpen : level2.chests[i].texClosed;
		
		if (tex != 0) {
			iShowImage(cx - 30, cy, 60, 60, tex);
		} else {
			SetColor(120, 80, 30, 255);
			iFilledRectangle(cx - 30, cy, 60, 40);
			if (level2.chests[i].opened) {
				SetColor(200, 200, 50, 255); // "Gold" inside
				iFilledRectangle(cx - 20, cy+10, 40, 20);
			}
		}

		if (level2.chests[i].showHint && !level2.chests[i].opened) {
			SetColor(255, 255, 255, 200);
			iText(cx - 30, cy + 50, "PRESS E TO OPEN", GLUT_BITMAP_HELVETICA_12);
		}
	}
}

void DrawCollectibleIcon(float cx, float cy, int type, float scale = 1.0f) {
	float w = 40 * scale;
	float h = 40 * scale;
	
	// We lookup the texture from the collectibles array assuming index == type
	// since we initialize type 0 to 4 in order.
	unsigned int tex = 0;
	if (type >= 0 && type < level2.numCollectibles) {
		tex = level2.collectibles[type].texture;
	}
	
	if (tex != 0) {
		iShowImage(cx - w / 2, cy - h / 2, w, h, tex);
	}
}

void DrawCollectibles() {
	if (currentLocation != 1) return; // Only in loc 1
	for (int i = 0; i < level2.numCollectibles; i++) {
		if (!level2.collectibles[i].collected) {
			float cx = level2.collectibles[i].x - level2.cameraX;
			float cy = level2.collectibles[i].y + sin(globalTime * 3 + i) * 10.0f; // Hover effect

			// Glow
			float glow = (sin(globalTime * 5 + i) + 1) / 2.0f;
			SetColor(255, 255, 100, (int)(50 + glow * 50));
			iFilledCircle(cx, cy, 30);

			DrawCollectibleIcon(cx, cy, level2.collectibles[i].type);

			if (level2.collectibles[i].showHint) {
				SetColor(255, 255, 255, 200);
				iText(cx - 30, cy + 40, "PRESS F TO COLLECT", GLUT_BITMAP_HELVETICA_12);
			}
		}
	}
}

void DrawLevel2Bubbles() {
	float camX = level2.cameraX;
	for (int i = 0; i < level2.numBubbles; i++) {
		if (level2.bubbles[i].active) {
			float bx = level2.bubbles[i].x - camX;
			float by = level2.bubbles[i].y;

			int currentTex = level2.bubbles[i].anim.textures[level2.bubbles[i].anim.currentFrame];
			iShowImage(bx - 40, by - 40, 80, 80, currentTex);

			if (level2.bubbles[i].showHint) {
				SetColor(255, 255, 255, 200);
				iText(bx - 50, by + 50, "PRESS F TO POP", GLUT_BITMAP_HELVETICA_12);
			}
		}
	}
}

void DrawLevel2Gate() {
	float gx = 2200 - level2.cameraX;
	float gy = level2.gateY;
	iShowImage(gx - 75, gy, 150, 625, level2.gateTexture);
}

void DrawInventory() {
	// Draw inventory bar at bottom center
	float startX = SCREEN_WIDTH / 2 - (level2.numCollectibles * 60) / 2;
	float startY = 20;

	SetColor(20, 20, 25, 200);
	iFilledRectangle(startX - 20, startY - 10, level2.numCollectibles * 60 + 40, 60);
	SetColor(200, 180, 100, 255);
	iRectangle(startX - 20, startY - 10, level2.numCollectibles * 60 + 40, 60);

	for (int i = 0; i < level2.numCollectibles; i++) {
		float slotX = startX + i * 60;
		float slotY = startY;

		// Slot background
		SetColor(50, 50, 60, 255);
		iFilledRectangle(slotX, slotY, 40, 40);
		SetColor(100, 100, 110, 255);
		iRectangle(slotX, slotY, 40, 40);

		if (level2.collectibles[i].collected && !level2.collectibles[i].placedOnScale) {
			DrawCollectibleIcon(slotX + 20, slotY + 20, level2.collectibles[i].type, 0.8f);
		}
	}
	SetColor(255, 255, 255, 200);
	iText(startX + 100, startY + 60, "INVENTORY", GLUT_BITMAP_HELVETICA_12);
}

void DrawLevel2Location2Elements() {
	// Location 2 gate
	float gx = level2.gate2X - level2.cameraX;
	float gy = level2.gate2Y;
	iShowImage(gx - 75, gy, 150, 625, level2.gateTexture);
}

void DrawLevel2() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float shakeY = (screenShakeTimer > 0) ? (rand() % (int)(shakeIntensity * 2) - shakeIntensity) : 0;
	float shakeX = (screenShakeTimer > 0) ? (rand() % (int)(shakeIntensity * 2) - shakeIntensity) : 0;
	glPushMatrix();
	glTranslatef(shakeX, shakeY, 0);

	// Draw Backgrounds - single scrolling layer with two alternating images
	int startIndex = (currentLocation == 1) ? 0 : 2;
	float totalW = 2.0f * SCREEN_WIDTH;

	float startX = -fmod(level2.cameraX, totalW);
	while (startX > 0) startX -= totalW;
	while (startX <= -totalW) startX += totalW;

	for (int i = 0; i < 4; i++) {
		float drawX = startX + (i * SCREEN_WIDTH);
		if (drawX + SCREEN_WIDTH > 0 && drawX < SCREEN_WIDTH) {
			iShowImage((int)drawX, 0, SCREEN_WIDTH, SCREEN_HEIGHT, level2.bgImgs[startIndex + (i % 2)]);
		}
	}

	if (currentLocation == 1) {
		DrawLevel2Gate();
		DrawSpikeTraps();
		DrawCollectibles();
		DrawLevel2Bubbles();
	} else {
		DrawSpikeTraps(); // Spikes can appear in Loc 2 too
		DrawLevel2Bubbles();
		DrawWreckingBalls();
		DrawChests();
		DrawLevel2Location2Elements();
	}

	// Draw Player
	unsigned int currentTex;
	if (player.isAttacking) {
		currentTex = player.attack.textures[player.attack.currentFrame];
	}
	else if (player.isJumping) {
		currentTex = player.jump.textures[player.jump.currentFrame];
	}
	else if (player.isMoving) {
		currentTex = player.isFacingRight ? player.runRight.textures[player.runRight.currentFrame] : player.runLeft.textures[player.runLeft.currentFrame];
	}
	else {
		currentTex = player.idle.textures[player.idle.currentFrame];
	}

	iShowImage(player.x - level2.cameraX - 40, player.y, 80, 80, currentTex);

	// Gate/Bridge Transition
	glPopMatrix();

	// Level Start Text
	if (levelTextTimer > 0) {
		SetColor(200, 200, 220, 255); // Light shade matching background
		// Dynamically display location name
		char locName[50];
		sprintf_s(locName, "LEVEL 2");
		DrawStrokeText(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 0, locName, 0.8f, 5.0f);
	}

	// Health Bar (Top Left)
	SetColor(50, 50, 50, 200);
	iFilledRectangle(20, SCREEN_HEIGHT - 60, 200, 30);
	SetColor(255, 0, 0, 255);
	if (player.health < 0) player.health = 0;
	iFilledRectangle(20, SCREEN_HEIGHT - 60, (int)(2 * player.health), 30);
	SetColor(255, 255, 255, 255);
	iRectangle(20, SCREEN_HEIGHT - 60, 200, 30);
	iText(30, SCREEN_HEIGHT - 50, "HEALTH", GLUT_BITMAP_HELVETICA_12);
	
	char potStr[64];
	sprintf_s(potStr, "POTIONS: %d (Press H)", player.healthPotions);
	SetColor(200, 255, 200, 255);
	iText(230, SCREEN_HEIGHT - 50, potStr, GLUT_BITMAP_HELVETICA_12);
	
	if (player.potionCollectTimer > 0) {
		SetColor(255, 255, 0, 255);
		iText(230, SCREEN_HEIGHT - 30, "COLLECTED!", GLUT_BITMAP_HELVETICA_12);
	}

	// Score Display (Top Right)
	char scoreStr[32];
	sprintf_s(scoreStr, "SCORE: %ld", currentScore);
	SetColor(255, 215, 0, 255); // Gold color
	iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);

	// Lives Display (Top Right below Score)
	char livesStr[32];
	sprintf_s(livesStr, "LIVES: %d", player.lives);
	SetColor(255, 100, 100, 255); // Red color
	iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 80, livesStr, GLUT_BITMAP_HELVETICA_18);

	// Timer Display
	int timerMins2 = (int)(levelTimer / 60);
	int timerSecs2 = (int)(levelTimer) % 60;
	char timerStr2[20];
	sprintf_s(timerStr2, "%d:%02d", timerMins2, timerSecs2);
	if (levelTimer < 30) SetColor(255, 50, 50, 255);
	else SetColor(200, 200, 255, 255);
	iText(SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT - 50, timerStr2, GLUT_BITMAP_TIMES_ROMAN_24);

	DrawInventory();

	// Draw Potion Notification over character
	if (player.potionCollectTimer > 0) {
		SetColor(255, 255, 255, 255);
		iText(player.x - 30, player.y + 120, "+1 HEALTH POTION", GLUT_BITMAP_HELVETICA_12);
	}

	glDisable(GL_BLEND);
}

void UpdateLevel2(float dt) {
	if (player.potionCollectTimer > 0) player.potionCollectTimer -= dt;

	if (isTransitioning) {
		transitionAlpha += 2.0f * dt;
		if (transitionAlpha >= 1.0f) {
			transitionAlpha = 1.0f;
			if (currentLocation == 1) {
				currentLocation = 2;
				player.x = 100;
				player.y = level2.groundY;
				level2.cameraX = 0;
				level2.levelWidth = SCREEN_WIDTH * 3; // Loc 2 is 3 screens wide
				
				// Reposition a few spikes for Loc 2
				level2.spikeTraps[0].x = 600;
				level2.spikeTraps[1].x = 1200;
				level2.spikeTraps[2].x = 1800;
				for(int i=3; i<5; i++) level2.spikeTraps[i].x = -1000; // Disable others

				// Reposition Wrecking Balls for Loc 2
				level2.wreckingBalls[0].x = 700; level2.wreckingBalls[0].chainLength = 300; level2.wreckingBalls[0].swingSpeed = 2.5f;
				level2.wreckingBalls[1].x = 1400; level2.wreckingBalls[1].chainLength = 400; level2.wreckingBalls[1].swingSpeed = 1.5f;
				level2.wreckingBalls[2].x = 2200; level2.wreckingBalls[2].chainLength = 250; level2.wreckingBalls[2].swingSpeed = 3.5f;
				level2.wreckingBalls[3].x = 2800; level2.wreckingBalls[3].chainLength = 350; level2.wreckingBalls[3].swingSpeed = 2.0f;
				for(int i=4; i<level2.numWreckingBalls; i++) level2.wreckingBalls[i].x = -1000; // Disable

				// Reposition Bubbles for Loc 2
				level2.bubbles[0].x = 900; level2.bubbles[0].active = true;
				level2.bubbles[1].x = 1700; level2.bubbles[1].active = true;
				level2.bubbles[2].x = 2500; level2.bubbles[2].active = true;

				// Reposition Chests
				level2.chests[0].x = 1000; level2.chests[0].opened = false;
				level2.chests[1].x = 1900; level2.chests[1].opened = false;
				level2.chests[2].x = 2600; level2.chests[2].opened = false;
			}
			isTransitioning = false;
		}
		return;
	}
	if (transitionAlpha > 0) transitionAlpha -= 2.0f * dt;

	if (levelTextTimer > 0) levelTextTimer -= dt;
	if (screenFreezeTimer > 0) { screenFreezeTimer -= dt; return; }

	// Health Potion Usage ('H')
	static bool hPrev2 = false;
	bool hDown2 = isKeyPressed('h') || isKeyPressed('H');
	if (hDown2 && !hPrev2) {
		if (player.healthPotions > 0 && player.health < 100) {
			player.healthPotions--;
			player.health += 30;
			if (player.health > 100) player.health = 100;
		}
	}
	hPrev2 = hDown2;

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

	// Health Death -> Level 2 Restart
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

		if (player.y <= level2.groundY) {
			player.y = level2.groundY;
			player.isJumping = false;
			player.jumpVel = 0;
			player.jumpCount = 0;
		}
	}

	// Run Sound Logic with Grace Period
	static bool isRunSoundPlaying = false;
	static int runGraceFrames = 0;
	if (player.isMoving && !player.isJumping) {
		runGraceFrames = 10; // Reset grace period
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
	if (player.x > level2.levelWidth - 40) player.x = level2.levelWidth - 40;

	// Camera Follow
	level2.cameraX = player.x - SCREEN_WIDTH / 2;
	if (level2.cameraX < 0) level2.cameraX = 0;
	if (level2.cameraX > level2.levelWidth - SCREEN_WIDTH) level2.cameraX = level2.levelWidth - SCREEN_WIDTH;

	// Animation Update
	UpdateAnim(player.idle, dt);
	UpdateAnim(player.runRight, dt);
	UpdateAnim(player.runLeft, dt);
	if (player.isJumping) UpdateAnim(player.jump, dt);
	if (player.isAttacking) {
		float prevTimer = player.attack.timer;
		UpdateAnim(player.attack, dt);
		if (player.attack.timer < prevTimer) player.isAttacking = false;
	}

	// Attack Trigger ('F')
	bool fPressed = (isKeyPressed('f') != 0) || (isKeyPressed('F') != 0) || isSpecialKeyPressed(GLUT_KEY_DOWN);
	if (fPressed && !player.isAttacking) {
		player.isAttacking = true;
		player.attack.currentFrame = 0;
		player.attack.timer = 0;
	}

	// Update Spike Traps
	for (int i = 0; i < level2.numSpikes; i++) {
		if (currentLocation == 1 && level2.spikeTraps[i].x > level2.levelWidth) continue;
		
		level2.spikeTraps[i].timer += dt;
		if (level2.spikeTraps[i].timer > 3.0f) {
			level2.spikeTraps[i].extended = !level2.spikeTraps[i].extended;
			level2.spikeTraps[i].timer = 0.0f;
		}

		// Collision & Anim logic
		if (level2.spikeTraps[i].extended) {
			UpdateAnim(level2.spikeTraps[i].anim, dt);
			if (abs(player.x - level2.spikeTraps[i].x) < 80 && player.y <= level2.groundY + 160) {
				player.health -= 30 * dt; // Damage over time if standing on it
			}
		}
	}

	if (currentLocation == 1) {
		// Update Collectibles
		for (int i = 0; i < level2.numCollectibles; i++) {
			if (!level2.collectibles[i].collected) {
				float dist = sqrt(pow(player.x - level2.collectibles[i].x, 2) + pow(player.y - level2.collectibles[i].y, 2));
				level2.collectibles[i].showHint = (dist < 100);
				
				if (player.isAttacking && dist < 50) {
					level2.collectibles[i].collected = true;
					AddScore(50);
					PlayCollectSound();
					// player.isAttacking = false; // Removed to let animation finish naturally
					
					bool allPopped = true;
					bool allCollected = true;
					for (int j = 0; j < level2.numBubbles; j++) if (level2.bubbles[j].active) allPopped = false;
					for (int k = 0; k < level2.numCollectibles; k++) if (!level2.collectibles[k].collected) allCollected = false;
					if (allPopped && allCollected) { 
						StopRunSound();
						InitScalePuzzle(); 
						gameState = STATE_PUZZLE_SCALE; 
					}
				}
			}
		}

		// Update Bubbles
		for (int i = 0; i < level2.numBubbles; i++) {
			if (level2.bubbles[i].active) {
				level2.bubbles[i].y = level2.bubbles[i].baseY + sin(globalTime * 2 + i) * 15;
				UpdateAnim(level2.bubbles[i].anim, dt);
				
				float dist = sqrt(pow(player.x - level2.bubbles[i].x, 2) + pow(player.y + 40 - level2.bubbles[i].y, 2));
				level2.bubbles[i].showHint = (dist < 150);
				// Added leeway to the bubble hit detection. Level 1 distances used (dist < 50) and sometimes (dist < 80).
				// Since player's bounding box is wide, we will allow dist < 80 for more generous popping.
				if (player.isAttacking && dist < 80) {
					level2.bubbles[i].active = false;
					AddScore(100);
					screenFreezeTimer = 0.5f;
					// player.isAttacking = false; // Let animation finish naturally
					
					// Check if all popped AND all items collected
					bool allPopped = true;
					bool allCollected = true;
					for (int j = 0; j < level2.numBubbles; j++) if (level2.bubbles[j].active) allPopped = false;
					for (int k = 0; k < level2.numCollectibles; k++) if (!level2.collectibles[k].collected) allCollected = false;
					
					if (allPopped && allCollected) { 
						StopRunSound();
						InitScalePuzzle(); 
						gameState = STATE_PUZZLE_SCALE; 
					}
					PlayBubblePopSound();
				}
			}
		}

		// Gate Logic
		if (isGateUnlocked && level2.gateY < 600) level2.gateY += 100 * dt;

		// Block player
		if ((!isGateUnlocked || level2.gateY < 400) && player.x > 2100) player.x = 2100;
		if (isGateUnlocked && level2.gateY >= 500 && player.x > 2500) isTransitioning = true;
	} else {
		// Location 2 Traps & Chests Update
		
		// Update Wrecking Balls
		for (int i = 0; i < level2.numWreckingBalls; i++) {
			if (level2.wreckingBalls[i].x < 0) continue;
			
			level2.wreckingBalls[i].timer += dt * level2.wreckingBalls[i].swingSpeed;
			level2.wreckingBalls[i].angle = sin(level2.wreckingBalls[i].timer) * level2.wreckingBalls[i].swingRange;

			// Hit detection
			float bx = level2.wreckingBalls[i].x + sin(level2.wreckingBalls[i].angle) * level2.wreckingBalls[i].chainLength;
			float by = level2.wreckingBalls[i].y - cos(level2.wreckingBalls[i].angle) * level2.wreckingBalls[i].chainLength;

			float dist = sqrt(pow(player.x - bx, 2) + pow(player.y + 40 - by, 2));
			if (dist < level2.wreckingBalls[i].radius + 30) {
				player.health -= 50 * dt; // Heavy damage
				screenShakeTimer = 0.1f;
			}
		}

		// Update Chests Interaction (E Key)
		bool ePressed = isKeyPressed('e') != 0 || isKeyPressed('E') != 0;
		for (int i = 0; i < level2.numChests; i++) {
			if (level2.chests[i].x < 0) continue;
			float dist = sqrt(pow(player.x - level2.chests[i].x, 2) + pow(player.y - level2.chests[i].y, 2));
			level2.chests[i].showHint = (dist < 80);
			
			if (level2.chests[i].showHint && !level2.chests[i].opened && ePressed) {
				level2.chests[i].opened = true;
				player.healthPotions++;
				player.potionCollectTimer = 0.5f;
				AddScore(200);
				PlayCollectSound();
			}
		}

		// Location 2 Gate Logic (Puzzle hook)
		if (statuePuzzle.isSolved && level2.gate2Y < 600) level2.gate2Y += 100 * dt; 
		if (player.x > level2.gate2X) {
			if (!statuePuzzle.isSolved) {
				player.x = level2.gate2X - 5;
				// Instead of triggering here, we require popping all bubbles!
				// We can display a hint UI here if they reach the door without popping bubbles.
				SetColor(255, 100, 100, 255);
				iText(SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 + 50, "POP ALL BUBBLES TO UNLOCK DOOR!", GLUT_BITMAP_HELVETICA_18);
			} else if (statuePuzzle.isSolved && level2.gate2Y >= 500 && player.x > level2.gate2X + 150) {
				ApplyTimeBonus();
				gameState = STATE_LEVEL_COMPLETED;
			}
		}

		// Update Bubbles for Loc 2
		for (int i = 0; i < level2.numBubbles; i++) {
			if (level2.bubbles[i].active) {
				level2.bubbles[i].y = level2.bubbles[i].baseY + sin(globalTime * 2 + i) * 15;
				UpdateAnim(level2.bubbles[i].anim, dt);
				
				float dist = sqrt(pow(player.x - level2.bubbles[i].x, 2) + pow(player.y + 40 - level2.bubbles[i].y, 2));
				level2.bubbles[i].showHint = (dist < 150);
				if (player.isAttacking && dist < 80) {
					level2.bubbles[i].active = false;
					AddScore(100);
					screenFreezeTimer = 0.5f;
					
					bool allPopped = true;
					for (int j = 0; j < level2.numBubbles; j++) if (level2.bubbles[j].active) allPopped = false;
					
					if (allPopped) { 
						StopRunSound();
						InitStatueRotationPuzzle(); 
						gameState = STATE_PUZZLE_STATUE_ROTATION; 
					}
					PlayBubblePopSound();
				}
			}
		}
	}
}

#endif // LEVEL2_HPP
