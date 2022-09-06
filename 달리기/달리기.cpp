#include <bangtal.h>

#include <time.h>
#include <stdlib.h>

SceneID scene;
ObjectID restart;

ObjectID background;
int backgroundX = 0;
const int backgroundMaxX = 6240;

TimerID timer;
float animationTime = 0.01f;
int animationSpeed = 5;

ObjectID player;
const int playerX = 200, playerY = 100;
int playerAnimator = 0;
const int playerAnimatorMax = 5;
bool stand = true;

int jumpAnimator = 0;
const int jumpAnimatorMax = 40;
const int jumpY = 100;
bool jump2 = false;

const char* playerImage[playerAnimatorMax * 2] = {
	"Images/char1.png",
	"Images/char2.png",
	"Images/char3.png",
	"Images/char4.png",
	"Images/char5.png",
	"Images/d_char1.png",
	"Images/d_char2.png",
	"Images/d_char3.png",
	"Images/d_char4.png",
	"Images/d_char5.png",
};

const int obstacleMax = 10;
ObjectID obstacle[obstacleMax];

int obstacleX[obstacleMax];
const int obstacleY = playerY;
bool obstacleFly[obstacleMax];

const int obstacleImageMax = 4;
const char* obstacleImage[obstacleImageMax] = {
	"Images/O1.png",
	"Images/O2.png",
	"Images/O3.png",
	"Images/O4.png",
};

TimerID score;
SoundID bgm, jumpSound, crawlSound, loseSound;

int ABS(int x, int y) {
	return (x > y) ? (x - y) : (y - x);
}

int newObstacleX() {
	return 1500 + 300 * (rand() % 10);
}

int newObstacleImage() {
	return rand() % obstacleImageMax;
}

void initGame() {
	backgroundX = 0;

	for (int i = 0; i < obstacleMax; ++i) {
		obstacleX[i] = newObstacleX();
		int type = newObstacleImage();
		obstacleFly[i] = (type == 3);
		locateObject(obstacle[i], scene, obstacleX[i], obstacleY + (obstacleFly[i] ? 50 : 0));
		setObjectImage(obstacle[i], obstacleImage[type]);
	}

	setTimer(timer, animationTime);
	startTimer(timer);

	setTimer(score, 0);

	playSound(bgm, true);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == restart) {
		hideObject(restart);
		initGame();
	}
}

void keyboardCallback(KeyCode code, KeyState state) {
	if (code == KeyCode::KEY_SPACE) {
		if (state == KeyState::KEY_PRESSED) {
			if (jumpAnimator > 0) jump2 = true;
			else jump2 = false;

			jumpAnimator = 1;

			playSound(jumpSound);
		}
	}
	else if (code == KeyCode::KEY_C) {
		if (state == KeyState::KEY_PRESSED)
			playSound(crawlSound);

		stand = (state == KeyState::KEY_RELEASED);
	}
}

void timerCallback(TimerID timer) {
	// scroll background
	backgroundX -= animationSpeed;
	if (backgroundX < -backgroundMaxX) backgroundX += backgroundMaxX;
	locateObject(background, scene, backgroundX, 0);

	// scroll obstacle
	for (int i = 0; i < obstacleMax; ++i) {
		obstacleX[i] -= animationSpeed;

		if (obstacleX[i] < -100) { // 사라짐
			increaseTimer(score, 1);
			int type = newObstacleImage();
			obstacleFly[i] = (type == 3);
			setObjectImage(obstacle[i], obstacleImage[type]);

			obstacleX[i] = newObstacleX();
		}
		locateObject(obstacle[i], scene, obstacleX[i], obstacleY + (obstacleFly[i] ? 50 : 0));
	}

	// player animation
	if (jumpAnimator > 0) {
		jumpAnimator = (jumpAnimator + 1) % jumpAnimatorMax;
		playerAnimator = 0;
	}
	else playerAnimator = (playerAnimator + 1) % playerAnimatorMax;

	setObjectImage(player, stand ? playerImage[playerAnimator] : playerImage[playerAnimatorMax + playerAnimator]);
	locateObject(player, scene, playerX, playerY + (jumpAnimator > 0 ? (jump2 ? 2 * jumpY : jumpY) : 0));

	// check collision
	for (int i = 0; i < obstacleMax; ++i) {
		if ((obstacleFly[i] && ((stand && jumpAnimator == 0) || (jumpAnimator > 0 && !jump2)) && playerX + 65 > obstacleX[i] && playerX < obstacleX[i] + 95) ||
			(!obstacleFly[i] && jumpAnimator == 0 && playerX + 65 > obstacleX[i] && playerX < obstacleX[i] + 45)) {
			showObject(restart);

			stopSound(bgm);
			playSound(loseSound);

			return;
		}
	}

	setTimer(timer, animationTime);
	startTimer(timer);
}

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) showObject(object);

	return object;
}

int main() {
	srand((unsigned int)time(NULL));

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setTimerCallback(timerCallback);
	setMouseCallback(mouseCallback);
	setKeyboardCallback(keyboardCallback);

	scene = createScene("달리기");
	background = createObject("Images/background.png", scene, 0, 0);

	for (int i = 0; i < obstacleMax; ++i) {
		obstacleX[i] = -100;
		obstacle[i] = createObject(obstacleImage[0], scene, obstacleX[i], obstacleY);
	}

	player = createObject(playerImage[0], scene, playerX, playerY);

	restart = createObject("Images/restart.png", scene, 0, 0, false);

	timer = createTimer(animationTime);

	score = createTimer(0);
	showTimer(score);

	bgm = createSound("Sounds/bgm.mp3");
	jumpSound = createSound("Sounds/jump.mp3");
	crawlSound = createSound("Sounds/crawl.mp3");
	loseSound = createSound("Sounds/lose.mp3");

	initGame();
	startGame(scene);

	return 0;
}