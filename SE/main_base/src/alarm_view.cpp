#include "alarm_view.h"
#include "photo_viewer.h"

//static Coord numPos[10];

SList<UString> StatusTXT;

SList<sf::Sprite*> icones;

AlarmView::AlarmView(sf::RenderWindow *screen, int fps){
	this->screen = screen;
	screen->SetFramerateLimit(fps);
	_exit = false;
	unlockPass = "";
	lastTouchEvent.pos = Coord(-100, -100);
	lastTouchEvent.lastTime.Reset();
	init();
}

void AlarmView::init(){
	std::cout << "[THR1] Init started" << std::endl;
	StatusTXT << "OK";
	StatusTXT << "WARNING";
	StatusTXT << "LOW BATTERY";
	StatusTXT << "ERROR";
	StatusTXT << "EMERGENCY";
	StatusTXT << "NO RESPONSE";

	sensorsList << new Sensor(0, 0, "Sensor 1");
	sensorsList << new Sensor(0, 0, "Sensor 2");
	sensorsList << new Sensor(0, 0, "Sensor 3");
	sensorsList << new Sensor(0, 0, "Sensor 4");
	sensorsList << new Sensor(0, 0, "Sensor 5");
	sensorsList << new Sensor(0, 0, "Sensor 6");
	sensorsList << new Sensor(0, 0, "Sensor 7");
	sensorsList << new Sensor(0, 0, "Sensor 8");
	sensorsList << new Sensor(0, 0, "Sensor 9");
	sensorsList << new Sensor(0, 0, "Sensor 10");

	icones << loadHWImg(UString("./data/imgs/empty.png")); /* DUMMY ICONE */
	icones << loadHWImg(UString("./data/imgs/pir.png"));
	icones << loadHWImg(UString("./data/imgs/chock.png"));
	icones << loadHWImg(UString("./data/imgs/rfid.png"));

	std::cout << "[THR1] Creating thread" << std::endl;
	uartTh = new UartThread(this);

	refreshDisplay = true;

	uartTh->start();

	alarmEnabled = false;
	lockScreen = false;

	int margin = (screen->GetWidth() - 256*4) / 5;
	int x = margin+(margin+256)*3;
	int y = margin+(margin+256)*0;
	int buttonsMarginX = (screen->GetWidth() - x - 128*2 - margin) / 2;
	int buttonsMarginY = (screen->GetHeight() - y - 128*4 - margin) / 4;

	Button *lock = new Button(loadHWImg(UString("./data/imgs/lock.png")), Coord(x + (128 + buttonsMarginX)*1, y + (128+buttonsMarginY)*3));


	Button *pageUp = new Button(loadHWImg(UString("./data/imgs/up.png")), Coord(x + (128 + buttonsMarginX)*0, y + (128+buttonsMarginY)*0));
	Button *pageDown = new Button(loadHWImg(UString("./data/imgs/down.png")), Coord(x + (128 + buttonsMarginX)*0, y + (128+buttonsMarginY)*3));

	lock->connect(this, ASLOT(enablePhotoViewer));
	pageUp->connect(this, ASLOT(prevPage));
	pageDown->connect(this, ASLOT(nextPage));

	lock->setVisibility(true);
	pageUp->setVisibility(true);
	pageDown->setVisibility(true);

	buttons << lock;
	buttons << pageUp;
	buttons << pageDown;

	sensorsButtons << lock;
	sensorsButtons << pageUp;
	sensorsButtons << pageDown;


	Coord numPos[10] = {				 								  Coord(screen->GetWidth() - 8 - 128 - 8 - 128, 416),
			Coord(screen->GetWidth() - 8 - 128 - 8 - 128 - 8 - 128, 280), Coord(screen->GetWidth() - 8 - 128 - 8 - 128, 280), Coord(screen->GetWidth() - 8 - 128, 280), 
			Coord(screen->GetWidth() - 8 - 128 - 8 - 128 - 8 - 128, 144), Coord(screen->GetWidth() - 8 - 128 - 8 - 128, 144), Coord(screen->GetWidth() - 8 - 128, 144), 
			Coord(screen->GetWidth() - 8 - 128 - 8 - 128 - 8 - 128,   8), Coord(screen->GetWidth() - 8 - 128 - 8 - 128,   8), Coord(screen->GetWidth() - 8 - 128,   8)};

	for(int i=0;i<10;i++){
		Button *b = new Button(loadHWImg(UString("./data/imgs/") << i << ".png"), numPos[i]);
		numButtons << b;
		buttons << b;
		b->setVisibility(false);
	}

	numButtons[0]->connect(this, ASLOT(numClicked0));
	numButtons[1]->connect(this, ASLOT(numClicked1));
	numButtons[2]->connect(this, ASLOT(numClicked2));
	numButtons[3]->connect(this, ASLOT(numClicked3));
	numButtons[4]->connect(this, ASLOT(numClicked4));
	numButtons[5]->connect(this, ASLOT(numClicked5));
	numButtons[6]->connect(this, ASLOT(numClicked6));
	numButtons[7]->connect(this, ASLOT(numClicked7));
	numButtons[8]->connect(this, ASLOT(numClicked8));
	numButtons[9]->connect(this, ASLOT(numClicked9));

	Button *unlockBut = new Button(loadHWImg("./data/imgs/l.png"), Coord(screen->GetWidth() - 8 - 128, 416));
	Button *deleteBut = new Button(loadHWImg("./data/imgs/d.png"), Coord(screen->GetWidth() - 8 - 128 - 8 - 128 - 8 - 128, 416));
	unlockBut->connect(this, ASLOT(unlockClicked));
	deleteBut->connect(this, ASLOT(deleteCharacter));
	buttons << unlockBut;
	buttons << deleteBut;
	numButtons << unlockBut;
	numButtons << deleteBut;

	unlockBut->setVisibility(false);
	deleteBut->setVisibility(false);

	passwordLabel = new Label("", Coord(10,50), font, sf::Color(0, 64, 59), 100);
	std::cout << "[THR1] Init done" << std::endl;
}

