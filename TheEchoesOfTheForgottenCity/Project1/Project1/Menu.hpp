#ifndef MENU_HPP
#define MENU_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"

// Function to initialize buttons
void InitializeButtons() { // Creates and positions all 7 main menu buttons (Start, Continue, Instructions, Settings, Leaderboard, Credits, Exit) with their target game states
	char labels[7][20] = { "Start Game", "Continue", "Instructions", "Settings", "Leaderboard", "Credits", "Exit" };
	GameState states[7] = { STATE_SLIDESHOW, STATE_CONTINUE, STATE_INSTRUCTIONS, STATE_SETTINGS, STATE_SCOREBOARD, STATE_CREDITS, STATE_EXIT };

	int startY = 450;
	int gap = 60; // Slightly reduced gap to fit more buttons
	
	numButtons = 7; // Explicitly set to 7.

	for (int i = 0; i < numButtons; i++) {
		strcpy_s(menuButtons[i].text, labels[i]);
		menuButtons[i].width = 250;
		menuButtons[i].height = 50;
		menuButtons[i].x = (SCREEN_WIDTH - menuButtons[i].width) / 2;
		menuButtons[i].y = startY - (i * gap);
		menuButtons[i].isHovered = false;
		menuButtons[i].floatOffset = 0.0f;
		menuButtons[i].floatSpeed = 0.05f + (i * 0.01f); // slightly different speeds
		menuButtons[i].targetState = states[i];
	}
}

// Settings Buttons
Button settingsButtons[4];

void InitializeSettingsButtons() { // Creates and positions the 4 settings menu buttons (Music On/Off, SFX On/Off) for audio control
	char labels[4][20] = { "Music On", "Music Off", "SFX On", "SFX Off" };
	
	int startY = 450;
	int gap = 60;

	for (int i = 0; i < 4; i++) {
		strcpy_s(settingsButtons[i].text, labels[i]);
		settingsButtons[i].width = 250;
		settingsButtons[i].height = 50;
		settingsButtons[i].x = (SCREEN_WIDTH - settingsButtons[i].width) / 2;
		settingsButtons[i].y = startY - (i * gap);
		settingsButtons[i].isHovered = false;
		settingsButtons[i].floatOffset = 0.0f;
		settingsButtons[i].floatSpeed = 0.05f + (i * 0.01f);
	}
}

void DrawSettings() { // Renders the settings screen showing music and SFX toggle buttons with active/inactive visual states
	// Enable Blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 1. Background (Same as Menu)
	SetColor(255, 255, 255, 255);
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menuBgID);

	// 2. Title
	DrawGlowingText(SCREEN_WIDTH / 2 - 50, 550, "SETTINGS", GLUT_BITMAP_TIMES_ROMAN_24);

	// 3. Buttons
	for (int i = 0; i < 4; i++) {
		int bx = settingsButtons[i].x;
		int by = settingsButtons[i].y + (int)settingsButtons[i].floatOffset;
		int bw = settingsButtons[i].width;
		int bh = settingsButtons[i].height;

		bool isActive = false;
		if (i == 0) isActive = musicOn;
		else if (i == 1) isActive = !musicOn;
		else if (i == 2) isActive = sfxOn;
		else if (i == 3) isActive = !sfxOn;

		if (settingsButtons[i].isHovered || isActive) {
			// Hover/Active State
			SetColor(50, 150, 200, 150); // Cyan glow
			DrawRoundedRect(bx - 5, by - 5, bw + 10, bh + 10, 15);
			
			if (isActive) SetColor(0, 100, 0, 200); // Green tint for active
			else SetColor(20, 40, 60, 200);   // Darker center

			DrawRoundedRect(bx, by, bw, bh, 10);
			SetColor(255, 255, 255, 255); // White text
		}
		else {
			// Normal State
			SetColor(30, 60, 80, 100);   // Semi-transparent blue-grey
			DrawRoundedRect(bx, by, bw, bh, 10);
		}

		// Text Centering
		int textLen = strlen(settingsButtons[i].text);
		int textX = bx + (bw - (textLen * 9)) / 2;
		int textY = by + (bh - 13) / 2;

		DrawGlowingText(textX, textY, settingsButtons[i].text, GLUT_BITMAP_9_BY_15);
	}

	// Back instruction
	SetColor(255, 255, 255, 200);
	iText(SCREEN_WIDTH / 2 - 100, 100, "Press 'M' to return to Menu");

	glDisable(GL_BLEND);
}

