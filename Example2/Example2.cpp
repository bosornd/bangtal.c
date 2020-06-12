#include <bangtal.h>

SceneID scene;
ObjectID ball;
int x = 640, y = 360;

int dx = 0, dy = 0;

#define ANIMATION_TIME		0.01f
#define ANIMATION_STEP		5

void keyboardCallback(KeyCode code, KeyState state)
{
	if (code == 84) {			// UP
		dy += (state == KeyState::KEYBOARD_PRESSED ? ANIMATION_STEP : -ANIMATION_STEP);
	}
	else if (code == 85) {		// DOWN
		dy -= (state == KeyState::KEYBOARD_PRESSED ? ANIMATION_STEP : -ANIMATION_STEP);
	}
	else if (code == 83) {		// RIGHT
		dx += (state == KeyState::KEYBOARD_PRESSED ? ANIMATION_STEP : -ANIMATION_STEP);
	}
	else if (code == 82) {		// LEFT
		dx -= (state == KeyState::KEYBOARD_PRESSED ? ANIMATION_STEP : -ANIMATION_STEP);
	}
}

void timerCallback(TimerID timer)
{
	x += dx; y += dy;
	locateObject(ball, scene, x, y);

	setTimer(timer, ANIMATION_TIME);
	startTimer(timer);
}

int main()
{
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);

	scene = createScene("게임");
	ball = createObject("Images/ball.png");
	locateObject(ball, scene, x, y);
	showObject(ball);

	TimerID timer = createTimer(ANIMATION_TIME);
	startTimer(timer);

	startGame(scene);
}
