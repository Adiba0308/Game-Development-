#ifndef PUZZLES_HPP
#define PUZZLES_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"

// Forward declaration
void CheckFlowConnectivity();
void InitPuzzle();
void InitFlowPuzzle();

void DrawCard(int i) {
	Card &c = puzzle.cards[i];
	if (c.isMatched) return;

	float cx = c.x + c.w / 2;
	float cy = c.y + c.h / 2;
	float dw = c.displayW;
	float halfW = dw / 2;

	// Card shadow
	SetColor(0, 0, 0, 100);
	iFilledRectangle(c.x + 5, c.y - 5, dw, c.h);

	// Main Card Body (Stone texture)
	if (c.isFlipped) {
		SetColor(180, 185, 190, 255); // Weathered stone front
	} else {
		SetColor(40, 45, 50, 255); // Dark granite back
	}
	iFilledRectangle(cx - halfW, c.y, dw, c.h);

	// Crack patterns (Deterministic offsets based on index)
	SetColor(20, 20, 20, 80);
	for (int j = 0; j < 5; j++) {
		float ox = (((i * (j + 1) * 123 + 456) % 100) - 50) * (dw / 120.0f);
		float oy = (((i * (j + 1) * 789 + 123) % 160) - 80);
		float lx = (((i + j + 5) % 20) - 10) * (dw / 120.0f);
		float ly = (((i + j + 10) % 20) - 10);
		iLine(cx + ox, cy + oy, cx + ox + lx, cy + oy + ly);
	}

	// Glowing Border
	float pulse = (sin(globalTime * 3 + i) + 1) / 2.0f;
	SetColor(0, 200, 255, (int)(150 + pulse * 105));
	iRectangle(cx - halfW, c.y, dw, c.h);

	if (c.isFlipped && dw > 20) {
		// Draw Symbol (Magical glow)
		float symbolPulse = (sin(globalTime * 5 + i) + 1) / 2.0f;
		float scale = 1.0f + symbolPulse * 0.1f;
		float r = 25 * scale * (dw / 120.0f);
		
		// Outer Glow Layer
		SetColor(0, 255, 255, (int)(50 + symbolPulse * 50));
		
		// Inner Symbol Core
		SetColor(255, 255, 255, 255);
		
		// We'll draw the symbol twice for glow effect
		for (int layer = 0; layer < 2; layer++) {
			if (layer == 0) SetColor(0, 255, 255, 100); // Glow
			else SetColor(255, 255, 255, 255); // Core

			float lr = (layer == 0) ? r + 5 : r;

			switch (c.symbolID) {
				case 0: // Triangle
					iLine(cx, cy + lr, cx - lr, cy - lr);
					iLine(cx - lr, cy - lr, cx + lr, cy - lr);
					iLine(cx + lr, cy - lr, cx, cy + lr);
					break;
				case 1: // Concentric Rings (Ancient Portal)
					iCircle(cx, cy, lr);
					iCircle(cx, cy, lr * 0.6f);
					iCircle(cx, cy, lr * 0.3f);
					break;
				case 2: // Runic Square
					iRectangle(cx - lr, cy - lr, lr * 2, lr * 2);
					iRectangle(cx - lr + 5, cy - lr + 5, lr * 2 - 10, lr * 2 - 10);
					break;
				case 3: // Lunar Crescent
					iCircle(cx, cy, lr);
					SetColor(180, 185, 190, 255); // Card color to "cut"
					if (layer == 0) SetColor(0, 255, 255, 50); // Transparent glow
					iFilledCircle(cx + lr*0.4f, cy, lr);
					break;
				case 4: // Mystic Diamond
					iLine(cx, cy + lr, cx + lr, cy);
					iLine(cx + lr, cy, cx, cy - lr);
					iLine(cx, cy - lr, cx - lr, cy);
					iLine(cx - lr, cy, cx, cy + lr);
					iCircle(cx, cy, lr * 0.4f);
					break;
				case 5: // Stellar Flare
					iCircle(cx, cy, lr * 0.5f);
					for (int a = 0; a < 12; a++) {
						float ang = a * 30 * 3.14159f / 180.0f;
						float r1 = lr * 0.5f;
						float r2 = (a % 2 == 0) ? lr : lr * 0.7f;
						iLine(cx + cos(ang)*r1, cy + sin(ang)*r1, cx + cos(ang)*r2, cy + sin(ang)*r2);
					}
					break;
			}
		}
	}
}

void DrawPuzzle() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Overlay (Deep Midnight Blue)
	SetColor(5, 10, 20, 240);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Vignette effect (Multiple semi-transparent rectangles)
	for (int i = 0; i < 10; i++) {
		SetColor(0, 0, 0, 20);
		iRectangle(i * 10, i * 10, SCREEN_WIDTH - i * 20, SCREEN_HEIGHT - i * 20);
	}

	SetColor(255, 255, 255, 255);
	iText(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 60, "ANCIENT MEMORY OF FORMS", GLUT_BITMAP_TIMES_ROMAN_24);
	SetColor(0, 200, 255, 180);
	iText(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 90, "Match the ancient symbols to unlock the ruins gate", GLUT_BITMAP_HELVETICA_18);

	// Display Timer (Glowing clock feel)
	char timeStr[20];
	sprintf_s(timeStr, "TIMER: %.1fs", puzzleTimer);
	if (puzzleTimer < 10.0f) {
		float clockPulse = (sin(globalTime * 10) + 1) / 2.0f;
		SetColor(255, (int)(50 + clockPulse * 150), 50, 255);
	} else SetColor(200, 255, 255, 230);
	iText(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 50, timeStr, GLUT_BITMAP_TIMES_ROMAN_24);

	for (int i = 0; i < 12; i++) {
		DrawCard(i);
	}

	glDisable(GL_BLEND);
}

void InitPuzzle() {
	puzzle.numFlipped = 0;
	puzzle.delayTimer = 0;
	puzzle.isSolved = false;
	puzzleTimer = 20.0f;

	int symbols[12] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	// Simple Shuffle
	for (int i = 0; i < 12; i++) {
		int r = rand() % 12;
		int temp = symbols[i];
		symbols[i] = symbols[r];
		symbols[r] = temp;
	}

	float startX = (SCREEN_WIDTH - (4 * 120 + 3 * 20)) / 2;
	float startY = (SCREEN_HEIGHT - (3 * 160 + 2 * 20)) / 2;

	for (int i = 0; i < 12; i++) {
		puzzle.cards[i].symbolID = symbols[i];
		puzzle.cards[i].isFlipped = false;
		puzzle.cards[i].isMatched = false;
		puzzle.cards[i].w = 120;
		puzzle.cards[i].h = 160;
		puzzle.cards[i].displayW = 120;
		puzzle.cards[i].flipSpeed = 1.0f; // Multiplier for W
		int col = i % 4;
		int row = i / 4;
		puzzle.cards[i].x = startX + col * (120 + 20);
		puzzle.cards[i].y = startY + row * (160 + 20);
	}
}

