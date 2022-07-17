#include <bangtal.h>

SceneID scene;
ObjectID start;

const int sizeX = 8;
const int sizeY = 8;
const int sizeZ = 5;
ObjectID block[sizeZ][sizeY][sizeX];
int tower[sizeZ][sizeX];

ObjectID problem;
int target[sizeZ][sizeX] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 2, 3, 4, 4, 3, 2, 1 },
	{ 1, 3, 4, 6, 6, 4, 3, 1 },
	{ 1, 2, 3, 4, 4, 3, 2, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
};

void initGame() {
	for (int z = 0; z < sizeZ; ++z)
		for (int y = 1; y < sizeY; ++y)
			for (int x = 0; x < sizeX; ++x)
				hideObject(block[z][y][x]);

	for (int z = 0; z < sizeZ; ++z)
		for (int x = 0; x < sizeX; ++x)
			tower[z][x] = 1;

}

void addBlock(int x, int z) {
	if (tower[z][x] < sizeY) {
		showObject(block[z][tower[z][x]][x]);
		++tower[z][x];
	}
}

void removeBlock(int x, int z) {
	if (tower[z][x] > 1) {
		--tower[z][x];
		hideObject(block[z][tower[z][x]][x]);
	}
}

void findBlock(ObjectID object, int& x, int& y, int& z) {
	for (z = 0; z < sizeZ; ++z)
		for (y = 0; y < sizeY; ++y)
			for (x = 0; x < sizeX; ++x)
				if (block[z][y][x] == object) return;
}

bool checkTarget() {
	for (int z = 0; z < sizeZ; ++z)
		for (int x = 0; x < sizeX; ++x)
			if (tower[z][x] != target[z][x]) return false;

	return true;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == start) {
		initGame();
	}
	else {
		int z;
		findBlock(object, x, y, z);

		if (action == MouseAction::MOUSE_CLICK) {
			addBlock(x, z);
		}
		else removeBlock(x, z);

		if (checkTarget())
			showMessage("SUCCESS!!!");
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

	setMouseCallback(mouseCallback);

	scene = createScene("탑쌓기");

	for (int z = 0; z < sizeZ; ++z)
		for(int y = 0; y < sizeY; ++y)
			for (int x = 0; x < sizeX; ++x) {
				block[z][y][x] = createObject("Images/block.png", scene, 250 + 120 * x - 50 * z, 250 + 30 * y - 50 * z);
			}

	problem = createObject("Images/problem.png", scene, 100, 600);
	start = createObject("Images/start.png", scene, 1100, 20);

	initGame();

	startGame(scene);

	return 0;
}
