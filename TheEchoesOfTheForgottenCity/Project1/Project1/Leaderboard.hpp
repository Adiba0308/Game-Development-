#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"
#include <stdio.h>
#include <string.h>

// ===================== DATA =====================

#define MAX_LEADERBOARD_ENTRIES 100
#define MAX_NAME_LENGTH 20
#define LEADERBOARD_FILE "leaderboard.txt"
#define TOP_DISPLAY_COUNT 10

struct LeaderboardEntry {
	char name[MAX_NAME_LENGTH + 1];
	long score;
};

struct LeaderboardState {
	LeaderboardEntry entries[MAX_LEADERBOARD_ENTRIES];
	int count;
	bool loaded;
};

LeaderboardState leaderboard = { {}, 0, false };

// Leaderboard screen buttons: 0=Back, 1=Clear
Button lbButtons[2];
// Confirm dialog buttons: 0=Yes, 1=No
Button confirmButtons[2];

// Name input state
char nameInputBuffer[MAX_NAME_LENGTH + 1] = "";
int nameInputLen = 0;
float nameInputCursorBlink = 0.0f;

// ===================== FILE I/O =====================

void LoadLeaderboard() {
	leaderboard.count = 0;
	leaderboard.loaded = true;

	FILE* fp;
	fopen_s(&fp, LEADERBOARD_FILE, "r");
	if (!fp) return; // File doesn't exist yet — that's fine

	char line[256];
	while (fgets(line, sizeof(line), fp) && leaderboard.count < MAX_LEADERBOARD_ENTRIES) {
		LeaderboardEntry entry;
		// Format: "name|score"
		char* sep = strchr(line, '|');
		if (!sep) continue; // Skip corrupted lines

		int nameLen = (int)(sep - line);
		if (nameLen > MAX_NAME_LENGTH) nameLen = MAX_NAME_LENGTH;
		strncpy_s(entry.name, line, nameLen);
		entry.name[nameLen] = '\0';

		entry.score = atol(sep + 1);
		leaderboard.entries[leaderboard.count++] = entry;
	}

	fclose(fp);
}

void SaveLeaderboardEntry(const char* name, long score) {
	if (strlen(name) == 0) return; // Don't save empty names

	FILE* fp;
	fopen_s(&fp, LEADERBOARD_FILE, "a");
	if (!fp) return;

	fprintf(fp, "%s|%ld\n", name, score);
	fclose(fp);
}

void ClearLeaderboardFile() {
	FILE* fp;
	fopen_s(&fp, LEADERBOARD_FILE, "w");
	if (fp) fclose(fp);
	leaderboard.count = 0;
}

// ===================== SORTING =====================

void SortLeaderboard() {
	// Insertion sort descending by score
	for (int i = 1; i < leaderboard.count; i++) {
		LeaderboardEntry key = leaderboard.entries[i];
		int j = i - 1;
		while (j >= 0 && leaderboard.entries[j].score < key.score) {
			leaderboard.entries[j + 1] = leaderboard.entries[j];
			j--;
		}
		leaderboard.entries[j + 1] = key;
	}
}

// ===================== BUTTON INIT =====================

void InitLeaderboardButtons() {
	// Back button (bottom-left area)
	strcpy_s(lbButtons[0].text, "Back");
	lbButtons[0].width = 200;
	lbButtons[0].height = 45;
	lbButtons[0].x = 100;
	lbButtons[0].y = 50;
	lbButtons[0].isHovered = false;
	lbButtons[0].floatOffset = 0.0f;
	lbButtons[0].floatSpeed = 0.05f;

	// Clear Leaderboard button (bottom-right area)
	strcpy_s(lbButtons[1].text, "Clear Leaderboard");
	lbButtons[1].width = 250;
	lbButtons[1].height = 45;
	lbButtons[1].x = SCREEN_WIDTH - 350;
	lbButtons[1].y = 50;
	lbButtons[1].isHovered = false;
	lbButtons[1].floatOffset = 0.0f;
	lbButtons[1].floatSpeed = 0.06f;

	// Confirm Yes
	strcpy_s(confirmButtons[0].text, "Yes, Clear All");
	confirmButtons[0].width = 200;
	confirmButtons[0].height = 45;
	confirmButtons[0].x = SCREEN_WIDTH / 2 - 220;
	confirmButtons[0].y = SCREEN_HEIGHT / 2 - 60;
	confirmButtons[0].isHovered = false;
	confirmButtons[0].floatOffset = 0.0f;
	confirmButtons[0].floatSpeed = 0.05f;

	// Confirm No
	strcpy_s(confirmButtons[1].text, "Cancel");
	confirmButtons[1].width = 200;
	confirmButtons[1].height = 45;
	confirmButtons[1].x = SCREEN_WIDTH / 2 + 20;
	confirmButtons[1].y = SCREEN_HEIGHT / 2 - 60;
	confirmButtons[1].isHovered = false;
	confirmButtons[1].floatOffset = 0.0f;
	confirmButtons[1].floatSpeed = 0.06f;
}

