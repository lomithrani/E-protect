#ifndef WIDGETS_H_
#define WIDGETS_H_

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

#include "utils/functions.h"

using namespace std;

class AlarmView;

class Button{
public:
	Button(sf::Sprite*,int,int);
	Button(sf::Sprite*,Coord);

	void clicked(int,int);
	void clicked(Coord);

	void setVisibility(bool visible){ this->visible = visible; }

	void move(Coord coordinate){ pos = coordinate; }

	void show(sf::RenderWindow*);

	void connect(AlarmView *caller,void (AlarmView::*callBack)(void)){ caller_a = caller; callBack_a = callBack; }

private:
	Coord pos;
	sf::Sprite *image;

	bool visible;

	AlarmView *caller_a;
	void (AlarmView::*callBack_a)(void);
};

class Label {
public:
	Label(UString text, const Coord & pos, sf::Font *font, sf::Color color, int fontSize = 16);

	Label(UString text, uint x, uint y, sf::Font *font, sf::Color color, int fontSize = 16);

	void show(sf::RenderWindow *);

	void hidden(bool h) { _hidden = h; }

	bool hidden() { return _hidden; }

	void text(UString text) { _text = text; }

	const UString & text() { return _text; }

	const Coord & pos() { return _pos; }

	void pos(const Coord & pos) { _pos = pos; }

	void pos(uint x, uint y, bool xmid = false, bool ymid = false) {
		_pos.x = x;
		_pos.y = y;
		xm = xmid;
		ym = ymid;
	}

	void setFont(sf::Font *f) { _font = f; }

	void setBackground(sf::Color background){ _bckgnd = background; }

	int pixsize;
protected:
	bool xm,ym;
	int _size;
	bool _hidden;
	UString _text;
	Coord _pos;
	sf::Color _color,_bckgnd;
	sf::Font *_font;
};

#endif