void UpdatePuzzle(float dt) {
	if (puzzle.delayTimer > 0) {
		puzzle.delayTimer -= dt;
		if (puzzle.delayTimer <= 0) {
			puzzle.delayTimer = 0;
			// Flip back
			puzzle.cards[puzzle.flippedIndices[0]].isFlipped = false;
			puzzle.cards[puzzle.flippedIndices[1]].isFlipped = false;
			puzzle.numFlipped = 0;
		}
	}

	bool allMatched = true;
	for (int i = 0; i < 12; i++) {
		if (!puzzle.cards[i].isMatched) allMatched = false;
	}

	if (allMatched && !puzzle.isSolved) {
		puzzle.isSolved = true;
		AddScore(500);
		energyWaveRadius = 1.0f;
		isGateUnlocked = true;
		gameState = STATE_PLAY;
	}

	// Timer Logic
	puzzleTimer -= dt;
	if (puzzleTimer <= 0) {
		puzzleTimer = 0;
		gameState = STATE_GAMEOVER;
		PlayGameOverMusic();
	}
}

void UpdateFlowPuzzle(float dt) { // Success Check
	if (flowPuzzle.isSolved) {
		level1.vinesActive = false;
		gameState = STATE_PLAY;
		return;
	}

	// Timer Logic
	puzzleTimer -= dt;
	if (puzzleTimer <= 0) {
		puzzleTimer = 0;
		gameState = STATE_GAMEOVER;
		PlayGameOverMusic();
	}
}

void DrawPipeSegment(int r, int c) {
	PipeSegment &p = flowPuzzle.grid[r][c];
	float cx = p.x + p.w/2;
	float cy = p.y + p.h/2;
	float pw = 20;

	SetColor(30, 30, 40, 255);
	iFilledRectangle(p.x, p.y, p.w, p.h);
	SetColor(0, 255, 255, p.hasEnergy ? 255 : 60);
	iRectangle(p.x, p.y, p.w, p.h);

	SetColor(180, 180, 200, 255);
	if (p.hasEnergy) SetColor(200, 0, 0, 255); // Red energy for red bubbles?

	glPushMatrix();
	glTranslatef(cx, cy, 0);
	glRotatef((float)p.rotation, 0, 0, 1);
	if (p.type == 0) iFilledRectangle(-p.w/2, -pw/2, p.w, pw);
	else if (p.type == 1) { 
		iFilledRectangle(-p.w/2, -pw/2, p.w/2+pw/2, pw); 
		iFilledRectangle(-pw/2, -p.h/2, pw, p.h/2+pw/2); 
	} else if (p.type == 2) { 
		iFilledRectangle(-p.w/2, -pw/2, p.w, pw); 
		iFilledRectangle(-pw/2, -p.h/2, pw, p.h/2+pw/2); 
	}
	glPopMatrix();
}

void DrawFlowPuzzle() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SetColor(10, 15, 25, 240);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	SetColor(255, 255, 255, 255);
	iText(SCREEN_WIDTH/2-150, SCREEN_HEIGHT-50, "SOLID AIR FLOW GRID", GLUT_BITMAP_TIMES_ROMAN_24);
	
	// Display Timer
	char timeStr[20];
	sprintf_s(timeStr, "TIME: %.1fs", puzzleTimer);
	if (puzzleTimer < 10.0f) SetColor(255, 50, 50, 255);
	else SetColor(255, 255, 255, 255);
	iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50, timeStr, GLUT_BITMAP_HELVETICA_18);

	for(int r=0; r<3; r++) {
		for(int c=0; c<4; c++) {
			DrawPipeSegment(r, c);
		}
	}
	
	// Start node
	SetColor(255, 255, 0, 255);
	iFilledCircle(400 - 40, 200 + 1*130 + 60, 25);
	// End node
	SetColor(0, 255, 0, 255);
	iFilledCircle(400 + 4*130 + 10, 200 + 1*130 + 60, 25);
	
	glDisable(GL_BLEND);
}

bool HasPort(PipeSegment &p, int port) {
	int r = p.rotation / 90;
	if (p.type == 0) return (port == (0 + r) % 4) || (port == (2 + r) % 4);
	if (p.type == 1) return (port == (2 + r) % 4) || (port == (3 + r) % 4);
	if (p.type == 2) return (port == (0 + r) % 4) || (port == (2 + r) % 4) || (port == (3 + r) % 4);
	return false;
}

void CheckFlowConnectivity() {
	for (int r = 0; r < 3; r++) for (int c = 0; c < 4; c++) flowPuzzle.grid[r][c].hasEnergy = false;
	
	struct Pos { int r, c; } queue[12];
	int head = 0, tail = 0;
	
	if (HasPort(flowPuzzle.grid[1][0], 2)) {
		flowPuzzle.grid[1][0].hasEnergy = true;
		queue[tail++] = { 1, 0 };
	}

	int dr[] = { 0, 1, 0, -1 }; // R, U, L, D
	int dc[] = { 1, 0, -1, 0 };
	int opp[] = { 2, 3, 0, 1 };

	while (head < tail) {
		Pos curr = queue[head++];
		for (int i = 0; i < 4; i++) {
			if (HasPort(flowPuzzle.grid[curr.r][curr.c], i)) {
				int nr = curr.r + dr[i];
				int nc = curr.c + dc[i];
				if (nr >= 0 && nr < 3 && nc >= 0 && nc < 4) {
					if (!flowPuzzle.grid[nr][nc].hasEnergy && HasPort(flowPuzzle.grid[nr][nc], opp[i])) {
						flowPuzzle.grid[nr][nc].hasEnergy = true;
						queue[tail++] = { nr, nc };
					}
				}
			}
		}
	}
	
	if (flowPuzzle.grid[1][3].hasEnergy && HasPort(flowPuzzle.grid[1][3], 0)) {
		flowPuzzle.isSolved = true;
		AddScore(500);
	}
}

