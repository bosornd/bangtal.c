#include <bangtal.h>

SceneID scene;
ObjectID background;
ObjectID background2;

ObjectID player;
const int playerX_Min = 350, playerX_Max = 700, playerX_Init = 550;
int playerX = playerX_Init;
int playerDir = 1;
int playerY = 577;

bool hold = false;
ObjectID holdBlock;

TimerID timer, newItemTimer;
float animationTime = 0.005f;
float newItemTime = 1.f;

const int blockMax = 35;
int blockX[blockMax];
int blockNew = 0;

const int showBlockMax = 4;
ObjectID block[showBlockMax];
ObjectID block2[blockMax];

ObjectID gameover, game_over, game_clear;
ObjectID restart;

SoundID	bgm, blockSound, gameClearSound, gameOverSound;

void showBlock() {
	int max = showBlockMax - (hold ? 1 : 0);
	if (max > blockNew) max = blockNew;

	int start = blockNew - max;
	for (int i = 0; i < max; ++i) {
		locateObject(block[i], scene, blockX[i + start] - 100, i * 100);
		showObject(block[i]);
	}
	if (hold) hideObject(block[max]);

	locateObject(background, scene, 0, -100 * start);
}

int checkEnd() {
	if (blockNew == blockMax) {
		showMessage("탑 쌓기 성공~~~");
		return 1;
	}

	int center = blockX[blockNew - 1], blocks = 1;
	for (int i = blockNew - 2; i >= 0; --i) {
		const int x = blockX[i];
		if (center < x - 100 || center > x + 100) {
			for (int i = 0; i < showBlockMax; ++i)
				hideObject(block[i]);

			showObject(gameover);

			return -1;
		}
		center = (x + center * blocks);
		++ blocks;
		center /= blocks;
	}

	return 0;
}

void dropBlock() {
	hold = false;
	hideObject(holdBlock);

	blockX[blockNew] = playerX + 100;
	locateObject(block2[blockNew], scene, 1035 + blockX[blockNew] / 5, 20 * blockNew + 5);
	showObject(block2[blockNew]);

	blockNew++;

	int end = checkEnd();
	if (end) {
		stopSound(bgm);
		playSound(end == 1 ? gameClearSound : gameOverSound);

		showObject(end == 1 ? game_clear : game_over);
		showObject(restart);
	}
	else {
		showBlock();	
		playSound(blockSound);

		setTimer(newItemTimer, newItemTime);
		startTimer(newItemTimer);
	}
}

void initGame() {
	hideObject(gameover);
	hideObject(game_over);
	hideObject(game_clear);
	hideObject(restart);

	blockNew = 0;

	for (int i = 0; i < showBlockMax; ++i)
		hideObject(block[i]);

	for (int i = 0; i < blockMax; ++i)
		hideObject(block2[i]);

	setTimer(timer, animationTime);
	startTimer(timer);

	setTimer(newItemTimer, newItemTime);
	startTimer(newItemTimer);

	playSound(bgm);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == restart) {
		stopSound(bgm);

		initGame();
	}
}

void keyboardCallback(KeyCode code, KeyState state)
{
	if (code == KeyCode::KEY_SPACE && state == KeyState::KEY_PRESSED) {
		if (hold) dropBlock();
	}
}

void timerCallback(TimerID timer)
{
	if (timer == newItemTimer) {
		hold = true;
		showObject(holdBlock);

		showBlock();
	}
	else {	// default animation timer
		playerX += playerDir;
		if (playerX < playerX_Min || playerX > playerX_Max) {
			playerDir *= -1;
		}

		locateObject(player, scene, playerX, playerY);
		locateObject(holdBlock, scene, playerX, playerY - 90);

		setTimer(timer, animationTime);
		startTimer(timer);
	}
}

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) showObject(object);

	return object;
}

int main() {
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setTimerCallback(timerCallback);
	setMouseCallback(mouseCallback);
	setKeyboardCallback(keyboardCallback);

	scene = createScene("탑쌓기");
	background = createObject("Images/background.png", scene, 0, 0);
	background2 = createObject("Images/background2.png", scene, 0, 0);

	holdBlock = createObject("Images/apt.png", scene, playerX, playerY - 90);
	player = createObject("Images/player.png", scene, playerX, playerY);

	for (int i = 0; i < showBlockMax; ++i) {
		block[i] = createObject("Images/apt.png", scene, 0, 0, false);
	}

	for (int i = 0; i < blockMax; ++i) {
		block2[i] = createObject("Images/apt2.png", 0, 0, false);
	}

	gameover = createObject("Images/gameover.png", scene, 0, 0, false);
	game_over = createObject("Images/game_over.png", scene, 0, 0, false);
	game_clear = createObject("Images/game_clear.png", scene, 0, 0, false);
	restart = createObject("Images/restart.png", scene, 485, 242, false);

	timer = createTimer(animationTime);
	newItemTimer = createTimer(newItemTime);

	bgm = createSound("Sounds/BGM.mp3");
	gameClearSound = createSound("Sounds/GameClear.mp3");
	gameOverSound = createSound("Sounds/GameOver.mp3");
	blockSound = createSound("Sounds/Block.mp3");

	initGame();

	startGame(scene);

	return 0;
}
