#ifndef LEVEL3_HPP
#define LEVEL3_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"

// Forward declaration
extern void ILOADIMAGE(char*);

void LoadLevel3() {
	// Backgrounds (Location 1- b1.1, b1.2)
	level3.bgImgs[0] = iLoadImage("asset/background/level.3/b1.1.png");
	level3.bgImgs[1] = iLoadImage("asset/background/level.3/b1.2.png");

	StopRunSound();

	level3.parallaxSpeeds[0] = 0.3f; // Far
	level3.parallaxSpeeds[1] = 1.0f; // Fore (Ground speed)

	level3.cameraX = 0;
	level3.levelWidth = SCREEN_WIDTH * 3; // 3 screens wide — difficult corridor
	level3.groundY = 100;
	level3.gateY = level3.groundY;
	level3.gateTexture = iLoadImage("asset/gate/g.1.png");
	levelTextTimer = 1.0f;
	levelTimer = 180.0f;
	currentLocation = 1;

	// Reset Player
	player.x = 200;
	player.y = level3.groundY;
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

	// Checkpoint (placed after first major beam corridor)
	level3.checkpointX = 1200;
	level3.checkpointY = level3.groundY;
	level3.checkpointActive = false;

	// Gate position (end of level)
	level3.gateX = 3500;
	isGateUnlocked = false;
	eyeAlignmentPuzzle.isSolved = false;

	// ========================================
	// Eye Beam Obelisks — 8 positioned across the corridor
	// ========================================
	level3.numObelisks = 8;

	// Obelisk 0: Intro — easy, fires right, low beam
	level3.obelisks[0].x = 600;
	level3.obelisks[0].y = level3.groundY;
	level3.obelisks[0].beamY = level3.groundY + 40; // Low beam
	level3.obelisks[0].beamHigh = false;
	level3.obelisks[0].state = EYE_DORMANT;
	level3.obelisks[0].timer = 0;
	level3.obelisks[0].dormantTime = 2.5f;
	level3.obelisks[0].chargeTime = 1.0f;
	level3.obelisks[0].fireTime = 2.0f;
	level3.obelisks[0].cooldownTime = 0.5f;
	level3.obelisks[0].beamLength = 400;
	level3.obelisks[0].direction = 1;

	// Obelisk 1: Intro — fires left, high beam (jump over gap)
	level3.obelisks[1].x = 900;
	level3.obelisks[1].y = level3.groundY;
	level3.obelisks[1].beamY = level3.groundY + 120; // High beam — must duck/wait
	level3.obelisks[1].beamHigh = true;
	level3.obelisks[1].state = EYE_DORMANT;
	level3.obelisks[1].timer = 1.5f; // Staggered
	level3.obelisks[1].dormantTime = 2.0f;
	level3.obelisks[1].chargeTime = 0.8f;
	level3.obelisks[1].fireTime = 1.8f;
	level3.obelisks[1].cooldownTime = 0.5f;
	level3.obelisks[1].beamLength = 400;
	level3.obelisks[1].direction = -1;

	// Obelisk 2: Mid section — fires right, low
	level3.obelisks[2].x = 1400;
	level3.obelisks[2].y = level3.groundY;
	level3.obelisks[2].beamY = level3.groundY + 50;
	level3.obelisks[2].beamHigh = false;
	level3.obelisks[2].state = EYE_DORMANT;
	level3.obelisks[2].timer = 0.5f;
	level3.obelisks[2].dormantTime = 1.8f;
	level3.obelisks[2].chargeTime = 0.8f;
	level3.obelisks[2].fireTime = 2.0f;
	level3.obelisks[2].cooldownTime = 0.4f;
	level3.obelisks[2].beamLength = 350;
	level3.obelisks[2].direction = 1;

	// Obelisk 3: Mid section — fires left, high
	level3.obelisks[3].x = 1700;
	level3.obelisks[3].y = level3.groundY;
	level3.obelisks[3].beamY = level3.groundY + 130;
	level3.obelisks[3].beamHigh = true;
	level3.obelisks[3].state = EYE_DORMANT;
	level3.obelisks[3].timer = 1.0f;
	level3.obelisks[3].dormantTime = 1.5f;
	level3.obelisks[3].chargeTime = 0.7f;
	level3.obelisks[3].fireTime = 1.5f;
	level3.obelisks[3].cooldownTime = 0.4f;
	level3.obelisks[3].beamLength = 350;
	level3.obelisks[3].direction = -1;

	// Obelisk 4: Hard cluster — fires right, low, fast
	level3.obelisks[4].x = 2000;
	level3.obelisks[4].y = level3.groundY;
	level3.obelisks[4].beamY = level3.groundY + 45;
	level3.obelisks[4].beamHigh = false;
	level3.obelisks[4].state = EYE_DORMANT;
	level3.obelisks[4].timer = 0;
	level3.obelisks[4].dormantTime = 1.2f;
	level3.obelisks[4].chargeTime = 0.6f;
	level3.obelisks[4].fireTime = 1.5f;
	level3.obelisks[4].cooldownTime = 0.3f;
	level3.obelisks[4].beamLength = 300;
	level3.obelisks[4].direction = 1;

	// Obelisk 5: Hard cluster — fires left, high, fast
	level3.obelisks[5].x = 2300;
	level3.obelisks[5].y = level3.groundY;
	level3.obelisks[5].beamY = level3.groundY + 110;
	level3.obelisks[5].beamHigh = true;
	level3.obelisks[5].state = EYE_DORMANT;
	level3.obelisks[5].timer = 0.8f;
	level3.obelisks[5].dormantTime = 1.0f;
	level3.obelisks[5].chargeTime = 0.5f;
	level3.obelisks[5].fireTime = 1.8f;
	level3.obelisks[5].cooldownTime = 0.3f;
	level3.obelisks[5].beamLength = 350;
	level3.obelisks[5].direction = -1;

	// Obelisk 6: Near bubbles — fires right, mid-height
	level3.obelisks[6].x = 2600;
	level3.obelisks[6].y = level3.groundY;
	level3.obelisks[6].beamY = level3.groundY + 80;
	level3.obelisks[6].beamHigh = false;
	level3.obelisks[6].state = EYE_DORMANT;
	level3.obelisks[6].timer = 0.3f;
	level3.obelisks[6].dormantTime = 1.5f;
	level3.obelisks[6].chargeTime = 0.7f;
	level3.obelisks[6].fireTime = 2.0f;
	level3.obelisks[6].cooldownTime = 0.4f;
	level3.obelisks[6].beamLength = 300;
	level3.obelisks[6].direction = 1;

	// Obelisk 7: Final approach — fires left, low, aggressive
	level3.obelisks[7].x = 3100;
	level3.obelisks[7].y = level3.groundY;
	level3.obelisks[7].beamY = level3.groundY + 55;
	level3.obelisks[7].beamHigh = false;
	level3.obelisks[7].state = EYE_DORMANT;
	level3.obelisks[7].timer = 0.6f;
	level3.obelisks[7].dormantTime = 1.0f;
	level3.obelisks[7].chargeTime = 0.5f;
	level3.obelisks[7].fireTime = 2.0f;
	level3.obelisks[7].cooldownTime = 0.3f;
	level3.obelisks[7].beamLength = 350;
	level3.obelisks[7].direction = -1;

	// ========================================
	// Bubbles — 4 in dangerous positions
	// ========================================
	level3.numBubbles = 4;
	float bubbleXs[4] = { 1100, 1900, 2500, 3200 };
	float bubbleYs[4] = { 280, 350, 300, 320 };
	for (int i = 0; i < level3.numBubbles; i++) {
		level3.bubbles[i].x = bubbleXs[i];
		level3.bubbles[i].y = bubbleYs[i];
		level3.bubbles[i].baseY = bubbleYs[i];
		level3.bubbles[i].active = true;
		level3.bubbles[i].showHint = false;
		LoadAnim(level3.bubbles[i].anim, "bubble", "bb", 4, 0.15f);
	}

	// ========================================
	// Treasure Chests — 2 with healing potions
	// ========================================
	level3.numChests = 2;
	// Chest 0: Risky mid-level reward (after first beam cluster)
	level3.chests[0].x = 1550;
	level3.chests[0].y = level3.groundY;
	level3.chests[0].opened = false;
	level3.chests[0].showHint = false;
	level3.chests[0].texClosed = iLoadImage((char*)"asset/treasure chest/t.1.png");
	level3.chests[0].texOpen = iLoadImage((char*)"asset/treasure chest/t.2.png");

	// Chest 1: Near final approach (before last bubbles)
	level3.chests[1].x = 2900;
	level3.chests[1].y = level3.groundY;
	level3.chests[1].opened = false;
	level3.chests[1].showHint = false;
	level3.chests[1].texClosed = iLoadImage((char*)"asset/treasure chest/t.1.png");
	level3.chests[1].texOpen = iLoadImage((char*)"asset/treasure chest/t.2.png");

	// ========================================
	// Sacred Sword Collectible
	// ========================================
	level3.swordTexture = iLoadImage((char*)"asset/sword.png");
	level3.swordX = 2050;
	level3.swordY = 420;
	level3.swordCollected = false;
	level3.swordPickupTimer = 0;
	player.hasSword = false;

	// Elevated stone platforms leading to the sword (x, y, width)
	level3.swordPlatforms[0][0] = 1960; level3.swordPlatforms[0][1] = 200; level3.swordPlatforms[0][2] = 70;
	level3.swordPlatforms[1][0] = 2030; level3.swordPlatforms[1][1] = 270; level3.swordPlatforms[1][2] = 60;
	level3.swordPlatforms[2][0] = 1980; level3.swordPlatforms[2][1] = 340; level3.swordPlatforms[2][2] = 65;
	level3.swordPlatforms[3][0] = 2040; level3.swordPlatforms[3][1] = 400; level3.swordPlatforms[3][2] = 70;

	// ========================================
	// Location 2 Data Initialization
	// ========================================
	level3.loc2BgImgs[0] = iLoadImage("asset/background/level.3/b1.3.png");
	level3.loc2BgImgs[1] = iLoadImage("asset/background/level.3/b1.4.png");

	// Load Wolf Animations
	for (int w = 0; w < 3; w++) {
		LoadAnim(wolves[w].runRight, (char*)"wolfe/w.right run", (char*)"wr", 9, 0.08f, true);
		LoadAnim(wolves[w].runLeft, (char*)"wolfe/w.left run", (char*)"wl", 9, 0.08f, true);
		LoadAnim(wolves[w].attackRight, (char*)"wolfe/w.attack", (char*)"right attack", 4, 0.12f, true);
		LoadAnim(wolves[w].attackLeft, (char*)"wolfe/w.attack", (char*)"left attack", 4, 0.12f, true);
		wolves[w].active = false;
		wolves[w].health = 100;
		wolves[w].speed = 250.0f + w * 20.0f;
		wolves[w].attackTimer = 0;
		wolves[w].attackCooldown = 0;
		wolves[w].isAttacking = false;
		wolves[w].facingRight = true;
	}

	// Load Sword Attack Animations
	player.swordAttackLeft.numFrames = 4;
	player.swordAttackLeft.currentFrame = 0;
	player.swordAttackLeft.timer = 0;
	player.swordAttackLeft.frameDuration = 0.2f;
	player.swordAttackLeft.textures[0] = iLoadImage((char*)"asset/sword attack/sl.attack (1).png");
	player.swordAttackLeft.textures[1] = iLoadImage((char*)"asset/sword attack/sl.attack (2).png");
	player.swordAttackLeft.textures[2] = iLoadImage((char*)"asset/sword attack/sl.attack (3).png");
	player.swordAttackLeft.textures[3] = iLoadImage((char*)"asset/sword attack/sl.attack (4).png");
	player.swordAttackRight.numFrames = 4;
	player.swordAttackRight.currentFrame = 0;
	player.swordAttackRight.timer = 0;
	player.swordAttackRight.frameDuration = 0.2f;
	player.swordAttackRight.textures[0] = iLoadImage((char*)"asset/sword attack/sr.attack (1).png");
	player.swordAttackRight.textures[1] = iLoadImage((char*)"asset/sword attack/sr.attack (2).png");
	player.swordAttackRight.textures[2] = iLoadImage((char*)"asset/sword attack/sr.attack (3).png");
	player.swordAttackRight.textures[3] = iLoadImage((char*)"asset/sword attack/sr.attack (4).png");
	player.isSwordAttacking = false;

	// Loc2 spike traps
	level3.numLoc2Spikes = 4;
	float l3spikeXs[4] = { 800, 1400, 2200, 3200 };
	for (int i = 0; i < level3.numLoc2Spikes; i++) {
		level3.loc2Spikes[i].x = l3spikeXs[i];
		level3.loc2Spikes[i].y = level3.groundY;
		level3.loc2Spikes[i].timer = (i % 2) * 1.5f;
		level3.loc2Spikes[i].extended = false;
		LoadAnim(level3.loc2Spikes[i].anim, (char*)"spike traps", (char*)"s", 4, 0.15f, true);
	}

	// Loc2 bubbles
	level3.numLoc2Bubbles = 3;
	float l3bXs[3] = { 600, 1800, 3000 };
	float l3bYs[3] = { 300, 350, 320 };
	for (int i = 0; i < level3.numLoc2Bubbles; i++) {
		level3.loc2Bubbles[i].x = l3bXs[i];
		level3.loc2Bubbles[i].y = l3bYs[i];
		level3.loc2Bubbles[i].baseY = l3bYs[i];
		level3.loc2Bubbles[i].active = true;
		level3.loc2Bubbles[i].showHint = false;
		LoadAnim(level3.loc2Bubbles[i].anim, "bubble", "bb", 4, 0.15f);
	}

	// Loc2 chests
	level3.numLoc2Chests = 2;
	level3.loc2Chests[0].x = 1000; level3.loc2Chests[0].y = level3.groundY;
	level3.loc2Chests[0].opened = false; level3.loc2Chests[0].showHint = false;
	level3.loc2Chests[0].texClosed = iLoadImage((char*)"asset/treasure chest/t.1.png");
	level3.loc2Chests[0].texOpen = iLoadImage((char*)"asset/treasure chest/t.2.png");
	level3.loc2Chests[1].x = 2800; level3.loc2Chests[1].y = level3.groundY;
	level3.loc2Chests[1].opened = false; level3.loc2Chests[1].showHint = false;
	level3.loc2Chests[1].texClosed = level3.loc2Chests[0].texClosed;
	level3.loc2Chests[1].texOpen = level3.loc2Chests[0].texOpen;

	// Loc2 checkpoint and final door
	level3.loc2CheckpointX = 2000;
	level3.loc2CheckpointY = level3.groundY;
	level3.loc2CheckpointActive = false;
	level3.finalDoorX = 4800;
	level3.finalDoorOpen = false;
	level3.wolvesTriggered = false;
	level3.howlTimer = 0;
	moonSealPuzzle.isSolved = false;
}