// ===================== DRAWING HELPERS =====================

// Draw a decorative horizontal line with fading ends
void DrawOrnamentalLine(int x1, int y, int x2) {
	int cx = (x1 + x2) / 2;
	int hw = (x2 - x1) / 2;
	for (int i = 0; i < hw; i++) {
		int alpha = (int)(120.0f * (1.0f - (float)i / hw));
		SetColor(100, 200, 255, alpha);
		iLine(cx - i, y, cx - i + 1, y);
		iLine(cx + i, y, cx + i + 1, y);
	}
	// Center diamond ornament
	SetColor(100, 200, 255, 180);
	iFilledCircle(cx, y, 3);
	SetColor(255, 255, 255, 200);
	iFilledCircle(cx, y, 1.5);
}

// Draw a medal circle with rank number
void DrawMedal(int cx, int cy, int rank) {
	int r1, g1, b1; // outer
	int r2, g2, b2; // inner
	int r3, g3, b3; // text
	float outerR = 14.0f;
	float innerR = 10.0f;

	if (rank == 1) { r1=255; g1=200; b1=0;   r2=255; g2=215; b2=50;  r3=80; g3=50; b3=0; }
	else if (rank == 2) { r1=160; g1=160; b1=170; r2=200; g2=200; b2=210; r3=60; g3=60; b3=70; }
	else { r1=180; g1=110; b1=40;  r2=205; g2=140; b2=60;  r3=80; g3=40; b3=10; }

	// Outer glow
	float pulse = (float)(sin(globalTime * 3 + rank) + 1.0f) / 2.0f;
	SetColor(r1, g1, b1, (int)(40 + pulse * 40));
	iFilledCircle(cx, cy, outerR + 4);

	// Outer ring
	SetColor(r1, g1, b1, 255);
	iFilledCircle(cx, cy, outerR);

	// Inner fill
	SetColor(r2, g2, b2, 255);
	iFilledCircle(cx, cy, innerR);

	// Rank number
	char rk[4];
	sprintf_s(rk, "%d", rank);
	SetColor(r3, g3, b3, 255);
	iText(cx - 3, cy - 5, rk, GLUT_BITMAP_HELVETICA_12);
}

// Draw a styled leaderboard button with glow layers
void DrawLeaderboardButton(Button& btn) {
	int bx = btn.x;
	int by = btn.y + (int)btn.floatOffset;
	int bw = btn.width;
	int bh = btn.height;

	if (btn.isHovered) {
		// Multi-layer glow
		for (int g = 3; g > 0; g--) {
			SetColor(50, 180, 255, 30 + g * 15);
			DrawRoundedRect(bx - g * 3, by - g * 3, bw + g * 6, bh + g * 6, 15);
		}
		// Button body
		SetColor(20, 50, 80, 230);
		DrawRoundedRect(bx, by, bw, bh, 10);
		// Top highlight strip
		SetColor(80, 200, 255, 60);
		iFilledRectangle(bx + 10, by + bh - 8, bw - 20, 3);
	} else {
		// Subtle border
		SetColor(50, 100, 140, 80);
		DrawRoundedRect(bx - 1, by - 1, bw + 2, bh + 2, 11);
		// Button body
		SetColor(20, 35, 55, 180);
		DrawRoundedRect(bx, by, bw, bh, 10);
	}

	int textLen = strlen(btn.text);
	int textX = bx + (bw - (textLen * 9)) / 2;
	int textY = by + (bh - 13) / 2;
	DrawGlowingText(textX, textY, btn.text, GLUT_BITMAP_9_BY_15);
}

// ===================== LEADERBOARD SCREEN =====================

