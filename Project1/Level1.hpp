#ifndef LEVEL1_HPP
#define LEVEL1_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"

// Forward declaration from Level1/iMain
extern void ILOADIMAGE(char*); // Not actually needed as we use internal calls

void DrawBladeTraps() {
	// Calculate current frame based on globalTime (approx 10 fps)
	int currentFrame = (int)(globalTime * 10) % 4;

	for (int i = 0; i < 4; i++) {
		if (level1.traps[i].active) {
			float tx = level1.traps[i].x - level1.cameraX;
			float ty = level1.traps[i].y;
			float r = level1.traps[i].radius;
			
			// Draw the current frame centered
			// Radius is 80, so diameter is 160. Adjust size if needed.
			// The procedural one had radius 'r', so width/height = 2*r
			iShowImage(tx - r, ty - r, 2 * r, 2 * r, level1.spikeAnimTex[currentFrame]);
		}
	}
}

void DrawRedBubbles() {
	for (int i = 0; i < 3; i++) {
		if (level1.redBubbles[i].active) {
			float bx = level1.redBubbles[i].x - level1.cameraX;
			float by = level1.redBubbles[i].y;
			
			// Draw Animation (Same size as Level 1 bubbles)
			int currentTex = level1.redBubbles[i].anim.textures[level1.redBubbles[i].anim.currentFrame];
			iShowImage(bx - 40, by - 40, 80, 80, currentTex);

			// Proximity Hint
			float dist = sqrt(pow(player.x - level1.redBubbles[i].x, 2) + pow(player.y + 40 - by, 2));
			if (dist < 150) {
				SetColor(255, 255, 255, 200);
				iText(bx - 50, by + 50, "PRESS F TO POP", GLUT_BITMAP_HELVETICA_12);
			}
		}
	}
}

void DrawLocation2Elements() {
	DrawBladeTraps();
	DrawRedBubbles();
	
	// Draw Gate 2
	float gx = level1.gate2X - level1.cameraX;
	float gy = level1.gate2Y;
	// Draw gate image 25% bigger (same as Location 1)
	iShowImage(gx - 75, gy, 150, 625, level1.gateTexture);
	
	if (level1.vinesActive) {
		// Draw blocked paths (Vines & Thorn Bushes)
		SetColor(30, 80, 20, 230);
		// Massive vine wall at the end of Location 2 placeholder
		iFilledRectangle(2000 - level1.cameraX, 0, 150, 720); 
		SetColor(255, 255, 255, 200);
		iText(2000 - level1.cameraX - 100, 360, "ANCIENT VINES BLOCK THE PATH", GLUT_BITMAP_9_BY_15);
	}
}

void DrawFirePillar() {
	float camX = level1.cameraX;
	float baseW = 128, baseH = 20;

	for (int i = 0; i < 2; i++) {
		if (!firePillars[i].active) continue;

		float px = firePillars[i].x - camX;
		float py = firePillars[i].y;
		float s = firePillars[i].scale;

		// 1. Pedestal (Cracked Stone)
		// Scale pedestal width slightly, height same
		SetColor(60, 60, 70, 255);
		iFilledRectangle(px - (baseW * s) / 2, py, baseW * s, baseH);
		SetColor(40, 40, 50, 255);
		iRectangle(px - (baseW * s) / 2, py, baseW * s, baseH);
		
		// 2. Cyan Runes (Pulsing)
		float runeGlow = (sin(globalTime * 5) + 1) / 2.0f;


		// 3. Fire Animation
		if (firePillars[i].state == FIRE_BURST) {
			unsigned int currentFrame = firePillars[i].fireAnim.textures[firePillars[i].fireAnim.currentFrame];
			// Draw fire image scaled
			iShowImage(px - 60 * s, py + baseH, 120 * s, 250 * s, currentFrame);
			
			// Reflection on ground
			SetColor(255, 100, 0, 50);
			iFilledEllipse(px, py - 10, 100 * s, 20);
		}
		else if (firePillars[i].state == FIRE_IGNITION) {
			SetColor(0, 255, 255, 100);
			iCircle(px, py + baseH + 20, (10 + runeGlow * 10) * s);
		}
	}
}

void UpdateFirePillar(float dt) {
	for (int i = 0; i < 2; i++) {
		if (!firePillars[i].active) continue;

		firePillars[i].timer += dt;
		// Logic remains same (Transitions)
		if (firePillars[i].state == FIRE_IDLE && firePillars[i].timer > 2.0f) {
			firePillars[i].state = FIRE_IGNITION; firePillars[i].timer = 0;
		}
		else if (firePillars[i].state == FIRE_IGNITION && firePillars[i].timer > 0.5f) {
			firePillars[i].state = FIRE_BURST; firePillars[i].timer = 0;
		}
		else if (firePillars[i].state == FIRE_BURST && firePillars[i].timer > 2.0f) {
			firePillars[i].state = FIRE_COOLDOWN; firePillars[i].timer = 0;
		}
		else if (firePillars[i].state == FIRE_COOLDOWN && firePillars[i].timer > 0.5f) {
			firePillars[i].state = FIRE_IDLE; firePillars[i].timer = 0;
		}

		// Update fire animation during FIRE_BURST
		if (firePillars[i].state == FIRE_BURST) {
			UpdateAnim(firePillars[i].fireAnim, dt);
		}
	}
}