// ========================================
// Draw Eye Beam Obelisks
// ========================================
void DrawEyeBeamObelisks() {
	for (int i = 0; i < level3.numObelisks; i++) {
		float ox = level3.obelisks[i].x - level3.cameraX;
		float oy = level3.obelisks[i].y;
		EyeBeamState st = level3.obelisks[i].state;

		// Skip if off-screen
		if (ox < -200 || ox > SCREEN_WIDTH + 200) continue;

		// ---- Stone Pillar Body ----
		// Base pillar (128 wide x 200 tall)
		SetColor(45, 40, 50, 255);
		iFilledRectangle(ox - 40, oy, 80, 200);
		// Dark border
		SetColor(30, 28, 35, 255);
		iRectangle(ox - 40, oy, 80, 200);

		// Rune engravings on pillar
		SetColor(20, 60, 70, 150);
		for (int r = 0; r < 4; r++) {
			float ry = oy + 20 + r * 40;
			iLine(ox - 25, ry, ox - 10, ry + 15);
			iLine(ox - 10, ry + 15, ox + 10, ry + 15);
			iLine(ox + 10, ry + 15, ox + 25, ry);
		}

		// Cracked texture lines
		SetColor(25, 23, 30, 200);
		iLine(ox - 30, oy + 50, ox - 15, oy + 90);
		iLine(ox + 20, oy + 30, ox + 10, oy + 70);
		iLine(ox - 5, oy + 120, ox + 15, oy + 160);

		// Moss patches (small green accents)
		SetColor(30, 55, 25, 120);
		iFilledCircle(ox - 30, oy + 10, 8);
		iFilledCircle(ox + 25, oy + 180, 6);

		// ---- Eye Socket ----
		float eyeX = ox;
		float eyeY = oy + 160;
		float eyeRadius = 18.0f;

		// Eye socket frame
		SetColor(35, 32, 42, 255);
		iFilledCircle(eyeX, eyeY, eyeRadius + 6);

		// State-dependent eye rendering
		float glowIntensity = 0.0f;
		if (st == EYE_DORMANT) {
			glowIntensity = 0.2f + (sin(globalTime * 2 + i) + 1) * 0.1f;
			// Closed eye — just a dim slit
			SetColor(0, (int)(80 * glowIntensity), (int)(120 * glowIntensity), 200);
			iFilledRectangle(eyeX - 12, eyeY - 3, 24, 6);
		}
		else if (st == EYE_CHARGING) {
			float chargeProgress = level3.obelisks[i].timer / level3.obelisks[i].chargeTime;
			glowIntensity = 0.3f + chargeProgress * 0.7f;
			// Opening eye
			float openSize = eyeRadius * chargeProgress;
			SetColor(0, (int)(180 * glowIntensity), (int)(255 * glowIntensity), (int)(200 + chargeProgress * 55));
			iFilledCircle(eyeX, eyeY, openSize);
			// Pupil
			SetColor(0, 255, 255, (int)(255 * chargeProgress));
			iFilledCircle(eyeX, eyeY, openSize * 0.4f);
			// Charging particles
			for (int p = 0; p < 4; p++) {
				float pAngle = globalTime * 5 + p * 1.57f;
				float px = eyeX + cos(pAngle) * (eyeRadius + 10 - chargeProgress * 8);
				float py = eyeY + sin(pAngle) * (eyeRadius + 10 - chargeProgress * 8);
				SetColor(0, 255, 255, (int)(100 * chargeProgress));
				iFilledCircle(px, py, 3);
			}
		}
		else if (st == EYE_FIRING) {
			glowIntensity = 1.0f;
			// Fully open eye — bright
			SetColor(0, 220, 255, 255);
			iFilledCircle(eyeX, eyeY, eyeRadius);
			// Bright pupil
			SetColor(200, 255, 255, 255);
			iFilledCircle(eyeX, eyeY, eyeRadius * 0.35f);
			// Outer glow
			SetColor(0, 255, 255, 80);
			iFilledCircle(eyeX, eyeY, eyeRadius + 12);

			// ---- BEAM ----
			float beamStartX = ox;
			float beamEndX = ox + level3.obelisks[i].beamLength * level3.obelisks[i].direction;
			float beamDrawY = level3.obelisks[i].beamY;

			// Beam glow layers (outer to inner)
			float beamPulse = (sin(globalTime * 15 + i) + 1) / 2.0f;
			glLineWidth(12.0f);
			SetColor(0, 100, 150, (int)(40 + beamPulse * 30));
			iLine(beamStartX, beamDrawY, beamEndX, beamDrawY);
			glLineWidth(6.0f);
			SetColor(0, 200, 255, (int)(100 + beamPulse * 50));
			iLine(beamStartX, beamDrawY, beamEndX, beamDrawY);
			glLineWidth(2.0f);
			SetColor(180, 255, 255, 255);
			iLine(beamStartX, beamDrawY, beamEndX, beamDrawY);
			glLineWidth(1.0f);

			// Beam crackling particles
			for (int p = 0; p < 6; p++) {
				float px = beamStartX + (beamEndX - beamStartX) * ((float)(p + 1) / 7.0f);
				float py = beamDrawY + (sin(globalTime * 20 + p * 3 + i) * 8);
				SetColor(0, 255, 255, 180);
				iFilledCircle(px, py, 2 + sin(globalTime * 10 + p) * 1);
			}

			// Flash at eye
			SetColor(255, 255, 255, (int)(150 + beamPulse * 100));
			iFilledCircle(eyeX, eyeY, eyeRadius + 5 + beamPulse * 3);
		}
		else if (st == EYE_COOLDOWN) {
			float coolProgress = level3.obelisks[i].timer / level3.obelisks[i].cooldownTime;
			glowIntensity = 1.0f - coolProgress;
			// Closing eye
			float openSize = eyeRadius * (1.0f - coolProgress);
			SetColor(0, (int)(180 * glowIntensity), (int)(255 * glowIntensity), (int)(200 * glowIntensity));
			iFilledCircle(eyeX, eyeY, openSize);
		}

		// ---- Rune glow lines from eye (always present, intensity varies) ----
		SetColor(0, (int)(100 * glowIntensity), (int)(150 * glowIntensity), (int)(100 * glowIntensity));
		iLine(eyeX - 30, eyeY, eyeX - 40, eyeY - 40);
		iLine(eyeX + 30, eyeY, eyeX + 40, eyeY - 40);
		iLine(eyeX, eyeY - eyeRadius - 5, eyeX, eyeY - eyeRadius - 25);

		// ---- Pillar cap ----
		SetColor(50, 45, 58, 255);
		iFilledRectangle(ox - 45, oy + 195, 90, 15);
		SetColor(30, 28, 35, 255);
		iRectangle(ox - 45, oy + 195, 90, 15);
	}
}

