#include "widget.h"

Button::Button(sf::Sprite *img, int x, int y){
	this->callBack_a = 0;
	this->caller_a = 0;
	this->pos.x = x;
	this->pos.y = y;
	this->image = img;
	visible = true;
}

Button::Button(sf::Sprite *img,Coord pos){
	this->callBack_a = 0;
	this->caller_a = 0;
	this->pos = pos;
	this->image = img;
	visible = true;
}

void Button::show(sf::RenderWindow *screen){
	if(visible){
		image->SetPosition(pos.x, pos.y);
		screen->Draw(*image);
	}
}

void Button::clicked(int posx, int posy){
	if(visible)
		if(pos.x <= posx && pos.x+image->GetSize().x > posx && pos.y <= posy && pos.y+image->GetSize().y > posy){
			if(caller_a != 0 && callBack_a != 0)
				(caller_a->*callBack_a)();
		}
}

void Button::clicked(Coord c){
	if(visible)
		if(pos.x <= c.x && pos.x+image->GetSize().x > c.x && pos.y <= c.y && pos.y+image->GetSize().y > c.y){
			if(caller_a != 0 && callBack_a != 0)
				(caller_a->*callBack_a)();
		}
}

Label::Label(UString text, const Coord & pos, sf::Font *font, sf::Color color, int fontSize) {
	_font = font;
	_text = text;
	_pos = pos;
	_color = color;
	_hidden = false;
	_size = fontSize;
	_bckgnd = sf::Color(0,0,0,0);
	pixsize = 4;
	xm = false;
	ym = false;
}

Label::Label(UString text, uint x, uint y, sf::Font *font, sf::Color color, int fontSize) {
	_font = font;
	_text = text;
	_pos.x = x;
	_pos.y = y;
	_color = color;
	_hidden = false;
	_size = fontSize;
	_bckgnd = sf::Color(0,0,0,0);
	pixsize = 4;
	xm = false;
	ym = false;
}

void Label::show(sf::RenderWindow *surface){
	if (!_hidden) {
		sf::String img(_text.c_str(), *_font);
		img.SetSize(_size);
		img.SetColor(_color);
		img.SetPosition(_pos.x, _pos.y);

		/*sf::FloatRect backgroundRect = img.GetLocalBounds();

		float posx = _pos.x, posy = _pos.y;

		if(xm){
			posx = _pos.x - backgroundRect.GetWidth * 0.5;
		}
		if(ym){
			posy = _pos.y - backgroundRect.GetHeight * 0.5;
		}

		img.SetPosition(posx, posy);

		sf::RectangleShape background(sf::Vector2f(backgroundRect.GetWidth+1, backgroundRect.GetHeight+pixsize));
		background.SetFillColor(_bckgnd);

		surface->Draw(background, img.GetTransform());*/
		surface->Draw(img);
	}
}