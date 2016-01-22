#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <vector>

#include <ctime>


#include "photo_viewer.h"
#include "alarm_view.h"

PhotoViewer::PhotoViewer(AlarmView *av){
	this->av = av;
	this->screen = this->av->screen;
	prevTime = 0;
	running = true;
	refresh = true;
	lastTime.Reset();
	curImage = 0;

	imagesList << "./data/images/img1.jpg";
	imagesList << "./data/images/img2.jpg";
	imagesList << "./data/images/img3.jpg";

	loadedIMG.LoadFromFile(imagesList[curImage]);

}

void PhotoViewer::run(){
	int count=120;
	while(running){
		events();
		if(refresh){
			refresh = false;
			display();
		}
		if(count--<0){
			count=120;
			nextImage();
			refresh = true;
		}
		usleep(50000);
	}
}


void PhotoViewer::nextImage(){
	curImage++;
	if(curImage == (int)imagesList.size())
		curImage=0;

	loadedIMG.LoadFromFile(imagesList[curImage]);
	/*sf::IntRect rect;

	rect.Left = 0;
	rect.Top = 0;
	rect.Right = loadedIMG.GetWidth();
	rect.Bottom = loadedIMG.GetHeight();

	sprite.SetSubRect(rect);*/

	refresh= true;
}

void PhotoViewer::previousImage(){
	curImage--;
	if(curImage<0)
		curImage=imagesList.size()-1;

	loadedIMG.LoadFromFile(imagesList[curImage]);
	/*sf::IntRect rect;

	rect.Left = 0;
	rect.Top = 0;
	rect.Right = loadedIMG.GetWidth();
	rect.Bottom = loadedIMG.GetHeight();

	sprite.SetSubRect(rect);*/

	refresh= true;
}

void PhotoViewer::events(){
	sf::Event event;

	while(screen->GetEvent(event)){
		switch (event.Type) {
		case sf::Event::MouseButtonPressed:
			lastTime.Reset();
			break;
		case sf::Event::MouseButtonReleased:
			if(lastTime.GetElapsedTime() > 2.0)
				running = false;
			break;
		default:
			break;
		}
	}
}

void PhotoViewer::display(){
	sf::Sprite sprite;
	sprite.SetImage(loadedIMG);

	float imgW = (float)sprite.GetSize().x;
	float imgH = (float)sprite.GetSize().y;

	float scrW = (float)screen->GetWidth();
	float scrH = (float)screen->GetHeight();

	float scaleFactor = 1.0f;

	int posX,posY;

	if(imgW/scrW > imgH/scrH){
		scaleFactor = scrW / imgW;
		posX = 0;
		posY = scrH/2 - (imgH*scaleFactor)/2;
	}else{
		scaleFactor = scrH / imgH;
		posX = scrW/2 - (imgW*scaleFactor)/2;
		posY = 0;
	}
	sprite.SetPosition(posX, posY);

	sprite.SetScale(scaleFactor, scaleFactor);

	screen->Clear();
	screen->Draw(sprite);
	screen->Display();
}