// ========================================
// Draw Level 3 Bubbles
// ========================================
void DrawLevel3Bubbles() {
	float camX = level3.cameraX;
	for (int i = 0; i < level3.numBubbles; i++) {
		if (level3.bubbles[i].active) {
			float bx = level3.bubbles[i].x - camX;
			float by = level3.bubbles[i].y;

			int currentTex = level3.bubbles[i].anim.textures[level3.bubbles[i].anim.currentFrame];
			iShowImage(bx - 40, by - 40, 80, 80, currentTex);

			if (level3.bubbles[i].showHint) {
				SetColor(255, 255, 255, 200);
				iText(bx - 50, by + 50, "PRESS F TO POP", GLUT_BITMAP_HELVETICA_12);
			}
		}
	}
}

// ========================================
// Draw Level 3 Chests
// ========================================
void DrawLevel3Chests() {
	for (int i = 0; i < level3.numChests; i++) {
		if (level3.chests[i].x < 0) continue;

		float cx = level3.chests[i].x - level3.cameraX;
		float cy = level3.chests[i].y;

		unsigned int tex = level3.chests[i].opened ? level3.chests[i].texOpen : level3.chests[i].texClosed;

		if (tex != 0) {
			iShowImage(cx - 30, cy, 60, 60, tex);
		} else {
			SetColor(120, 80, 30, 255);
			iFilledRectangle(cx - 30, cy, 60, 40);
			if (level3.chests[i].opened) {
				SetColor(200, 200, 50, 255);
				iFilledRectangle(cx - 20, cy + 10, 40, 20);
			}
		}

		if (level3.chests[i].showHint && !level3.chests[i].opened) {
			SetColor(255, 255, 255, 200);
			iText(cx - 30, cy + 50, "PRESS E TO OPEN", GLUT_BITMAP_HELVETICA_12);
		}
	}
}

// ========================================
// Draw Level 3 Checkpoint
// ========================================
void DrawLevel3Checkpoint() {
	float cx = level3.checkpointX - level3.cameraX;
	float cy = level3.groundY;

	if (level3.checkpointActive) {
		// Active checkpoint — bright cyan flame
		float pulse = (sin(globalTime * 5) + 1) / 2.0f;
		SetColor(0, 255, 255, (int)(150 + pulse * 105));
		iFilledCircle(cx, cy + 60, 12 + pulse * 4);
		SetColor(200, 255, 255, 255);
		iFilledCircle(cx, cy + 60, 6);
	} else {
		// Inactive checkpoint — dim marker
		SetColor(60, 60, 70, 200);
		iFilledCircle(cx, cy + 60, 10);
	}
	// Stone pedestal
	SetColor(55, 50, 60, 255);
	iFilledRectangle(cx - 15, cy, 30, 50);
	SetColor(40, 38, 48, 255);
	iRectangle(cx - 15, cy, 30, 50);
}

// ========================================
// Draw Level 3 Gate
// ========================================
void DrawLevel3Gate() {
	float gx = level3.gateX - level3.cameraX;
	float gy = level3.gateY;
	iShowImage(gx - 75, gy, 150, 625, level3.gateTexture);
}

// ========================================
// Draw Atmospheric Effects (dark temple ambiance)
// ========================================
void DrawLevel3Atmosphere() {
	// Floating dust / magical particles
	for (int i = 0; i < 15; i++) {
		float px = fmod(globalTime * 20 + i * 200, (float)SCREEN_WIDTH);
		float py = 150 + sin(globalTime * 0.5f + i * 1.3f) * 100 + i * 30;
		float alpha = (sin(globalTime * 2 + i) + 1) / 2.0f;
		SetColor(0, 200, 255, (int)(20 + alpha * 30));
		iFilledCircle(px, py, 2 + alpha * 2);
	}

	// Dark vignette overlay at edges
	for (int i = 0; i < 8; i++) {
		SetColor(0, 0, 5, 15);
		iFilledRectangle(0, 0, 60 - i * 7, SCREEN_HEIGHT);
		iFilledRectangle(SCREEN_WIDTH - 60 + i * 7, 0, 60 - i * 7, SCREEN_HEIGHT);
	}

	// Cursed symbols on walls (faded wall carvings)
	float symbolAlpha = (sin(globalTime * 1.5f) + 1) / 2.0f;
	SetColor(0, 80, 100, (int)(20 + symbolAlpha * 25));
	// Eye symbols scattered on background
	for (int i = 0; i < 5; i++) {
		float sx = 200 + i * 700 - fmod(level3.cameraX * 0.1f, 3500.0f);
		if (sx < -100 || sx > SCREEN_WIDTH + 100) continue;
		float sy = 400 + sin(i * 2.1f) * 100;
		// Simple eye shape
		iCircle(sx, sy, 20);
		iFilledCircle(sx, sy, 8);
		iLine(sx - 30, sy, sx + 30, sy);
	}
}

