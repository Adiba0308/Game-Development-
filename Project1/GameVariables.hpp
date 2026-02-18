#ifndef GAME_VARIABLES_HPP
#define GAME_VARIABLES_HPP

#include "iGraphics.hpp"
#include <math.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// Screen Constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Game States
enum GameState {
	STATE_MENU,
	STATE_PLAY,
	STATE_INSTRUCTIONS,
	STATE_SCOREBOARD,
	STATE_CREDITS,
	STATE_SLIDESHOW,
	STATE_PUZZLE,
	STATE_PUZZLE_FLOW, // Pipe puzzle
	STATE_GAMEOVER,
	STATE_LEVEL_COMPLETED,
	STATE_CONTINUE, // New State
	STATE_SETTINGS, // New State
	STATE_EXIT
};

GameState gameState = STATE_MENU;
int currentLocation = 1; // 1 = Awakening Ruins, 2 = Whispering Path
float transitionAlpha = 0.0f;
bool isTransitioning = false;
float gameOverTimer = 0;
float screenShakeTimer = 0;
float shakeIntensity = 5.0f;
float levelTextTimer = 0.0f;

// --- Scoring System ---
long currentScore = 0;
bool musicOn = true;

void AddScore(int points) {
	currentScore += points;
	// Potential future hook for sound effect: if(points > 0) PlaySound("point.wav");
}

// --- Level 1 Structures & Globals ---

// --- Level 1 Structures & Globals ---

struct AnimSequence {
	unsigned int textures[20];
	int numFrames;
	int currentFrame;
	float timer;
	float frameDuration;
};

struct Player {
	float x, y;
	float vx, vy;
	int width, height;
	bool isMoving;
	bool isFacingRight;
	float health;

	AnimSequence idle;
	AnimSequence runRight;
	AnimSequence runLeft;
	AnimSequence attack;
	AnimSequence jump;

	bool isAttacking;
	bool isJumping;
	float jumpVel;
	int jumpCount;
	bool spacePrev;
	bool isMovementDisabled;
} player;

struct BgLayer {
	unsigned int textureID;
	float scrollSpeed;
	float width;
};

// --- Hazards & Entities ---

struct BladeTrap {
	float x, y;
	float angle;
	bool active;
	float radius;
};

// Bubbles
struct BubbleParticle {
	float x, y;
	float angle;
	bool active;
};
struct Bubble {
	float x, y;
	float baseY;
	bool active;
	bool isKey;
	bool showHint;
	float brightness;
	BubbleParticle particles[10];
	AnimSequence anim;
} bubbles[5];

// Fire Pillar
enum FireState { FIRE_IDLE, FIRE_IGNITION, FIRE_BURST, FIRE_COOLDOWN };
struct FireParticle {
	float x, y;
	float vx, vy;
	float alpha;
	bool active;
};
struct FirePillar {
	float x, y;
	FireState state;
	float timer;
	bool active;
	float scale; // New scale factor
	FireParticle particles[30];
	AnimSequence fireAnim; // Fire animation frames
} firePillars[2];

// --- Level 1 (Combined Locations) ---

struct Level1 {
	// Location 1 Data
	unsigned int bgImgs[2]; // b1.1, b1.2 (Shared with Loc2 bg slots essentially)
	float parallaxSpeeds[2]; // far, fore
	float cameraX;
	float levelWidth;
	float groundY;
	float gateY;
	float rumblingTimer;
	unsigned int gateTexture; // Gate image texture

	// Location 2 Specifics
	unsigned int loc2BgImgs[2]; // b1.4, b1.5
	BladeTrap traps[5];
	Bubble redBubbles[3];
	bool vinesActive;
	float checkpointX, checkpointY;
	unsigned int bladeTexSpin, bladeTexStatic;
	unsigned int vineTex;
	unsigned int pipeTexs[3];
	float gate2X, gate2Y;
	unsigned int spikeAnimTex[4]; // New animation textures
} level1;

// --- Pipe Puzzle ---

