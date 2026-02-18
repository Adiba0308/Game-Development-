#ifndef MENU_HPP
#define MENU_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"

// Function to initialize buttons
void InitializeButtons() {
	char labels[7][20] = { "Start Game", "Continue", "Instructions", "Settings", "Scoreboard", "Credits", "Exit" };
	GameState states[7] = { STATE_SLIDESHOW, STATE_CONTINUE, STATE_INSTRUCTIONS, STATE_SETTINGS, STATE_SCOREBOARD, STATE_CREDITS, STATE_EXIT };

	int startY = 450;
	int gap = 60; // Slightly reduced gap to fit more buttons
	
	numButtons = 7; // Explicitly set to 7

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
Button settingsButtons[2];

void InitializeSettingsButtons() {
	char labels[2][20] = { "Music On", "Music Off" };
	
	int startY = 400;
	int gap = 80;

	for (int i = 0; i < 2; i++) {
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

void DrawSettings() {
	// Enable Blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 1. Background (Same as Menu)
	SetColor(255, 255, 255, 255);
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menuBgID);

	// 2. Title
	DrawGlowingText(SCREEN_WIDTH / 2 - 50, 550, "SETTINGS", GLUT_BITMAP_TIMES_ROMAN_24);

	// 3. Buttons
	for (int i = 0; i < 2; i++) {
		int bx = settingsButtons[i].x;
		int by = settingsButtons[i].y + (int)settingsButtons[i].floatOffset;
		int bw = settingsButtons[i].width;
		int bh = settingsButtons[i].height;

		bool isActive = (i == 0 && musicOn) || (i == 1 && !musicOn);

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

void DrawMenu() {
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

void DrawCredits() {
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


void DrawInstructions() {
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

void DrawGameState(char* title) {
	iSetColor(10, 10, 10);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	iSetColor(255, 255, 255);
	iText(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, title);
	iText(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 30, "Press 'M' to return to Menu");
}

#endif // MENU_HPP
