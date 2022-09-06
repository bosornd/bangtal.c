#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

SceneID scene;
ObjectID restart;

int shooterX[4] = { 344, 344, 876, 876 };
int shooterY[4] = { 576, 98, 576, 98 };
ObjectID shooter[4];

enum Direction {
	DIRECTION_NONE,
	DIRECTION_LEFT,
	DIRECTION_LEFT_UP,
	DIRECTION_LEFT_DOWN,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_RIGHT_UP,
	DIRECTION_RIGHT_DOWN,
	DIRECTION_DOWN
};

ObjectID player;
int playerX = 600, playerY = 300;
Direction playerDirection = DIRECTION_NONE;
int playerSpeed = 1;
int playerSize = 50;

const int MAX_BULLET = 20;
ObjectID bullet[MAX_BULLET];
bool bulletShoot[MAX_BULLET];
int bulletX[MAX_BULLET], bulletY[MAX_BULLET];
Direction bulletDirection[MAX_BULLET];
int bulletSpeed = 5;
int bulletSize = 10;

const int MAX_ENERMY = 100;
ObjectID enemy[MAX_ENERMY];
bool enemyLive[MAX_ENERMY];
int enemyX[MAX_ENERMY], enemyY[MAX_ENERMY];
Direction enemyDirection[MAX_ENERMY];
int enemySpeed = 3;
int enemySize = 50;

int enemyCreationTime = 100;
int enemyTimer = enemyCreationTime;

ObjectID emotion;
const char* emotion_images[4] = {
	"Images/emotion.png",
	"Images/emotion_kill.png",
	"Images/emotion_hit.png",
	"Images/emotion_die.png",
};

int score = 0;
ObjectID score_display;

const int MAX_HEART = 3;
ObjectID heart[MAX_HEART];
int heartNumber = MAX_HEART;

TimerID emotionTimer;
float emotionTime = 1.f;

TimerID timer;
float animationTime = 0.01f;

SoundID	bgm, shootSound, hitSound, killSound, gameOverSound;

void changeEmotion(int e = 0) {
	setObjectImage(emotion, emotion_images[e]);

	if (e > 0) {
		setTimer(emotionTimer, emotionTime);
		startTimer(emotionTimer);
	}
}

void showScore(int score) {
	char buf[4];
	sprintf(buf, "%d", score);

	setObjectText(score_display, buf);
}

void initGame() {
	playerX = 600, playerY = 300;
	locateObject(player, scene, playerX, playerY);
	playerDirection = DIRECTION_NONE;

	heartNumber = MAX_HEART;
	for (int i = 0; i < heartNumber; ++i) {
		showObject(heart[i]);
	}

	score = 0;
	showScore(score);

	for (int i = 0; i < MAX_BULLET; ++i) {
		bulletShoot[i] = false;
		hideObject(bullet[i]);
	}

	for (int i = 0; i < MAX_ENERMY; ++i) {
		enemyLive[i] = false;
		hideObject(enemy[i]);
	}

	enemyTimer = enemyCreationTime;

	setTimer(timer, animationTime);
	startTimer(timer);

	playSound(bgm);
}

void reflect(Direction& dir, bool x) {
	switch (dir) {
		case DIRECTION_LEFT:		dir = DIRECTION_RIGHT; break;
		case DIRECTION_LEFT_UP:		dir = x ? DIRECTION_RIGHT_UP : DIRECTION_LEFT_DOWN; break;
		case DIRECTION_LEFT_DOWN:	dir = x ? DIRECTION_RIGHT_DOWN : DIRECTION_LEFT_UP; break;
		case DIRECTION_UP:			dir = DIRECTION_DOWN; break;
		case DIRECTION_RIGHT:		dir = DIRECTION_LEFT; break;
		case DIRECTION_RIGHT_UP:	dir = x ? DIRECTION_LEFT_UP : DIRECTION_RIGHT_DOWN; break;
		case DIRECTION_RIGHT_DOWN:	dir = x ? DIRECTION_LEFT_DOWN : DIRECTION_RIGHT_UP; break;
		case DIRECTION_DOWN:		dir = DIRECTION_UP; break;
	}
}

