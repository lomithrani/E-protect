#ifndef UART_COM_H
#define UART_COM_H

extern"C"{
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <netdb.h>
	#include <string.h>
	#include <sys/ioctl.h>
}
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

#include <wiringSerial.h>
#include "utils/thread.h"
#include "utils/functions.h"
#include "sensors.h"

class AlarmView;

class UartThread : public Thread{
public:
	UartThread(AlarmView *);
	~UartThread();

	void run();

	UString getLine();

	bool running;

	bool updatedData;

	sf::SocketTCP socketClient;

	void socket_send(UString);
	UString socket_get();

	void socketConnect(bool force = false);

	void processSocket(UString);
	void processLine(UString);

private:
	int portUART;
	int socketErrorCount;
	UString socketData;

	AlarmView *av;

	sf::Clock lastTry, pingTime;
	bool pingDone;
	bool socketConnected;
};

#endif