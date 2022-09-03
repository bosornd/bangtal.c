#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

SceneID scene;
ObjectID restart;

enum Direction {
	LEFT  = 0,
	RIGHT = 1,
	UP    = 2,
	DOWN  = 3
};

ObjectID player;
int playerX = 4, playerY = 4;

const char* playerImages[4][4] = {
	{ "Images/char_L0.png", "Images/char_L1.png", "Images/char_L2.png", "Images/char_L3.png" },
	{ "Images/char_R0.png", "Images/char_R1.png", "Images/char_R2.png", "Images/char_R3.png" },
	{ "Images/char_U0.png", "Images/char_U1.png", "Images/char_U2.png", "Images/char_U3.png" },
	{ "Images/char_D0.png", "Images/char_D1.png", "Images/char_D2.png", "Images/char_D3.png" },
};

Direction playerDirection = LEFT;
bool playerMoving = false;
int playerMovingStep = 0;

enum Emotion {
	NORMAL,
	KILL,
	HIT,
	CLEAR,
	END
};
const char* emotionImages[5] = {
	"Images/emotion.png",
	"Images/emotion_kill.png",
	"Images/emotion_hit.png",
	"Images/emotion_clear.png",
	"Images/emotion_end.png",
};

ObjectID emotion;
int emotionTime = 0;

const int enemyMax = 20;
ObjectID enemy[enemyMax];
bool enemyLive[enemyMax];

int enemyX[enemyMax], enemyY[enemyMax];
Direction enemyDirection[enemyMax];
bool enemyMoving[enemyMax];
int enemyMovingStep[enemyMax];

int numberOfHeart = 3;
ObjectID heart[3];

int enemyKilled = 0;
ObjectID enemyKilledLabel;

int enemyTarget = 0;
ObjectID enemyTargetLabel;

const int ballonMax = 10;
int numberOfBallon = ballonMax;
ObjectID ballonLabel;

ObjectID ballon[ballonMax], ballonEffect[ballonMax][4];
bool ballonInstalled[ballonMax];
int ballonExplosionTime[ballonMax];
int ballonX[ballonMax], ballonY[ballonMax];

const int bombMax = 5;
int numberOfBomb = bombMax;
ObjectID bombLabel;
ObjectID bomb[bombMax], bombEffect[bombMax];
bool bombInstalled[bombMax];
int bombExplosionTime[bombMax];
int bombX[bombMax], bombY[bombMax];

int stage = 0;
ObjectID stageLabel;

int enemyCreationTimeMax = 50;
int enemyCreationTime = enemyCreationTimeMax;

TimerID timer;
float animationTime = 0.05f;

int getPlayerX() {
	int x = 308 + playerX * 68;

	if (playerMoving) {
		switch (playerDirection) {
			case LEFT: x -= 17 * playerMovingStep; break;
			case RIGHT: x += 17 * playerMovingStep; break;
		}
	}

	return x;
}

int getPlayerY() {
	int y = 645 - playerY * 69;

	if (playerMoving) {
		switch (playerDirection) {
			case UP: y += 17 * playerMovingStep; break;
			case DOWN: y -= 17 * playerMovingStep; break;
		}
	}

	return y;
}

void drawPlayer() {
	if (playerMoving) {
		setObjectImage(player, playerImages[playerDirection][playerMovingStep]);
	}
	else {
		setObjectImage(player, "Images/char.png");
	}

	locateObject(player, scene, getPlayerX(), getPlayerY());
}

void movePlayer() {
	if (playerMovingStep < 4) {
		drawPlayer();
		++ playerMovingStep;
	}
	else {
		switch (playerDirection) {
			case LEFT: --playerX; break;
			case RIGHT: ++playerX; break;
			case UP: --playerY; break;
			case DOWN: ++playerY; break;
		}
		playerMoving = false;

		drawPlayer();
	}
}

void changeEmotion(Emotion e = NORMAL) {
	setObjectImage(emotion, emotionImages[e]);
	emotionTime = e > 0 ? 100 : 0;
}

int getEnemyX(int e) {
	int x = 303 + enemyX[e] * 68;

	if (enemyMoving[e]) {
		switch (enemyDirection[e]) {
			case LEFT: x -= 17 * enemyMovingStep[e]; break;
			case RIGHT: x += 17 * enemyMovingStep[e]; break;
		}
	}

	return x;
}

int getEnemyY(int e) {
	int y = 640 - enemyY[e] * 69;

	if (enemyMoving[e]) {
		switch (enemyDirection[e]) {
			case UP: y += 17 * enemyMovingStep[e]; break;
			case DOWN: y -= 17 * enemyMovingStep[e]; break;
		}
	}

	return y;
}

