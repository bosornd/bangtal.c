#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>

SceneID scene;
ObjectID background;
ObjectID restart;

struct Base {
	ObjectID b, c[3], s;
	int choco[3];
	bool made;
};
struct Base* bases[16];
const int baseX[16] = { 96, 226, 358, 495, 109, 239, 375, 515, 126, 263, 403, 545, 140, 285, 426, 567 };
const int baseY[16] = { 492, 492, 492, 492, 387, 387, 387, 387, 244, 244, 244, 244, 140, 140, 140, 140 };
int made = 0;

const char* base_images[3][3] = {
	{ "Images/choco11.png", "Images/choco12.png", "Images/choco13.png" },
	{ "Images/choco21.png", "Images/choco22.png", "Images/choco23.png" },
	{ "Images/choco31.png", "Images/choco32.png", "Images/choco33.png" },
};

ObjectID choco_button[3];
const char* choco_button_images[3][2] = {
	{ "Images/choco1_off.png", "Images/choco1_on.png" },
	{ "Images/choco2_off.png", "Images/choco2_on.png" },
	{ "Images/choco3_off.png", "Images/choco3_on.png" },
};

ObjectID snack_button;
ObjectID snack;

int choco_selected = 0;
bool snack_selected = false;

ObjectID cookie[4];
int cookie_plated = 0;

const char* cookie_images[3][3][3] = {
	"Images/result/111.png", "Images/result/112.png", "Images/result/113.png",
	"Images/result/121.png", "Images/result/122.png", "Images/result/123.png",
	"Images/result/131.png", "Images/result/132.png", "Images/result/133.png",
	"Images/result/211.png", "Images/result/212.png", "Images/result/213.png",
	"Images/result/221.png", "Images/result/222.png", "Images/result/223.png",
	"Images/result/231.png", "Images/result/232.png", "Images/result/233.png",
	"Images/result/311.png", "Images/result/312.png", "Images/result/313.png",
	"Images/result/321.png", "Images/result/322.png", "Images/result/323.png",
	"Images/result/331.png", "Images/result/332.png", "Images/result/333.png"
};

int stage = 0;
ObjectID next;
ObjectID freeze1, freeze2;
bool freezed1, freezed2;

ObjectID score_board;
int score = 0;

TimerID gameTimer;
float gameTime = 300.f;

TimerID freezeTimer;
float freezeTime = 1.f;

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) showObject(object);

	return object;
}

void setScore(int s) {
	score = s;

	char buf[10];
	sprintf(buf, "%d", score);

	setObjectText(score_board, buf);
}

void initBase(struct Base* base) {
	hideObject(base->c[0]);
	hideObject(base->c[1]);
	hideObject(base->c[2]);
	hideObject(base->s);

	base->choco[0] = 0;
	base->choco[1] = 0;
	base->choco[2] = 0;
	base->made = false;
}

struct Base* createBase(int x, int y) {
	Base* base = (Base*)malloc(sizeof(struct Base));

	base->b = createObject("Images/base.png", scene, x, y);
	base->c[0] = createObject("Images/base.png", scene, x, y, false);
	base->c[1] = createObject("Images/base.png", scene, x, y, false);
	base->c[2] = createObject("Images/base.png", scene, x, y, false);
	base->s = createObject("Images/snack_put.png", scene, x + 25, y + 25, false);

	initBase(base);

	return base;
}

void deleteBase(struct Base* base) {
	free(base);
}

void putChoco(struct Base* base, int choco) {
	int i = 0;
	for (; i < 3; ++i) {
		if (base->choco[i] == 0) break;
	}

	if (i < 3) {
		base->choco[i] = choco;
		setObjectImage(base->c[i], base_images[choco - 1][i]);
		showObject(base->c[i]);
	}
}

void putSnack(struct Base* base) {
	if (base->made) return;
	if (base->choco[2] > 0) {
		showObject(base->s);
		base->made = true;

		++made;
		if (made == 16) {
			void initStage2();

			initStage2();
		}
	}
}

void plateCookie(struct Base* base) {
	if (base->made) {
		if (cookie_plated == 4) {
			hideObject(cookie[0]);
			hideObject(cookie[1]);
			hideObject(cookie[2]);
			hideObject(cookie[3]);
			cookie_plated = 0;
		}

		setObjectImage(cookie[cookie_plated], cookie_images[base->choco[0] - 1][base->choco[1] - 1][base->choco[2] - 1]);
		showObject(cookie[cookie_plated]);
		cookie_plated++;

		initBase(base);

		--made;
		if (made == 0) {
			showObject(next);
		}

		setScore(score + 1);
	}
}

struct Base* findClickedBase(ObjectID object) {
	for (int i = 0; i < 16; ++i) {
		if (object == bases[i]->b || object == bases[i]->c[0] || object == bases[i]->c[1] || object == bases[i]->c[2] || object == bases[i]->s)
			return bases[i];
	}