void DrawLeaderboard() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 1. Background image
	SetColor(255, 255, 255, 255);
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menuBgID);

	// Removed full-screen dark overlay to match Credits screen style

	// 3. Main panel with layered border
	int px = 100, py = 40, pw = SCREEN_WIDTH - 200, ph = SCREEN_HEIGHT - 80;

	// Outer glow border (subtle greenish-cyan theme)
	for (int g = 3; g > 0; g--) {
		SetColor(50, 165, 180, 15 + g * 10);
		DrawRoundedRect(px - g * 3, py - g * 3, pw + g * 6, ph + g * 6, 22);
	}
	// Panel body (subtle greenish theme)
	SetColor(30, 65, 70, 140);
	DrawRoundedRect(px, py, pw, ph, 18);
	// Inner border highlight
	SetColor(60, 150, 180, 50);
	iRectangle(px + 3, py + 3, pw - 6, ph - 6);

	// 4. Corner ornaments (small dots in corners)
	int corners[4][2] = { {px+15, py+15}, {px+pw-15, py+15}, {px+15, py+ph-15}, {px+pw-15, py+ph-15} };
	for (int i = 0; i < 4; i++) {
		SetColor(80, 190, 220, 100);
		iFilledCircle(corners[i][0], corners[i][1], 4);
		SetColor(200, 230, 255, 180);
		iFilledCircle(corners[i][0], corners[i][1], 2);
	}

	// 5. Title with stroke text + subtitle (Subtle bluish tint)
	SetColor(220, 240, 255, 255);
	DrawStrokeText(SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT - 105, 0, "LEADERBOARD", 0.35f, 3.5f);

	// Ornamental line under title
	DrawOrnamentalLine(px + 60, SCREEN_HEIGHT - 125, px + pw - 60);

	// Subtitle (Light bluish-grey)
	SetColor(180, 210, 240, 180);
	iText(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT - 145, "- TOP SCORES -", GLUT_BITMAP_HELVETICA_12);

	// 6. Column headers
	int headerY = SCREEN_HEIGHT - 175;

	// Header background bar
	SetColor(30, 60, 90, 120);
	DrawRoundedRect(px + 20, headerY - 8, pw - 40, 30, 5);

	SetColor(200, 235, 255, 255); // Brighter bluish headers
	iText(175, headerY, "RANK", GLUT_BITMAP_HELVETICA_18);
	iText(350, headerY, "PLAYER", GLUT_BITMAP_HELVETICA_18);
	iText(820, headerY, "SCORE", GLUT_BITMAP_HELVETICA_18);

	// 7. Entries
	if (!leaderboard.loaded) {
		LoadLeaderboard();
		SortLeaderboard();
	}

	if (leaderboard.count == 0) {
		// Empty state with icon
		SetColor(60, 100, 140, 150);
		iCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20, 30);
		SetColor(60, 100, 140, 100);
		iText(SCREEN_WIDTH / 2 - 4, SCREEN_HEIGHT / 2 + 14, "?", GLUT_BITMAP_TIMES_ROMAN_24);

		SetColor(120, 160, 200, 180);
		iText(SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 - 25, "No entries yet.", GLUT_BITMAP_HELVETICA_18);
		SetColor(80, 120, 160, 120);
		iText(SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2 - 50, "Start a game to see scores here!", GLUT_BITMAP_HELVETICA_12);
	} else {
		int displayCount = leaderboard.count < TOP_DISPLAY_COUNT ? leaderboard.count : TOP_DISPLAY_COUNT;
		int rowY = headerY - 48;
		int rowGap = 44;

		for (int i = 0; i < displayCount; i++) {
			char scoreStr[20];
			sprintf_s(scoreStr, "%ld", leaderboard.entries[i].score);

			int rowBgY = rowY - 8;
			int rowBgH = 38;

			// Alternating row backgrounds
			if (i % 2 == 0) {
				SetColor(255, 255, 255, 10);
			} else {
				SetColor(0, 0, 0, 30);
			}
			DrawRoundedRect(px + 25, rowBgY, pw - 50, rowBgH, 5);

			// Top 3 get special left-edge accent bar
			if (i < 3) {
				int acR, acG, acB;
				if (i == 0) { acR=255; acG=215; acB=0; }
				else if (i == 1) { acR=192; acG=192; acB=210; }
				else { acR=205; acG=140; acB=55; }

				// Left accent bar
				SetColor(acR, acG, acB, 180);
				iFilledRectangle(px + 25, rowBgY, 4, rowBgH);

				// Subtle row tint
				SetColor(acR, acG, acB, 15);
				DrawRoundedRect(px + 25, rowBgY, pw - 50, rowBgH, 5);
			}

			// Medal or rank number
			if (i < 3) {
				DrawMedal(155, rowY + 7, i + 1);
			} else {
				char rankStr[10];
				sprintf_s(rankStr, "#%d", i + 1);
				SetColor(180, 210, 240, 255); // Brighter unranked text
				iText(148, rowY + 2, rankStr, GLUT_BITMAP_HELVETICA_18);
			}

			// Name text — brighter icy blue
			SetColor(245, 255, 255, 255);
			iText(350, rowY + 2, leaderboard.entries[i].name, GLUT_BITMAP_HELVETICA_18);

			// Score — brighter icy blue
			SetColor(245, 255, 255, 255);
			iText(820, rowY + 2, scoreStr, GLUT_BITMAP_HELVETICA_18);

			rowY -= rowGap;
		}

		// Bottom ornamental line after entries
		DrawOrnamentalLine(px + 100, rowY + 15, px + pw - 100);
	}

	// 8. Buttons
	DrawLeaderboardButton(lbButtons[0]); // Back
	DrawLeaderboardButton(lbButtons[1]); // Clear

	// 9. Confirm dialog overlay
	if (gameState == STATE_LEADERBOARD_CONFIRM) {
		// Full-screen darken
		SetColor(0, 0, 0, 190);
		iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		// Dialog panel
		int dw = 520, dh = 200;
		int dx = (SCREEN_WIDTH - dw) / 2;
		int dy = (SCREEN_HEIGHT - dh) / 2;

		// Glow border
		for (int g = 3; g > 0; g--) {
			SetColor(255, 60, 60, 20 + g * 12);
			DrawRoundedRect(dx - g * 3, dy - g * 3, dw + g * 6, dh + g * 6, 18);
		}
		SetColor(18, 22, 35, 245);
		DrawRoundedRect(dx, dy, dw, dh, 15);
		SetColor(255, 70, 70, 120);
		iRectangle(dx + 2, dy + 2, dw - 4, dh - 4);

		// Warning icon (red circle with !)
		SetColor(200, 50, 50, 200);
		iFilledCircle(SCREEN_WIDTH / 2, dy + dh - 40, 16);
		SetColor(255, 255, 255, 255);
		iText(SCREEN_WIDTH / 2 - 3, dy + dh - 46, "!", GLUT_BITMAP_HELVETICA_18);

		// Warning text
		SetColor(255, 120, 120, 255);
		iText(dx + 70, dy + 120, "Are you sure you want to clear", GLUT_BITMAP_HELVETICA_18);
		iText(dx + 100, dy + 95, "ALL leaderboard data?", GLUT_BITMAP_HELVETICA_18);

		SetColor(180, 140, 140, 150);
		iText(dx + 120, dy + 70, "This action cannot be undone.", GLUT_BITMAP_HELVETICA_12);

		DrawLeaderboardButton(confirmButtons[0]); // Yes
		DrawLeaderboardButton(confirmButtons[1]); // No
	}

	// Navigation hint
	SetColor(255, 255, 255, 120);
	iText(SCREEN_WIDTH / 2 - 100, 18, "Press 'M' to return to Menu");

	glDisable(GL_BLEND);
}