// ========================================
// Draw Sacred Sword Collectible + Platforms
// ========================================
void DrawSacredSword() {
	float camX = level3.cameraX;

	// -- Draw the 4 stone platforms (always visible, even after sword collected) --
	for (int i = 0; i < 4; i++) {
		float px = level3.swordPlatforms[i][0] - camX;
		float py = level3.swordPlatforms[i][1];
		float pw = level3.swordPlatforms[i][2];
		float ph = 14.0f;

		if (px < -200 || px > SCREEN_WIDTH + 200) continue;

		// Dark stone base
		SetColor(50, 44, 55, 255);
		iFilledRectangle(px - pw / 2, py, pw, ph);
		// Top highlight
		SetColor(65, 58, 72, 255);
		iFilledRectangle(px - pw / 2, py + ph - 3, pw, 3);
		// Border
		SetColor(35, 30, 40, 255);
		iRectangle(px - pw / 2, py, pw, ph);
		// Crack detail
		SetColor(30, 26, 34, 180);
		iLine(px - pw / 4, py + 4, px - pw / 4 + 10, py + ph - 2);
		iLine(px + pw / 6, py + 2, px + pw / 6 - 5, py + ph - 3);
	}

	// -- Wall inscription near the platforms --
	float inscX = level3.swordPlatforms[0][0] - 60 - camX;
	float inscY = level3.swordPlatforms[0][1] - 30;
	if (inscX > -300 && inscX < SCREEN_WIDTH + 100) {
		float inscAlpha = (sin(globalTime * 1.2f) + 1) / 2.0f;
		SetColor(0, 150, 180, (int)(40 + inscAlpha * 50));
		iText((int)inscX, (int)inscY, "No beast of shadow falls", GLUT_BITMAP_HELVETICA_12);
		iText((int)inscX, (int)inscY - 16, "without the sacred blade.", GLUT_BITMAP_HELVETICA_12);
	}

	if (level3.swordCollected) return;

	// -- Altar pedestal under the sword --
	float altarX = level3.swordX - camX;
	float altarY = level3.swordY - 10;
	if (altarX < -200 || altarX > SCREEN_WIDTH + 200) return;

	// Altar base
	SetColor(55, 48, 62, 255);
	iFilledRectangle(altarX - 25, altarY - 8, 50, 12);
	SetColor(40, 35, 48, 255);
	iRectangle(altarX - 25, altarY - 8, 50, 12);

	// Faint circular aura beneath the sword
	float auraPulse = (sin(globalTime * 3.0f) + 1) / 2.0f;
	SetColor(0, 200, 255, (int)(20 + auraPulse * 30));
	iFilledCircle(altarX, altarY + 10, 30 + auraPulse * 5);

	// -- Floating sword --
	float bobOffset = sin(globalTime * 2.5f) * 8.0f; // Gentle vertical bob
	float swordDrawX = altarX - 40;
	float swordDrawY = altarY + 10 + bobOffset;

	// Sword glow behind (cyan pulse)
	float glowPulse = (sin(globalTime * 4.0f) + 1) / 2.0f;
	SetColor(0, 220, 255, (int)(30 + glowPulse * 50));
	iFilledCircle(altarX, swordDrawY + 40, 45 + glowPulse * 8);
	SetColor(0, 255, 255, (int)(15 + glowPulse * 25));
	iFilledCircle(altarX, swordDrawY + 40, 55 + glowPulse * 10);

	// Draw the sword image (80x80)
	iShowImage((int)swordDrawX, (int)swordDrawY, 80, 80, level3.swordTexture);

	// Magical orbiting particles
	for (int p = 0; p < 6; p++) {
		float pAngle = globalTime * 2.0f + p * 1.047f; // 60 degrees apart
		float pRadius = 35 + sin(globalTime * 3 + p) * 8;
		float ppx = altarX + cos(pAngle) * pRadius;
		float ppy = swordDrawY + 40 + sin(pAngle) * pRadius * 0.5f;
		float pAlpha = (sin(globalTime * 5 + p * 2) + 1) / 2.0f;
		SetColor(100, 255, 255, (int)(60 + pAlpha * 120));
		iFilledCircle(ppx, ppy, 2 + pAlpha * 2);
	}

	// Shimmer rune sparks (small upward drifting particles)
	for (int s = 0; s < 4; s++) {
		float sx = altarX - 20 + fmod(globalTime * 30 + s * 80, 40.0f);
		float sy = swordDrawY + fmod(globalTime * 40 + s * 50, 80.0f);
		float sAlpha = 1.0f - fmod(globalTime * 40 + s * 50, 80.0f) / 80.0f;
		SetColor(0, 255, 200, (int)(sAlpha * 150));
		iFilledCircle(sx, sy, 1.5f);
	}
}

