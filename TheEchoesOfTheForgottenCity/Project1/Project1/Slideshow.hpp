#ifndef SLIDESHOW_HPP
#define SLIDESHOW_HPP

#include "GameVariables.hpp"
#include "Utilities.hpp"

// Load Slideshow Resources
void LoadSlideshow() {
	slideshow.currentSlide = 0;
	slideshow.state = SLIDE_FADE_IN;
	slideshow.timer = 0.0f;
	slideshow.alpha = 0.0f;
	slideshow.zoom = 1.0f;

	slideshow.fadeInDuration = 1.0f;   // 1 second fade in
	slideshow.displayDuration = 3.0f;  // Reduced to 3 seconds
	slideshow.fadeOutDuration = 1.0f;  // 1 second fade out

	// Initialize Particles
	for (int i = 0; i<50; i++) {
		slideshow.particles[i].active = false;
	}

	// Slide 1
	strcpy_s(slideshow.slides[0].imagePath, "asset/slide show/slide 1.png");
	strcpy_s(slideshow.slides[0].title, "The Arrival");
	strcpy_s(slideshow.slides[0].text, "For centuries, the Great Aether City has remained silent a tomb of stone and frozen air. I have come to wake the echoes of those who lived here.");

	// Slide 2
	strcpy_s(slideshow.slides[1].imagePath, "asset/slide show/slide 2.png");
	strcpy_s(slideshow.slides[1].title, "The Frozen Air");
	strcpy_s(slideshow.slides[1].text, "The Architects did not use mortar; they used Solid Air magical bubbles that held their world together. Now, these bubbles are stagnant, locking the city in a timeless sleep.");

	// Slide 3
	strcpy_s(slideshow.slides[2].imagePath, "asset/slide show/slide 3.png");
	strcpy_s(slideshow.slides[2].title, "The Resonator's Task");
	strcpy_s(slideshow.slides[2].text, "Only a Resonator can break the silence. By popping these bubbles, I can release the energy trapped within and restore the flow of logic to these ruins.");

	// Slide 4
	strcpy_s(slideshow.slides[3].imagePath, "asset/slide show/slide 4.png");
	strcpy_s(slideshow.slides[3].title, "The Path Forward");
	strcpy_s(slideshow.slides[3].text, "But the path is not easy. The city is a labyrinth of forgotten puzzles. Every pop triggers a change opening gates, shifting platforms, disabling traps, or revealing hidden paths.");

	// Slide 5
	strcpy_s(slideshow.slides[4].imagePath, "asset/slide show/slide 5.png");
	strcpy_s(slideshow.slides[4].title, "The Hunt Begins");
	strcpy_s(slideshow.slides[4].text, "As night falls, the city turns hostile. Guided only by hurricane lights, I am chased by wild wolves awakened by my actions. I must think fast, avoid traps, and use speed-boost tools to escape the relentless hunt.");

	// Slide 6
	strcpy_s(slideshow.slides[5].imagePath, "asset/slide show/slide 6.png");
	strcpy_s(slideshow.slides[5].title, "The Gate of Return");
	strcpy_s(slideshow.slides[5].text, "At the edge of the ruins stands the final gate. By surviving the chase, solving the last puzzles, and popping the final binding bubbles, I unlock my escape leaving the forgotten city behind and returning to the normal world.");

	// Load Images
	for (int i = 0; i<6; i++) {
		// Note: Ensure these images exist in the asset/slideshow folder
		slideshow.slides[i].textureID = iLoadImage(slideshow.slides[i].imagePath);
	}
}

// Particle System Logic
void UpdateParticles() {
	for (int i = 0; i<50; i++) {
		if (slideshow.particles[i].active) {
			slideshow.particles[i].y += slideshow.particles[i].speedY;
			slideshow.particles[i].x += slideshow.particles[i].speedX;
			slideshow.particles[i].alpha -= 0.005f;

			if (slideshow.particles[i].alpha <= 0.0f) {
				slideshow.particles[i].active = false;
			}
		}
		else {
			// Respawn occasionally
			if (rand() % 100 < 2) {
				slideshow.particles[i].active = true;
				slideshow.particles[i].x = rand() % SCREEN_WIDTH;
				slideshow.particles[i].y = rand() % SCREEN_HEIGHT;
				slideshow.particles[i].speedY = (rand() % 5 + 1) * 0.1f; // Slow float up
				slideshow.particles[i].speedX = (rand() % 3 - 1) * 0.05f;
				slideshow.particles[i].alpha = (rand() % 50 + 50) / 255.0f; // Initial alpha
				slideshow.particles[i].size = (rand() % 3 + 1);
			}
		}
	}
}