void AlarmView::run(){
	//sf::Clock clk;
	realFPS = 0;
	int ref = 100;
	while(!_exit){
		//realFPS = 1.f / screen->GetFrameTime();

		events();

		/*if(clk.GetElapsedTime() > 5.0){
			clk.Reset();
			refreshDisplay = true;
		}*/

		if(!lockScreen){
			refreshDisplay = uartTh->updatedData;
			uartTh->updatedData = false;
		}

		if(0>--ref || refreshDisplay){
			ref = 100;
			realFPS += 1.0;
			display();
			refreshDisplay = false;
			screen->Display();
			//clk.Reset();
		}
		usleep(50000);
	}
}

void AlarmView::events(){
	sf::Event event;

	while(screen->GetEvent(event)){
		switch (event.Type) {
		case sf::Event::KeyReleased:
			if((int)event.Key.Code == sf::Key::Escape)
				_exit = true;
			break;
		case sf::Event::Closed:
			_exit = true;
			break;
		case sf::Event::MouseButtonPressed:
			screenPressEvent(event);
			break;
		case sf::Event::MouseButtonReleased:
			screenReleaseEvent(event);
			break;
		default:
			break;
		}
	}
}

void AlarmView::screenPressEvent(const sf::Event & event){
	/* Touch event used to detect a press & release event (click) */
	lastTouchEvent.pos = Coord(event.MouseButton.X, event.MouseButton.Y);
	lastTouchEvent.lastTime.Reset();
}

void AlarmView::screenReleaseEvent(const sf::Event & event){
	/* Touch event used to detect a press & release event (click) */
	if(event.MouseButton.X-10 < lastTouchEvent.pos.x && event.MouseButton.X+10 > lastTouchEvent.pos.x &&
	event.MouseButton.Y-10 < lastTouchEvent.pos.y && event.MouseButton.Y+10 > lastTouchEvent.pos.y &&
	lastTouchEvent.lastTime.GetElapsedTime() < 1.0)
		screenClickEvent(event);
	lastTouchEvent.pos = Coord(-100, -100);
	lastTouchEvent.lastTime.Reset();
}

void AlarmView::screenClickEvent(const sf::Event & event){
	for(uint i=0;i<buttons.size();i++)
		buttons[i]->clicked(lastTouchEvent.pos);
}