void DrawBubbles() {
	float camX = level1.cameraX;
	for (int i = 0; i < 2; i++) {
		if (bubbles[i].active) {
			float bx = bubbles[i].x - camX;
			float by = bubbles[i].y;

			// Draw Animation
			// Original radius was ~30-40, so diameter ~60-80. 
			// Let's us 80x80 centered.
			int currentTex = bubbles[i].anim.textures[bubbles[i].anim.currentFrame];
			iShowImage(bx - 40, by - 40, 80, 80, currentTex);

			// Hint
			if (bubbles[i].showHint) {
				SetColor(255, 255, 255, 200);
				iText(bx - 50, by + 50, "PRESS F TO POP", GLUT_BITMAP_HELVETICA_12);
			}
		}
	}
}

void UpdateBubbles(float dt) {
	for (int i = 0; i < 2; i++) {
		if (bubbles[i].active) {
			// Floating motion
			bubbles[i].y = bubbles[i].baseY + sin(globalTime * 2 + i) * 15;

			// Update Animation
			UpdateAnim(bubbles[i].anim, dt);

			// Interaction: Approach brightness (Logic kept, though brightness not used in Draw currently, functionality preserved)
			float dist = sqrt(pow(player.x - bubbles[i].x, 2) + pow(player.y + 32 - bubbles[i].y, 2));
			if (dist < 150) bubbles[i].brightness = (150 - dist) / 150.0f;
			else bubbles[i].brightness = 0;
		}
	}
}

void DrawGate() {
	float gx = 2200 - level1.cameraX;
	float gy = level1.gateY;
	
	// Draw gate image 25% bigger (150 width, 625 height)
	iShowImage(gx - 75, gy, 150, 625, level1.gateTexture);
}

void LoadLevel1() {
	// Backgrounds
	level1.bgImgs[0] = iLoadImage("asset/background/level.1/b1.1.png");
	level1.bgImgs[1] = iLoadImage("asset/background/level.1/b1.2.png");

	level1.parallaxSpeeds[0] = 0.3f; // Far
	level1.parallaxSpeeds[1] = 1.0f; // Fore (Ground speed)

	level1.cameraX = 0;
	level1.levelWidth = SCREEN_WIDTH * 2; // 2 screens for the 2 images
	level1.groundY = 100;
	level1.gateY = level1.groundY;
	level1.rumblingTimer = 0;
	level1.gateTexture = iLoadImage("asset/gate/g.1.png");
	levelTextTimer = 1.0f; // Show text for 1 second
	levelTextTimer = 1.0f; // Show text for 1 second
	currentScore = 0; // Reset Score
	currentLocation = 1; // Restart at Location 1

	// Player
	player.x = 200;
	player.y = level1.groundY;
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
	level1.gateY = 100;

	LoadAnim(player.idle, "idle", "i", 4, 0.15f);
	LoadAnim(player.runRight, "right run", "rr", 9, 0.1f);
	LoadAnim(player.runLeft, "left run", "lr", 9, 0.1f);
	LoadAnim(player.attack, "attack", "a", 1, 1.0f);
	LoadAnim(player.jump, "jump", "j", 16, 0.05f, true);

	// Fire Pillar 1 (Original)
	firePillars[0].x = 800;
	firePillars[0].y = level1.groundY;
	firePillars[0].state = FIRE_IDLE;
	firePillars[0].scale = 1.0f;
	firePillars[0].timer = 0;
	firePillars[0].active = true;
	LoadAnim(firePillars[0].fireAnim, "fire", "f", 4, 0.1f);

	// Fire Pillar 2 (Large, before 2nd bubble)
	firePillars[1].x = 1700; // Before 1800 (bubble 2)
	firePillars[1].y = level1.groundY;
	firePillars[1].state = FIRE_IDLE;
	firePillars[1].scale = 2.0f; // 100% bigger
	firePillars[1].timer = 1.0f; // Offset timing
	firePillars[1].active = true;
	LoadAnim(firePillars[1].fireAnim, "fire", "f", 4, 0.1f);

	// Bubbles
	bubbles[0] = { 1300, 300, 300, true, false, 0 };
	bubbles[1] = { 1800, 450, 450, true, true, 0 }; // Key bubble
	// Load bubble animations
	for (int i = 0; i < 2; i++) {
		LoadAnim(bubbles[i].anim, "bubble", "bb", 4, 0.15f);
	}

	// --- Location 2 Initialization ---
	level1.loc2BgImgs[0] = iLoadImage("asset/background/level.1/b1.4.png");
	level1.loc2BgImgs[1] = iLoadImage("asset/background/level.1/b1.5.png");
	level1.bladeTexSpin = iLoadImage("asset/traps/blade_spin.png");
	level1.bladeTexStatic = iLoadImage("asset/traps/blade_static.png");
	level1.vinesActive = true;
	level1.checkpointX = 100;
	level1.checkpointY = level1.groundY;

	// Traps
	level1.traps[0] = { 400, 150, 0, true, 120 };
	level1.traps[1] = { 800, 400, 45, true, 120 };
	level1.traps[2] = { 1200, 200, 90, true, 120 };
	level1.traps[3] = { 1600, 500, 0, true, 120 };

	// Red Bubbles
	level1.redBubbles[0] = { 600, 300, 300, true, false, 0 };
	level1.redBubbles[1] = { 1000, 450, 450, true, false, 0 };
	level1.redBubbles[2] = { 1400, 350, 350, true, true, 0 }; // Key red bubble
	// Load bubble animations Location 2 (Blue now)
	for (int i = 0; i < 3; i++) {
		LoadAnim(level1.redBubbles[i].anim, "bubble", "bb", 4, 0.15f);
	}

	level1.gate2X = 2400;
	level1.gate2Y = level1.groundY;
	level1.levelWidth = 2600; // Extend level width for Location 2 gate

	// Load Spike Animation
	level1.spikeAnimTex[0] = iLoadImage("asset/spike/s.1.png");
	level1.spikeAnimTex[1] = iLoadImage("asset/spike/s.2.png");
	level1.spikeAnimTex[2] = iLoadImage("asset/spike/s.3.png");
	level1.spikeAnimTex[3] = iLoadImage("asset/spike/s.4.png");
}

