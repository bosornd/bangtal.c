#include <bangtal.h>

#include <time.h>
#include <stdlib.h>

SceneID scene;

const int BUTTON_WIDTH = 40;
const int BUTTON_HEIGHT = 40;

const int COLUMN = 20;
const int ROW = 10;
const int BOMBS = 25;

int game[ROW][COLUMN];
ObjectID board[ROW][COLUMN];
bool opened[ROW][COLUMN];

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

			opened[y][x] = false;
			setObjectImage(board[y][x], "Images/button.png");
		}
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
			if (opened[y][x] == false) ++count;

	return count == BOMBS;
}

void openButton(int x, int y) {
	if (opened[y][x] == false) {
		if (game[y][x] < 0) {
			setObjectImage(board[y][x], "Images/bomb.png");		// bomb
		}
		else setObjectImage(board[y][x], buttonImage[game[y][x]]);
		opened[y][x] = true;

		if (game[y][x] == 0) {
			for (int j = y - 1; j <= y + 1; ++j)
				for (int k = x - 1; k <= x + 1; ++k) {
					if (j >= 0 && j < ROW && k >= 0 && k < COLUMN)
						openButton(k, j);
				}
		}
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == start) {
		initGame();
	}
	else {
		buttonToXY(object, x, y);
		openButton(x, y);

		if (game[y][x] < 0) {
			// open bomb
			showMessage("game failed");
		}
		else if (checkEnd())
			showMessage("game successful");
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

	scene = createScene("지뢰찾기");	

	for (int y = 0; y < ROW; ++y)
		for (int x = 0; x < COLUMN; ++x)	
			board[y][x] = createObject("Images/button.png", scene, x * BUTTON_WIDTH, y * BUTTON_HEIGHT);

	start = createObject("Images/start.png", scene, 1000, 300);

	initGame();

	startGame(scene);

	return 0;
}