void InitFlowPuzzle() {
	flowPuzzle.isSolved = false;
	puzzleTimer = puzzleTimeLimit;
	// grid 3x4
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 4; c++) {
			flowPuzzle.grid[r][c].type = rand() % 3; // 0=straight, 1=corner, 2=T
			flowPuzzle.grid[r][c].rotation = (rand() % 4) * 90;
			flowPuzzle.grid[r][c].x = 400 + c * 130;
			flowPuzzle.grid[r][c].y = 200 + r * 130;
			flowPuzzle.grid[r][c].w = 120;
			flowPuzzle.grid[r][c].h = 120;
			flowPuzzle.grid[r][c].hasEnergy = false;
		}
	}
	flowPuzzle.startX = 0; flowPuzzle.startY = 1; // Middle left start
	flowPuzzle.endX = 3;   flowPuzzle.endY = 1;   // Middle right end
}

void InitScalePuzzle() {
	scalePuzzle.isSolved = false;
	scalePuzzle.leftWeight = 0;
	scalePuzzle.rightWeight = 0;

	// Reset collectibles scale side
	for (int i = 0; i < level2.numCollectibles; i++) {
		if (level2.collectibles[i].collected) {
			level2.collectibles[i].placedOnScale = false;
			level2.collectibles[i].scaleSide = 0; // 0=Inventory, 1=Left, 2=Right
		}
	}
}

void DrawScalePuzzle() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Overlay (Deep Gold / Ancient Room)
	SetColor(20, 15, 5, 240);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Title & Clue
	SetColor(255, 215, 0, 255);
	iText(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 60, "BALANCE THE ANCIENT SCALES", GLUT_BITMAP_TIMES_ROMAN_24);
	SetColor(200, 255, 255, 180);
	iText(SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT - 100, "\"A diamond weighs less than truth.\"", GLUT_BITMAP_HELVETICA_18);

	// Draw the Ancient Scale
	float cx = SCREEN_WIDTH / 2;
	float cy = 250;

	// Center Pillar
	SetColor(180, 150, 50, 255); // Golden brass
	iFilledRectangle(cx - 20, cy - 150, 40, 300);
	SetColor(120, 90, 20, 255);
	iRectangle(cx - 20, cy - 150, 40, 300);
	
	// Base
	iFilledRectangle(cx - 80, cy - 180, 160, 40);

	// Calculate tilt angle based on weight difference
	// Max tilt angle ~ 20 degrees
	int wDiff = scalePuzzle.rightWeight - scalePuzzle.leftWeight;
	float targetAngle = (wDiff * 5.0f); // 5 degrees per weight diff
	if (targetAngle > 20) targetAngle = 20;
	if (targetAngle < -20) targetAngle = -20;
	
	// Smoothly interpolate angle? Let's just snap for simplicity or use sin.
	// We'll use static angle based on current sum.
	float angleRad = targetAngle * 3.14159f / 180.0f;

	// Crossbar
	float barLen = 400; // Total 800
	float bx1 = cx - cos(angleRad) * barLen;
	float by1 = cy + 130 + sin(angleRad) * barLen; // Flipped sign: down is negative y
	float bx2 = cx + cos(angleRad) * barLen;
	float by2 = cy + 130 - sin(angleRad) * barLen; // Flipped sign: down is negative y

	glLineWidth(10.0f);
	SetColor(180, 150, 50, 255);
	iLine(bx1, by1, bx2, by2);
	glLineWidth(1.0f);

	// Left Plate (Hangs straight down)
	float pDrop = 150;
	SetColor(150, 120, 30, 255);
	glLineWidth(3.0f);
	iLine(bx1, by1, bx1 - 80, by1 - pDrop);
	iLine(bx1, by1, bx1 + 80, by1 - pDrop);
	glLineWidth(1.0f);
	// Left Plate Base
	SetColor(200, 170, 60, 255);
	iFilledRectangle(bx1 - 100, by1 - pDrop - 10, 200, 20);

	// Right Plate
	glLineWidth(3.0f);
	SetColor(150, 120, 30, 255);
	iLine(bx2, by2, bx2 - 80, by2 - pDrop);
	iLine(bx2, by2, bx2 + 80, by2 - pDrop);
	glLineWidth(1.0f);
	// Right Plate Base
	SetColor(200, 170, 60, 255);
	iFilledRectangle(bx2 - 100, by2 - pDrop - 10, 200, 20);

	// Draw placed items on scales
	int leftCount = 0;
	int rightCount = 0;
	for (int i = 0; i < level2.numCollectibles; i++) {
		if (level2.collectibles[i].collected && level2.collectibles[i].placedOnScale) {
			if (level2.collectibles[i].scaleSide == 1) { // Left
				DrawCollectibleIcon(bx1 - 60 + leftCount * 40, by1 - pDrop + 10, level2.collectibles[i].type, 0.8f);
				leftCount++;
			} else if (level2.collectibles[i].scaleSide == 2) { // Right
				DrawCollectibleIcon(bx2 - 60 + rightCount * 40, by2 - pDrop + 10, level2.collectibles[i].type, 0.8f);
				rightCount++;
			}
		}
	}

	// Inventory UI at bottom (to drag from)
	float invStartX = SCREEN_WIDTH / 2 - (level2.numCollectibles * 80) / 2;
	float invStartY = 50;

	SetColor(40, 30, 20, 200);
	iFilledRectangle(invStartX - 20, invStartY - 20, level2.numCollectibles * 80 + 40, 100);
	SetColor(180, 150, 50, 255);
	iRectangle(invStartX - 20, invStartY - 20, level2.numCollectibles * 80 + 40, 100);

	for (int i = 0; i < level2.numCollectibles; i++) {
		float slotX = invStartX + i * 80;
		float slotY = invStartY;

		SetColor(50, 40, 30, 255);
		iFilledRectangle(slotX, slotY, 60, 60);
		SetColor(120, 90, 40, 255);
		iRectangle(slotX, slotY, 60, 60);

		if (level2.collectibles[i].collected && !level2.collectibles[i].placedOnScale) {
			DrawCollectibleIcon(slotX + 30, slotY + 30, level2.collectibles[i].type, 1.2f);
		}
	}

	// Verify Button
	SetColor(50, 150, 50, 255);
	iFilledRectangle(SCREEN_WIDTH - 200, 50, 150, 50);
	SetColor(255, 255, 255, 255);
	iText(SCREEN_WIDTH - 160, 65, "VERIFY", GLUT_BITMAP_HELVETICA_18);

	// Success Glow
	if (scalePuzzle.isSolved) {
		float pulse = (sin(globalTime * 5) + 1) / 2.0f;
		SetColor(255, 215, 0, (int)(100 + pulse * 155));
		iFilledCircle(cx, cy + 130, 150);
		SetColor(255, 255, 255, 255);
		iText(cx - 60, cy + 200, "BALANCED", GLUT_BITMAP_TIMES_ROMAN_24);
	}

	glDisable(GL_BLEND);
}

