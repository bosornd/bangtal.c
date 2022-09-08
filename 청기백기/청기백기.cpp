#include <bangtal.h>

#include <time.h>
#include <stdlib.h>

SceneID sceneMain;
ObjectID startButton;

SceneID sceneGame;
ObjectID clear, nextButton, exitButton, restartButton;

ObjectID character;
int character_x = 231, character_y = 187;
int character_stand = 0;		// 0: stand, 1: sit
int character_flag = 0;			// 0: none,  1: blue, 2: white, 3: all
bool character_jump = false;
const char* character_images[2][4] = {
	{ "Images/char_0.png", "Images/char_1.png", "Images/char_2.png", "Images/char_3.png" },
	{ "Images/charS_0.png", "Images/charS_1.png", "Images/charS_2.png", "Images/charS_3.png" }
};

ObjectID emotion_display;
int emotion = 0;
const char* emotion_images[3] = {
	"Images/none.png",
	"Images/success.png",
	"Images/fail.png",
};

ObjectID order_display;
int order = 0;

ObjectID level_display;
int level = 0;

struct LevelData {
	const char* image;
	int minOrder, maxOrder;
} level_data[3] = {
	{ "Images/L1.png", 1, 8 },
	{ "Images/L2.png", 8, 24 },
	{ "Images/L3.png", 8, 42 },
};

struct ConditionData {
	int blue, white, stand;		// -1: ignore, blue/white: 0(내려), 1(올려), stand: 0(stand), 1(sit)
};

struct OrderData {
	const char* image;
	ConditionData condition;
	bool jump;
} order_data[] = {
	{ "images/order/Lv1/none.png", { -1, -1, -1 }, false },
	{ "images/order/Lv1/청기올려.png", { 1, -1, -1 }, false },
	{ "images/order/Lv1/백기올려.png", { -1, 1, -1 }, false },
	{ "images/order/Lv1/청기내려.png", { 0, -1, -1 }, false },
	{ "images/order/Lv1/백기내려.png", { -1, 0, -1 }, false },
	{ "images/order/Lv1/점프.png", { -1, -1, -1 }, true },
	{ "images/order/Lv1/앉아.png", { -1, -1, 1 }, false },
	{ "images/order/Lv1/일어나.png", { -1, -1, 0 }, false },
	{ "images/order/Lv2/백기내리고앉아.png", { -1, 0, 1 }, false },
	{ "images/order/Lv2/백기내리고점프.png", { -1, 0, -1 }, true },
	{ "images/order/Lv2/백기올리고앉아.png", { -1, 1, 1 }, false },
	{ "images/order/Lv2/백기올리고점프.png", { -1, 1, -1 }, true },
	{ "images/order/Lv2/백기올리고청기내려.png", { 0, 1, -1 }, false },
	{ "images/order/Lv2/일어나고백기내려.png", { -1, 0, 0 }, false },
	{ "images/order/Lv2/일어나고백기올려.png", { -1, 1, 0 }, false },
	{ "images/order/Lv2/일어나고청기내려.png", { 0, -1, 0 }, false },
	{ "images/order/Lv2/일어나고청기올려.png", { 1, -1, 0 }, false },
	{ "images/order/Lv2/청기내리고백기내려.png", { 0, 0, -1 }, false },
	{ "images/order/Lv2/청기내리고앉아.png", { 0, -1, 1 }, false },
	{ "images/order/Lv2/청기내리고점프.png", { 0, -1, -1 }, true },
	{ "images/order/Lv2/청기올리고백기내려.png", { 1, 0, -1 }, false },
	{ "images/order/Lv2/청기올리고백기올려.png", { 1, 1, -1 }, false },
	{ "images/order/Lv2/청기올리고앉아.png", { 1, -1, 1 }, false },
	{ "images/order/Lv2/청기올리고점프.png", { 1, -1, -1 }, true },
	{ "images/order/Lv3/백기올리지말고앉아.png", { -1, 0, 1 }, false },
	{ "images/order/Lv3/백기올리지말고청기올려.png", { 1, 0, -1 }, false },
	{ "images/order/Lv3/앉고점프하고백기내려.png", { -1, 0, 1 }, true },
	{ "images/order/Lv3/앉고점프하고백기올려.png", { -1, 1, 1 }, true },
	{ "images/order/Lv3/앉고점프하고청기내려.png", { 0, -1, 1 }, true },
	{ "images/order/Lv3/앉고점프하고청기올려.png", { 1, -1, 1 }, true },
	{ "images/order/Lv3/앉지말고백기내려.png", { -1, 0, 0 }, false },
	{ "images/order/Lv3/앉지말고백기올려.png", { -1, 1, 0 }, false },
	{ "images/order/Lv3/앉지말고점프.png", { -1, -1, 0 }, true },
	{ "images/order/Lv3/앉지말고청기내려.png", { 0, -1, 0 }, false },
	{ "images/order/Lv3/앉지말고청기올려.png", { 1, -1, 0 }, false },
	{ "images/order/Lv3/점프하지말고백기내려.png", { -1, 0, -1 }, false },
	{ "images/order/Lv3/점프하지말고백기올려.png", { -1, 1, -1 }, false },
	{ "images/order/Lv3/점프하지말고앉아.png", { -1, -1, 1 }, false },
	{ "images/order/Lv3/점프하지말고청기내려.png", { 0, -1, -1 }, false },
	{ "images/order/Lv3/점프하지말고청기올려.png", { 1, -1, -1 }, false },
	{ "images/order/Lv3/청기올리지말고백기올려.png", { 0, 1, -1 }, false },
	{ "images/order/Lv3/청기올리지말고앉아.png", { 0, -1, 1 }, false }
};
int score = 0;