// ===================== NAME INPUT SCREEN =====================

void DrawNameInput() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 1. Menu background
	SetColor(255, 255, 255, 255);
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menuBgID);

	// Removed full-screen overlay to match Credits screen style

	// 3. Floating decorative particles (static positions based on globalTime)
	for (int i = 0; i < 15; i++) {
		float px = fmod(i * 97.3f + globalTime * (3 + i % 3), (float)SCREEN_WIDTH);
		float py = fmod(i * 131.7f + globalTime * (2 + i % 4), (float)SCREEN_HEIGHT);
		float sz = 1.0f + (i % 3);
		float alpha = 30.0f + 20.0f * (float)sin(globalTime * 2 + i);
		SetColor(100, 180, 255, (int)alpha);
		iFilledCircle(px, py, sz);
	}

	// 4. Main panel
	int panelW = 560, panelH = 300;
	int panelX = (SCREEN_WIDTH - panelW) / 2;
	int panelY = (SCREEN_HEIGHT - panelH) / 2;

	// Panel outer glow (subtle greenish-cyan theme)
	for (int g = 3; g > 0; g--) {
		float pulse = (float)(sin(globalTime * 2) + 1.0f) / 2.0f;
		SetColor(50, 165, 180, (int)(15 + g * 10 + pulse * 10));
		DrawRoundedRect(panelX - g * 3, panelY - g * 3, panelW + g * 6, panelH + g * 6, 20);
	}

	// Panel body (subtle greenish theme)
	SetColor(30, 65, 70, 140);
	DrawRoundedRect(panelX, panelY, panelW, panelH, 16);

	// Inner border
	SetColor(60, 150, 180, 60);
	iRectangle(panelX + 3, panelY + 3, panelW - 6, panelH - 6);

	// Corner dots
	int pcorners[4][2] = {
		{panelX+12, panelY+12}, {panelX+panelW-12, panelY+12},
		{panelX+12, panelY+panelH-12}, {panelX+panelW-12, panelY+panelH-12}
	};
	for (int i = 0; i < 4; i++) {
		SetColor(80, 190, 220, 80);
		iFilledCircle(pcorners[i][0], pcorners[i][1], 3);
		SetColor(200, 230, 255, 150);
		iFilledCircle(pcorners[i][0], pcorners[i][1], 1.5);
	}

	// 5. Icon above title (player silhouette circle)
	int iconCX = SCREEN_WIDTH / 2;
	int iconCY = panelY + panelH - 30; // Moved up to stop text overlap
	SetColor(40, 100, 160, 150);
	iFilledCircle(iconCX, iconCY, 22);
	SetColor(60, 160, 220, 200);
	iCircle(iconCX, iconCY, 22);
	// Simple person icon (head + body lines)
	SetColor(150, 220, 255, 200);
	iFilledCircle(iconCX, iconCY + 5, 6); // Head
	iLine(iconCX, iconCY - 1, iconCX, iconCY - 12); // Body
	iLine(iconCX - 8, iconCY - 4, iconCX + 8, iconCY - 4); // Arms

	// 6. Title text (Subtle bluish tint)
	SetColor(220, 240, 255, 255);
	DrawStrokeText(SCREEN_WIDTH / 2 - 130, panelY + panelH - 85, 0, "ENTER YOUR NAME", 0.18f, 2.5f);

	// Ornamental line under title
	DrawOrnamentalLine(panelX + 40, panelY + panelH - 100, panelX + panelW - 40);

	// 7. Input field with enhanced styling
	int fieldX = panelX + 60;
	int fieldY = panelY + panelH / 2 - 25;
	int fieldW = panelW - 120;
	int fieldH = 50;

	// Field glow (subtle pulse, greenish)
	float fPulse = (float)(sin(globalTime * 2.5) + 1.0f) / 2.0f;
	for (int g = 3; g > 0; g--) {
		SetColor(40, 155, 190, (int)(10 + fPulse * 15 + g * 5));
		DrawRoundedRect(fieldX - g * 2, fieldY - g * 2, fieldW + g * 4, fieldH + g * 4, 10 + g);
	}

	// Field body
	SetColor(5, 14, 18, 230);
	DrawRoundedRect(fieldX, fieldY, fieldW, fieldH, 8);

	// Field border
	SetColor(60, 165, 190, (int)(100 + fPulse * 80));
	iRectangle(fieldX, fieldY, fieldW, fieldH);

	// Left accent bar in field
	SetColor(50, 165, 200, 120);
	iFilledRectangle(fieldX + 1, fieldY + 5, 3, fieldH - 10);

	// Text content
	SetColor(255, 255, 255, 255);
	iText(fieldX + 20, fieldY + 17, nameInputBuffer, GLUT_BITMAP_HELVETICA_18);

	// Blinking cursor
	nameInputCursorBlink += 0.1f;
	if (fmod(nameInputCursorBlink, 2.0f) < 1.0f) {
		int cursorX = fieldX + 20 + nameInputLen * 10;
		SetColor(100, 200, 255, 220);
		iFilledRectangle(cursorX, fieldY + 10, 2, fieldH - 20);
	}

	// 8. Character count bar
	int barX = fieldX;
	int barY = fieldY - 18;
	int barW = fieldW;
	float fillPct = (float)nameInputLen / MAX_NAME_LENGTH;

	// Bar background
	SetColor(30, 50, 70, 100);
	DrawRoundedRect(barX, barY, barW, 6, 3);
	// Bar fill
	if (fillPct > 0) {
		int fillR = (int)(60 + fillPct * 195);
		int fillG = (int)(200 - fillPct * 100);
		int fillB = (int)(255 - fillPct * 155);
		SetColor(fillR, fillG, fillB, 180);
		DrawRoundedRect(barX, barY, (int)(barW * fillPct), 6, 3);
	}

	char charCount[30];
	sprintf_s(charCount, "%d / %d", nameInputLen, MAX_NAME_LENGTH);
	SetColor(180, 210, 240, 200); // Light bluish text
	iText(fieldX + fieldW - 45, barY - 2, charCount, GLUT_BITMAP_HELVETICA_12);

	// 9. Instructions at bottom
	SetColor(200, 230, 255, 220); // Light bluish text
	iText(SCREEN_WIDTH / 2 - 75, panelY + 45, "Press  ENTER  to confirm", GLUT_BITMAP_HELVETICA_12);
	SetColor(160, 200, 240, 180);
	iText(SCREEN_WIDTH / 2 - 55, panelY + 25, "ESC to go back", GLUT_BITMAP_HELVETICA_12);

	glDisable(GL_BLEND);
}

