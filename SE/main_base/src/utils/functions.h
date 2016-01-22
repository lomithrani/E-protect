#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

typedef unsigned int uint;
typedef unsigned long int uint64;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned char uchar;
typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef long int int64;

using namespace std;

template<class T>
string toStr(T number) {
	stringstream ss;
	ss << number;
	return ss.str();
}

class Vector {
public:
	Vector(double x = 0.f, double y = 0.f) {
		this->x = x;
		this->y = y;
	}
	Vector(const Vector & v) {
		this->x = v.x;
		this->y = v.y;
	}

	double x, y;

	const Vector & operator=(const Vector & v) {
		this->x = v.x;
		this->y = v.y;
		return *this;
	}

	bool operator!=(const Vector & v) {
		if (this->x != v.x || this->y != v.y)
			return true;
		else
			return false;
	}

	bool operator==(const Vector & v) {
		if (this->x == v.x && this->y == v.y)
			return true;
		else
			return false;
	}

	const Vector & operator+=(const Vector & v) {
		this->x += v.x;
		this->y += v.y;
		return (*this);
	}

	const Vector & operator-=(const Vector & v) {
		this->x -= v.x;
		this->y -= v.y;
		return (*this);
	}

	const Vector & operator*=(double a) {
		this->x *= a;
		this->y *= a;
		return *this;
	}

	/*const Vector & operator*(double a){
	 Vector p(*this);
	 p *= a;
	 return p;
	 }*/

	const Vector & operator=(sf::Vector2i v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	const Vector & operator=(sf::Vector2f v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	/*const Vector & operator=(sf::Vector2u v) {
		x = v.x;
		y = v.y;
		return *this;
	}*/
};

class Coord: public Vector {
public:
	Coord(double x = 0.f, double y = 0.f) :
			Vector(x, y) {

	}

	Coord(sf::Vector2i v) {
		x = v.x;
		y = v.y;
	}

	static Coord __ret;
	static Coord & Rand(int radius) {
		__ret.x = rand() % (radius * 2) - radius;
		__ret.y = rand() % (radius * 2) - radius;
		return __ret;
	}

	const Coord & operator+(const Coord & v) {
		__ret.x = this->x + v.x;
		__ret.y = this->y + v.y;
		return __ret;
	}
};

template<class T>
class SList: public vector<T> {
public:
	SList() :
			vector<T>() {
	}

	SList operator<<(T _item) {
		this->push_back(_item);
		return (*this);
	}

	void remove(int i) {
		//T _tmp = this->at(i);
		this->erase(this->begin() + i);
		//return _tmp;
	}

	T removeLast() {
		T _tmp = (*this->end());
		this->erase(this->end());
		return _tmp;
	}

	T removeFirst() {
		T _tmp = (*this->begin());
		this->erase(this->begin());
		return _tmp;
	}

	SList operator>>(T _item) {
		_item = removeLast();
		return (*this);
	}
};

class UString : public std::string{
public:
	UString();
	UString(const std::string &);
	UString(const UString &);
	UString(const char *);

	static UString number(unsigned int);
	static UString number(int);
	static UString number(long int);
	static UString number(unsigned long int);
	static UString number(float);
	static UString number(double);

	UString replace(UString before, UString after);
	UString replace(char before, char after);

	UString operator=(const UString &);

	UString left(unsigned int);
	UString right(unsigned int);

	UString operator<<(unsigned int);
	UString operator<<(int);
	UString operator<<(long int);
	UString operator<<(unsigned long int);
	UString operator<<(float);
	UString operator<<(double);
	UString operator<<(const UString &);
	UString operator<<(const string &);
	UString operator<<(const char *);
	UString operator<<(char);

	UString jsonData(const UString &);

//	UString operator+=(unsigned int);
//	UString operator+=(int);
//	UString operator+=(long int);
//	UString operator+=(unsigned long int);
//	UString operator+=(float);
//	UString operator+=(double);
//	UString operator+=(const UString &);
//	UString operator+=(const string &);
//	UString operator+=(const char *);
//	UString operator+=(char);

//	UString operator+(unsigned int);
//	UString operator+(int);
//	UString operator+(long int);
//	UString operator+(unsigned long int);
//	UString operator+(float);
//	UString operator+(double);
//	UString operator+(const UString &);
//	UString operator+(const string &);
//	UString operator+(const char *);
//	UString operator+(char);

private:
	template<class T>
	static string __n(T number){
		stringstream ss;
		ss << number;
		return ss.str();
	}
};

sf::Image *loadImg(string file);
sf::Sprite *loadHWImg(string file, float scale = 1.0, bool originCenter = false);

void drawLine(sf::RenderWindow *screen, const Coord & start, const Coord & end, const sf::Color & color);
void drawFillRect(sf::RenderWindow *screen, const Coord & start, const Coord & end, const sf::Color & color);
void drawRect(sf::RenderWindow *screen, const Coord & start, const Coord & end, const sf::Color & color);
void drawPixel(sf::RenderWindow *screen, const Coord & pos, const sf::Color & color);

extern sf::Clock cl;

void loadFonts();

extern sf::Font *font;

#endif
