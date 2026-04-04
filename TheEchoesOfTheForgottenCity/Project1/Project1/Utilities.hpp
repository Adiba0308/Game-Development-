#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "GameVariables.hpp"

// Colors
void SetColor(int r, int g, int b, int a) {
	glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void DrawStrokeText(float x, float y, float z, char* str, float scale=1.0f, float lineWidth=2.0f) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(scale, scale, scale);
	glLineWidth(lineWidth);
	for (int i=0; str[i]; i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glLineWidth(1.0f);
	glPopMatrix();
}

// Draw a rounded rectangle (for bubble effect)
void DrawRoundedRect(int x, int y, int w, int h, int r) {
	// Center main body
	iFilledRectangle(x + r, y, w - 2 * r, h);
	iFilledRectangle(x, y + r, w, h - 2 * r);
	// Corners
	iFilledCircle(x + r, y + r, r);
	iFilledCircle(x + w - r, y + r, r);
	iFilledCircle(x + r, y + h - r, r);
	iFilledCircle(x + w - r, y + h - r, r);
}

// Helper: Draw a glowing box (bubble effect)
void DrawGlowingBox(int x, int y, int w, int h) {
	// Outer Glow (fading layers)
	for (int i = 5; i > 0; i--) {
		SetColor(100, 200, 255, 30 - (i * 5)); // Light Cyan glow
		DrawRoundedRect(x - i, y - i, w + (2 * i), h + (2 * i), 15);
	}
	// Inner Body
	SetColor(20, 20, 40, 200); // Semi-transparent dark background
	DrawRoundedRect(x, y, w, h, 10);
	// Border Highlight
	SetColor(150, 220, 255, 200);
	iRectangle(x, y, w, h); // Sharp border
}

// Helper: Draw glowing text
void DrawGlowingText(int x, int y, char* str, void* font) {
	// Glow Effect (surrounding text)
	SetColor(100, 200, 255, 100);
	iText(x - 1, y, str, font);
	iText(x + 1, y, str, font);
	iText(x, y - 1, str, font);
	iText(x, y + 1, str, font);

	// Main Text
	SetColor(255, 255, 255, 255); // Bright White
	iText(x, y, str, font);
}

void LoadAnim(AnimSequence &anim, char* folder, char* prefix, int count, float duration, bool parenthesis) {
	anim.numFrames = count;
	anim.currentFrame = 0;
	anim.timer = 0;
	anim.frameDuration = duration;
	for (int i = 0; i < count; i++) {
		char path[100];
		if (parenthesis)
			sprintf_s(path, "asset/%s/%s (%d).png", folder, prefix, i + 1);
		else
			sprintf_s(path, "asset/%s/%s.%d.png", folder, prefix, i + 1);
		anim.textures[i] = iLoadImage(path);
	}
}

void UpdateAnim(AnimSequence &anim, float dt) {
	anim.timer += dt;
	if (anim.timer >= anim.frameDuration) {
		anim.timer = 0;
		anim.currentFrame = (anim.currentFrame + 1) % anim.numFrames;
	}
}

#endif // UTILITIES_HPP