void AlarmView::display(){
	if(alarmEnabled){
		screen->Clear(sf::Color(0, 0, 0));
		return;
	}

	screen->Clear(sf::Color(49, 179, 170));

	//Label l(UString("frame: ") << realFPS, Coord(5, 480-30), font, sf::Color(0, 64, 59));
	//l.show(screen);

	if(!lockScreen){
		int margin = (screen->GetWidth() - 256*4) / 5;
		for(uint i=0;i<sensorsList.size();i++){
			if(sensorsList[i]->type == SENSOR_NONE)
				continue;

			Coord pos(margin+(margin+256)*(i%3), margin+(margin+256)*(int)(i/3));
			sf::Color col;
			int stat = sensorsList[i]->status;
			int typ  = sensorsList[i]->type;

			switch(stat){
				case STATUS_OK:
					col = sf::Color(0, 64, 59); break;
				case STATUS_EMERGENCY:
					col = sf::Color(255, 0, 0); break;
				case STATUS_WARNING:
					col = sf::Color(200, 130, 0); break;
				case STATUS_ERROR:
					col = sf::Color(255, 0, 0); break;
				case STATUS_LOW_BAT:
					col = sf::Color(200, 130, 0); break;
				case STATUS_TIMEOUT:
					col = sf::Color(100, 100, 0); break;
				default:
					col = sf::Color(0, 64, 59); break;
					break;
			}


			drawRect(screen, Coord(pos.x, pos.y), Coord(pos.x+256,pos.y+256), col);
			drawRect(screen, Coord(pos.x+1, pos.y+1), Coord(pos.x+255,pos.y+255), col);
			drawRect(screen, Coord(pos.x+2, pos.y+2), Coord(pos.x+254,pos.y+254), col);
			drawRect(screen, Coord(pos.x+3, pos.y+3), Coord(pos.x+253,pos.y+253), col);
			drawRect(screen, Coord(pos.x+4, pos.y+4), Coord(pos.x+252,pos.y+252), col);
			drawRect(screen, Coord(pos.x+5, pos.y+5), Coord(pos.x+251,pos.y+251), col);
			drawRect(screen, Coord(pos.x+6, pos.y+6), Coord(pos.x+250,pos.y+250), col);
			drawRect(screen, Coord(pos.x+7, pos.y+7), Coord(pos.x+249,pos.y+249), col);

			icones[typ]->SetPosition(pos.x+64, pos.y+64);
			screen->Draw(*(icones[typ]));

			Label l(StatusTXT[sensorsList[i]->status], Coord(pos.x+10, pos.y+64+128+10), font, col, 32);
			l.show(screen);

			Label nm(sensorsList[i]->name, Coord(pos.x+10, pos.y+10), font, sf::Color(0, 64, 59), 28);
			nm.show(screen);
		}
	}else{
		UString hiddenStr;

		for(uint i=0;i<unlockPass.size();i++)
			hiddenStr << "*";
		passwordLabel->text(hiddenStr);
		passwordLabel->show(screen);
	}

	for(uint i=0;i<buttons.size();i++)
		buttons[i]->show(screen);
}

void AlarmView::unlockClicked(){
	if(unlockPass == "1234"){
		for(uint i=0;i<numButtons.size();i++)
			numButtons[i]->setVisibility(false);
		lockScreen = false;

		for(uint i=0;i<sensorsButtons.size();i++)
			sensorsButtons[i]->setVisibility(true);
	}
	unlockPass = "";
	refreshDisplay = true;
}

void AlarmView::deleteCharacter(){
	if(unlockPass.size() > 0){
		unlockPass = unlockPass.substr(0, unlockPass.size()-1);
		refreshDisplay = true;
	}
}

void AlarmView::enableAlarm(){
	for(uint i=0;i<numButtons.size();i++)
		numButtons[i]->setVisibility(false);
	for(uint i=0;i<sensorsButtons.size();i++)
		sensorsButtons[i]->setVisibility(false);
	alarmEnabled = true;
	refreshDisplay = true;
}

void AlarmView::disableAlarm(){
	for(uint i=0;i<sensorsButtons.size();i++)
		sensorsButtons[i]->setVisibility(true);
	alarmEnabled = false;
	refreshDisplay = true;
}

void AlarmView::enablePhotoViewer(){
	for(uint i=0;i<buttons.size();i++)
		buttons[i]->setVisibility(false);

	// RUN THE PHOTOVIEWER
	PhotoViewer pw(this);
	pw.run();

	disablePhotoViewer();
}

void AlarmView::disablePhotoViewer(){
	for(uint i=0;i<numButtons.size();i++)
		numButtons[i]->setVisibility(true);

	lockScreen = true;

}

void AlarmView::nextPage(){

}

void AlarmView::prevPage(){

}