void DrawMenu() { // Renders the main menu screen with background image, floating animated buttons, hover effects, and glowing text
	// Enable Blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 1. Background (Image)
	SetColor(255, 255, 255, 255); // Reset color to full opaque white for image
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menuBgID);



	// 3. Buttons
	for (int i = 0; i < numButtons; i++) {
		int bx = menuButtons[i].x;
		int by = menuButtons[i].y + (int)menuButtons[i].floatOffset;
		int bw = menuButtons[i].width;
		int bh = menuButtons[i].height;

		if (menuButtons[i].isHovered) {
			// Hover State: Brighter, slightly larger glow
			SetColor(50, 150, 200, 150); // Cyan glow
			DrawRoundedRect(bx - 5, by - 5, bw + 10, bh + 10, 15);
			SetColor(20, 40, 60, 200);   // Darker center
			DrawRoundedRect(bx, by, bw, bh, 10);
			SetColor(255, 255, 255, 255); // White text
		}
		else {
			// Normal State: Translucent bubble
			SetColor(30, 60, 80, 100);   // Semi-transparent blue-grey
			DrawRoundedRect(bx, by, bw, bh, 10);
			SetColor(100, 200, 255, 200); // Light blue border hint (optional, simpler to just use text color)
		}

		// Text Centering
		int textLen = strlen(menuButtons[i].text);
		int textX = bx + (bw - (textLen * 9)) / 2;
		int textY = by + (bh - 13) / 2;

		// Glowing Text for buttons
		DrawGlowingText(textX, textY, menuButtons[i].text, GLUT_BITMAP_9_BY_15);
	}

	glDisable(GL_BLEND);
}

void DrawCredits() { // Renders the credits screen showing the credits image centered over the menu background
	// Enable Blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 1. Background (Same as Menu)
	SetColor(255, 255, 255, 255);
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menuBgID);

	// 2. Credits Image Pop-up (Centered, 70% scale)
	float scale = 0.70f;
	float w = SCREEN_WIDTH * scale;
	float h = SCREEN_HEIGHT * scale;
	float x = (SCREEN_WIDTH - w) / 2;
	float y = (SCREEN_HEIGHT - h) / 2 - 50;

	iShowImage(x, y, w, h, creditsImageID);

	// 3. Back instruction
	SetColor(255, 255, 255, 200);
	iText(SCREEN_WIDTH / 2 - 100, 100, "Press 'M' to return to Menu");

	glDisable(GL_BLEND);
}


void DrawInstructions() { // Renders the instructions/how-to-play screen showing the instructions image centered over the menu background
	// Enable Blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 1. Background (Same as Menu)
	SetColor(255, 255, 255, 255);
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menuBgID);

	// 2. Instructions Image Pop-up (Centered, 70% scale)
	float scale = 0.70f;
	float w = SCREEN_WIDTH * scale;
	float h = SCREEN_HEIGHT * scale;
	float x = (SCREEN_WIDTH - w) / 2;
	float y = (SCREEN_HEIGHT - h) / 2 - 50;

	iShowImage(x, y, w, h, instructionsImageID);

	// 3. Back instruction
	SetColor(255, 255, 255, 200);
	iText(SCREEN_WIDTH / 2 - 100, 100, "Press 'M' to return to Menu");

	glDisable(GL_BLEND);
}

void DrawGameState(char* title) { // Renders a simple placeholder game state screen with white text on a dark background (used for unimplemented states)
	iSetColor(10, 10, 10);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	iSetColor(255, 255, 255);
	iText(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, title);
	iText(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 30, "Press 'M' to return to Menu");
}

// ===================== IN-GAME HUD BUTTONS =====================

// 2 small HUD buttons: [0]=Pause  [1]=Quit
Button ingameHudButtons[2];

// Continue button shown on the pause overlay
Button ingameContinueButton;

void InitIngameHudButtons() { // Creates and positions the in-game HUD buttons (Pause, Quit) at the top-right corner and the Continue button for the pause overlay
	// Stack at top-right corner
	int btnW = 120, btnH = 32;
	int bx = SCREEN_WIDTH - btnW - 12;

	char labels[2][20] = { "|| Pause", "X  Quit" };
	for (int i = 0; i < 2; i++) {
		strcpy_s(ingameHudButtons[i].text, labels[i]);
		ingameHudButtons[i].width  = btnW;
		ingameHudButtons[i].height = btnH;
		ingameHudButtons[i].x = bx;
		ingameHudButtons[i].y = SCREEN_HEIGHT - 130 - i * (btnH + 6);
		ingameHudButtons[i].isHovered   = false;
		ingameHudButtons[i].floatOffset = 0.0f;
		ingameHudButtons[i].floatSpeed  = 0.0f;
	}

	// Continue button (pause overlay, centred)
	strcpy_s(ingameContinueButton.text, "Continue");
	ingameContinueButton.width  = 220;
	ingameContinueButton.height = 50;
	ingameContinueButton.x = (SCREEN_WIDTH - 220) / 2;
	ingameContinueButton.y = SCREEN_HEIGHT / 2 - 80;
	ingameContinueButton.isHovered   = false;
	ingameContinueButton.floatOffset = 0.0f;
	ingameContinueButton.floatSpeed  = 0.0f;
}

