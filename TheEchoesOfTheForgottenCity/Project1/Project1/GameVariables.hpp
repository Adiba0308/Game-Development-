#ifndef GAME_VARIABLES_HPP
#define GAME_VARIABLES_HPP

#include "iGraphics.hpp"
#include <math.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// Screen Height and Width
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
	STATE_PUZZLE_SCALE, // Scale puzzle for Level 2
	STATE_PUZZLE_STATUE_ROTATION, // Statue Rotation puzzle for Level 2 Location 2
	STATE_PUZZLE_EYE_ALIGNMENT, // Eye Alignment puzzle for Level 3 Location 1
	STATE_PUZZLE_MOON_SEAL, // Moon Seal puzzle for Level 3 Location 2
	STATE_CONTINUE, // New State
	STATE_SETTINGS, // New State
	STATE_NAME_INPUT, // Player name entry before starting
	STATE_LEADERBOARD_CONFIRM, // Confirm dialog for clearing leaderboard
	STATE_PAUSED,        // In-game Pause overlay
	STATE_EXIT
};

GameState gameState = STATE_MENU;
int currentLevel = 1; // 1 = Awakening Ruins, 2 = level 2
int currentLocation = 1; // 1 = Awakening Ruins, 2 = Whispering Path
float transitionAlpha = 0.0f;
bool isTransitioning = false;
float gameOverTimer = 0;
float screenShakeTimer = 0;
float shakeIntensity = 5.0f;
float levelTextTimer = 0.0f;
float lastSPressTime = 0.0f;
bool sPressedOnce = false;
GameState prePauseState = STATE_PLAY; // State to return to when unpausing

// --- Scoring System ---
long currentScore = 0;
bool musicOn = true;
bool sfxOn = true;
char playerName[21] = "Player"; // Default player name
bool leaderboardScoreSaved = false; // Tracks if score was saved this game-over

void AddScore(int points) {
	currentScore += points;
	// Potential future hook for sound effect: if(points > 0) PlaySound("point.wav");
}

// --- Level Timer ---
float levelTimer = 180.0f; // 3 minutes countdown
long levelTimeBonus = 0;   // Bonus calculated on level completion

void ApplyTimeBonus() {
	float elapsed = 180.0f - levelTimer;
	if (elapsed < 1.0f) elapsed = 1.0f; // Prevent division by zero
	levelTimeBonus = (long)(currentScore / elapsed * 60.0f); // Score/time * 60 for bonus
	currentScore += levelTimeBonus;
}

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
	int healthPotions;
	float potionCollectTimer;
	bool hasSword;
	bool isSwordAttacking;
	AnimSequence swordAttackRight;
	AnimSequence swordAttackLeft;
	int lives;
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

// --- Level 2 Variables ---

struct SpikeTrap {
	float x, y;
	float timer;
	bool extended;
	AnimSequence anim; // For future or static draws
};

struct WreckingBall {
	float x, y;          // Anchor point coordinates
	float angle;         // Current swing angle (radians, 0 is straight down)
	float swingSpeed;    // Base speed multiplier
	float chainLength;   // Length of chain
	float swingRange;    // Maximum angle extent (radians)
	float timer;         // To control the swing
	float radius;        // Hitbox size
	unsigned int texture;
};

struct TreasureChest {
	float x, y;
	bool opened;
	bool showHint;
	unsigned int texClosed;
	unsigned int texOpen;
};

struct Collectible {
	int type; // 0=Gold, 1=Stone, 2=Skull, 3=Diamond, 4=Truth
	int weight;
	float x, y;
	bool collected;
	bool placedOnScale;
	int scaleSide; // 0=none, 1=left, 2=right
	unsigned int texture;
	bool showHint;
};

struct ScalePuzzle {
	bool isSolved;
	int leftWeight;
	int rightWeight;
};

struct StatueData {
	int currentDir; // 0=N, 1=E, 2=S, 3=W (0, 90, 180, 270 degrees)
	int targetDir;
	float rotationAnim; // For smooth rotation effect
};

struct StatueRotationPuzzleData {
	StatueData statues[3];
	int selectedStatue;
	bool isSolved;
	float successTimer;
};

struct Level2 {
	unsigned int bgImgs[4]; // b1.1 to b1.4
	float parallaxSpeeds[2]; // far, fore
	float cameraX;
	float levelWidth;
	float groundY;
	float gateY;
	unsigned int gateTexture;
	
	SpikeTrap spikeTraps[5];
	int numSpikes;

	WreckingBall wreckingBalls[10];
	int numWreckingBalls;

	TreasureChest chests[5];
	int numChests;

	Collectible collectibles[5];
	int numCollectibles;