// ========================================
// Draw Level 3 Location 2
// ========================================
void DrawLevel3Loc2() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float shakeY = (screenShakeTimer > 0) ? (rand() % (int)(shakeIntensity * 2) - shakeIntensity) : 0;
	float shakeX = (screenShakeTimer > 0) ? (rand() % (int)(shakeIntensity * 2) - shakeIntensity) : 0;
	glPushMatrix();
	glTranslatef(shakeX, shakeY, 0);

	// Parallax backgrounds (loc2)
	for (int layer = 0; layer < 2; layer++) {
		float speed = level3.parallaxSpeeds[layer];
		float totalW = 2.0f * SCREEN_WIDTH;
		float startX = -fmod(level3.cameraX * speed, totalW);
		while (startX > 0) startX -= totalW;
		while (startX <= -totalW) startX += totalW;
		for (int i = 0; i < 4; i++) {
			float drawX = startX + (i * SCREEN_WIDTH);
			if (drawX + SCREEN_WIDTH > 0 && drawX < SCREEN_WIDTH) {
				iShowImage((int)drawX, 0, SCREEN_WIDTH, SCREEN_HEIGHT, level3.loc2BgImgs[i % 2]);
			}
		}
	}

	// Darker overlay for final area
	SetColor(0, 0, 10, 90);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Atmospheric dust particles
	for (int i = 0; i < 20; i++) {
		float px = fmod(globalTime * 15 + i * 180, (float)SCREEN_WIDTH);
		float py = 120 + sin(globalTime * 0.4f + i * 1.5f) * 120 + i * 25;
		float alpha = (sin(globalTime * 2 + i) + 1) / 2.0f;
		SetColor(0, 150, 200, (int)(15 + alpha * 25));
		iFilledCircle(px, py, 2 + alpha * 2);
	}

	// Dark vignette
	for (int i = 0; i < 8; i++) {
		SetColor(0, 0, 5, 20);
		iFilledRectangle(0, 0, 70 - i * 8, SCREEN_HEIGHT);
		iFilledRectangle(SCREEN_WIDTH - 70 + i * 8, 0, 70 - i * 8, SCREEN_HEIGHT);
	}

	// Draw Spike Traps
	for (int i = 0; i < level3.numLoc2Spikes; i++) {
		float sx = level3.loc2Spikes[i].x - level3.cameraX;
		float sy = level3.loc2Spikes[i].y;
		if (sx < -200 || sx > SCREEN_WIDTH + 200) continue;
		if (level3.loc2Spikes[i].extended) {
			unsigned int spTex = level3.loc2Spikes[i].anim.textures[level3.loc2Spikes[i].anim.currentFrame];
			iShowImage(sx - 80, sy, 160, 160, spTex);
		} else {
			SetColor(30, 30, 35, 255);
			iFilledRectangle(sx - 60, sy - 5, 120, 10);
		}
	}

	// Draw Loc2 Bubbles
	for (int i = 0; i < level3.numLoc2Bubbles; i++) {
		if (level3.loc2Bubbles[i].active) {
			float bx = level3.loc2Bubbles[i].x - level3.cameraX;
			float by = level3.loc2Bubbles[i].y;
			if (bx < -100 || bx > SCREEN_WIDTH + 100) continue;
			int bTex = level3.loc2Bubbles[i].anim.textures[level3.loc2Bubbles[i].anim.currentFrame];
			iShowImage(bx - 40, by - 40, 80, 80, bTex);
			if (level3.loc2Bubbles[i].showHint) {
				SetColor(255, 255, 255, 200);
				iText(bx - 50, by + 50, "PRESS F TO POP", GLUT_BITMAP_HELVETICA_12);
			}
		}
	}

	// Draw Loc2 Chests
	for (int i = 0; i < level3.numLoc2Chests; i++) {
		if (level3.loc2Chests[i].x < 0) continue;
		float cx = level3.loc2Chests[i].x - level3.cameraX;
		float cy = level3.loc2Chests[i].y;
		if (cx < -100 || cx > SCREEN_WIDTH + 100) continue;
		unsigned int cTex = level3.loc2Chests[i].opened ? level3.loc2Chests[i].texOpen : level3.loc2Chests[i].texClosed;
		if (cTex != 0) iShowImage(cx - 30, cy, 60, 60, cTex);
		if (level3.loc2Chests[i].showHint && !level3.loc2Chests[i].opened) {
			SetColor(255, 255, 255, 200);
			iText(cx - 30, cy + 50, "PRESS E TO OPEN", GLUT_BITMAP_HELVETICA_12);
		}
	}

	// Draw Loc2 Checkpoint
	float cpx = level3.loc2CheckpointX - level3.cameraX;
	if (level3.loc2CheckpointActive) {
		float pulse = (sin(globalTime * 5) + 1) / 2.0f;
		SetColor(0, 255, 255, (int)(150 + pulse * 105));
		iFilledCircle(cpx, level3.groundY + 60, 12 + pulse * 4);
		SetColor(200, 255, 255, 255);
		iFilledCircle(cpx, level3.groundY + 60, 6);
	} else {
		SetColor(60, 60, 70, 200);
		iFilledCircle(cpx, level3.groundY + 60, 10);
	}
	SetColor(55, 50, 60, 255);
	iFilledRectangle(cpx - 15, level3.groundY, 30, 50);

	// Draw Wolves
	for (int w = 0; w < numWolves; w++) {
		if (!wolves[w].active) continue;
		float wx = wolves[w].x - level3.cameraX;
		float wy = wolves[w].y;
		if (wx < -150 || wx > SCREEN_WIDTH + 150) continue;

		// Red aura glow
		float wPulse = (sin(globalTime * 6 + w) + 1) / 2.0f;
		SetColor(200, 0, 0, (int)(30 + wPulse * 40));
		iFilledCircle(wx, wy + 40, 50 + wPulse * 10);

		unsigned int wTex;
		if (wolves[w].isAttacking) {
			wTex = wolves[w].facingRight ?
				wolves[w].attackRight.textures[wolves[w].attackRight.currentFrame] :
				wolves[w].attackLeft.textures[wolves[w].attackLeft.currentFrame];
		} else {
			wTex = wolves[w].facingRight ?
				wolves[w].runRight.textures[wolves[w].runRight.currentFrame] :
				wolves[w].runLeft.textures[wolves[w].runLeft.currentFrame];
		}
		iShowImage(wx - 50, wy, 100, 80, wTex);
	}

	// Draw Final Door
	float doorX = level3.finalDoorX - level3.cameraX;
	if (doorX > -200 && doorX < SCREEN_WIDTH + 200) {
		// Door frame
		SetColor(40, 35, 50, 255);
		iFilledRectangle(doorX - 80, level3.groundY, 160, 500);
		SetColor(30, 25, 40, 255);
		iRectangle(doorX - 80, level3.groundY, 160, 500);

		// Inner door
		if (level3.finalDoorOpen) {
			// Open — bright cyan/gold glow
			float doorPulse = (sin(globalTime * 4) + 1) / 2.0f;
			SetColor(0, 255, 200, (int)(150 + doorPulse * 105));
			iFilledRectangle(doorX - 60, level3.groundY + 10, 120, 450);
			SetColor(255, 255, 200, (int)(100 + doorPulse * 80));
			iFilledRectangle(doorX - 40, level3.groundY + 30, 80, 400);
			// Victory glow
			SetColor(0, 255, 255, (int)(40 + doorPulse * 40));
			iFilledCircle(doorX, level3.groundY + 300, 100 + doorPulse * 20);
		} else {
			// Closed — standard gate texture for consistency
			iShowImage(doorX - 75, level3.groundY, 150, 625, level3.gateTexture);
		}

		// Pillar details
		SetColor(50, 45, 58, 255);
		iFilledRectangle(doorX - 85, level3.groundY, 25, 500);
		iFilledRectangle(doorX + 60, level3.groundY, 25, 500);
		SetColor(35, 30, 43, 255);
		iRectangle(doorX - 85, level3.groundY, 25, 500);
		iRectangle(doorX + 60, level3.groundY, 25, 500);
	}

	// Draw Player
	unsigned int currentTex;
	float drawW = 80, drawH = 80;
	float drawOffX = -40; // default center offset
	if (player.isSwordAttacking) {
		currentTex = player.isFacingRight ?
			player.swordAttackRight.textures[player.swordAttackRight.currentFrame] :
			player.swordAttackLeft.textures[player.swordAttackLeft.currentFrame];
		drawW = 120;
		drawH = 80;
		drawOffX = player.isFacingRight ? -40 : -80; // extend sword in facing direction
	} else if (player.isAttacking) {
		currentTex = player.attack.textures[player.attack.currentFrame];
	} else if (player.isJumping) {
		currentTex = player.jump.textures[player.jump.currentFrame];
	} else if (player.isMoving) {
		currentTex = player.isFacingRight ? player.runRight.textures[player.runRight.currentFrame] : player.runLeft.textures[player.runLeft.currentFrame];
	} else {
		currentTex = player.idle.textures[player.idle.currentFrame];
	}
	iShowImage(player.x - level3.cameraX + drawOffX, player.y, drawW, drawH, currentTex);

	glPopMatrix();

	// ======== HUD ========
	if (levelTextTimer > 0) {
		SetColor(150, 200, 220, 255);
		DrawStrokeText(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, 0, "FINAL CHAMBER", 0.7f, 5.0f);
		SetColor(200, 50, 50, 180);
		iText(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 40, "The Cursed Exit", GLUT_BITMAP_HELVETICA_18);
	}

	// Health Bar
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

	char scoreStr[32];
	sprintf_s(scoreStr, "SCORE: %ld", currentScore);
	SetColor(255, 215, 0, 255);
	iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);

	char livesStr[32];
	sprintf_s(livesStr, "LIVES: %d", player.lives);
	SetColor(255, 100, 100, 255);
	iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 80, livesStr, GLUT_BITMAP_HELVETICA_18);

	// Timer Display
	int tMins3b = (int)(levelTimer / 60);
	int tSecs3b = (int)(levelTimer) % 60;
	char tStr3b[20];
	sprintf_s(tStr3b, "%d:%02d", tMins3b, tSecs3b);
	if (levelTimer < 30) SetColor(255, 50, 50, 255);
	else SetColor(200, 200, 255, 255);
	iText(SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT - 50, tStr3b, GLUT_BITMAP_TIMES_ROMAN_24);

	if (level3.loc2CheckpointActive) {
		SetColor(0, 255, 255, 120);
		iText(20, SCREEN_HEIGHT - 80, "CHECKPOINT ACTIVE", GLUT_BITMAP_HELVETICA_12);
	}

	// Wolf warning
	if (level3.wolvesTriggered && level3.howlTimer > 0) {
		float wAlpha = (level3.howlTimer > 0.5f) ? 1.0f : level3.howlTimer / 0.5f;
		SetColor(255, 50, 50, (int)(255 * wAlpha));
		iText(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 100, "WOLVES APPROACHING!", GLUT_BITMAP_TIMES_ROMAN_24);
	}

	// Sword HUD
	if (player.hasSword) {
		SetColor(0, 200, 255, 180);
		iText(SCREEN_WIDTH - 220, 30, "WOLF-BANE RELIC", GLUT_BITMAP_HELVETICA_12);
		iShowImage(SCREEN_WIDTH - 65, 15, 50, 50, level3.swordTexture);
	}

	if (player.potionCollectTimer > 0) {
		SetColor(255, 255, 255, 255);
		iText(player.x - 30, player.y + 120, "+1 HEALTH POTION", GLUT_BITMAP_HELVETICA_12);
	}

	glDisable(GL_BLEND);
}

