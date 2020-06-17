#include <bangtal.h>

SceneID scene1;
ObjectID problem;

ObjectID left[7], right[7];									// 왼쪽, 오른쪽 체크 마크

int leftX[7] = { 546, 77, 361, 379, 39, 570, 298 };			// 왼쪽 틀린 그림의 X좌표(중앙)
int rightX[7] = { 1164, 695, 979, 997, 657, 1188, 916 };	// 오른쪽 틀린 그림의 X좌표(중앙)
int Y[7] = { 542, 499, 430, 106, 203, 369, 65 };			// 틀린 그림의 Y좌표(중앙)
int radius[7] = { 54, 17, 16, 27, 36, 35, 13 };				// 틀린 그림을 체크하는 반지름

// 찾았는 지, 찾지 못했는 지를 저장하는 논리형 변수 배열을 선언한다.
bool checked[7];											// 찾았는 지(true), 찾지 못했는지(false)


bool checkIn(int x, int y, int cx, int cy, int r) {
	return (x > cx - r && x < cx + r && y > cy - r && y < cy + r);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (object == problem) {
		bool wrong = true;


		for (int i = 0; i < 7; i++) {
			// 이미 찾은 경우에는 검사할 필요가 없다. 조건을 추가한다.
			if (checked[i] == false && (checkIn(x, y, leftX[i], Y[i], radius[i]) || checkIn(x, y, rightX[i], Y[i], radius[i]))) {
				showObject(left[i]);
				showObject(right[i]);

				// 찾은 경우에 checked[i]를 true로 변경한다.
				checked[i] = true;
				wrong = false;
			}
		}

		if (wrong) {
			endGame();
		}
		// 새로 찾은 경우에 모두 찾았으면 게임을 종료한다.
		else {
			// 모두 찾았는가를 저장하는 변수를 선언한다.
			bool completed = true;

			// 7개 중에서 하나라도 찾지 못한 것이 있으면 completed = false로 변경한다.
			// i는 0부터 7보다 작을 때(6)까지
			for (int i = 0; i < 7; i++) {
				// checked[i]가 false이면 completed를 false로 변경한다.
				if (checked[i] == false) {
					completed = false;
				}
			}

			// completed 가 true 이면(즉, 하나도 checked[i]가 false인 것이 없으면)
			if (completed) {
				// 게임을 종료한다.
				endGame();
			}
		}
	}
}

int main()
{
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mouseCallback);

	scene1 = createScene("Spot Difference");

	problem = createObject("Images/problem.png");
	locateObject(problem, scene1, 22, 52);
	showObject(problem);

	for (int i = 0; i < 7; i++) {
		left[i] = createObject("Images/check.png");
		locateObject(left[i], scene1, leftX[i] - 3, Y[i] + 27);	  // -25 + 22, -25 + 52

		right[i] = createObject("Images/check.png");
		locateObject(right[i], scene1, rightX[i] - 3, Y[i] + 27); // -25 + 22, -25 + 52
	}

	showMessage("좌우에 틀린 곳을 찾아보세요.");
	startGame(scene1);
}