void UpdateScalePuzzle(float dt) {
	if (scalePuzzle.isSolved) {
		// Transition to Loc 2 equivalent
		isGateUnlocked = true;
		gameState = STATE_PLAY;
		level2.gate2Y = level2.gateY; // Ensure some smooth transitions
	}
}

void InitStatueRotationPuzzle() {
	// Statue 0: Rising Sun (East)
	statuePuzzle.statues[0].targetDir = 1;
	// Statue 1: Silent Watcher (North)
	statuePuzzle.statues[1].targetDir = 0;
	// Statue 2: Fallen King (West)
	statuePuzzle.statues[2].targetDir = 3;

	for (int i = 0; i < 3; i++) {
		// Random initial direction (0-3), but not the target
		do {
			statuePuzzle.statues[i].currentDir = rand() % 4;
		} while (statuePuzzle.statues[i].currentDir == statuePuzzle.statues[i].targetDir);
		statuePuzzle.statues[i].rotationAnim = statuePuzzle.statues[i].currentDir * 90.0f;
	}

	statuePuzzle.selectedStatue = 0;
	statuePuzzle.isSolved = false;
	statuePuzzle.successTimer = 0.0f;
}

void DrawStatueRotationPuzzle() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Dark Overlay
	SetColor(10, 10, 15, 240);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Mural / Title
	SetColor(200, 200, 220, 255);
	DrawStrokeText(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT - 80, 0, "GUARDIAN'S ALIGNMENT", 0.5f, 4.0f);

	// Clues / Inscriptions
	SetColor(150, 150, 170, 200);
	iText(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 140, "1. The guardian faces the rising sun.", GLUT_BITMAP_HELVETICA_18);
	iText(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 170, "2. The silent watcher guards the north.", GLUT_BITMAP_HELVETICA_18);
	iText(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 200, "3. The fallen king bows to the west.", GLUT_BITMAP_HELVETICA_18);

	// Draw Statues
	float startX = SCREEN_WIDTH / 2 - 250;
	float centerY = SCREEN_HEIGHT / 2 - 50;
	float spacing = 250;

	for (int i = 0; i < 3; i++) {
		float sx = startX + i * spacing;
		float sy = centerY;

		// Base
		SetColor(60, 60, 70, 255);
		iFilledCircle(sx, sy, 60, 32);
		SetColor(100, 100, 110, 255);
		iCircle(sx, sy, 60, 32);

		// Selection Highlight
		if (i == statuePuzzle.selectedStatue && !statuePuzzle.isSolved) {
			float pulse = (sin(globalTime * 5) + 1) / 2.0f;
			SetColor(0, 255, 255, (int)(100 + pulse * 155));
			iCircle(sx, sy, 70 + pulse * 5, 32);
			iText(sx - 40, sy - 100, "[ SELECTED ]", GLUT_BITMAP_HELVETICA_12);
		}

		// Statue Body (Simple Guardian Shape)
		glPushMatrix();
		glTranslatef(sx, sy, 0);
		glRotatef(-statuePuzzle.statues[i].rotationAnim, 0, 0, 1); // Rotate clockwise

		// Stone Statue visual
		SetColor(120, 120, 130, 255);
		// "Head" (Triangle pointing "North" by default)
		double headX[] = { -30, 30, 0 };
		double headY[] = { 10, 10, 50 };
		iFilledPolygon(headX, headY, 3);
		
		// "Shoulders"
		iFilledRectangle(-40, -30, 80, 40);
		
		// Eyes (Glowing cyan)
		if (statuePuzzle.isSolved) SetColor(255, 215, 0, 255); // Gold eyes if solved
		else SetColor(0, 255, 255, 255);
		iFilledCircle(-15, 25, 5);
		iFilledCircle(15, 25, 5);

		glPopMatrix();
	}

	// Hint
	SetColor(200, 200, 200, 150);
	iText(SCREEN_WIDTH / 2 - 220, 50, "A/D to Select  |  E to Rotate (90 deg)  |  Escape to Exit", GLUT_BITMAP_HELVETICA_12);

	if (statuePuzzle.isSolved) {
		float pulse = (sin(globalTime * 5) + 1) / 2.0f;
		SetColor(255, 215, 0, (int)(155 + pulse * 100));
		iText(SCREEN_WIDTH / 2 - 120, 120, "ANCIENT SEAL BROKEN!", GLUT_BITMAP_TIMES_ROMAN_24);
	}

	glDisable(GL_BLEND);
}

void UpdateStatueRotationPuzzle(float dt) {
	if (statuePuzzle.isSolved) {
		statuePuzzle.successTimer += dt;
		if (statuePuzzle.successTimer > 2.0f) {
			isGateUnlocked = true;
			gameState = STATE_PLAY;
			statuePuzzle.successTimer = 0.0f;
		}
		return;
	}

	// Smooth Rotation Animation
	for (int i = 0; i < 3; i++) {
		float targetAngle = statuePuzzle.statues[i].currentDir * 90.0f;
		// Handle wrapping (e.g., from 270 back to 0) - simplistic for now
		float diff = targetAngle - statuePuzzle.statues[i].rotationAnim;
		if (abs(diff) > 0.1f) {
			statuePuzzle.statues[i].rotationAnim += diff * 10.0f * dt;
		} else {
			statuePuzzle.statues[i].rotationAnim = targetAngle;
		}
	}

	// Input Polling
	static bool aPrev = false;
	static bool dPrev = false;
	static bool ePrev = false;

	bool aDown = isKeyPressed('a') || isKeyPressed('A');
	bool dDown = isKeyPressed('d') || isKeyPressed('D');
	bool eDown = isKeyPressed('e') || isKeyPressed('E');

	if (aDown && !aPrev) {
		statuePuzzle.selectedStatue = (statuePuzzle.selectedStatue - 1 + 3) % 3;
	}
	if (dDown && !dPrev) {
		statuePuzzle.selectedStatue = (statuePuzzle.selectedStatue + 1) % 3;
	}
	if (eDown && !ePrev) {
		statuePuzzle.statues[statuePuzzle.selectedStatue].currentDir = (statuePuzzle.statues[statuePuzzle.selectedStatue].currentDir + 1) % 4;
	}

	aPrev = aDown;
	dPrev = dDown;
	ePrev = eDown;

	if (isKeyPressed(27)) gameState = STATE_PLAY;

	// Check Success
	bool allCorrect = true;
	for (int i = 0; i < 3; i++) {
		if (statuePuzzle.statues[i].currentDir != statuePuzzle.statues[i].targetDir) allCorrect = false;
	}

	if (allCorrect) {
		statuePuzzle.isSolved = true;
		AddScore(1000);
	}
}

