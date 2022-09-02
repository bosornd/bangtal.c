#include <bangtal.h>

SceneID scene1, scene2;
ObjectID door1, door2, door3;
bool closed1 = true, closed2 = true, closed3 = true;	// 문이 닫혀 있으면 true, 열려 있으면 false
bool locked3 = true, power = true;

ObjectID key, flowerpot, keypad, button, password;

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (object == door1) {
		if (closed1 == true) {
			if (getHandObject() == key) {
				setObjectImage(door1, "Images/문-오른쪽-열림.png");
				closed1 = false;
			}
			else {
				showMessage("열쇠가 필요해!!!");
			}
		}
		else {
			enterScene(scene2);
		}
	}
	else if (object == door2) {
		if (closed2 == true) {
			setObjectImage(door2, "Images/문-왼쪽-열림.png");
			closed2 = false;
		}
		else {
			enterScene(scene1);
		}
	}
	else if (object == door3) {
		if (locked3 == true) {
			showMessage("잠겨 있네. 어떻게 열지?");
		}
		else if (closed3 == true) {
			setObjectImage(door3, "Images/문-오른쪽-열림.png");
			closed3 = false;
		}
		else {
			endGame();
		}
	}
	else if (object == flowerpot) {
		if (action == MouseAction::MOUSE_DRAG_LEFT) {
			locateObject(flowerpot, scene1, 450, 110);
		}
		else if (action == MouseAction::MOUSE_DRAG_RIGHT) {
			locateObject(flowerpot, scene1, 650, 110);
		}
	}
	else if (object == key) {
		pickObject(key);
	}
	else if (object == keypad) {
		showKeypad("BANGTAL", door3);
	}
	else if (object == button) {
		power = !power;			// power를 toggle 한다.

		if (power == true) {
			// power가 true(불이 켜진 상태)이면, 조명을 높인다.
			setSceneLight(scene2, 1.f);

			// 불이 켜지면 암호가 보이지 않는다.
			hideObject(password);
		}
		else {
			// power가 false(불이 꺼진 상태)이면, 조명을 낮춘다.
			setSceneLight(scene2, 0.2f);

			// 불이 꺼지면 암호가 보인다.
			showObject(password);
		}
	}
}

void objectCallback(ObjectID object, EventID event)
{
	if (object == door3) {
		if (event == EventID::EVENT_KEYPAD) {
			locked3 = false;
			showMessage("잠금 해제!!!");
		}
	}
}

int main()
{
	setMouseCallback(mouseCallback);
	setObjectCallback(objectCallback);

	scene1 = createScene("룸1", "Images/배경-1.png");

	door1 = createObject("Images/문-오른쪽-닫힘.png");
	locateObject(door1, scene1, 755, 257);
	showObject(door1);

	scene2 = createScene("룸2", "Images/배경-2.png");

	door2 = createObject("Images/문-왼쪽-닫힘.png");
	locateObject(door2, scene2, 129, 222);
	showObject(door2);

	door3 = createObject("Images/문-오른쪽-닫힘.png");
	locateObject(door3, scene2, 904, 245);
	showObject(door3);

	key = createObject("Images/열쇠.png");
	locateObject(key, scene1, 620, 150);
	scaleObject(key, 0.2f);
	showObject(key);

	flowerpot = createObject("Images/화분.png");
	locateObject(flowerpot, scene1, 550, 110);
	showObject(flowerpot);

	keypad = createObject("Images/키패드.png");
	locateObject(keypad, scene2, 880, 460);
	showObject(keypad);

	button = createObject("Images/스위치.png");
	locateObject(button, scene2, 875, 480);
	showObject(button);

	// 아직 암호는 보이진 않는다.
	password = createObject("Images/암호.png");
	locateObject(password, scene2, 400, 100);

	startGame(scene1);
}