void drawEnemy(int e) {
	locateObject(enemy[e], scene, getEnemyX(e), getEnemyY(e));
}

void createEnemy() {
	int e = 0;
	for (; e < enemyMax; ++e)
		if (enemyLive[e] == false) break;

	if (e < enemyMax) {
		int x, y;

		do {
			x = rand() % 10;
			y = rand() % 10;
		} while (x > playerX - 3 && x < playerX + 3 && y > playerY - 3 && y < playerY + 3);

		enemyX[e] = x;
		enemyY[e] = y;
		enemyMoving[e] = false;
		enemyLive[e] = true;

		showObject(enemy[e]);
		drawEnemy(e);
	}
}

void moveEnemy(int e) {
	if (enemyMovingStep[e] < 4) {
		drawEnemy(e);
		++enemyMovingStep[e];
	}
	else {
		switch (enemyDirection[e]) {
			case LEFT: --enemyX[e]; break;
			case RIGHT: ++enemyX[e]; break;
			case UP: --enemyY[e]; break;
			case DOWN: ++enemyY[e]; break;
		}
		enemyMoving[e] = false;

		drawEnemy(e);
	}
}

void setLabel(ObjectID label, int number) {
	char buf[4];
	sprintf(buf, "%d", number);

	setObjectText(label, buf);
}

int getBombX(int b) {
	return 303 + bombX[b] * 68;
}

int getBombY(int b) {
	return 640 - bombY[b] * 69;
}

void dropBomb(int x, int y) {
	int b = 0;
	for (; b < bombMax; ++b) {
		if (bombInstalled[b] == false) break;
	}

	if (b < bombMax) {
		bombInstalled[b] = true;
		bombX[b] = x;
		bombY[b] = y;

		showObject(bomb[b]);
		locateObject(bomb[b], scene, getBombX(b), getBombY(b));

		bombExplosionTime[b] = 50;

		--numberOfBomb;
		setLabel(bombLabel, numberOfBomb);
	}
}

int getBallonX(int b) {
	return 303 + ballonX[b] * 68;
}

int getBallonY(int b) {
	return 640 - ballonY[b] * 69;
}

void dropBallon(int x, int y) {
	int b = 0;
	for (; b < ballonMax; ++b) {
		if (ballonInstalled[b] == false) break;
	}

	if (b < ballonMax) {
		ballonInstalled[b] = true;
		ballonX[b] = x;
		ballonY[b] = y;

		showObject(ballon[b]);
		locateObject(ballon[b], scene, getBallonX(b), getBallonY(b));

		ballonExplosionTime[b] = 50;

		--numberOfBallon;
		setLabel(ballonLabel, numberOfBallon);
	}
}

int waterKill(int x, int y) {
	int killed = 0;
	for (int e = 0; e < enemyMax; ++e) {
		if (enemyLive[e]) {
			int ex = getEnemyX(e), ey = getEnemyY(e);
			if ((ex - x) * (ex - x) + (ey - y) * (ey - y) < 2500) {
				enemyLive[e] = false;
				hideObject(enemy[e]);

				++killed;
			}
		}
	}

	return killed;
}

void initGame() {
	hideObject(restart);

	playerX = 4, playerY = 4;
	playerMoving = false;
	drawPlayer();

	changeEmotion(NORMAL);

	for (int e = 0; e < enemyMax; ++e) {
		hideObject(enemy[e]);
		enemyLive[e] = false;
	}

	enemyCreationTime = enemyCreationTimeMax;

	for (int b = 0; b < bombMax; ++b) {
		hideObject(bomb[b]);
		bombInstalled[b] = false;
	}

	for (int b = 0; b < ballonMax; ++b) {
		hideObject(ballon[b]);
		ballonInstalled[b] = false;
	}

	numberOfHeart = 3;
	for (int h = 0; h < numberOfHeart; ++h) {
		showObject(heart[h]);
	}

	enemyKilled = 0;
	setLabel(enemyKilledLabel, enemyKilled);

	enemyTarget = 20;
	setLabel(enemyTargetLabel, enemyTarget);

	numberOfBallon = ballonMax;
	setLabel(ballonLabel, numberOfBallon);

	numberOfBomb = bombMax;
	setLabel(bombLabel, numberOfBomb);

	stage = 1;
	setLabel(stageLabel, stage);

	setTimer(timer, animationTime);
	startTimer(timer);
}