// ========================================
// Eye Alignment Ritual Puzzle (Level 3)
// ========================================

void InitEyeAlignmentPuzzle() {
	eyeAlignmentPuzzle.isSolved = false;
	eyeAlignmentPuzzle.selectedEye = 0;
	eyeAlignmentPuzzle.successTimer = 0.0f;

	// 5 eyes positioned around a circle
	// Each eye's target direction points toward the center
	// Position angles: 90, 162, 234, 306, 18 degrees (evenly spaced)
	float posAngles[5] = { 90.0f, 162.0f, 234.0f, 306.0f, 18.0f };
	// Target directions (pointing toward center = opposite of position angle)
	// 0=N(up), 1=NE, 2=E(right), 3=SE, 4=S(down), 5=SW, 6=W(left), 7=NW
	// Position at 90deg (top) -> must face South (4)
	// Position at 162deg (upper-left) -> must face SE (3)
	// Position at 234deg (lower-left) -> must face NE (1)
	// Position at 306deg (lower-right) -> must face NW (7)
	// Position at 18deg (upper-right) -> must face SW (5)
	int targetDirs[5] = { 4, 3, 1, 7, 5 };

	for (int i = 0; i < 5; i++) {
		eyeAlignmentPuzzle.eyes[i].posAngle = posAngles[i] * 3.14159f / 180.0f;
		eyeAlignmentPuzzle.eyes[i].targetDir = targetDirs[i];
		// Random initial direction different from target
		do {
			eyeAlignmentPuzzle.eyes[i].currentDir = rand() % 8;
		} while (eyeAlignmentPuzzle.eyes[i].currentDir == targetDirs[i]);
		eyeAlignmentPuzzle.eyes[i].rotationAnim = eyeAlignmentPuzzle.eyes[i].currentDir * 45.0f;
	}
}

