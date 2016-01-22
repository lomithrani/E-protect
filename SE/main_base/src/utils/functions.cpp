#include "functions.h"

sf::Font *font;



void loadFonts() {
	font = new sf::Font;

	font->LoadFromFile("./data/sfd/FreeSans.ttf");
}


sf::Image *loadImg(string file){
	sf::Image *img = new sf::Image();
	if(!img->LoadFromFile(file))
		cout << "error loading: " << file << endl;
	//img.SetSmooth(false);
	return img;
}

sf::Sprite *loadHWImg(string file, float scale, bool originCenter) {
	sf::Image *img = new sf::Image();
	img->LoadFromFile(file);
	sf::Sprite *sprt = new sf::Sprite(*img);
	//sprt->SetSubRect(sf::IntRect(0,0,sprt->GetSize().x, sprt->GetSize().y));
	//if (originCenter)
	//	sprt->SetCenter(sprt->GetSize().x / 2, sprt->GetSize().y / 2);

	sprt->Scale(scale, scale);
	//sprt->SetBlendMode(sf::Blend::None);
	return sprt;
}

void drawLine(sf::RenderWindow *screen, const Coord & start, const Coord & end, const sf::Color & color) {
	//sf::Vertex line[2] = { sf::Vertex(sf::Vector2f(start.x, start.y), color), sf::Vertex(sf::Vector2f(end.x, end.y), color) };
	sf::Shape line = sf::Shape::Line(start.x, start.y, end.x, end.y, 1, color);
	screen->Draw(line);
}

void drawFillRect(sf::RenderWindow *screen, const Coord & start, const Coord & end, const sf::Color & color) {
	sf::Shape rect = sf::Shape::Rectangle(start.x, start.y, end.x, end.y, color);
	screen->Draw(rect);
}

void drawRect(sf::RenderWindow *screen, const Coord & start, const Coord & end, const sf::Color & color) {
	drawLine(screen, Coord(start.x, start.y), Coord(start.x, end.y), color);
	drawLine(screen, Coord(start.x, start.y), Coord(end.x, start.y), color);
	drawLine(screen, Coord(end.x, start.y), Coord(end.x, end.y), color);
	drawLine(screen, Coord(start.x, end.y), Coord(end.x, end.y), color);
}

void drawPixel(sf::RenderWindow *screen, const Coord & pos, const sf::Color & color) {
	drawRect(screen, pos, pos, color);
}

UString::UString(){

}

UString::UString(const string & str) : std::string(str){

}

UString::UString(const UString & str) : std::string((const std::string)str){

}

UString::UString(const char * str) : std::string(str){

}

UString UString::replace(UString before, UString after){
	bool rep = false;
	size_t i=0, j=0;

	for(i=0;i<size();i++){
		for(j=0;j<before.size();j++){
			if(i+j<size()){
				if((*this)[i+j] == before[j]){
					rep = true;
				}else{rep = false; break;}
			}else{rep = false; break;}
		}
		if(rep) break;
	}

	if(rep){
		std::string::replace(i,before.size(),(std::string)after);
	}

	return *this;
}

UString UString::replace(char before, char after){

	for(size_t i=0;i<size();i++)
		if((*this)[i]==before) std::string::replace(i,1,1,after);

	return *this;
}

UString UString::operator= (const UString & str){
	this->std::string::clear();
	this->std::string::append((std::string)str);
	return (*this);
}

UString UString::left(unsigned int l){
	UString ret;

	for(unsigned int i=0;i<l;i++)
		if(i<size()) ret += (*this)[i];

	return ret;
}

UString UString::right(unsigned int l){
	UString ret;

	unsigned int start = 0;

	if(size()-l<0)
		start = 0;
	else start = size()-l;

	for(unsigned int i=start;i<size();i++)
		if(i<size()) ret += (*this)[i];

	return ret;
}

UString UString::number(unsigned int num){return __n<unsigned int>(num);}
UString UString::number(int num){return __n<int>(num);}
UString UString::number(long int num){return __n<long int>(num);}
UString UString::number(unsigned long int num){return __n<unsigned long int>(num);}
UString UString::number(float num){return __n<float>(num);}
UString UString::number(double num){return __n<double>(num);}

UString UString::operator<<(unsigned int num){this->std::string::append((std::string)__n<unsigned int>(num)); return (*this);}
UString UString::operator<<(int num){this->std::string::append((std::string)__n<int>(num)); return (*this);}
UString UString::operator<<(long int num){this->std::string::append((std::string)__n<long int>(num)); return (*this);}
UString UString::operator<<(unsigned long int num){this->std::string::append((std::string)__n<unsigned long int>(num)); return (*this);}
UString UString::operator<<(float num){this->std::string::append((std::string)__n<float>(num)); return (*this);}
UString UString::operator<<(double num){this->std::string::append((std::string)__n<double>(num)); return (*this);}
UString UString::operator<<(const UString & str){this->std::string::append((std::string)str); return (*this);}
UString UString::operator<<(const string & str){this->std::string::append(str); return (*this);}
UString UString::operator<<(const char * str){this->std::string::append(str); return (*this);}
UString UString::operator<<(char c){this->std::string::append(1,c); return (*this);}

//UString UString::operator+=(unsigned int num){this->std::string::append((std::string)__n<unsigned int>(num)); return (*this);}
//UString UString::operator+=(int num){this->std::string::append((std::string)__n<int>(num)); return (*this);}
//UString UString::operator+=(long int num){this->std::string::append((std::string)__n<long int>(num)); return (*this);}
//UString UString::operator+=(unsigned long int num){this->std::string::append((std::string)__n<unsigned long int>(num)); return (*this);}
//UString UString::operator+=(float num){this->std::string::append((std::string)__n<float>(num)); return (*this);}
//UString UString::operator+=(double num){this->std::string::append((std::string)__n<double>(num)); return (*this);}
//UString UString::operator+=(const UString & str){(*this)+=str; return (*this);}
//UString UString::operator+=(const string & str){}
//UString UString::operator+=(const char * str){}
//UString UString::operator+=(char c){}

//UString UString::operator+(unsigned int num){UString r(*this); r+=num; return r;}
//UString UString::operator+(int num){UString r(*this); r+=num; return r;}
//UString UString::operator+(long int num){UString r(*this); r+=num; return r;}
//UString UString::operator+(unsigned long int num){UString r(*this); r+=num; return r;}
//UString UString::operator+(float num){UString r(*this); r+=num; return r;}
//UString UString::operator+(double num){UString r(*this); r+=num; return r;}
//UString UString::operator+(const UString & str){UString r(*this); r+=(std::string)str; return r;}
//UString UString::operator+(const string & str){UString r(*this); r+=str; return r;}
//UString UString::operator+(const char * str){UString r(*this); r+=str; return r;}
//UString UString::operator+(char c){UString r(*this); r+=c; return r;}


UString UString::jsonData(const UString & key){
  char rep = 0;
  int i=0, j=0, dataLen = size(), keyLen = key.size();
  UString __parsed;

  for(i=0;i<dataLen;i++){
    for(j=0;j<keyLen;j++){
      if(i+j<dataLen && (*this)[i+j] == key[j]){rep = 1;}else{rep = 0; break;}
    } if(rep){j += i; break;}
  }
  if(rep){
    while((*this)[j] != ':' && (*this)[j] != 0) j++; while((*this)[j] != '"' && (*this)[j] != 0) j++; j++; i=0;
    while((*this)[j] != '"' && (*this)[j] != 0 && i < (42))__parsed << (*this)[j++];
  }
  return __parsed;
}