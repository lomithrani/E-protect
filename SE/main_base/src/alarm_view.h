#ifndef ALARM_VIEW_H_
#define ALARM_VIEW_H_


#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

#include <ctime>
#include <cmath>

#include "utils/functions.h"
#include "widget.h"

#include "uart_com.h"

#define ASLOT(a) &AlarmView::a

using namespace std;

class AlarmView{
public:
	AlarmView(sf::RenderWindow *win, int fps = 30);

	void run();

	bool alarmEnabled, lockScreen;
	
	void enableAlarm();
	void disableAlarm();
	
	sf::RenderWindow *screen;

private:
	typedef struct TouchEvent{
		Coord pos;
		sf::Clock lastTime;
	}TouchEvent;

	void events();
	void display();

	void init();

	void screenPressEvent(const sf::Event &);
	void screenReleaseEvent(const sf::Event &);
	void screenClickEvent(const sf::Event &);
	TouchEvent lastTouchEvent;

	bool _exit;

	SList<Button*> buttons, numButtons, sensorsButtons;

	UString unlockPass;

	Label *passwordLabel;

	float realFPS;


	bool refreshDisplay;

	UartThread *uartTh;

	/* CALLBACK functions */

	void numClicked0(){unlockPass << 0; refreshDisplay = true;}
	void numClicked1(){unlockPass << 1; refreshDisplay = true;}
	void numClicked2(){unlockPass << 2; refreshDisplay = true;}
	void numClicked3(){unlockPass << 3; refreshDisplay = true;}
	void numClicked4(){unlockPass << 4; refreshDisplay = true;}
	void numClicked5(){unlockPass << 5; refreshDisplay = true;}
	void numClicked6(){unlockPass << 6; refreshDisplay = true;}
	void numClicked7(){unlockPass << 7; refreshDisplay = true;}
	void numClicked8(){unlockPass << 8; refreshDisplay = true;}
	void numClicked9(){unlockPass << 9; refreshDisplay = true;}
	void unlockClicked();
	void deleteCharacter();

	void enablePhotoViewer();
	void disablePhotoViewer();

	void nextPage();
	void prevPage();

};

#endif