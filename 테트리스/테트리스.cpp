#include <bangtal.h>

#include <time.h>
#include <stdlib.h>

SceneID scene;
ObjectID restart;

const int sizeX = 10;
const int sizeY = 20;
ObjectID board_object[sizeY][sizeX];
int board[sizeY][sizeX];

const char* block[] = {
	"Images/block0.png",
	"Images/block1.png",
	"Images/block2.png",
	"Images/block3.png",
	"Images/block4.png",
	"Images/block5.png",
	"Images/block6.png",
	"Images/block7.png",
};

TimerID timer;
float animationTime = 0.5f;

const int number_of_piece = 7;
const int number_of_rotate = 4;
/* piece type
0             1            2             3             4             5            6
*             **           *               *           **             **           *
*             **           ***           ***            **           **           ***
*
*
*/
int piece_shape[number_of_piece][number_of_rotate][4][4] = {
	{
		{
			1, 0, 0, 0,
			1, 0, 0, 0,
			1, 0, 0, 0,
			1, 0, 0, 0 },
		{
			1, 1, 1, 1,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 },
		{
			1, 0, 0, 0,
			1, 0, 0, 0,
			1, 0, 0, 0,
			1, 0, 0, 0 },
		{
			1, 1, 1, 1,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 },
	},
	{
		{ 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	},
	{
		{
			1, 0, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0 },
		{
			1, 1, 0, 0,
			1, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0 },
		{
			1, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0 },
		{
			0, 1, 0, 0,
			0, 1, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0 },
	},
	{
		{
			0, 0, 1, 0,
			1, 1, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0 },
		{
			1, 0, 0, 0,
			1, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0 },
		{
			1, 1, 1, 0,
			1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 },
		{
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0, 0, 0, 0, 0 },
	},
	{
		{
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0 },
		{
			0, 1, 0, 0,
			1, 1, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0 },
		{
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0 },
		{
			0, 1, 0, 0,
			1, 1, 0, 0,
			1, 0, 0, 0, 0, 0, 0, 0 },
	},
	{
		{
			0, 1, 1, 0,
			1, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 },
		{
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 1, 0, 0, 0, 0, 0 },
		{
			0, 1, 1, 0,
			1, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 },
		{
			1, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0, 0, 0, 0, 0 },
	},
	{
		{
			0, 1, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0 },
		{
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 1, 0, 0, 0, 0, 0, 0 },
		{
			1, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0 },
		{
			0, 1, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0, 0, 0, 0, 0 },
	}
};
int piece;
int piece_x, piece_y;
int piece_r;		// 0, 1, 2, 3으로 4방향을 가리킨다.

void removeLines() {
	int removed = 0;
	for (int y = sizeY - 1; y >= 0; --y) {
		bool packed = true;
		for(int x = 0; x < sizeX; ++x)
			if (board[y][x] == 0) packed = false;

		if (packed) ++removed;
		else {
			for (int x = 0; x < sizeX; ++x)
				board[y + removed][x] = board[y][x];
		}
	}

	for (int y = 0; y < removed; ++y)
		for (int x = 0; x < sizeX; ++x)
			board[y][x] = 0;
}

void createPiece() {
	piece = rand() % number_of_piece;
	piece_x = 5;
	piece_y = 0;
}

void putPiece() {
	for (int y = 0; y < 4; ++y)
		for (int x = 0; x < 4; ++x) {
			if (piece_shape[piece][piece_r][y][x]) {
				board[piece_y + y][piece_x + x] = piece + 1;
			}
		}

	piece = -1;

	removeLines();
}

bool possible(int dx, int dy, int dr) {
	if (piece < 0) return false;

	int r = (piece_r + dr + number_of_rotate) % number_of_rotate;
	for (int y = 0; y < 4; ++y)
		for (int x = 0; x < 4; ++x) {
			if (piece_shape[piece][r][y][x]) {
				const int tx = piece_x + x + dx;
				const int ty = piece_y + y + dy;

				if (tx < 0 || tx >= sizeX || ty < 0 || ty >= sizeY) return false;
				if (board[ty][tx]) return false;
			}
		}
	return true;
}

void redrawBoard() {
	for (int y = 0; y < sizeY; ++y)
		for (int x = 0; x < sizeX; ++x) {
			setObjectImage(board_object[y][x], block[board[y][x]]);
		}

	if (piece > -1) {
		for (int y = 0; y < 4; ++y)
			for (int x = 0; x < 4; ++x) {
				if (piece_shape[piece][piece_r][y][x])
					setObjectImage(board_object[piece_y + y][piece_x + x], block[piece + 1]);
			}
	}
}

void initGame() {
	hideObject(restart);

	for (int y = 0; y < sizeY; ++y)
		for (int x = 0; x < sizeX; ++x) {
			board[y][x] = 0;
		}

	piece = -1;
	redrawBoard();

	setTimer(timer, animationTime);
	startTimer(timer);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == restart) {
		initGame();
	}
}

void keyboardCallback(KeyCode code, KeyState state)
{
	if (state == KeyState::KEY_PRESSED) {
		if (code == KeyCode::KEY_LEFT_ARROW) {
			if (possible(-1, 0, 0))
				--piece_x;
		}
		else if (code == KeyCode::KEY_RIGHT_ARROW) {
			if (possible(1, 0, 0))
				++piece_x;
		}
		else if (code == KeyCode::KEY_UP_ARROW) {
			if (possible(0, 0, 1))
				piece_r = (piece_r + 1) % number_of_rotate;
		}
		else if (code == KeyCode::KEY_DOWN_ARROW) {
			while (possible(0, 1, 0))
				++piece_y;
		}
	}
}

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) showObject(object);

	return object;
}

void timerCallback(TimerID timer)
{
	bool end = false;
	if (piece == -1) {
		createPiece();
		if (possible(0, 0, 0) == false) {
			showObject(restart);
			end = true;
		}
	}
	else {
		if (possible(0, 1, 0)) {
			++piece_y;
		}
		else putPiece();
	}

	redrawBoard();

	if (!end){
		setTimer(timer, animationTime);
		startTimer(timer);
	}
}

int main() {
	srand((unsigned int)time(NULL));

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setTimerCallback(timerCallback);
	setMouseCallback(mouseCallback);
	setKeyboardCallback(keyboardCallback);

	scene = createScene("테트리스", "Images/background.png");

	const int positionX = 502;
	const int positionY = 102;

	for (int y = 0; y < sizeY; ++y)
		for (int x = 0; x < sizeX; ++x) {
			board_object[y][x] = createObject("Images/block0.png", scene, positionX + 20 * x, positionY + 380 - 20 * y);
		}

	timer = createTimer(animationTime);
	startTimer(timer);

	restart = createObject("Images/restart.png", scene, 0, 0, false);

	initGame();

	startGame(scene);

	return 0;
}
