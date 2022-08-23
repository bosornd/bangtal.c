#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

SceneID scene;
ObjectID hit_button, stand_button, start_button;

const int NUMBER_OF_TYPE = 4;
const int NUMBER_OF_CARD = 13;
const int TOTAL_NUMBER_OF_CARD = NUMBER_OF_TYPE * NUMBER_OF_CARD;

int deck[TOTAL_NUMBER_OF_CARD];
int next_card;

const int INVALID_CARD = -1;

const int MAX_CARD = 11;
ObjectID player_card_object[MAX_CARD], dealer_card_object[MAX_CARD];
int player_card[MAX_CARD], dealer_card[MAX_CARD];
int player_card_num, dealer_card_num;
int player_score, dealer_score;
ObjectID player_board, dealer_board;

const char* card_image[TOTAL_NUMBER_OF_CARD] = {
	"Images/S1.png", "Images/S2.png", "Images/S3.png", "Images/S4.png", "Images/S5.png",
	"Images/S6.png", "Images/S7.png", "Images/S8.png", "Images/S9.png", "Images/S10.png",
	"Images/S11.png", "Images/S12.png", "Images/S13.png",
	"Images/D1.png", "Images/D2.png", "Images/D3.png", "Images/D4.png", "Images/D5.png",
	"Images/D6.png", "Images/D7.png", "Images/D8.png", "Images/D9.png", "Images/D10.png",
	"Images/D11.png", "Images/D12.png", "Images/D13.png",
	"Images/H1.png", "Images/H2.png", "Images/H3.png", "Images/H4.png", "Images/H5.png",
	"Images/H6.png", "Images/H7.png", "Images/H8.png", "Images/H9.png", "Images/H10.png",
	"Images/H11.png", "Images/H12.png", "Images/H13.png",
	"Images/C1.png", "Images/C2.png", "Images/C3.png", "Images/C4.png", "Images/C5.png",
	"Images/C6.png", "Images/C7.png", "Images/C8.png", "Images/C9.png", "Images/C10.png",
	"Images/C11.png", "Images/C12.png", "Images/S13.png",
};

TimerID timer;
float animationTime = 1.f;

enum GameState {
	INIT,
	PLAYER_TURN,
	DEALER_TURN,
	GAME_END,
} game_state;

SoundID bgm, card_sound, win_sound, lose_sound;

void setScore(ObjectID board, int score) {
	if (score > 0) {
		char buf[4];
		sprintf(buf, "%d", score);
		setObjectText(board, buf);
	}
	else setObjectText(board);		// text = "", default
}

void shuffle() {
	for (int i = 0; i < TOTAL_NUMBER_OF_CARD; ++i) deck[i] = INVALID_CARD;
	for (int i = 0; i < TOTAL_NUMBER_OF_CARD; ++i) {
		int j;
		do {
			j = rand() % TOTAL_NUMBER_OF_CARD;
		} while (deck[j] != INVALID_CARD);
		deck[j] = i;
	}
	next_card = 0;
}

void initGame() {
	hideObject(start_button);

	shuffle();

	for (int i = 0; i < MAX_CARD; ++i) {
		player_card[i] = INVALID_CARD;
		dealer_card[i] = INVALID_CARD;

		hideObject(player_card_object[i]);
		hideObject(dealer_card_object[i]);
	}

	player_card_num = dealer_card_num = 0;
	player_score = dealer_score = 0;

	setScore(player_board, player_score);
	setScore(dealer_board, dealer_score);

	game_state = INIT;

	setTimer(timer, animationTime);
	startTimer(timer);
}

int calcScore(int* cards, int num) {
	int sum = 0, ones = 0;
	for (int i = 0; i < num; ++i) {
		int card = cards[i] % NUMBER_OF_CARD + 1;
		if (card > 10) card = 10;
		else if (card == 1) ++ones;

		sum += card;
	}

	while (ones > 0 && sum < 12) {
		sum += 10;
		--ones;
	}

	return sum;
}
	