// Helper: draw a small pill-shaped HUD button
static void DrawHudPill(Button& btn, int accentR, int accentG, int accentB) { // Draws a single pill-shaped HUD button with accent color border, hover glow, and centered label text
	int bx = btn.x, by = btn.y;
	int bw = btn.width, bh = btn.height;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (btn.isHovered) {
		SetColor(accentR, accentG, accentB, 255);
		DrawRoundedRect(bx - 2, by - 2, bw + 4, bh + 4, 10);
		SetColor(20, 20, 30, 240);
		DrawRoundedRect(bx, by, bw, bh, 8);
	} else {
		SetColor(0, 0, 0, 200);
		DrawRoundedRect(bx, by, bw, bh, 8);
		SetColor(accentR, accentG, accentB, 160);
		DrawRoundedRect(bx, by, bw, bh, 8);
	}

	int textLen = (int)strlen(btn.text);
	int textX = bx + (bw - textLen * 9) / 2;
	int textY = by + (bh - 15) / 2;
	SetColor(255, 255, 255, 255);
	iText(textX, textY, btn.text, GLUT_BITMAP_HELVETICA_18);

	glDisable(GL_BLEND);
}

// Draw the 2 in-game HUD buttons at top-right (call after level draw)
void DrawIngameHud() { // Draws the two persistent in-game HUD buttons (blue Pause and red Quit) on top of gameplay scenes
	DrawHudPill(ingameHudButtons[0], 80, 160, 255);   // Pause  — blue
	DrawHudPill(ingameHudButtons[1], 220, 60, 60);    // Quit   — red
}

// Draw pause overlay (dark translucent cover + PAUSED text + Continue button)
void DrawPauseOverlay() { // Renders the pause overlay with a dark translucent cover, "PAUSED" title, resume hint, and a clickable Continue button
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Dark cover
	SetColor(0, 0, 0, 170);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Panel
	int pw = 420, ph = 280;
	int px = (SCREEN_WIDTH - pw) / 2, py = (SCREEN_HEIGHT - ph) / 2 - 20;
	for (int g = 3; g > 0; g--) {
		SetColor(50, 120, 255, 15 + g * 10);
		DrawRoundedRect(px - g * 3, py - g * 3, pw + g * 6, ph + g * 6, 20);
	}
	SetColor(15, 20, 40, 230);
	DrawRoundedRect(px, py, pw, ph, 16);
	SetColor(60, 130, 255, 80);
	iRectangle(px + 2, py + 2, pw - 4, ph - 4);

	// Title
	SetColor(200, 230, 255, 255);
	DrawStrokeText(SCREEN_WIDTH / 2 - 100, py + ph - 55, 0, "PAUSED", 0.45f, 4.0f);

	// Subtitle hint
	SetColor(160, 200, 255, 160);
	iText(SCREEN_WIDTH / 2 - 90, py + ph - 90, "Press  P  to resume", GLUT_BITMAP_HELVETICA_18);

	// Continue button
	int bx = ingameContinueButton.x, by = ingameContinueButton.y + (int)ingameContinueButton.floatOffset;
	int bw = ingameContinueButton.width, bh = ingameContinueButton.height;
	if (ingameContinueButton.isHovered) {
		SetColor(50, 180, 255, 200);
		DrawRoundedRect(bx - 4, by - 4, bw + 8, bh + 8, 14);
		SetColor(20, 50, 90, 230);
		DrawRoundedRect(bx, by, bw, bh, 10);
	} else {
		SetColor(30, 70, 120, 180);
		DrawRoundedRect(bx, by, bw, bh, 10);
	}
	int textLen = (int)strlen(ingameContinueButton.text);
	int textX = bx + (bw - textLen * 9) / 2;
	int textY = by + (bh - 13) / 2;
	DrawGlowingText(textX, textY, ingameContinueButton.text, GLUT_BITMAP_9_BY_15);

	glDisable(GL_BLEND);
}



#endif // MENU_HPP