struct PipeSegment {
	int type; // 0=straight, 1=corner, 2=T
	int rotation; // 0, 90, 180, 270
	bool isLocked;
	bool hasEnergy;
	float x, y, w, h;
};

struct FlowPuzzle {
	PipeSegment grid[3][4];
	int startX, startY;
	int endX, endY;
	bool isSolved;
} flowPuzzle;

// Memory Puzzle
struct Card {
	int symbolID;
	bool isFlipped;
	bool isMatched;
	float x, y, w, h;
	float displayW;
	float flipSpeed; // Animation progress
};

struct MemoryPuzzle {
	Card cards[12];
	int flippedIndices[2];
	int numFlipped;
	float delayTimer;
	bool isSolved;
} puzzle;

// --- Transitions & Effects ---
float screenFreezeTimer = 0.0f;
float energyWaveRadius = 0.0f;
bool isGateUnlocked = false;
bool puzzleJustSolved = false;
float puzzleTimer = 0.0f;
const float puzzleTimeLimit = 60.0f;

// Slideshow Structures (Moved to top)
struct SlideData {
	char imagePath[100];
	char text[300];     // Narrative text
	char title[50];     // Slide title
	unsigned int textureID;
};

enum SlideState {
	SLIDE_FADE_IN,
	SLIDE_DISPLAY,
	SLIDE_FADE_OUT
};

struct Particle {
	float x, y;
	float speedX, speedY;
	float alpha;
	float size;
	bool active;
};

struct Slideshow {
	int currentSlide;
	SlideState state;
	float timer;       // General timer for phases
	float alpha;       // For fade effects (0.0 to 1.0)
	float zoom;        // For zoom effect (1.0 to 1.05)

	// Configuration
	float fadeInDuration;
	float displayDuration;
	float fadeOutDuration;

	Particle particles[50];
	SlideData slides[6];
} slideshow;

// Button Structure
struct Button {
	char text[50];
	int x, y;
	int width, height;
	bool isHovered;
	float floatOffset;
	float floatSpeed;
	GameState targetState;
};

// Menu Buttons
Button menuButtons[7];
int numButtons = 7;

// Animation Variables
float titleGlow = 0.0f;
float titleGlowSpeed = 0.05f;
bool titleGlowDirection = true; // true = increasing, false = decreasing
float globalTime = 0.0f;

// Textures
unsigned int menuBgID;
unsigned int creditsImageID;
unsigned int instructionsImageID;

// Forward Declarations
void StopMusic();
void PlayBackgroundMusic();
void PlayGameOverMusic();
void LoadSlideshow();
void DrawSlideshow();
void UpdateSlideshow();
void SkipSlide();
// void UpdateParticles(); // Commented out
void LoadLevel1();
void DrawLevel1();
void UpdatePlay(float dt);
void DrawPuzzle();
void DrawCard(int idx);
void DrawGate();
void DrawGameOver();
void InitPuzzle();
void UpdatePuzzle(float dt);
void InitFlowPuzzle();
void UpdateFlowPuzzle(float dt);
void DrawFlowPuzzle();
void CheckFlowConnectivity();
void DrawLocation2Elements();
void iScreenShake(float duration);
void DrawFirePillar();
void UpdateFirePillar(float dt);
void DrawBubbles();
void UpdateBubbles(float dt);
void DrawBladeTraps();
void DrawRedBubbles();
void UpdateEffects(float dt);
void LoadAnim(AnimSequence &anim, char* folder, char* prefix, int count, float duration, bool parenthesis = false);
void UpdateAnim(AnimSequence &anim, float dt);
void SetColor(int r, int g, int b, int a);
void InitializeButtons();
void DrawRoundedRect(int x, int y, int w, int h, int r);
void DrawGlowingBox(int x, int y, int w, int h);
void DrawGlowingText(int x, int y, char* str, void* font = GLUT_BITMAP_HELVETICA_18);
void DrawMenu();
void DrawGameState(char* title);
void AnimationUpdate();
void fixedUpdate();

#endif // GAME_VARIABLES_HPP