// Draw Slideshow
void DrawSlideshow() {
	// 1. Draw Background/Image with zoom and opacity
	// We use iShowImage. If we want zoom, we can adjust width/height and center it.
	// iShowImage(int x, int y, int width, int height, unsigned int texture)

	int w = SCREEN_WIDTH * slideshow.zoom;
	int h = SCREEN_HEIGHT * slideshow.zoom;
	int x = (SCREEN_WIDTH - w) / 2;
	int y = (SCREEN_HEIGHT - h) / 2;

	iShowImage(x, y, w, h, slideshow.slides[slideshow.currentSlide].textureID);

	// 2. Dark Overlay for mood (always present but subtle, or part of image)
	// Here we can rely on the image + fade in/out

	// 3. Draw Particles (Soft glowing dust)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending for glow
	/*
	for (int i = 0; i<50; i++) {
		if (slideshow.particles[i].active) {
			SetColor(200, 240, 255, slideshow.particles[i].alpha * 255);
			iFilledCircle(slideshow.particles[i].x, slideshow.particles[i].y, slideshow.particles[i].size);
		}
	}
	*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Reset blending

	// 4. Draw Text

	// Title Box
	int titleW = 300;
	int titleX = (SCREEN_WIDTH - titleW) / 2;
	DrawGlowingBox(titleX, 190, titleW, 40);

	// Title Text (Light & Large)
	DrawGlowingText(SCREEN_WIDTH / 2 - 100, 200, slideshow.slides[slideshow.currentSlide].title, GLUT_BITMAP_TIMES_ROMAN_24);

	// Narrative Text Box
	int navW = 800;
	int navH = 100;
	int navX = (SCREEN_WIDTH - navW) / 2;
	int navY = 50;
	DrawGlowingBox(navX, navY, navW, navH);

	// Narrative Text
	SetColor(255, 255, 255, 255 * slideshow.alpha);
	char* textPtr = slideshow.slides[slideshow.currentSlide].text;
	int len = strlen(textPtr);
	int lineLen = 80;
	int yPos = navY + navH - 30; // Start near top of box

	char buffer[100];
	int charIdx = 0;
	int lineStart = 0;

	for (int i = 0; i <= len; i++) {
		if (charIdx >= lineLen && (textPtr[i] == ' ' || textPtr[i] == '\0')) {
			strncpy_s(buffer, 100, textPtr + lineStart, charIdx);
			buffer[charIdx] = '\0';
			iText(SCREEN_WIDTH / 2 - (charIdx * 4), yPos, buffer, GLUT_BITMAP_HELVETICA_18);
			yPos -= 25;
			lineStart = i + 1;
			charIdx = 0;
		}
		else {
			charIdx++;
		}
	}
	if (charIdx > 0) {
		strncpy_s(buffer, 100, textPtr + lineStart, charIdx);
		buffer[charIdx] = '\0';
		iText(SCREEN_WIDTH / 2 - (charIdx * 4), yPos, buffer, GLUT_BITMAP_HELVETICA_18);
	}


	// 5. Fade Overlay (Black Rect with Alpha)
	if (slideshow.state == SLIDE_FADE_IN) {
		float fadeAlpha = 1.0f - slideshow.alpha; // 1 to 0
		if (fadeAlpha > 0) {
			SetColor(0, 0, 0, fadeAlpha * 255);
			iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
	else if (slideshow.state == SLIDE_FADE_OUT) {
		float fadeAlpha = 1.0f - slideshow.alpha; // 0 to 1 (alpha goes 1->0 in logic, so 1-alpha goes 0->1)
		// Wait, logic: alpha is 'visibility of slide'.
		// Fade In: alpha 0->1. Overlay alpha: 1->0.
		// Fade Out: alpha 1->0. Overlay alpha: 0->1.
		SetColor(0, 0, 0, (1.0f - slideshow.alpha) * 255);
		iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	glDisable(GL_BLEND);
}

// Update Slideshow Logic
void UpdateSlideshow() {
	float dt = 0.016f; // Approx dt for 60fps called in AnimationUpdate or separate timer
	// Or we rely on fixed timer calls.

	// Zoom effect
	if (slideshow.state == SLIDE_DISPLAY || slideshow.state == SLIDE_FADE_IN)
		slideshow.zoom += 0.0002f; // Slow zoom

	if (slideshow.state == SLIDE_FADE_IN) {
		slideshow.alpha += dt / slideshow.fadeInDuration;
		if (slideshow.alpha >= 1.0f) {
			slideshow.alpha = 1.0f;
			slideshow.state = SLIDE_DISPLAY;
			slideshow.timer = 0.0f;
		}
	}
	else if (slideshow.state == SLIDE_DISPLAY) {
		slideshow.timer += dt;
		if (slideshow.timer >= slideshow.displayDuration) {
			slideshow.state = SLIDE_FADE_OUT;
			slideshow.timer = 0.0f;
		}
	}
	else if (slideshow.state == SLIDE_FADE_OUT) {
		slideshow.alpha -= dt / slideshow.fadeOutDuration;
		if (slideshow.alpha <= 0.0f) {
			slideshow.alpha = 0.0f;
			slideshow.currentSlide++;
			if (slideshow.currentSlide >= 6) {
				// End of slideshow -> Ask for name
				gameState = STATE_NAME_INPUT;
				
				extern char nameInputBuffer[];
				extern int nameInputLen;
				extern float nameInputCursorBlink;
				
				nameInputBuffer[0] = '\0';
				nameInputLen = 0;
				nameInputCursorBlink = 0.0f;
			}
			else {
				// Next Slide
				slideshow.state = SLIDE_FADE_IN;
				slideshow.zoom = 1.0f; // Reset zoom
			}
		}
	}

	// UpdateParticles();
}

void SkipSlide() {
	if (slideshow.state == SLIDE_FADE_OUT) return; // Already leaving
	slideshow.state = SLIDE_FADE_OUT;
	slideshow.alpha = 1.0f; // Start fading out immediately from opaque
}

#endif // SLIDESHOW_HPP
