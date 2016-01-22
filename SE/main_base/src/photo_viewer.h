#ifndef PHOTO_VIEWER_H_
#define PHOTO_VIEWER_H_

#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <vector>

#include <ctime>

#include "utils/functions.h"

#define PSLOT(a) &PhotoViewer::a

using namespace std;

class AlarmView;

/**
	MEMO

	(_caller->*_callBack)();

	void connect(EventClass *caller,void (EventClass::*callBack)(void)){ _caller = caller; _callBack = callBack; }

	EventClass *_caller;
	void (EventClass::*_callBack)(void);
**/


class PhotoViewer{
public:
	PhotoViewer(AlarmView *);

	void run();

	void nextImage();
	void previousImage();

	void events();

	void display();

private:
	SList<UString> imagesList;
	sf::Clock lastTime;

	sf::Image loadedIMG;

	bool running, refresh;
	uint64 prevTime;

	int curImage;

	AlarmView *av;
	sf::RenderWindow *screen;
};

#endif