ObjectID heart_display[3];
int heart = 3;

TimerID initTimer;
float initTime = 3.f;

TimerID gameTimer;
float gameTime = 3.f;

TimerID jumpTimer;
float jumpTime = 0.1f;

TimerID emotionTimer;
float emotionTime = 1.f;

SoundID bgm, nextSound, endSound, successSound, failSound, standSound, jumpSound, flagSound, orderSound, clickSound;

const ConditionData reverseCondition(const ConditionData& condition) {
	ConditionData reversed;

	reversed.blue = (condition.blue == 0 ? 1 : condition.blue == 1 ? 0 : -1);
	reversed.white = (condition.white == 0 ? 1 : condition.white == 1 ? 0 : -1);
	reversed.stand = (condition.stand == 0 ? 1 : condition.stand == 1 ? 0 : -1);

	return reversed;
}

bool checkCondition(const ConditionData& condition) {
	if (condition.blue > -1 && condition.blue != character_flag % 2) return false;
	if (condition.white > -1 && condition.white != character_flag / 2) return false;
	if (condition.stand > -1 && condition.stand != character_stand) return false;
	if (order_data[order].jump && !character_jump) return false;

	return true;
}

void nextOrder() {
	do {
		order = rand() % (level_data[level].maxOrder - level_data[level].minOrder) + level_data[level].minOrder;
	} while (!checkCondition(reverseCondition(order_data[order].condition)));

	setObjectImage(order_display, order_data[order].image);

	character_jump = false;

	playSound(orderSound);

	setTimer(gameTimer, gameTime);
	startTimer(gameTimer);
}

void nextLevel() {
	setObjectImage(clear, "Images/stageClear.png");
	showObject(clear);
	showObject(nextButton);
	playSound(nextSound);
}

void nextGame() {
	setObjectImage(clear, "Images/stageFail.png");
	showObject(clear);
	showObject(restartButton);
	showObject(exitButton);
	playSound(endSound);
}

void checkOrder() {
	bool success = checkCondition(order_data[order].condition);
	if (order_data[order].jump && !character_jump) success = false;

	emotion = success ? 1 : 2;
	setObjectImage(emotion_display, emotion_images[emotion]);

	if (success) {
		++score;
		playSound(successSound);
	}
	else {
		--heart;
		hideObject(heart_display[heart]);
		playSound(failSound);
	}

	if ((level == 0 && score > 10) || (level == 1 && score > 30)) {
		nextLevel();
	}
	else if (heart == 0) {
		nextGame();
	}
	else {
		setTimer(emotionTimer, emotionTime);
		startTimer(emotionTimer);
	}
}

void initGame() {
	score = 0;
	heart = 3;
	for (int i = 0; i < heart; ++i)
		showObject(heart_display[i]);

	level = 0;
	setObjectImage(level_display, level_data[level].image);

	order = 0;
	setObjectImage(order_display, order_data[order].image);

	character_stand = 0;
	character_flag = 0;
	character_jump = false;
	setObjectImage(character, character_images[character_stand][character_flag]);

	emotion = 0;
	setObjectImage(emotion_display, emotion_images[emotion]);

	hideObject(clear);
	hideObject(restartButton);
	hideObject(exitButton);

	hideTimer();
	setTimer(initTimer, initTime);
	startTimer(initTimer);
}