// ===================== INPUT HANDLING =====================

void HandleNameInputKey(unsigned char key) {
	if (key == 13) { // Enter
		if (nameInputLen > 0) {
			strcpy_s(playerName, nameInputBuffer);

			// === Full New Game Reset ===
			currentScore = 0;
			leaderboardScoreSaved = false;
			currentLevel = 1;
			currentLocation = 1;
			levelTimer = 180.0f;
			levelTimeBonus = 0;
			isGateUnlocked = false;
			transitionAlpha = 0.0f;
			isTransitioning = false;
			gameOverTimer = 0;
			energyWaveRadius = 0.0f;
			puzzleJustSolved = false;

			// Reset puzzle states
			puzzle.isSolved = false;
			flowPuzzle.isSolved = false;
			scalePuzzle.isSolved = false;
			statuePuzzle.isSolved = false;
			eyeAlignmentPuzzle.isSolved = false;
			moonSealPuzzle.isSolved = false;

			// Reset player
			player.health = 100;
			player.x = 100;
			player.y = level1.groundY;
			player.isMoving = false;
			player.isJumping = false;
			player.isAttacking = false;
			player.jumpCount = 0;
			player.jumpVel = 0;
			player.healthPotions = 0;
			player.hasSword = false;
			player.lives = 3;

			// Reload Level 1
			LoadLevel1();

			gameState = STATE_PLAY;
			PlayBackgroundMusic();
		}
		return;
	}
	if (key == 27) { // Escape — cancel
		gameState = STATE_MENU;
		return;
	}
	if (key == 8) { // Backspace
		if (nameInputLen > 0) {
			nameInputLen--;
			nameInputBuffer[nameInputLen] = '\0';
		}
		return;
	}
	// Printable characters (exclude pipe delimiter to protect save file)
	if (key >= 32 && key <= 126 && key != '|' && nameInputLen < MAX_NAME_LENGTH) {
		nameInputBuffer[nameInputLen] = (char)key;
		nameInputLen++;
		nameInputBuffer[nameInputLen] = '\0';
	}
}