void keyboardCallback(KeyCode code, KeyState state) {
	if (code == KeyCode::KEY_LEFT_ARROW || code == KeyCode::KEY_RIGHT_ARROW || code == KeyCode::KEY_UP_ARROW || code == KeyCode::KEY_DOWN_ARROW) {
		if (state == KeyState::KEY_PRESSED && playerMoving == false) {
			switch (code) {
				case KeyCode::KEY_LEFT_ARROW: if (playerX > 0) { playerDirection = LEFT; playerMoving = true; } break;
				case KeyCode::KEY_RIGHT_ARROW: if (playerX < 9){ playerDirection = RIGHT; playerMoving = true; } break;
				case KeyCode::KEY_UP_ARROW: if (playerY > 0){ playerDirection = UP; playerMoving = true; } break;
				case KeyCode::KEY_DOWN_ARROW: if (playerY < 9){ playerDirection = DOWN; playerMoving = true; } break;
			}
			playerMovingStep = 0;
		}
	}
	else if (code == KeyCode::KEY_B) {
		if (state == KeyState::KEY_PRESSED) {
			if(numberOfBomb > 0) dropBomb(playerX, playerY);
		}
	}
	else if (code == KeyCode::KEY_SPACE ) {
		if (state == KeyState::KEY_PRESSED) {
			if (numberOfBallon > 0) dropBallon(playerX, playerY);
		}
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == restart) {
		hideObject(restart);
		initGame();
	}
}