void DrawEyeAlignmentPuzzle() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Dark Overlay — deep cursed chamber
	SetColor(5, 5, 12, 245);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Cracked stone background texture lines
	SetColor(15, 15, 22, 255);
	for (int i = 0; i < 20; i++) {
		float x1 = (i * 137 + 50) % SCREEN_WIDTH;
		float y1 = (i * 89 + 30) % SCREEN_HEIGHT;
		float x2 = x1 + ((i % 3) - 1) * 80;
		float y2 = y1 + ((i % 5) - 2) * 60;
		iLine(x1, y1, x2, y2);
	}

	// Runic border decoration
	SetColor(0, 80, 100, 100);
	iRectangle(30, 30, SCREEN_WIDTH - 60, SCREEN_HEIGHT - 60);
	iRectangle(35, 35, SCREEN_WIDTH - 70, SCREEN_HEIGHT - 70);
	// Corner runes
	for (int c = 0; c < 4; c++) {
		float cx = (c % 2 == 0) ? 50 : SCREEN_WIDTH - 50;
		float cy = (c < 2) ? 50 : SCREEN_HEIGHT - 50;
		SetColor(0, 120, 150, 120);
		iCircle(cx, cy, 15);
		iCircle(cx, cy, 8);
	}

	// Title
	SetColor(200, 200, 220, 255);
	DrawStrokeText(SCREEN_WIDTH / 2 - 320, SCREEN_HEIGHT - 80, 0, "EYE ALIGNMENT RITUAL", 0.45f, 4.0f);

	// Clue inscription
	SetColor(0, 180, 220, 180);
	iText(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT - 120, "\"When all watchers gaze upon the sinner, the path of the dead shall open.\"", GLUT_BITMAP_HELVETICA_18);
	SetColor(150, 150, 170, 150);
	iText(SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT - 150, "Rotate each eye to face the central cursed seal.", GLUT_BITMAP_HELVETICA_12);

	// Center of the puzzle
	float centerX = SCREEN_WIDTH / 2.0f;
	float centerY = SCREEN_HEIGHT / 2.0f - 30;
	float circleRadius = 180.0f;

	// ---- Central Cursed Seal ----
	float sealPulse = (sin(globalTime * 3) + 1) / 2.0f;
	if (eyeAlignmentPuzzle.isSolved) {
		// Bright golden seal when solved
		SetColor(255, 215, 0, (int)(180 + sealPulse * 75));
		iFilledCircle(centerX, centerY, 60);
		SetColor(255, 255, 200, 255);
		iFilledCircle(centerX, centerY, 35);
		// Crack effect
		SetColor(255, 255, 255, 200);
		iLine(centerX - 40, centerY, centerX + 40, centerY);
		iLine(centerX, centerY - 40, centerX, centerY + 40);
		iLine(centerX - 30, centerY - 30, centerX + 30, centerY + 30);
		iLine(centerX + 30, centerY - 30, centerX - 30, centerY + 30);
	} else {
		// Dark seal with faint glow
		SetColor(20, 25, 35, 255);
		iFilledCircle(centerX, centerY, 55);
		SetColor(0, 100, 130, (int)(100 + sealPulse * 80));
		iCircle(centerX, centerY, 55);
		iCircle(centerX, centerY, 45);
		// Inner runes
		SetColor(0, 80, 110, (int)(80 + sealPulse * 60));
		iCircle(centerX, centerY, 30);
		// Cross pattern
		iLine(centerX - 20, centerY, centerX + 20, centerY);
		iLine(centerX, centerY - 20, centerX, centerY + 20);
	}

	// Direction labels for reference
	char* dirNames[8] = { "N", "NE", "E", "SE", "S", "SW", "W", "NW" };

	// ---- Draw 5 Eye Obelisks around the circle ----
	for (int i = 0; i < 5; i++) {
		float angle = eyeAlignmentPuzzle.eyes[i].posAngle;
		float ex = centerX + cos(angle) * circleRadius;
		float ey = centerY + sin(angle) * circleRadius;

		// Obelisk base
		SetColor(50, 48, 58, 255);
		iFilledRectangle(ex - 35, ey - 45, 70, 90);
		SetColor(35, 33, 42, 255);
		iRectangle(ex - 35, ey - 45, 70, 90);

		// Rune lines on obelisk
		SetColor(0, 60, 80, 120);
		iLine(ex - 25, ey - 35, ex - 15, ey - 15);
		iLine(ex + 15, ey - 15, ex + 25, ey - 35);
		iLine(ex - 20, ey + 20, ex + 20, ey + 20);

		// Selection Highlight
		if (i == eyeAlignmentPuzzle.selectedEye && !eyeAlignmentPuzzle.isSolved) {
			float pulse = (sin(globalTime * 5) + 1) / 2.0f;
			SetColor(0, 255, 255, (int)(100 + pulse * 155));
			iRectangle(ex - 40, ey - 50, 80, 100);
			iRectangle(ex - 42, ey - 52, 84, 104);
			SetColor(0, 255, 255, 200);
			iText(ex - 30, ey - 65, "[ SELECTED ]", GLUT_BITMAP_HELVETICA_12);
		}

		// Eye socket
		float eyeAnimAngle = eyeAlignmentPuzzle.eyes[i].rotationAnim * 3.14159f / 180.0f;
		float eyeRadius = 20.0f;

		// Background of eye
		SetColor(25, 25, 35, 255);
		iFilledCircle(ex, ey, eyeRadius + 4);

		bool isCorrect = (eyeAlignmentPuzzle.eyes[i].currentDir == eyeAlignmentPuzzle.eyes[i].targetDir);

		// Eye iris
		if (isCorrect) {
			SetColor(0, 255, 200, 255);
		} else {
			SetColor(0, 150, 200, 200);
		}
		iFilledCircle(ex, ey, eyeRadius);

		// Pupil (directional indicator)
		float pupilDist = eyeRadius * 0.5f;
		float px = ex + cos(eyeAnimAngle) * pupilDist;
		float py = ey + sin(eyeAnimAngle) * pupilDist;
		SetColor(0, 50, 80, 255);
		iFilledCircle(px, py, eyeRadius * 0.35f);
		SetColor(0, 255, 255, 255);
		iFilledCircle(px, py, eyeRadius * 0.15f);

		// Direction beam line from eye toward its facing direction
		float beamDist = 60.0f;
		float bx = ex + cos(eyeAnimAngle) * beamDist;
		float by = ey + sin(eyeAnimAngle) * beamDist;

		if (isCorrect) {
			// Green/cyan beam when correct
			glLineWidth(3.0f);
			SetColor(0, 255, 200, 200);
			iLine(ex, ey, bx, by);
			glLineWidth(1.0f);
			// Show beam going to center
			SetColor(0, 255, 200, 100);
			glLineWidth(2.0f);
			iLine(ex, ey, centerX, centerY);
			glLineWidth(1.0f);
		} else {
			// Dim beam indicator when incorrect
			glLineWidth(2.0f);
			SetColor(0, 100, 150, 120);
			iLine(ex, ey, bx, by);
			glLineWidth(1.0f);
		}

		// Direction label
		int dir = eyeAlignmentPuzzle.eyes[i].currentDir;
		SetColor(200, 200, 200, 150);
		iText(ex - 5, ey + 50, dirNames[dir], GLUT_BITMAP_HELVETICA_12);
	}

	// ---- Floating dust particles ----
	for (int i = 0; i < 10; i++) {
		float px = fmod(globalTime * 15 + i * 130, (float)SCREEN_WIDTH);
		float py = 100 + sin(globalTime * 0.8f + i * 1.7f) * 200 + i * 40;
		float alpha = (sin(globalTime * 2 + i) + 1) / 2.0f;
		SetColor(0, 180, 220, (int)(15 + alpha * 20));
		iFilledCircle(px, py, 2);
	}

	// ---- Controls hint ----
	SetColor(200, 200, 200, 150);
	iText(SCREEN_WIDTH / 2 - 220, 40, "A/D to Select  |  E to Rotate (45 deg)  |  Escape to Exit", GLUT_BITMAP_HELVETICA_12);

	// ---- Solved feedback ----
	if (eyeAlignmentPuzzle.isSolved) {
		float pulse = (sin(globalTime * 5) + 1) / 2.0f;
		SetColor(255, 215, 0, (int)(155 + pulse * 100));
		DrawStrokeText(SCREEN_WIDTH / 2 - 280, 100, 0, "ANCIENT SEAL BROKEN!", 0.4f, 4.0f);

		// Dramatic radial glow from center
		SetColor(255, 215, 0, (int)(40 + pulse * 40));
		iFilledCircle(centerX, centerY, 100 + pulse * 30);
		SetColor(0, 255, 255, (int)(30 + pulse * 30));
		iFilledCircle(centerX, centerY, 150 + pulse * 20);
	}

	glDisable(GL_BLEND);
}