// ========================================
// Main Draw Function
// ========================================
void DrawLevel3() {
	if (currentLocation == 2) {
		DrawLevel3Loc2();
		return;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float shakeY = (screenShakeTimer > 0) ? (rand() % (int)(shakeIntensity * 2) - shakeIntensity) : 0;
	float shakeX = (screenShakeTimer > 0) ? (rand() % (int)(shakeIntensity * 2) - shakeIntensity) : 0;
	glPushMatrix();
	glTranslatef(shakeX, shakeY, 0);

	// Draw Backgrounds with Parallax (same system as Level 1/2)
	for (int layer = 0; layer < 2; layer++) {
		float speed = level3.parallaxSpeeds[layer];
		float totalW = 2.0f * SCREEN_WIDTH;

		float startX = -fmod(level3.cameraX * speed, totalW);
		while (startX > 0) startX -= totalW;
		while (startX <= -totalW) startX += totalW;

		for (int i = 0; i < 4; i++) {
			float drawX = startX + (i * SCREEN_WIDTH);
			if (drawX + SCREEN_WIDTH > 0 && drawX < SCREEN_WIDTH) {
				iShowImage((int)drawX, 0, SCREEN_WIDTH, SCREEN_HEIGHT, level3.bgImgs[i % 2]);
			}
		}
	}

	// Dark overlay tint for oppressive atmosphere
	SetColor(0, 0, 10, 60);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Atmospheric effects
	DrawLevel3Atmosphere();

	// Draw game elements
	DrawLevel3Checkpoint();
	DrawLevel3Gate();
	DrawEyeBeamObelisks();
	DrawLevel3Bubbles();
	DrawLevel3Chests();
	DrawSacredSword();

	// Draw Player (exact same rendering as Level 1/2)
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

	iShowImage(player.x - level3.cameraX - 40, player.y, 80, 80, currentTex);

	glPopMatrix();

	// ======== HUD (drawn after popMatrix so it's not affected by shake) ========

	// Level Start Text
	if (levelTextTimer > 0) {
		SetColor(150, 200, 220, 255);
		DrawStrokeText(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 0, "LEVEL 3", 0.8f, 5.0f);
		SetColor(0, 200, 255, 180);
		iText(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 40, "The Inner Cursed Sanctum", GLUT_BITMAP_HELVETICA_18);
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
	SetColor(255, 215, 0, 255);
	iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);

	// Lives Display (Top Right below Score)
	char livesStr[32];
	sprintf_s(livesStr, "LIVES: %d", player.lives);
	SetColor(255, 100, 100, 255);
	iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 80, livesStr, GLUT_BITMAP_HELVETICA_18);

	// Timer Display
	int tMins3 = (int)(levelTimer / 60);
	int tSecs3 = (int)(levelTimer) % 60;
	char tStr3[20];
	sprintf_s(tStr3, "%d:%02d", tMins3, tSecs3);
	if (levelTimer < 30) SetColor(255, 50, 50, 255);
	else SetColor(200, 200, 255, 255);
	iText(SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT - 50, tStr3, GLUT_BITMAP_TIMES_ROMAN_24);

	// Checkpoint notification
	if (level3.checkpointActive) {
		SetColor(0, 255, 255, 120);
		iText(20, SCREEN_HEIGHT - 80, "CHECKPOINT ACTIVE", GLUT_BITMAP_HELVETICA_12);
	}

	// Draw Potion Notification over character
	if (player.potionCollectTimer > 0) {
		SetColor(255, 255, 255, 255);
		iText(player.x - 30, player.y + 120, "+1 HEALTH POTION", GLUT_BITMAP_HELVETICA_12);
	}

	// Sword Pickup Message (centered, gold with glow)
	if (level3.swordPickupTimer > 0) {
		float msgAlpha = (level3.swordPickupTimer > 0.5f) ? 1.0f : level3.swordPickupTimer / 0.5f;
		// Glow behind text
		SetColor(0, 180, 200, (int)(80 * msgAlpha));
		iText(SCREEN_WIDTH / 2 - 121, SCREEN_HEIGHT / 2 + 99, "Wolf-Bane Relic Acquired", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(SCREEN_WIDTH / 2 - 119, SCREEN_HEIGHT / 2 + 101, "Wolf-Bane Relic Acquired", GLUT_BITMAP_TIMES_ROMAN_24);
		// Main text
		SetColor(255, 215, 0, (int)(255 * msgAlpha));
		iText(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 100, "Wolf-Bane Relic Acquired", GLUT_BITMAP_TIMES_ROMAN_24);
	}

	// Sword HUD Icon (bottom-right corner indicator)
	if (player.hasSword) {
		SetColor(0, 200, 255, 180);
		iText(SCREEN_WIDTH - 220, 30, "WOLF-BANE RELIC", GLUT_BITMAP_HELVETICA_12);
		// Small sword icon
		iShowImage(SCREEN_WIDTH - 65, 15, 50, 50, level3.swordTexture);
	}

	glDisable(GL_BLEND);
}

// ========================================
// Update Level 3
// ========================================
void UpdateLevel3(float dt) {
	if (player.potionCollectTimer > 0) player.potionCollectTimer -= dt;
	if (level3.swordPickupTimer > 0) level3.swordPickupTimer -= dt;

	if (isTransitioning) {
		transitionAlpha += 2.0f * dt;
		if (transitionAlpha >= 1.0f) {
			transitionAlpha = 1.0f;
			if (currentLocation == 1) {
				// Transition to Location 2
				currentLocation = 2;
				player.x = 200;
				player.y = level3.groundY;
				level3.cameraX = 0;
				level3.levelWidth = SCREEN_WIDTH * 4;
				levelTextTimer = 1.5f;
				level3.wolvesTriggered = false;
				level3.howlTimer = 0;
				level3.loc2CheckpointActive = false;
				level3.finalDoorOpen = false;
				for (int w = 0; w < numWolves; w++) {
					wolves[w].active = false;
					wolves[w].health = 100;
				}
			}
			isTransitioning = false;
		}
		return;
	}
	if (transitionAlpha > 0) transitionAlpha -= 2.0f * dt;

	if (levelTextTimer > 0) levelTextTimer -= dt;
	if (screenFreezeTimer > 0) { screenFreezeTimer -= dt; return; }

	// ========================================
	// Location 2 Update
	// ========================================
	if (currentLocation == 2) {
		// Health Potion
		static bool hPrev3b = false;
		bool hDown3b = isKeyPressed('h') || isKeyPressed('H');
		if (hDown3b && !hPrev3b) {
			if (player.healthPotions > 0 && player.health < 100) {
				player.healthPotions--;
				player.health += 30;
				if (player.health > 100) player.health = 100;
			}
		}
		hPrev3b = hDown3b;

		// Movement
		float speedL2 = 420.0f;
		player.isMoving = false;
		if (!player.isAttacking && !player.isSwordAttacking) {
			if (isKeyPressed('d') != 0 || isKeyPressed('D') != 0 || isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
				player.x += speedL2 * dt;
				player.isFacingRight = true;
				player.isMoving = true;
			}
			if (isKeyPressed('a') != 0 || isKeyPressed('A') != 0 || isSpecialKeyPressed(GLUT_KEY_LEFT)) {
				player.x -= speedL2 * dt;
				player.isFacingRight = false;
				player.isMoving = true;
			}
		}

		// Death
		if (player.health <= 0) {
			StopRunSound();
			player.health = 0;
			gameState = STATE_GAMEOVER;
			gameOverTimer = 0;
			PlayGameOverMusic();
			return;
		}

		// Jump
		bool spacePressed2 = (isKeyPressed(' ') != 0) || isSpecialKeyPressed(GLUT_KEY_UP);
		if (!player.isAttacking && !player.isSwordAttacking && spacePressed2 && !player.spacePrev) {
			if (player.jumpCount < 2) {
				player.isJumping = true;
				player.jumpCount++;
				player.jumpVel = (player.jumpCount == 1) ? 550.0f : 650.0f;
				player.jump.currentFrame = 0;
				player.jump.timer = 0;
				PlayJumpSound();
			}
		}
		player.spacePrev = spacePressed2;

		if (player.isJumping) {
			player.y += player.jumpVel * dt;
			player.jumpVel -= 1000.0f * dt;
			if (player.y <= level3.groundY) {
				player.y = level3.groundY;
				player.isJumping = false;
				player.jumpVel = 0;
				player.jumpCount = 0;
			}
		}

		// Boundaries
		if (player.x < 40) player.x = 40;
		if (player.x > level3.levelWidth - 40) player.x = level3.levelWidth - 40;

		// Camera
		level3.cameraX = player.x - SCREEN_WIDTH / 2;
		if (level3.cameraX < 0) level3.cameraX = 0;
		if (level3.cameraX > level3.levelWidth - SCREEN_WIDTH) level3.cameraX = level3.levelWidth - SCREEN_WIDTH;

		// Animation Update
		UpdateAnim(player.idle, dt);
		UpdateAnim(player.runRight, dt);
		UpdateAnim(player.runLeft, dt);
		if (player.isJumping) UpdateAnim(player.jump, dt);
		if (player.isAttacking) {
			float prevT = player.attack.timer;
			UpdateAnim(player.attack, dt);
			if (player.attack.timer < prevT) player.isAttacking = false;
		}
		if (player.isSwordAttacking) {
			AnimSequence &sa = player.isFacingRight ? player.swordAttackRight : player.swordAttackLeft;
			int prevFrame = sa.currentFrame;
			UpdateAnim(sa, dt);
			// Stop when animation loops back to frame 0 (played all 4 frames)
			if (sa.currentFrame < prevFrame) player.isSwordAttacking = false;
		}

		// Fork Attack (F) — same bubble pop mechanic
		bool fPressed2 = (isKeyPressed('f') != 0) || (isKeyPressed('F') != 0) || isSpecialKeyPressed(GLUT_KEY_DOWN);
		if (fPressed2 && !player.isAttacking && !player.isSwordAttacking) {
			player.isAttacking = true;
			player.attack.currentFrame = 0;
			player.attack.timer = 0;
		}

		// Sword Attack (J) — wolf combat
		bool jPressed = (isKeyPressed('j') != 0) || (isKeyPressed('J') != 0);
		if (jPressed && !player.isSwordAttacking && !player.isAttacking && player.hasSword) {
			player.isSwordAttacking = true;
			player.swordAttackRight.currentFrame = 0;
			player.swordAttackRight.timer = 0;
			player.swordAttackLeft.currentFrame = 0;
			player.swordAttackLeft.timer = 0;
		}

		// Run Sound
		static bool isRunSoundPlaying3b = false;
		static int runGraceFrames3b = 0;
		if (player.isMoving && !player.isJumping) {
			runGraceFrames3b = 10;
			if (!isRunSoundPlaying3b) { PlayRunSound(); isRunSoundPlaying3b = true; }
		} else {
			if (runGraceFrames3b > 0) runGraceFrames3b--;
			else if (isRunSoundPlaying3b) { StopRunSound(); isRunSoundPlaying3b = false; }
		}

		// Spike Traps
		for (int i = 0; i < level3.numLoc2Spikes; i++) {
			level3.loc2Spikes[i].timer += dt;
			if (level3.loc2Spikes[i].timer > 3.0f) {
				level3.loc2Spikes[i].extended = !level3.loc2Spikes[i].extended;
				level3.loc2Spikes[i].timer = 0;
			}
			if (level3.loc2Spikes[i].extended) {
				UpdateAnim(level3.loc2Spikes[i].anim, dt);
				if (abs(player.x - level3.loc2Spikes[i].x) < 80 && player.y <= level3.groundY + 160) {
					player.health -= 30 * dt;
				}
			}
		}

		// Checkpoint
		if (!level3.loc2CheckpointActive && player.x >= level3.loc2CheckpointX) {
			level3.loc2CheckpointActive = true;
		}

		// Wolf Triggers — staggered at different positions
		// Wolf 0: spawns from behind when player reaches x=2400
		if (!level3.wolvesTriggered && player.x >= 2400) {
			level3.wolvesTriggered = true;
			level3.howlTimer = 2.5f;
			PlayWolfHowlSound();
			screenShakeTimer = 0.5f;
			wolves[0].active = true;
			wolves[0].x = player.x - 500;
			wolves[0].y = level3.groundY;
			wolves[0].health = 400;
			wolves[0].isAttacking = false;
			wolves[0].attackCooldown = 0;
		}
		// Wolf 1: spawns from ahead when player reaches x=3000
		if (level3.wolvesTriggered && !wolves[1].active && wolves[1].health > 0 && player.x >= 3000) {
			wolves[1].active = true;
			wolves[1].x = player.x + 600;
			wolves[1].y = level3.groundY;
			wolves[1].health = 400;
			wolves[1].isAttacking = false;
			wolves[1].attackCooldown = 0;
			screenShakeTimer = 0.3f;
		}
		// Wolf 2: spawns from behind when player reaches x=3500
		if (level3.wolvesTriggered && !wolves[2].active && wolves[2].health > 0 && player.x >= 3500) {
			wolves[2].active = true;
			wolves[2].x = player.x - 700;
			wolves[2].y = level3.groundY;
			wolves[2].health = 400;
			wolves[2].isAttacking = false;
			wolves[2].attackCooldown = 0;
			screenShakeTimer = 0.3f;
		}
		if (level3.howlTimer > 0) level3.howlTimer -= dt;

		// Wolf AI
		for (int w = 0; w < numWolves; w++) {
			if (!wolves[w].active) continue;

			// Chase player
			float dx = player.x - wolves[w].x;
			wolves[w].facingRight = (dx > 0);
			float wDist = fabs(dx);

			if (wDist > 80) {
				// Run toward player
				wolves[w].x += (dx > 0 ? 1 : -1) * wolves[w].speed * dt;
				wolves[w].isAttacking = false;
				UpdateAnim(wolves[w].runRight, dt);
				UpdateAnim(wolves[w].runLeft, dt);
			} else {
				// Attack player
				if (wolves[w].attackCooldown <= 0) {
					wolves[w].isAttacking = true;
					wolves[w].attackTimer += dt;
					UpdateAnim(wolves[w].attackRight, dt);
					UpdateAnim(wolves[w].attackLeft, dt);
					player.health -= 25 * dt; // Wolf deals damage
					if (wolves[w].attackTimer > 0.5f) {
						wolves[w].attackTimer = 0;
						wolves[w].attackCooldown = 0.3f;
					}
				} else {
					wolves[w].attackCooldown -= dt;
					wolves[w].isAttacking = false;
				}
			}

			// Sword kill check
			if (player.isSwordAttacking && wDist < 100) {
				if (player.hasSword) {
					wolves[w].health -= 100 * dt;
					if (wolves[w].health <= 0) {
						wolves[w].active = false;
						AddScore(300);
						screenShakeTimer = 0.2f;
					}
				}
				// Without sword: attacks do nothing
			}
		}

		// Loc2 Bubbles
		for (int i = 0; i < level3.numLoc2Bubbles; i++) {
			if (level3.loc2Bubbles[i].active) {
				level3.loc2Bubbles[i].y = level3.loc2Bubbles[i].baseY + sin(globalTime * 2 + i) * 15;
				UpdateAnim(level3.loc2Bubbles[i].anim, dt);
				float dist = sqrt(pow(player.x - level3.loc2Bubbles[i].x, 2) + pow(player.y + 40 - level3.loc2Bubbles[i].y, 2));
				level3.loc2Bubbles[i].showHint = (dist < 150);
				if (player.isAttacking && dist < 80) {
					level3.loc2Bubbles[i].active = false;
					AddScore(100);
					screenFreezeTimer = 0.5f;
					bool allPopped = true;
					for (int j = 0; j < level3.numLoc2Bubbles; j++) if (level3.loc2Bubbles[j].active) allPopped = false;
					if (allPopped) {
						StopRunSound();
						InitMoonSealPuzzle();
						gameState = STATE_PUZZLE_MOON_SEAL;
					}
					PlayBubblePopSound();
				}
			}
		}

		// Loc2 Chests (E)
		bool ePressed3b = isKeyPressed('e') != 0 || isKeyPressed('E') != 0;
		for (int i = 0; i < level3.numLoc2Chests; i++) {
			if (level3.loc2Chests[i].x < 0) continue;
			float dist = sqrt(pow(player.x - level3.loc2Chests[i].x, 2) + pow(player.y - level3.loc2Chests[i].y, 2));
			level3.loc2Chests[i].showHint = (dist < 80);
			if (level3.loc2Chests[i].showHint && !level3.loc2Chests[i].opened && ePressed3b) {
				level3.loc2Chests[i].opened = true;
				player.healthPotions++;
				player.potionCollectTimer = 0.5f;
				AddScore(200);
				PlayCollectSound();
			}
		}

		// Final Door Logic
		if (moonSealPuzzle.isSolved && !level3.finalDoorOpen) {
			level3.finalDoorOpen = true;
		}

		if (!level3.finalDoorOpen && player.x > level3.finalDoorX - 100) {
			player.x = level3.finalDoorX - 100;
			SetColor(255, 100, 100, 255);
			iText(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 50, "POP ALL BUBBLES TO UNLOCK DOOR!", GLUT_BITMAP_HELVETICA_18);
		}

		// Victory!
		if (level3.finalDoorOpen && player.x > level3.finalDoorX + 50) {
			StopRunSound();
			ApplyTimeBonus();
			gameState = STATE_LEVEL_COMPLETED;
		}

		return;
	}

	// Health Potion Usage ('H')
	static bool hPrev3 = false;
	bool hDown3 = isKeyPressed('h') || isKeyPressed('H');
	if (hDown3 && !hPrev3) {
		if (player.healthPotions > 0 && player.health < 100) {
			player.healthPotions--;
			player.health += 30;
			if (player.health > 100) player.health = 100;
		}
	}
	hPrev3 = hDown3;

	// Movement
	float speed = 420.0f;
	player.isMoving = false;
	if (!player.isAttacking) {
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

	// Health Death -> Level 3 Restart
	if (player.health <= 0) {
		StopRunSound();
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

		// Check platform collision (sword platforms)
		bool landedOnPlatform = false;
		if (player.jumpVel < 0) { // Only when falling
			for (int pi = 0; pi < 4; pi++) {
				float platX = level3.swordPlatforms[pi][0];
				float platY = level3.swordPlatforms[pi][1];
				float platW = level3.swordPlatforms[pi][2];
				float platTop = platY + 14.0f;

				// Check if player is within horizontal range of platform
				if (player.x > platX - platW / 2 + 5 && player.x < platX + platW / 2 - 5) {
					// Check if player is crossing the platform top this frame
					if (player.y <= platTop && player.y > platTop - 30) {
						player.y = platTop;
						player.isJumping = false;
						player.jumpVel = 0;
						player.jumpCount = 0;
						landedOnPlatform = true;
						break;
					}
				}
			}
		}

		if (!landedOnPlatform && player.y <= level3.groundY) {
			player.y = level3.groundY;
			player.isJumping = false;
			player.jumpVel = 0;
			player.jumpCount = 0;
		}
	}

	// Fall off platforms when walking off edge
	if (!player.isJumping && player.y > level3.groundY) {
		bool onAnyPlatform = false;
		for (int pi = 0; pi < 4; pi++) {
			float platX = level3.swordPlatforms[pi][0];
			float platY = level3.swordPlatforms[pi][1];
			float platW = level3.swordPlatforms[pi][2];
			float platTop = platY + 14.0f;
			if (player.x > platX - platW / 2 + 5 && player.x < platX + platW / 2 - 5) {
				if (fabs(player.y - platTop) < 5.0f) {
					onAnyPlatform = true;
					break;
				}
			}
		}
		if (!onAnyPlatform) {
			player.isJumping = true;
			player.jumpVel = 0; // Start falling
		}
	}

	// Run Sound Logic with Grace Period
	static bool isRunSoundPlaying3 = false;
	static int runGraceFrames3 = 0;
	if (player.isMoving && !player.isJumping) {
		runGraceFrames3 = 10;
		if (!isRunSoundPlaying3) {
			PlayRunSound();
			isRunSoundPlaying3 = true;
		}
	} else {
		if (runGraceFrames3 > 0) {
			runGraceFrames3--;
		} else if (isRunSoundPlaying3) {
			StopRunSound();
			isRunSoundPlaying3 = false;
		}
	}

	// Boundaries
	if (player.x < 40) player.x = 40;
	if (player.x > level3.levelWidth - 40) player.x = level3.levelWidth - 40;

	// Camera Follow
	level3.cameraX = player.x - SCREEN_WIDTH / 2;
	if (level3.cameraX < 0) level3.cameraX = 0;
	if (level3.cameraX > level3.levelWidth - SCREEN_WIDTH) level3.cameraX = level3.levelWidth - SCREEN_WIDTH;

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

	// ========================================
	// Update Eye Beam Obelisks
	// ========================================
	for (int i = 0; i < level3.numObelisks; i++) {
		EyeBeamObelisk &ob = level3.obelisks[i];
		ob.timer += dt;

		switch (ob.state) {
		case EYE_DORMANT:
			if (ob.timer >= ob.dormantTime) {
				ob.state = EYE_CHARGING;
				ob.timer = 0;
			}
			break;
		case EYE_CHARGING:
			if (ob.timer >= ob.chargeTime) {
				ob.state = EYE_FIRING;
				ob.timer = 0;
			}
			break;
		case EYE_FIRING:
			if (ob.timer >= ob.fireTime) {
				ob.state = EYE_COOLDOWN;
				ob.timer = 0;
			}
			// Beam collision detection
			{
				float beamStartX = ob.x;
				float beamEndX = ob.x + ob.beamLength * ob.direction;
				float beamMinX = (beamStartX < beamEndX) ? beamStartX : beamEndX;
				float beamMaxX = (beamStartX > beamEndX) ? beamStartX : beamEndX;

				// Check if player is within beam horizontal range
				if (player.x > beamMinX - 30 && player.x < beamMaxX + 30) {
					// Check if player is at beam height (beam is ~20 pixels tall collision)
					float playerBottom = player.y;
					float playerTop = player.y + player.height;
					float beamBottom = ob.beamY - 15;
					float beamTop = ob.beamY + 15;

					if (playerBottom < beamTop && playerTop > beamBottom) {
						player.health -= 40 * dt; // High damage but not instant kill
						screenShakeTimer = 0.1f;
					}
				}
			}
			break;
		case EYE_COOLDOWN:
			if (ob.timer >= ob.cooldownTime) {
				ob.state = EYE_DORMANT;
				ob.timer = 0;
			}
			break;
		}
	}

	// ========================================
	// Checkpoint Logic
	// ========================================
	if (!level3.checkpointActive && player.x >= level3.checkpointX) {
		level3.checkpointActive = true;
	}

	// ========================================
	// Update Bubbles
	// ========================================
	for (int i = 0; i < level3.numBubbles; i++) {
		if (level3.bubbles[i].active) {
			level3.bubbles[i].y = level3.bubbles[i].baseY + sin(globalTime * 2 + i) * 15;
			UpdateAnim(level3.bubbles[i].anim, dt);

			float dist = sqrt(pow(player.x - level3.bubbles[i].x, 2) + pow(player.y + 40 - level3.bubbles[i].y, 2));
			level3.bubbles[i].showHint = (dist < 150);
			if (player.isAttacking && dist < 80) {
				level3.bubbles[i].active = false;
				AddScore(100);
				screenFreezeTimer = 0.5f;

				// Check if all popped
				bool allPopped = true;
				for (int j = 0; j < level3.numBubbles; j++) if (level3.bubbles[j].active) allPopped = false;

				if (allPopped) {
					StopRunSound();
					InitEyeAlignmentPuzzle();
					gameState = STATE_PUZZLE_EYE_ALIGNMENT;
				}
				PlayBubblePopSound();
			}
		}
	}

	// ========================================
	// Sacred Sword Collection
	// ========================================
	if (!level3.swordCollected) {
		float swordDist = sqrt(pow(player.x - level3.swordX, 2) + pow(player.y + 40 - (level3.swordY + 40), 2));
		if (swordDist < 60) {
			level3.swordCollected = true;
			player.hasSword = true;
			level3.swordPickupTimer = 2.5f;
			AddScore(500);
			PlayCollectSound();
		}
	}

	// ========================================
	// Update Chests Interaction (E Key)
	// ========================================
	bool ePressed3 = isKeyPressed('e') != 0 || isKeyPressed('E') != 0;
	for (int i = 0; i < level3.numChests; i++) {
		if (level3.chests[i].x < 0) continue;
		float dist = sqrt(pow(player.x - level3.chests[i].x, 2) + pow(player.y - level3.chests[i].y, 2));
		level3.chests[i].showHint = (dist < 80);

		if (level3.chests[i].showHint && !level3.chests[i].opened && ePressed3) {
			level3.chests[i].opened = true;
			player.healthPotions++;
			player.potionCollectTimer = 0.5f;
			AddScore(200);
			PlayCollectSound();
		}
	}

	// ========================================
	// Gate Logic
	// ========================================
	if (eyeAlignmentPuzzle.isSolved && level3.gateY < 600) level3.gateY += 100 * dt;

	// Block player at gate if not unlocked
	if (!eyeAlignmentPuzzle.isSolved && player.x > level3.gateX - 100) {
		player.x = level3.gateX - 100;
		// Show hint
		SetColor(255, 100, 100, 255);
		iText(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 50, "POP ALL BUBBLES TO UNLOCK GATE!", GLUT_BITMAP_HELVETICA_18);
	}

	// After gate opens, progress to Location 2
	if (eyeAlignmentPuzzle.isSolved && level3.gateY >= 500 && player.x > level3.gateX + 150) {
		// Level 3 Location 1 completed -> transition to Location 2
		isTransitioning = true;
	}
}

#endif // LEVEL3_HPP
