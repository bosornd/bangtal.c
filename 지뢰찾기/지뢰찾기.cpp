#include <bangtal.h>

#include <time.h>
#include <stdlib.h>

SceneID scene;

const int BUTTON_WIDTH = 40;
const int BUTTON_HEIGHT = 40;

const int COLUMN = 20;
const int ROW = 10;
const int BOMBS = 25;

enum State {
	CLOSED = 0,
	OPENED = 1,
	FLAGED = 2,
};

int game[ROW][COLUMN];
ObjectID board[ROW][COLUMN];
State state[ROW][COLUMN];

const char* buttonImage[9] = {
	"Images/0.png",
	"Images/1.png",
	"Images/2.png",
	"Images/3.png",
	"Images/4.png",
	"Images/5.png",
	"Images/6.png",
	"Images/7.png",
	"Images/8.png",
};

ObjectID start;

bool clickMode = true;

SoundID bgm, gameOverSound, clickSound;

void initGame() {
	for (int y = 0; y < ROW; ++y)
		for (int x = 0; x < COLUMN; ++x) game[y][x] = 0;

	for (int i = 0; i < BOMBS; ++i) {
		int x, y;
		do {
			x = rand() % COLUMN;
			y = rand() % ROW;
		} while (game[y][x] != 0);
		game[y][x] = -1;			// BOMB
	}

	for (int y = 0; y < ROW; ++y)
		for (int x = 0; x < COLUMN; ++x) {
			if (game[y][x] == 0) {
				int count = 0;
				for (int j = y - 1; j <= y + 1; ++j)
					for (int k = x - 1; k <= x + 1; ++k) {
						if (j >= 0 && j < ROW && k >= 0 && k < COLUMN)
							if (game[j][k] == -1) ++count;
					}
				game[y][x] = count;
			}

			state[y][x] = CLOSED;
			setObjectImage(board[y][x], "Images/button.png");
		}

	playSound(bgm);
}

void buttonToXY(ObjectID button, int& x, int& y) {
	for (y = 0; y < ROW; ++y)
		for (x = 0; x < COLUMN; ++x)
			if (button == board[y][x]) return;
}

bool checkEnd() {
	int count = 0;
	for (int y = 0; y < ROW; ++y)
		for (int x = 0; x < COLUMN; ++x)
			if (state[y][x] != OPENED) ++count;

	return count == BOMBS;
}

bool openButton(int x, int y) {
	if (state[y][x] == CLOSED) {
		state[y][x] = OPENED;

		if (game[y][x] < 0) {
			setObjectImage(board[y][x], "Images/bomb.png");		// bomb
			return false;
		}
		else setObjectImage(board[y][x], buttonImage[game[y][x]]);

		if (game[y][x] == 0) {
			for (int j = y - 1; j <= y + 1; ++j)
				for (int k = x - 1; k <= x + 1; ++k) {
					if (j >= 0 && j < ROW && k >= 0 && k < COLUMN)
						if (state[j][k] == CLOSED) {
							if (openButton(k, j) == false) return false;
						}
				}
		}
	}
	else if (state[y][x] == OPENED) {
		int flags = 0;
		for (int j = y - 1; j <= y + 1; ++j)
			for (int k = x - 1; k <= x + 1; ++k) {
				if (j >= 0 && j < ROW && k >= 0 && k < COLUMN)
					if (state[j][k] == FLAGED) ++flags;
			}

		if (flags == game[y][x]) {
			for (int j = y - 1; j <= y + 1; ++j)
				for (int k = x - 1; k <= x + 1; ++k) {
					if (j >= 0 && j < ROW && k >= 0 && k < COLUMN)
						if (state[j][k] == CLOSED) {
							if (openButton(k, j) == false) return false;
						}
				}
		}
	}

	return true;
}

void flagButton(int x, int y) {
	if (state[y][x] == CLOSED) {
		state[y][x] = FLAGED;
		setObjectImage(board[y][x], "Images/flag.png");
	}
	else if (state[y][x] == FLAGED) {
		state[y][x] = CLOSED;
		setObjectImage(board[y][x], "Images/button.png");
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == start) {
		stopSound(bgm);
		initGame();
	}
	else {
		playSound(clickSound);

		buttonToXY(object, x, y);

		int end = 0;
		if (clickMode) {
			if (openButton(x, y) == false) {
				end = -1;
			}
			else if (checkEnd())
				end = 1;
		}
		else {
			if (state[y][x] == OPENED) {
				if (openButton(x, y) == false) {
					end = -1;
				}
				else if (checkEnd())
					end = 1;
			}
			else flagButton(x, y);
		}

		if (end != 0) {
			showMessage(end == 1 ? "game successful" : "game failed");

			stopSound(bgm);
			if (end == -1) playSound(gameOverSound);
		}
	}
}

void keyboardCallback(KeyCode code, KeyState state) {
	if (code == KeyCode::KEY_LEFT_SHIFT || code == KeyCode::KEY_RIGHT_SHIFT) {
		if (state == KeyState::KEY_PRESSED) {
			clickMode = false;
		}
		else {
			clickMode = true;
		}
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
	setKeyboardCallback(keyboardCallback);

	scene = createScene("지뢰찾기", "Images/background.png");

	for (int y = 0; y < ROW; ++y)
		for (int x = 0; x < COLUMN; ++x)	
			board[y][x] = createObject("Images/button.png", scene, 5 + x * BUTTON_WIDTH, 5 + y * BUTTON_HEIGHT);

	start = createObject("Images/restart.png", scene, 950, 250);

	bgm = createSound("Sounds/BGM.mp3");
	gameOverSound = createSound("Sounds/GameOver.mp3");
	clickSound = createSound("Sounds/Click.mp3");

	initGame();

	startGame(scene);

	return 0;
}

