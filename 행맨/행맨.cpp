#include <bangtal.h>

#include <time.h>
#include <stdlib.h>
#include <string.h>

SceneID scene;
ObjectID restart;

const int max_word = 10;
ObjectID character[max_word];
ObjectID hangman;
bool found[max_word];
int error = 0;

const int sizeof_dictionary = 12;
const char* dictionary[sizeof_dictionary] = {
	"APPLE",
	"BANANA",
	"CHERRY",
	"GRAPES",
	"LEMON",
	"ORANGE",
	"PEACH",
	"PEAR",
	"PINEAPPLE",
	"PLUM",
	"STRAWBERRY",
	"WATERMELON",
};
const char* word;

const char* alphabet[] = {
	"Images/A.png",
	"Images/B.png",
	"Images/C.png",
	"Images/D.png",
	"Images/E.png",
	"Images/F.png",
	"Images/G.png",
	"Images/H.png",
	"Images/I.png",
	"Images/J.png",
	"Images/K.png",
	"Images/L.png",
	"Images/M.png",
	"Images/N.png",
	"Images/O.png",
	"Images/P.png",
	"Images/Q.png",
	"Images/R.png",
	"Images/S.png",
	"Images/T.png",
	"Images/U.png",
	"Images/V.png",
	"Images/W.png",
	"Images/X.png",
	"Images/Y.png",
	"Images/Z.png",
};

const char* hangmanImage[] = {
	"Images/1.png",
	"Images/2.png",
	"Images/3.png",
	"Images/4.png",
	"Images/5.png",
	"Images/6.png",
	"Images/7.png",
	"Images/8.png",
	"Images/9.png",
};

SoundID	bgm, rightSound, wrongSound, gameOverSound;

void initGame() {
	word = dictionary[rand() % sizeof_dictionary];
	int len = strlen(word);

	for (int i = 0; i < len; ++i) {
		locateObject(character[i], scene, 600 - 75 * (len / 2 - i), 100);
		setObjectImage(character[i], "Images/blank.png");
		showObject(character[i]);
		found[i] = false;
	}
	for (int i = len; i < max_word; ++i) hideObject(character[i]);

	error = 0;
	hideObject(hangman);
	hideObject(restart);

	playSound(bgm);
}


void keyboardCallback(KeyCode code, KeyState state)
{
	if (state == KeyState::KEY_PRESSED) {
		if (code >= KeyCode::KEY_A && code <= KeyCode::KEY_Z) {
			int input = (int)code - (int)KeyCode::KEY_A;
			int len = strlen(word);

			int count = 0;
			for (int i = 0; i < len; ++i) {
				if (input == word[i] - 'A') {
					found[i] = true;
					setObjectImage(character[i], alphabet[input]);
					++count;
				}
			}

			if (count == 0) {
				if (error < 9){
					showObject(hangman);
					setObjectImage(hangman, hangmanImage[error]);
					++error;

					playSound(wrongSound);
				}

				if (error > 8) {
					for (int i = 0; i < len; ++i) {
						setObjectImage(character[i], alphabet[word[i] - 'A']);
					}

					setObjectImage(restart, "Images/lose.png");
					showObject(restart);

					playSound(gameOverSound);
				}
			}
			else {
				playSound(rightSound);

				bool complete = true;
				for (int i = 0; i < len; ++i) {
					if (found[i] == false) complete = false;
				}

				if (complete) {
					setObjectImage(restart, "Images/win.png");
					showObject(restart);
				}
			}
		}
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == restart) {
		initGame();
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

	scene = createScene("행맨", "Images/background.png");

	for (int i = 0; i < max_word; ++i)
		character[i] = createObject("Images/blank.png", scene, 300 + i * 75, 100);

	hangman = createObject(hangmanImage[0], scene, 400, 150, false);
	restart = createObject("Images/win.png", scene, 0, 0, false);

	bgm = createSound("Sounds/BGM.mp3");
	rightSound = createSound("Sounds/Right.mp3");
	wrongSound = createSound("Sounds/Wrong.mp3");
	gameOverSound = createSound("Sounds/GameOver.mp3");

	initGame();

	startGame(scene);

	return 0;
}