	return nullptr;
}

void unselectButtons() {
	choco_selected = 0;
	snack_selected = false;

	setObjectImage(choco_button[0], choco_button_images[0][0]);
	setObjectImage(choco_button[1], choco_button_images[1][0]);
	setObjectImage(choco_button[2], choco_button_images[2][0]);

	hideObject(snack);
}

void selectChoco(int choco) {
	choco_selected = choco;
	setObjectImage(choco_button[choco_selected - 1], choco_button_images[choco_selected - 1][1]);
}

void selectSnack() {
	snack_selected = true;
	showObject(snack);
}

void initStage1() {
	stage = 1;

	setObjectImage(background, "Images/stage1.png");

	for (int i = 0; i < 16; ++i)
		initBase(bases[i]);
	made = 0;

	showObject(choco_button[0]);
	showObject(choco_button[1]);
	showObject(choco_button[2]);
	showObject(snack_button);

	hideObject(cookie[0]);
	hideObject(cookie[1]);
	hideObject(cookie[2]);
	hideObject(cookie[3]);

	unselectButtons();
	hideObject(next);
}

void initStage2() {
	stage = 2;

	setObjectImage(background, "Images/stage2.png");

	hideObject(choco_button[0]);
	hideObject(choco_button[1]);
	hideObject(choco_button[2]);
	hideObject(snack_button);
	hideObject(snack);

	showObject(freeze1);
	showObject(freeze2);
	freezed1 = freezed2 = false;
}

void initStage3() {
	stage = 3;

	setObjectImage(background, "Images/stage3.png");

	hideObject(freeze1);
	hideObject(freeze2);
}

void initGame() {
	setScore(0);
	initStage1();

	setTimer(gameTimer, gameTime);
	startTimer(gameTimer);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == restart) {
		hideObject(restart);
		initGame();
	}
	else if (object == freeze1) {
		freezed1 = true;
		if (freezed2) {
			setTimer(freezeTimer, freezeTime);
			startTimer(freezeTimer);
		}
	}
	else if (object == freeze2) {
		freezed2 = true;
		if (freezed1) {
			setTimer(freezeTimer, freezeTime);
			startTimer(freezeTimer);
		}
	}
	else if (object == next) {
		initStage1();
	}
	else if (object == choco_button[0] || object == choco_button[1] || object == choco_button[2] || object == snack_button) {
		unselectButtons();


		if (object == choco_button[0]) selectChoco(1);
		else if (object == choco_button[1]) selectChoco(2);
		else if (object == choco_button[2]) selectChoco(3);
		else selectSnack();
	}
	else {
		struct Base* base = findClickedBase(object);
		if (base != nullptr) {
			if (stage == 1) {
				if (choco_selected > 0) putChoco(base, choco_selected);
				else if (snack_selected) putSnack(base);
			}
			else if (stage == 3) {
				plateCookie(base);
			}
		}
	}
}

void timerCallback(TimerID timer) {
	if (timer == gameTimer) {
		showObject(restart);
	}
	else if (timer == freezeTimer) {
		initStage3();
	}
}

int main() {
	srand((unsigned int)time(NULL));

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene = createScene("얼초 만들기");
	background = createObject("Images/stage1.png", scene, 0, 0);

	choco_button[0] = createObject(choco_button_images[0][0], scene, 786, 504);
	choco_button[1] = createObject(choco_button_images[1][0], scene, 886, 504);
	choco_button[2] = createObject(choco_button_images[2][0], scene, 1003, 504);

	snack_button = createObject("Images/snacks.png", scene, 821, 108);
	snack = createObject("Images/snack.png", scene, 1100, 280, false);

	cookie[0] = createObject(cookie_images[0][0][0], scene, 830, 390, false);
	cookie[2] = createObject(cookie_images[0][0][0], scene, 950, 390, false);
	cookie[3] = createObject(cookie_images[0][0][0], scene, 880, 240, false);
	cookie[1] = createObject(cookie_images[0][0][0], scene, 1000, 240, false);

	for (int i = 0; i < 16; ++i) {
		bases[i] = createBase(baseX[i], baseY[i]);
	}

	next = createObject("Images/next.png", scene, 1170, 30);
	freeze1 = createObject("Images/freeze.png", scene, 720, 436, false);
	freeze2 = createObject("Images/freeze.png", scene, 784, 164, false);

	score_board = createObject("Images/score.png", scene, 110, 625);

	restart = createObject("Images/restart.png", scene, 0, 0, false);

	gameTimer = createTimer(gameTime);
	showTimer(gameTimer);

	freezeTimer = createTimer(freezeTime);;

	initGame();

	startGame(scene);

	for (int i = 0; i < 16; ++i)
		deleteBase(bases[i]);

	return 0;
}