void DrawLevel1() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float shakeY = (level1.rumblingTimer > 0 || screenShakeTimer > 0) ? (rand() % (int)(shakeIntensity * 2) - shakeIntensity) : 0;
	float shakeX = (screenShakeTimer > 0) ? (rand() % (int)(shakeIntensity * 2) - shakeIntensity) : 0;
	glPushMatrix();
	glTranslatef(shakeX, shakeY, 0);

	// Draw Backgrounds with Parallax
	int numBgs = 2;
	unsigned int* imgs = (currentLocation == 1) ? level1.bgImgs : level1.loc2BgImgs;
	
	for (int layer = 0; layer < 2; layer++) {
		float speed = level1.parallaxSpeeds[layer];
		float totalW = 2.0f * SCREEN_WIDTH;

		float startX = -fmod(level1.cameraX * speed, totalW);
		while (startX > 0) startX -= totalW;
		while (startX <= -totalW) startX += totalW;

		for (int i = 0; i < 4; i++) {
			float drawX = startX + (i * SCREEN_WIDTH);
			if (drawX + SCREEN_WIDTH > 0 && drawX < SCREEN_WIDTH) {
				iShowImage((int)drawX, 0, SCREEN_WIDTH, SCREEN_HEIGHT, imgs[i % 2]);
			}
		}
	}

	if (currentLocation == 1) {
		DrawGate();
		DrawFirePillar();
		DrawBubbles();
	} else {
		DrawLocation2Elements();
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

	iShowImage(player.x - level1.cameraX - 40, player.y, 80, 80, currentTex);

	// Energy Wave
	if (energyWaveRadius > 0) {
		SetColor(0, 255, 255, (int)(255 * (1.0f - energyWaveRadius / 1000.0f)));
		iCircle(player.x - level1.cameraX, player.y + 40, energyWaveRadius);
		iCircle(player.x - level1.cameraX, player.y + 40, energyWaveRadius * 0.8f);
	}

	// Gate/Bridge
	if (isGateUnlocked && level1.gateY < 500) {
		// Just labels for debug if needed, logic is in DrawGate
	}

	// UI
	glPopMatrix();

	// Level Start Text
	if (levelTextTimer > 0) {
		SetColor(200, 200, 220, 255); // Light shade matching background
		// Dynamically display location name
		char locName[50];
		sprintf_s(locName, "LEVEL %d", currentLocation);
		DrawStrokeText(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 0, locName, 0.8f, 5.0f);
	}

	// Health Bar (Top Left)
	SetColor(50, 50, 50, 200);
	iFilledRectangle(20, SCREEN_HEIGHT - 60, 200, 30);
	SetColor(255, 0, 0, 255);
	iFilledRectangle(20, SCREEN_HEIGHT - 60, (int)(2 * player.health), 30);
	SetColor(255, 255, 255, 255);
	iRectangle(20, SCREEN_HEIGHT - 60, 200, 30);
	iText(30, SCREEN_HEIGHT - 50, "HEALTH", GLUT_BITMAP_HELVETICA_12);

	// Score Display (Top Right)
	char scoreStr[32];
	sprintf_s(scoreStr, "SCORE: %ld", currentScore);
	SetColor(255, 215, 0, 255); // Gold color
	iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);

	glDisable(GL_BLEND);
}

void UpdateEffects(float dt) {
	if (energyWaveRadius > 0) {
		energyWaveRadius += 500 * dt;
		if (energyWaveRadius > 1000) energyWaveRadius = 0;
	}
	if (screenShakeTimer > 0) {
		screenShakeTimer -= dt;
		if (screenShakeTimer < 0) screenShakeTimer = 0;
	}
}

void iScreenShake(float duration) {
	screenShakeTimer = duration;
}

#endif // LEVEL1_HPP