bool move(int& x, int& y, Direction& dir, int speed, int size = 0) {
	switch (dir) {
	case DIRECTION_LEFT: x -= speed; break;
	case DIRECTION_LEFT_UP: x -= speed; y += speed; break;
	case DIRECTION_LEFT_DOWN: x -= speed; y -= speed; break;
	case DIRECTION_UP: y += speed; break;
	case DIRECTION_RIGHT: x += speed; break;
	case DIRECTION_RIGHT_UP: x += speed; y += speed; break;
	case DIRECTION_RIGHT_DOWN: x += speed; y -= speed; break;
	case DIRECTION_DOWN: y -= speed; break;
	}

	bool collision = false;

	if (x < 235) {
		if (y > 280 && y < 460 - size) {
			x = 1050 - size;
		}
		else {
			x = 235; reflect(dir, true);
		}
		collision = true;
	}
	else if (x > 1050 - size) {
		if (y > 280 && y < 460 - size) {
			x = 235;
		}
		else {
			x = 1050 - size; reflect(dir, true);
		}
		collision = true;
	}
	else if (y < 25) {
		if (x > 560 && x < 690 - size) {
			y = 690 - size;
		}
		else {
			y = 25; reflect(dir, false);
		}
		collision = true;
	}
	else if (y > 690 - size) {
		if (x > 560 && x < 690 - size) {
			y = 25;
		}
		else {
			y = 690 - size; reflect(dir, false);
		}
		collision = true;
	}

	return collision;
}

void shootBullet() {
	if (playerDirection == DIRECTION_NONE) return;

	int i = 0;
	for (; i < MAX_BULLET; ++i)
		if (bulletShoot[i] == false) break;

	if (i < MAX_BULLET) {
		bulletDirection[i] = playerDirection;

		switch (bulletDirection[i]) {
			case DIRECTION_LEFT:
				bulletX[i] = playerX - 5; bulletY[i] = playerY + 22;
				setObjectImage(bullet[i], "Images/bulletLeft.png");
				break;
			case DIRECTION_RIGHT:
				bulletX[i] = playerX + 45; bulletY[i] = playerY + 22;
				setObjectImage(bullet[i], "Images/bulletRight.png");
				break;
			case DIRECTION_UP:
				bulletX[i] = playerX + 20; bulletY[i] = playerY + 45;
				setObjectImage(bullet[i], "Images/bulletUp.png");
				break;
			case DIRECTION_DOWN:
				bulletX[i] = playerX + 20; bulletY[i] = playerY - 5;
				setObjectImage(bullet[i], "Images/bulletDown.png");
				break;
		}
		bulletShoot[i] = true;

		locateObject(bullet[i], scene, bulletX[i], bulletY[i]);
		showObject(bullet[i]);

		playSound(shootSound);
	}
}

bool checkBullet(int b, int e) {
	return (bulletX[b] - enemyX[e] - 20) * (bulletX[b] - enemyX[e] - 20) + (bulletY[b] - enemyY[e] - 20) * (bulletY[b] - enemyY[e] - 20) < 900;
}

void createEnemy() {
	static const int x[4] = { 344, 344, 876, 876 }, y[4] = { 576, 98, 576, 98 };
	static const Direction dir[4] = { DIRECTION_RIGHT_UP, DIRECTION_RIGHT_DOWN, DIRECTION_LEFT_UP, DIRECTION_LEFT_DOWN };
	static const char* images[3] = { "Images/enemy0.png", "Images/enemy1.png", "Images/enemy2.png" };

	int i = 0, j = rand() % 4;
	for (; i < MAX_ENERMY; ++i)
		if (enemyLive[i] == false) break;

	if (i < MAX_ENERMY) {
		enemyLive[i] = true;
		enemyX[i] = x[j];
		enemyY[i] = y[j];
		enemyDirection[i] = dir[j];

		setObjectImage(enemy[i], images[rand() % 3]);
		locateObject(enemy[i], scene, enemyX[i], enemyY[i]);
		showObject(enemy[i]);
	}
}

bool checkEnemy(int e) {
	return (playerX - enemyX[e]) * (playerX - enemyX[e]) + (playerY - enemyY[e]) * (playerY - enemyY[e]) < 2500;
}