void HandleLeaderboardMouseMove(int mx, int my) {
	if (gameState == STATE_LEADERBOARD_CONFIRM) {
		for (int i = 0; i < 2; i++) {
			int by = confirmButtons[i].y + (int)confirmButtons[i].floatOffset;
			confirmButtons[i].isHovered =
				(mx >= confirmButtons[i].x && mx <= confirmButtons[i].x + confirmButtons[i].width &&
				 my >= by && my <= by + confirmButtons[i].height);
		}
	} else {
		for (int i = 0; i < 2; i++) {
			int by = lbButtons[i].y + (int)lbButtons[i].floatOffset;
			lbButtons[i].isHovered =
				(mx >= lbButtons[i].x && mx <= lbButtons[i].x + lbButtons[i].width &&
				 my >= by && my <= by + lbButtons[i].height);
		}
	}
}

void HandleLeaderboardClick(int mx, int my) {
	if (gameState == STATE_LEADERBOARD_CONFIRM) {
		if (confirmButtons[0].isHovered) { // Yes
			ClearLeaderboardFile();
			leaderboard.loaded = false;
			gameState = STATE_SCOREBOARD;
		} else if (confirmButtons[1].isHovered) { // No
			gameState = STATE_SCOREBOARD;
		}
	} else {
		if (lbButtons[0].isHovered) { // Back
			gameState = STATE_MENU;
			PlayMenuMusic();
		} else if (lbButtons[1].isHovered) { // Clear
			gameState = STATE_LEADERBOARD_CONFIRM;
		}
	}
}

#endif // LEADERBOARD_HPP