void timerCallback(TimerID timer) {
	for (int b = 0; b < bombMax; ++b) {
		if (bombInstalled[b]) {
			--bombExplosionTime[b];

			if (bombExplosionTime[b] == 15) {
				locateObject(bombEffect[b], scene, getBombX(b) - 373, getBombY(b) - 306);
				setObjectImage(bombEffect[b], "Images/bomb_effect1.png");
				showObject(bombEffect[b]);
				hideObject(bomb[b]);
			}
			else if (bombExplosionTime[b] == 10) {
				setObjectImage(bombEffect[b], "Images/bomb_effect2.png");
			}
			else if (bombExplosionTime[b] == 5) {
				setObjectImage(bombEffect[b], "Images/bomb_effect3.png");
			}
			else if (bombExplosionTime[b] == 2) {
				setObjectImage(bombEffect[b], "Images/bomb_effect4.png");
			}
			else if (bombExplosionTime[b] == 0) {
				hideObject(bombEffect[b]);
				bombInstalled[b] = false;

				int killed = 0;
				for (int e = 0; e < enemyMax; ++e) {
					if (enemyLive[e] && enemyX[e] > bombX[b] - 5 && enemyX[e] < bombX[b] + 5 && enemyY[e] > bombY[b] - 5 && enemyY[e] < bombY[b] + 5) {
						enemyLive[e] = false;
						hideObject(enemy[e]);

						++killed;
					}
				}
				if (killed > 0) {
					enemyKilled += killed;
					setLabel(enemyKilledLabel, enemyKilled);

					changeEmotion(KILL);
				}
			}
		}
	}

	for (int b = 0; b < ballonMax; ++b) {
		if (ballonInstalled[b]) {
			--ballonExplosionTime[b];

			if (ballonExplosionTime[b] == 0) {
				for (int e = 0; e < 4; ++e) {
					hideObject(ballonEffect[b][e]);
				}
				ballonInstalled[b] = false;
			}
			else if (ballonExplosionTime[b] < 20) {
				if (ballonExplosionTime[b] == 19) {
					hideObject(ballon[b]);

					for (int e = 0; e < 4; ++e) {
						showObject(ballonEffect[b][e]);
					}
				}

				int killed = 0;
				int step = 10 - ballonExplosionTime[b] / 2;
				if (ballonX[b] - step < 0) hideObject(ballonEffect[b][0]);
				else {
					locateObject(ballonEffect[b][0], scene, 300 + (ballonX[b] - step) * 68, 635 - ballonY[b] * 69);
					killed += waterKill(300 + (ballonX[b] - step) * 68, 635 - ballonY[b] * 69);
				}
				if (ballonX[b] + step > 9) hideObject(ballonEffect[b][1]);
				else {
					locateObject(ballonEffect[b][1], scene, 300 + (ballonX[b] + step) * 68, 635 - ballonY[b] * 69);
					killed += waterKill(300 + (ballonX[b] + step) * 68, 635 - ballonY[b] * 69);
				}
				if (ballonY[b] + step > 9) hideObject(ballonEffect[b][2]);
				else {
					locateObject(ballonEffect[b][2], scene, 300 + ballonX[b] * 68, 635 - (ballonY[b] + step) * 69);
					killed += waterKill(300 + ballonX[b] * 68, 635 - (ballonY[b] + step) * 69);
				}
				if (ballonY[b] - step < 0) hideObject(ballonEffect[b][3]);
				else {
					locateObject(ballonEffect[b][3], scene, 300 + ballonX[b] * 68, 635 - (ballonY[b] - step) * 69);
					killed += waterKill(300 + ballonX[b] * 68, 635 - (ballonY[b] - step) * 69);
				}
				if (killed > 0) {
					enemyKilled += killed;
					setLabel(enemyKilledLabel, enemyKilled);

					changeEmotion(KILL);
				}
			}
		}
	}

	if (playerMoving) movePlayer();

	if (emotionTime > 0) {
		--emotionTime;
		if (emotionTime == 0) {
			changeEmotion(NORMAL);
		}
	}

	for (int e = 0; e < enemyMax; ++e) {
		if (enemyLive[e]) {
			if (enemyMoving[e]) moveEnemy(e);
			else {
				if (rand() % 5 == 0) {
					Direction d;

					while(true) {
						d = (Direction)(rand() % 4);

						if (d == LEFT) {
							if (enemyX[e] > 0) break;
						}
						else if (d == RIGHT) {
							if (enemyX[e] < 9) break;
						}
						else if (d == UP) {
							if (enemyY[e] > 0) break;
						}
						else if (d == DOWN) {
							if (enemyY[e] < 9) break;
						}
					}

					enemyDirection[e] = d;
					enemyMovingStep[e] = 0;
					enemyMoving[e] = true;
				}
			}
		}
	}

	--enemyCreationTime;
	if (enemyCreationTime == 0) {
		createEnemy();

		enemyCreationTime = enemyCreationTimeMax / 2;
		enemyCreationTime += rand() % enemyCreationTime;
	}

	bool end = false;

	for (int e = 0; e < enemyMax; ++e) {
		if (enemyLive[e]) {
			int ex = getEnemyX(e), ey = getEnemyY(e);
			int px = getPlayerX(), py = getPlayerY();
			int dx = ex - px + 6, dy = ey - py + 5;
			if (dx * dx + dy * dy < 1600) {
				--numberOfHeart;
				hideObject(heart[numberOfHeart]);

				if (numberOfHeart > 0) {
					enemyLive[e] = false;
					hideObject(enemy[e]);
					changeEmotion(HIT);
				}
			}
		}
	}

	if (numberOfHeart > 0) {
		setTimer(timer, animationTime);
		startTimer(timer);
	}
	else {
		changeEmotion(END);
		showObject(restart);
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

	scene = createScene("크레이지 아케이드", "Images/background.png");

	for (int e = 0; e < enemyMax; ++e)
		enemy[e] = createObject("Images/enemy.png", scene, 303, 640, false);

	for (int b = 0; b < bombMax; ++b)
		bomb[b] = createObject("Images/bomb.png", scene, 303, 640, false);

	for (int b = 0; b < ballonMax; ++b)
		ballon[b] = createObject("Images/ballon.png", scene, 303, 640, false);

	player = createObject("Images/char.png", scene, scene, 308 + playerX * 68, 645 - playerY * 69);
	emotion = createObject("Images/emotion.png", scene, 1018, 448);

	for (int b = 0; b < bombMax; ++b)
		bombEffect[b] = createObject("Images/bomb_effect1.png", scene, 0, 0, false);

	for (int b = 0; b < ballonMax; ++b) {
		ballonEffect[b][0] = createObject("Images/water_left.png", scene, 0, 0, false);
		ballonEffect[b][1] = createObject("Images/water_right.png", scene, 0, 0, false);
		ballonEffect[b][2] = createObject("Images/water_up.png", scene, 0, 0, false);
		ballonEffect[b][3] = createObject("Images/water_down.png", scene, 0, 0, false);
	}

	for (int h = 0; h < 3; ++h)
		heart[h] = createObject("Images/heart.png", scene, 1031 + 80 * h, 343);

	enemyKilledLabel = createObject("Images/label.png", scene, 160, 508);
	enemyTargetLabel = createObject("Images/label.png", scene, 210, 508);
	ballonLabel = createObject("Images/label.png", scene, 180, 210);
	bombLabel = createObject("Images/label.png", scene, 180, 135);
	stageLabel = createObject("Images/label.png", scene, 1170, 470);

	restart = createObject("Images/restart.png", scene, 0, 0, false);

	timer = createTimer(animationTime);

	initGame();

	startGame(scene);

	return 0;
}
