	#include <bangtal.h>

#include <stdlib.h>

SceneID scene;
ObjectID game_board[16], init_board[16], start;

const char* board_image[16] = {
"Images/1.png",
"Images/2.png",
"Images/3.png",
"Images/4.png",
"Images/5.png",
"Images/6.png",
"Images/7.png",
"Images/8.png",
"Images/9.png",
"Images/10.png",
"Images/11.png",
"Images/12.png",
"Images/13.png",
"Images/14.png",
"Images/15.png",
"Images/16.png",
};
int board_x[16] = { 300, 450, 600, 750, 300, 450, 600, 750, 300, 450, 600, 750, 300, 450, 600, 750 };
int board_y[16] = { 470, 470, 470, 470, 320, 320, 320, 320, 170, 170, 170, 170, 20, 20, 20, 20 };
int blank;

TimerID timer;
float animationTime = 0.1f;
int mixCount;
int mixCountInit = 100;

int board_index(ObjectID object) {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] == object) return i;
	}
	return -1;
}

void board_move(int index) {
	ObjectID t = game_board[blank];
	game_board[blank] = game_board[index];
	game_board[index] = t;

	locateObject(game_board[index], scene, board_x[index], board_y[index]);
	locateObject(game_board[blank], scene, board_x[blank], board_y[blank]);

	blank = index;
}

bool movable(int index) {
	if (index < 0 || index > 15) return false;
	if (blank % 4 != 0 and index == blank - 1) return true;
	if (blank % 4 != 3 and index == blank + 1) return true;
	if (blank / 4 != 0 and index == blank - 4) return true;
	if (blank / 4 != 3 and index == blank + 4) return true;
	return false;
}

void board_mix() {
	int index;

	do {
		switch (rand() % 4) {
		case 0: index = blank - 1; break;
		case 1: index = blank + 1; break;
		case 2: index = blank - 4; break;
		case 3: index = blank + 4; break;
		}
	} while (!movable(index));
	
	board_move(index);
}

bool completed() {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] != init_board[i]) return false;
	}
	return true;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == start) {
		hideObject(start);

		mixCount = mixCountInit;

		setTimer(timer, animationTime);
		startTimer(timer);
	}
	else {
		int index = board_index(object);
		if (movable(index)) {
			board_move(index);

			if (completed()) {
				showMessage("Completed!!!");
				showObject(start);
			}
		}
	}
}

void timerCallback(TimerID timer) {
	mixCount--;
	if (mixCount > 0) {
		board_mix();

		setTimer(timer, animationTime);
		startTimer(timer);
	}
}

int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene = createScene("겨울왕국2", "Images/배경.png");

	for (int i = 0; i < 16; i++) {
		game_board[i] = createObject(board_image[i]);
		init_board[i] = game_board[i];

		locateObject(game_board[i], scene, board_x[i], board_y[i]);
		showObject(game_board[i]);
	}

	blank = 15;
	hideObject(game_board[blank]);

	start = createObject("Images/start.png");
	locateObject(start, scene, 540, 100);
	showObject(start);

	timer = createTimer(animationTime);

	startGame(scene);

	return 0;
}