void sceneCallback(SceneID scene, EventID event) {
	if (scene == sceneGame && event == EventID::EVENT_ENTER_SCENE) {
		initGame();
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	playSound(clickSound);

	if (object == startButton) {
		enterScene(sceneGame);
	}
	else if (object == restartButton) {
		initGame();
	}
	else if (object == nextButton) {
		++level;
		setObjectImage(level_display, level_data[level].image);

		order = 0;
		setObjectImage(order_display, order_data[order].image);

		hideObject(clear);
		hideObject(nextButton);

		hideTimer();
		setTimer(initTimer, initTime);
		startTimer(initTimer);
	}
	else if (object == exitButton) {
		endGame();
	}
}

void timerCallback(TimerID timer) {
	if (timer == jumpTimer) {
		locateObject(character, sceneGame, character_x, character_y);
		locateObject(emotion_display, sceneGame, character_x + 355, character_y + (character_stand ? 120 : 142 ));

		character_jump = true;
	}
	else if (timer == emotionTimer) {
		emotion = 0;
		setObjectImage(emotion_display, emotion_images[emotion]);

		nextOrder();
	}
	else if (timer == initTimer) {
		showTimer(gameTimer);
		nextOrder();
	}
	else if (timer == gameTimer) {
		checkOrder();
	}
}

void keyboardCallback(KeyCode code, KeyState state) {
	if (state == KeyState::KEY_PRESSED) {
		if (code == KeyCode::KEY_A) {
			character_flag = character_flag + (character_flag % 2 ? -1 : 1);
			playSound(flagSound);
		}
		else if (code == KeyCode::KEY_D) {
			character_flag = (character_flag + 2) % 4;
			playSound(flagSound);
		}
		else if (code == KeyCode::KEY_W) {
			character_stand = 0;
			locateObject(emotion_display, sceneGame, character_x + 355, character_y + 142);
			playSound(standSound);
		}
		else if (code == KeyCode::KEY_S) {
			character_stand = 1;
			locateObject(emotion_display, sceneGame, character_x + 355, character_y + 120);
			playSound(standSound);
		}
		else if (code == KeyCode::KEY_SPACE) {
			locateObject(character, sceneGame, character_x, character_y + 10);
			locateObject(emotion_display, sceneGame, character_x + 355, character_y + 10 + (character_stand ? 120 : 142));
			playSound(jumpSound);

			setTimer(jumpTimer, jumpTime);
			startTimer(jumpTimer);
		}
		else if (code == KeyCode::KEY_1) {
			emotion = (emotion + 1) % 3;
			setObjectImage(emotion_display, emotion_images[emotion]);
		}
		setObjectImage(character, character_images[character_stand][character_flag]);
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

	setMouseCallback(mouseCallback);
	setSceneCallback(sceneCallback);
	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);

	sceneMain = createScene("메인 메뉴", "Images/main.png");
	startButton = createObject("Images/start_button.png", sceneMain, 530, 50);

	sceneGame = createScene("청기 백기", "Images/game.png");
	character = createObject(character_images[character_stand][character_flag], sceneGame, character_x, character_y);
	emotion_display = createObject(emotion_images[emotion], sceneGame, character_x + 355, character_y + 142);

	order_display = createObject("Images/none.png", sceneGame, 132, 516);

	level_display = createObject(level_data[level].image, sceneGame, 72, 35);
	for (int i = 0; i < 3; ++i)
		heart_display[i] = createObject("Images/heart.png", sceneGame, 1050 + i * 65, 32);

	clear = createObject("Images/stageClear.png", sceneGame, 0, 0, false);
	nextButton = createObject("images/next_button.png", sceneGame, 537, 295, false);
	restartButton = createObject("images/restart_button.png", sceneGame, 474, 290, false);
	exitButton = createObject("images/exit_button.png", sceneGame, 700, 290, false);

	initTimer = createTimer(initTime);
	gameTimer = createTimer(gameTime);
	jumpTimer = createTimer(jumpTime);
	emotionTimer = createTimer(emotionTime);

	bgm = createSound("Sounds/dizzy.mp3");
	nextSound = createSound("Sounds/next.mp3");
	endSound = createSound("Sounds/end.mp3");
	successSound = createSound("Sounds/success.mp3");
	failSound = createSound("Sounds/fail.mp3");
	standSound = createSound("Sounds/stand.mp3");
	jumpSound = createSound("Sounds/jump.mp3");
	flagSound = createSound("Sounds/flag.mp3");
	orderSound = createSound("Sounds/order.mp3");
	clickSound = createSound("Sounds/click.mp3");

	playSound(bgm, true);

	startGame(sceneMain);

	return 0;
}

