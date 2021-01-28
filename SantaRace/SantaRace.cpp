#include <bangtal.h>

SceneID scene;

ObjectID startButton, endButton, playButton;
ObjectID santa;

int santaX = 0, santaY = 500;

TimerID timer1;


// 게임 시작을 처리할 함수를 정의한다.
void startGame()
{
	hideObject(startButton);
	hideObject(endButton);

	showObject(playButton);

	setTimer(timer1, 10.f);
	startTimer(timer1);
}


// 게임 종료를 처리할 함수를 정의한다.
// 성공인 경우에 success에 true를 전달한다. 실패는 false.
void endGame(bool success)
{
	if (success) {
		showMessage("선물 배달 성공~~~");
	}
	else {
		showMessage("헉, 선물 배달 실패!!!");
	}

	hideObject(playButton);

	setObjectImage(startButton, "Images/restart.png");
	showObject(startButton);
	showObject(endButton);

	santaX = 0;
	locateObject(santa, scene, santaX, santaY);

	stopTimer(timer1);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (object == endButton) {
		endGame();
	}
	else if (object == startButton) {
		startGame();

	}
	else if (object == playButton) {
		santaX = santaX + 30;			// santaX += 30;
		locateObject(santa, scene, santaX, santaY);

		if (santaX > 1280) {
			endGame(true);
		}
	}
}

void timerCallback(TimerID timer)
{
	if (timer == timer1) {
		endGame(false);
	}
}

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown)
{
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);

	if (shown) {
		showObject(object);
	}

	return object;
}

int main()
{
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene = createScene("Santa Race", "Images/background.png");

	startButton = createObject("Images/start.png", scene, 590, 70, true);
	endButton = createObject("Images/end.png", scene, 590, 20, true);

	santa = createObject("Images/santa.png", scene, santaX, santaY, true);
	playButton = createObject("Images/play.png", scene, 610, 30, false);

	timer1 = createTimer(10.f);
	showTimer(timer1);

	startGame(scene);
}