void keyboardCallback(KeyCode code, KeyState state) {
	if (state == KeyState::KEY_PRESSED) {
		switch (code) {
			case KeyCode::KEY_LEFT_ARROW:	playerDirection = DIRECTION_LEFT; break;
			case KeyCode::KEY_RIGHT_ARROW:	playerDirection = DIRECTION_RIGHT; break;
			case KeyCode::KEY_UP_ARROW:		playerDirection = DIRECTION_UP; break;
			case KeyCode::KEY_DOWN_ARROW:	playerDirection = DIRECTION_DOWN; break;
			case KeyCode::KEY_SPACE:		shootBullet(); break;
		}
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == restart) {
		stopSound(bgm);

		hideObject(restart);
		initGame();
	}
}

void timerCallback(TimerID timer) {
	if (timer == emotionTimer) {
		changeEmotion();
		return;
	}

	// player movement
	move(playerX, playerY, playerDirection, playerSpeed, playerSize);
	locateObject(player, scene, playerX, playerY);

	// enemy movement
	for (int i = 0; i < MAX_ENERMY; ++i) {
		if (enemyLive[i]) {
			move(enemyX[i], enemyY[i], enemyDirection[i], enemySpeed, enemySize);
			locateObject(enemy[i], scene, enemyX[i], enemyY[i]);
		}
	}

	// bullet movement
	for (int i = 0; i < MAX_BULLET; ++i) {
		if (bulletShoot[i]) {
			bool collision = move(bulletX[i], bulletY[i], bulletDirection[i], bulletSpeed, bulletSize);

			for (int e = 0; e < MAX_ENERMY; ++e) {
				if (enemyLive[e] && checkBullet(i, e)) {
					showScore(++score);

					enemyLive[e] = false;
					hideObject(enemy[e]);

					changeEmotion(1);

					collision = true;

					playSound(killSound);
					break;
				}
			}

			if (collision) {
				bulletShoot[i] = false;
				hideObject(bullet[i]);
			}
			else {
				locateObject(bullet[i], scene, bulletX[i], bulletY[i]);
			}
		}
	}

	bool end = false;
	for (int i = 0; i < MAX_ENERMY; ++i) {
		if (enemyLive[i] && checkEnemy(i)) {
			changeEmotion(2);

			--heartNumber;
			hideObject(heart[heartNumber]);

			if (heartNumber == 0)
				end = true;
			else {
				enemyLive[i] = false;
				hideObject(enemy[i]);
			}

			playSound(hitSound);
		}
	}

	if (end) {
		changeEmotion(3);
		stopSound(bgm);

		showObject(restart);
		playSound(gameOverSound);
	}
	else {
		--enemyTimer;
		if (enemyTimer < 0) {
			createEnemy();
			enemyTimer = enemyCreationTime;
		}

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
	srand((unsigned int)time(NULL));

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setKeyboardCallback(keyboardCallback);
	setTimerCallback(timerCallback);
	setMouseCallback(mouseCallback);

	scene = createScene("슈팅", "Images/background.png");
	emotion = createObject(emotion_images[emotion], scene, 23, 502);
	emotionTimer = createTimer(emotionTime);

	for (int i = 0; i < MAX_HEART; ++i) {
		heart[i] = createObject("Images/heart.png", scene, 1112, 633 - 84 * i);
	}

	score_display = createObject("Images/score.png", scene, 143, 405);

	for (int i = 0; i < MAX_BULLET; ++i) {
		bulletShoot[i] = false;
		bullet[i] = createObject("Images/bulletLeft.png", scene, 0, 0, bulletShoot[i]);
	}

	player = createObject("Images/player.png", scene, playerX, playerY);

	for (int i = 0; i < 4; ++i)
		shooter[i] = createObject("Images/shooter.png", scene, shooterX[i], shooterY[i]);

	for (int i = 0; i < MAX_ENERMY; ++i) {
		enemyLive[i] = false;
		enemy[i] = createObject("Images/enemy0.png", scene, 0, 0, enemyLive[i]);
	}

	restart = createObject("Images/restart.png", scene, 0, 0, false);

	bgm = createSound("Sounds/BGM.mp3");
	shootSound = createSound("Sounds/Shoot.mp3");
	killSound = createSound("Sounds/Kill.mp3");
	hitSound = createSound("Sounds/Hit.mp3");
	gameOverSound = createSound("Sounds/GameOver.mp3");

	timer = createTimer(animationTime);

	initGame();

	startGame(scene);

	return 0;
}