void UpdateEyeAlignmentPuzzle(float dt) {
	if (eyeAlignmentPuzzle.isSolved) {
		eyeAlignmentPuzzle.successTimer += dt;
		if (eyeAlignmentPuzzle.successTimer > 2.0f) {
			isGateUnlocked = true;
			gameState = STATE_PLAY;
			eyeAlignmentPuzzle.successTimer = 0.0f;
		}
		return;
	}

	// Smooth Rotation Animation (8 directions, 45 degrees each)
	for (int i = 0; i < 5; i++) {
		float targetAngle = eyeAlignmentPuzzle.eyes[i].currentDir * 45.0f;
		float diff = targetAngle - eyeAlignmentPuzzle.eyes[i].rotationAnim;
		// Handle wrapping around 360
		if (diff > 180) diff -= 360;
		if (diff < -180) diff += 360;
		if (abs(diff) > 0.5f) {
			eyeAlignmentPuzzle.eyes[i].rotationAnim += diff * 8.0f * dt;
			// Keep in range [0, 360)
			if (eyeAlignmentPuzzle.eyes[i].rotationAnim < 0) eyeAlignmentPuzzle.eyes[i].rotationAnim += 360;
			if (eyeAlignmentPuzzle.eyes[i].rotationAnim >= 360) eyeAlignmentPuzzle.eyes[i].rotationAnim -= 360;
		} else {
			eyeAlignmentPuzzle.eyes[i].rotationAnim = targetAngle;
		}
	}

	// Input Polling
	static bool aPrevEye = false;
	static bool dPrevEye = false;
	static bool ePrevEye = false;

	bool aDownEye = isKeyPressed('a') || isKeyPressed('A');
	bool dDownEye = isKeyPressed('d') || isKeyPressed('D');
	bool eDownEye = isKeyPressed('e') || isKeyPressed('E');

	if (aDownEye && !aPrevEye) {
		eyeAlignmentPuzzle.selectedEye = (eyeAlignmentPuzzle.selectedEye - 1 + 5) % 5;
	}
	if (dDownEye && !dPrevEye) {
		eyeAlignmentPuzzle.selectedEye = (eyeAlignmentPuzzle.selectedEye + 1) % 5;
	}
	if (eDownEye && !ePrevEye) {
		eyeAlignmentPuzzle.eyes[eyeAlignmentPuzzle.selectedEye].currentDir = (eyeAlignmentPuzzle.eyes[eyeAlignmentPuzzle.selectedEye].currentDir + 1) % 8;
	}

	aPrevEye = aDownEye;
	dPrevEye = dDownEye;
	ePrevEye = eDownEye;

	if (isKeyPressed(27)) gameState = STATE_PLAY;

	// Check Success — all eyes facing correct direction
	bool allCorrect = true;
	for (int i = 0; i < 5; i++) {
		if (eyeAlignmentPuzzle.eyes[i].currentDir != eyeAlignmentPuzzle.eyes[i].targetDir) allCorrect = false;
	}

	if (allCorrect) {
		eyeAlignmentPuzzle.isSolved = true;
		AddScore(1500); // Higher score for final level puzzle
	}
}

// ========================================
// Moon Seal Constellation Ritual Puzzle (Level 3 Loc 2)
// ========================================

void InitMoonSealPuzzle() {
	moonSealPuzzle.isSolved = false;
	moonSealPuzzle.selectedRing = 0;
	moonSealPuzzle.successTimer = 0.0f;

	// 4 rings, each with a target direction (0-7, 45 degrees each)
	moonSealPuzzle.targetDirs[0] = 2; // Ring 0: East
	moonSealPuzzle.targetDirs[1] = 5; // Ring 1: SW
	moonSealPuzzle.targetDirs[2] = 0; // Ring 2: North
	moonSealPuzzle.targetDirs[3] = 7; // Ring 3: NW

	for (int i = 0; i < 4; i++) {
		do {
			moonSealPuzzle.ringDirs[i] = rand() % 8;
		} while (moonSealPuzzle.ringDirs[i] == moonSealPuzzle.targetDirs[i]);
		moonSealPuzzle.ringAnims[i] = moonSealPuzzle.ringDirs[i] * 45.0f;
	}
}

