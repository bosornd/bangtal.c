#include <bangtal.h>

SceneID scene;

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

const int blockMax = 100;
int blockX[blockMax];
int blockNew = 0;

const int showBlockMax = 4;
ObjectID block[showBlockMax];

ObjectID gameover;
ObjectID restart;

void showBlock() {
	int max = showBlockMax - (hold ? 1 : 0);
	if (max > blockNew) max = blockNew;

	int start = blockNew - max;
	for (int i = 0; i < max; ++i) {
		locateObject(block[i], scene, blockX[i + start] - 100, i * 100);
		showObject(block[i]);
	}
	if (hold) hideObject(block[max]);
}

bool checkEnd() {
	int center = blockX[blockNew - 1], blocks = 1;
	for (int i = blockNew - 2; i >= 0; --i) {
		const int x = blockX[i];
		if (center < x - 100 || center > x + 100) return true;
		center = (x + center * blocks);
		++ blocks;
		center /= blocks;
	}

	return false;
}

void dropBlock() {
	hold = false;
	hideObject(holdBlock);

	blockX[blockNew] = playerX + 100;
	blockNew++;

	if (checkEnd()) {
		for (int i = 0; i < showBlockMax; ++i)
			hideObject(block[i]);

		showObject(gameover);
		showObject(restart);
	}
	else {
		showBlock();

		setTimer(newItemTimer, newItemTime);
		startTimer(newItemTimer);
	}
}

void initGame() {
	hideObject(gameover);
	hideObject(restart);

	blockNew = 0;

	setTimer(timer, animationTime);
	startTimer(timer);

	setTimer(newItemTimer, newItemTime);
	startTimer(newItemTimer);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == restart) {
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

	scene = createScene("탑쌓기", "Images/background.png");

	holdBlock = createObject("Images/apt.png", scene, playerX, playerY - 90);
	player = createObject("Images/player.png", scene, playerX, playerY);

	for (int i = 0; i < showBlockMax; ++i) {
		block[i] = createObject("Images/apt.png");
	}

	gameover = createObject("Images/gameover.png", scene, 0, 0, false);
	restart = createObject("Images/restart.png", scene, 0, 0, false);

	timer = createTimer(animationTime);
	newItemTimer = createTimer(newItemTime);

	initGame();

	startGame(scene);

	return 0;
}