	Bubble bubbles[3];
	int numBubbles;
	
	float gate2X, gate2Y;
} level2;

ScalePuzzle scalePuzzle;
StatueRotationPuzzleData statuePuzzle;

// --- Level 3 Variables ---

enum EyeBeamState { EYE_DORMANT, EYE_CHARGING, EYE_FIRING, EYE_COOLDOWN };

struct EyeBeamObelisk {
	float x, y;
	float beamY;        // Y height of the beam line
	bool beamHigh;      // true = high beam (jump over), false = low beam (duck/wait)
	EyeBeamState state;
	float timer;
	float dormantTime;  // How long dormant before charging
	float chargeTime;   // How long charging before firing
	float fireTime;     // How long beam fires
	float cooldownTime; // How long cooldown before dormant
	float beamLength;   // How far the beam extends
	int direction;      // 1 = fires right, -1 = fires left
};

struct EyeAlignData {
	int currentDir;    // 0-7 for 8 directions (N, NE, E, SE, S, SW, W, NW)
	int targetDir;
	float rotationAnim; // For smooth rotation
	float posAngle;     // Position angle around the circle (radians)
};

struct EyeAlignmentPuzzleData {
	EyeAlignData eyes[5];
	int selectedEye;
	bool isSolved;
	float successTimer;
};

struct Level3 {
	unsigned int bgImgs[2]; // Location 1: b1.1, b1.2
	float parallaxSpeeds[2];
	float cameraX;
	float levelWidth;
	float groundY;
	float gateY;
	unsigned int gateTexture;

	EyeBeamObelisk obelisks[10];
	int numObelisks;

	Bubble bubbles[5];
	int numBubbles;

	TreasureChest chests[3];
	int numChests;

	float checkpointX, checkpointY;
	bool checkpointActive;

	float gateX; // Gate X position

	// Sacred Sword Collectible
	float swordX, swordY;
	bool swordCollected;
	unsigned int swordTexture;
	float swordPickupTimer;
	float swordPlatforms[4][3]; // [index][x, y, width]

	// Location 2 Data
	unsigned int loc2BgImgs[2]; // b1.3, b1.4
	SpikeTrap loc2Spikes[4];
	int numLoc2Spikes;
	Bubble loc2Bubbles[3];
	int numLoc2Bubbles;
	TreasureChest loc2Chests[2];
	int numLoc2Chests;
	float loc2CheckpointX, loc2CheckpointY;
	bool loc2CheckpointActive;
	float finalDoorX;
	bool finalDoorOpen;
	bool wolvesTriggered;
	float howlTimer;
} level3;

EyeAlignmentPuzzleData eyeAlignmentPuzzle;

// --- Wolf Entity ---
struct Wolf {
	float x, y;
	float speed;
	bool active;
	bool facingRight;
	bool isAttacking;
	float attackTimer;
	float attackCooldown;
	float health;
	AnimSequence runRight, runLeft;
	AnimSequence attackRight, attackLeft;
};

Wolf wolves[3];
int numWolves = 3;

// --- Moon Seal Puzzle ---
struct MoonSealPuzzleData {
	int ringDirs[4];      // Current rotation (0-7)
	int targetDirs[4];    // Target rotation
	float ringAnims[4];   // Smooth anim angles
	int selectedRing;
	bool isSolved;
	float successTimer;
};

MoonSealPuzzleData moonSealPuzzle;

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
void LoadLevel2();
void DrawLevel2();
void UpdateLevel2(float dt);
void LoadLevel3();
void DrawLevel3();
void UpdateLevel3(float dt);
void InitStatueRotationPuzzle();
void DrawStatueRotationPuzzle();
void UpdateStatueRotationPuzzle(float dt);
void InitScalePuzzle();
void DrawScalePuzzle();
void UpdateScalePuzzle(float dt);
void InitEyeAlignmentPuzzle();
void DrawEyeAlignmentPuzzle();
void UpdateEyeAlignmentPuzzle(float dt);
void DrawInventory();
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
void InitMoonSealPuzzle();
void DrawMoonSealPuzzle();
void UpdateMoonSealPuzzle(float dt);
void PlayWolfHowlSound();

// Leaderboard Forward Declarations
void DrawLeaderboard();
void DrawNameInput();
void HandleNameInputKey(unsigned char key);
void HandleLeaderboardMouseMove(int mx, int my);
void HandleLeaderboardClick(int mx, int my);
void InitLeaderboardButtons();
void SaveLeaderboardEntry(const char* name, long score);
void LoadLeaderboard();
void SortLeaderboard();

#endif // GAME_VARIABLES_HPP