void DrawMoonSealPuzzle() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Deep cosmic dark overlay
	SetColor(3, 3, 10, 248);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Cracked stone texture
	SetColor(12, 12, 20, 255);
	for (int i = 0; i < 25; i++) {
		float x1 = (i * 157 + 30) % SCREEN_WIDTH;
		float y1 = (i * 97 + 50) % SCREEN_HEIGHT;
		float x2 = x1 + ((i % 3) - 1) * 90;
		float y2 = y1 + ((i % 5) - 2) * 70;
		iLine(x1, y1, x2, y2);
	}

	// Runic border
	SetColor(0, 60, 90, 100);
	iRectangle(25, 25, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50);
	iRectangle(30, 30, SCREEN_WIDTH - 60, SCREEN_HEIGHT - 60);

	// Title
	SetColor(200, 200, 230, 255);
	DrawStrokeText(SCREEN_WIDTH / 2 - 380, SCREEN_HEIGHT - 75, 0, "MOON SEAL CONSTELLATION RITUAL", 0.38f, 3.5f);

	// Clue
	SetColor(0, 180, 220, 170);
	iText(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT - 110, "\"When moon and stars remember their order, the last path shall awaken.\"", GLUT_BITMAP_HELVETICA_18);

	float centerX = SCREEN_WIDTH / 2.0f;
	float centerY = SCREEN_HEIGHT / 2.0f - 40;

	// Draw 4 concentric rings
	float ringRadii[4] = { 80, 130, 180, 230 };
	int ringColors[4][3] = { {0, 200, 255}, {0, 180, 220}, {0, 150, 200}, {0, 120, 180} };

	for (int r = 3; r >= 0; r--) {
		float radius = ringRadii[r];
		float animAngle = moonSealPuzzle.ringAnims[r] * 3.14159f / 180.0f;
		bool isCorrect = (moonSealPuzzle.ringDirs[r] == moonSealPuzzle.targetDirs[r]);

		// Ring background
		SetColor(20, 20, 30, 200);
		iCircle(centerX, centerY, radius + 8);
		iCircle(centerX, centerY, radius - 8);

		// Ring body
		if (isCorrect) {
			SetColor(0, 255, 200, 200);
		} else {
			SetColor(ringColors[r][0], ringColors[r][1], ringColors[r][2], 150);
		}
		iCircle(centerX, centerY, radius);
		iCircle(centerX, centerY, radius + 2);

		// Moon symbol on ring (crescent shape)
		float moonX = centerX + cos(animAngle) * radius;
		float moonY = centerY + sin(animAngle) * radius;
		if (isCorrect) SetColor(255, 255, 200, 255);
		else SetColor(200, 200, 230, 200);
		iFilledCircle(moonX, moonY, 12);
		SetColor(20, 20, 30, 255);
		iFilledCircle(moonX + 5, moonY + 3, 10);

		// Star nodes around ring
		for (int s = 0; s < 4; s++) {
			float sAngle = animAngle + s * 1.5708f; // 90 degrees apart from moon
			float sx = centerX + cos(sAngle) * radius;
			float sy = centerY + sin(sAngle) * radius;
			if (isCorrect) SetColor(255, 215, 0, 220);
			else SetColor(100, 150, 200, 150);
			iFilledCircle(sx, sy, 4);
		}

		// Constellation lines (from moon to adjacent star)
		float nextAngle = animAngle + 1.5708f;
		float nx = centerX + cos(nextAngle) * radius;
		float ny = centerY + sin(nextAngle) * radius;
		if (isCorrect) {
			SetColor(0, 255, 200, 180);
			glLineWidth(2.0f);
		} else {
			SetColor(0, 100, 150, 100);
			glLineWidth(1.0f);
		}
		iLine(moonX, moonY, nx, ny);
		glLineWidth(1.0f);

		// Selection highlight
		if (r == moonSealPuzzle.selectedRing && !moonSealPuzzle.isSolved) {
			float pulse = (sin(globalTime * 5) + 1) / 2.0f;
			SetColor(0, 255, 255, (int)(100 + pulse * 155));
			iCircle(centerX, centerY, radius + 6 + pulse * 3);
			iCircle(centerX, centerY, radius - 6 - pulse * 3);
		}
	}

	// Central seal
	float sealPulse = (sin(globalTime * 3) + 1) / 2.0f;
	if (moonSealPuzzle.isSolved) {
		SetColor(255, 215, 0, (int)(200 + sealPulse * 55));
		iFilledCircle(centerX, centerY, 55);
		SetColor(255, 255, 200, 255);
		iFilledCircle(centerX, centerY, 35);
		// Radial burst
		for (int b = 0; b < 12; b++) {
			float bAngle = b * 30 * 3.14159f / 180.0f + globalTime;
			SetColor(255, 215, 0, 150);
			iLine(centerX + cos(bAngle) * 40, centerY + sin(bAngle) * 40,
				  centerX + cos(bAngle) * 70, centerY + sin(bAngle) * 70);
		}
	} else {
		SetColor(15, 15, 25, 255);
		iFilledCircle(centerX, centerY, 50);
		SetColor(0, 100, 140, (int)(80 + sealPulse * 60));
		iCircle(centerX, centerY, 50);
		iCircle(centerX, centerY, 35);
		// Moon crescent in center
		SetColor(0, 120, 160, (int)(100 + sealPulse * 50));
		iFilledCircle(centerX - 5, centerY, 20);
		SetColor(15, 15, 25, 255);
		iFilledCircle(centerX + 5, centerY + 5, 18);
	}

	// Floating particles
	for (int i = 0; i < 12; i++) {
		float px = fmod(globalTime * 12 + i * 110, (float)SCREEN_WIDTH);
		float py = 80 + sin(globalTime * 0.7f + i * 2.0f) * 200 + i * 40;
		float alpha = (sin(globalTime * 2.5f + i) + 1) / 2.0f;
		SetColor(0, 180, 220, (int)(10 + alpha * 20));
		iFilledCircle(px, py, 2);
	}

	// Ring labels
	char* ringLabels[4] = { "Inner", "Second", "Third", "Outer" };
	for (int r = 0; r < 4; r++) {
		SetColor(150, 150, 170, 150);
		iText((int)(centerX + ringRadii[r] + 15), (int)(centerY - 5), ringLabels[r], GLUT_BITMAP_HELVETICA_12);
	}

	// Controls
	SetColor(200, 200, 200, 150);
	iText(SCREEN_WIDTH / 2 - 220, 40, "A/D to Select Ring  |  E to Rotate (45 deg)  |  Escape to Exit", GLUT_BITMAP_HELVETICA_12);

	// Solved text
	if (moonSealPuzzle.isSolved) {
		float pulse = (sin(globalTime * 5) + 1) / 2.0f;
		SetColor(255, 215, 0, (int)(155 + pulse * 100));
		DrawStrokeText(SCREEN_WIDTH / 2 - 280, 100, 0, "THE FINAL SEAL IS BROKEN!", 0.4f, 4.0f);
		SetColor(0, 255, 255, (int)(100 + pulse * 80));
		iFilledCircle(centerX, centerY, 250 + pulse * 30);
	}

	glDisable(GL_BLEND);
}

void UpdateMoonSealPuzzle(float dt) {
	if (moonSealPuzzle.isSolved) {
		moonSealPuzzle.successTimer += dt;
		if (moonSealPuzzle.successTimer > 2.0f) {
			level3.finalDoorOpen = true;
			gameState = STATE_PLAY;
			moonSealPuzzle.successTimer = 0.0f;
		}
		return;
	}

	// Smooth Rotation Animation
	for (int i = 0; i < 4; i++) {
		float targetAngle = moonSealPuzzle.ringDirs[i] * 45.0f;
		float diff = targetAngle - moonSealPuzzle.ringAnims[i];
		if (diff > 180) diff -= 360;
		if (diff < -180) diff += 360;
		if (fabs(diff) > 0.5f) {
			moonSealPuzzle.ringAnims[i] += diff * 8.0f * dt;
			if (moonSealPuzzle.ringAnims[i] < 0) moonSealPuzzle.ringAnims[i] += 360;
			if (moonSealPuzzle.ringAnims[i] >= 360) moonSealPuzzle.ringAnims[i] -= 360;
		} else {
			moonSealPuzzle.ringAnims[i] = targetAngle;
		}
	}

	// Input
	static bool aPrevM = false;
	static bool dPrevM = false;
	static bool ePrevM = false;

	bool aDownM = isKeyPressed('a') || isKeyPressed('A');
	bool dDownM = isKeyPressed('d') || isKeyPressed('D');
	bool eDownM = isKeyPressed('e') || isKeyPressed('E');

	if (aDownM && !aPrevM) {
		moonSealPuzzle.selectedRing = (moonSealPuzzle.selectedRing - 1 + 4) % 4;
	}
	if (dDownM && !dPrevM) {
		moonSealPuzzle.selectedRing = (moonSealPuzzle.selectedRing + 1) % 4;
	}
	if (eDownM && !ePrevM) {
		moonSealPuzzle.ringDirs[moonSealPuzzle.selectedRing] = (moonSealPuzzle.ringDirs[moonSealPuzzle.selectedRing] + 1) % 8;
	}

	aPrevM = aDownM;
	dPrevM = dDownM;
	ePrevM = eDownM;

	if (isKeyPressed(27)) gameState = STATE_PLAY;

	// Check Success
	bool allCorrect = true;
	for (int i = 0; i < 4; i++) {
		if (moonSealPuzzle.ringDirs[i] != moonSealPuzzle.targetDirs[i]) allCorrect = false;
	}

	if (allCorrect) {
		moonSealPuzzle.isSolved = true;
		AddScore(2000);
	}
}

#endif // PUZZLES_HPP
