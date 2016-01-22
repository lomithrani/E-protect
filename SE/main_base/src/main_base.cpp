
#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

#include "utils/functions.h"
#include "alarm_view.h"

/* 
 * 800x480: screen size
 * /c/Program\ Files\ \(x86\)/Xming/Xming.exe :1 -query 10.4.133.94 -clipboard -screen 0 800x480+100+100@1 
 */

int main(int argc, char **argv){
	loadFonts();
	sf::WindowSettings sets;
	sets.AntialiasingLevel = 2;
	sf::VideoMode mode;

	mode.BitsPerPixel = 16;
	mode.Width = sf::VideoMode::GetDesktopMode().Width;
	mode.Height = sf::VideoMode::GetDesktopMode().Height;

	//for(int i=0;i<sf::VideoMode::GetModesCount();i++)
	//	cout << "w: " << sf::VideoMode::GetMode(i).Width << " - h: " << sf::VideoMode::GetMode(i).Height << endl;

	sf::RenderWindow *screen = new sf::RenderWindow(mode, std::string("E-protect"), sf::Style::Fullscreen, sets);
	screen->ShowMouseCursor(true);

	std::cout << "Video mode:\n\tWidth: " << mode.Width << "\n\tHeight: " << mode.Height
				<< "\n\tBits pp:" << mode.BitsPerPixel << endl;

	AlarmView av(screen);

	av.run();
	
	return 0;
}