void setPlayerTurn() {
	showObject(hit_button);
	showObject(stand_button);
	hideObject(start_button);

	game_state = PLAYER_TURN;
}

void setDealerTurn() {
	setObjectImage(dealer_card_object[0], card_image[dealer_card[0]]);
	setScore(dealer_board, dealer_score);

	hideObject(hit_button);
	hideObject(stand_button);

	setTimer(timer, animationTime);
	startTimer(timer);

	game_state = DEALER_TURN;
}

void setGameEnd(bool playerWin, const char* message) {
	hideObject(hit_button);
	hideObject(stand_button);
	showObject(start_button);

	playSound(playerWin ? win_sound : lose_sound);
	showMessage(message);

	game_state = GAME_END;
}

void givePlayerCard(bool front = true) {
	playSound(card_sound);

	int card = deck[next_card];
	++next_card;

	player_card[player_card_num] = card;
	setObjectImage(player_card_object[player_card_num], front ? card_image[card] : "Images/back.png");
	showObject(player_card_object[player_card_num]);
	++player_card_num;

	player_score = calcScore(player_card, player_card_num);
	setScore(player_board, player_score);

	if (player_score > 21)
		setGameEnd(false, "Player Burst!!! You lose.");
}

void giveDealerCard(bool front = true) {
	playSound(card_sound);

	int card = deck[next_card];
	++next_card;

	dealer_card[dealer_card_num] = card;
	setObjectImage(dealer_card_object[dealer_card_num], front ? card_image[card] : "Images/back.png");
	showObject(dealer_card_object[dealer_card_num]);
	++dealer_card_num;

	dealer_score = calcScore(dealer_card, dealer_card_num);

	if (dealer_score > 21)
		setGameEnd(true, "Dealer Burst!!! You win.");
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == start_button) {
		initGame();
	}
	else if (object == hit_button) {
		givePlayerCard();
	}
	else if (object == stand_button) {
		setDealerTurn();
	}
}


void timerCallback(TimerID timer)
{
	if (game_state == INIT) {
		if (player_card_num == 2 && dealer_card_num == 2)
			setPlayerTurn();
		else {
			if (player_card_num > dealer_card_num) giveDealerCard(dealer_card_num != 0);
			else givePlayerCard();

			setTimer(timer, animationTime);
			startTimer(timer);
		}
	}
	else if (game_state == DEALER_TURN) {
		if (dealer_score > player_score) {
			setGameEnd(false, "Dealer Win!!!");
		}
		else {
			giveDealerCard();

			setTimer(timer, animationTime);
			startTimer(timer);
		}
		setScore(dealer_board, dealer_score);
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

	setTimerCallback(timerCallback);
	setMouseCallback(mouseCallback);

	scene = createScene("블랙잭", "Images/background.png");

	for (int i = 0; i < MAX_CARD; ++i) {
		player_card_object[i] = createObject("Images/back.png", scene, 250 + i * 75, 150, false);
		dealer_card_object[i] = createObject("Images/back.png", scene, 250 + i * 50, 465, false);
	}

	hit_button = createObject("Images/hit.png", scene,	1100, 250, false);
	stand_button = createObject("Images/stand.png", scene, 1100, 100, false);
	start_button = createObject("Images/restart.png", scene, 0, 0, false);

	player_board = createObject("Images/score.png", scene, 35, 100);
	dealer_board = createObject("Images/score.png", scene, 35, 580);

	card_sound = createSound("Sounds/card.mp3");
	win_sound = createSound("Sounds/win.mp3");
	lose_sound = createSound("Sounds/lose.mp3");

	bgm = createSound("Sounds/bgm.mp3");
	playSound(bgm, true);

	timer = createTimer(animationTime);

	initGame();
	startGame(scene);

	return 0;
}
