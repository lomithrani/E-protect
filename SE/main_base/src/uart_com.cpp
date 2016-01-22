#include "uart_com.h"

#include <iostream>

#include "alarm_view.h"

#define SERVER_HOST "62.210.99.62"
#define SERVER_PORT 8124

/* REDIR PORT COMMAND: ssh -L 8124:localhost:8124 valentis@gandalf.ece.fr */

UartThread::UartThread(AlarmView *av){
	running = true;
	updatedData = false;

	this->av = av;

}


UartThread::~UartThread(){
	serialClose(portUART);
}

void UartThread::run(){
	portUART = serialOpen("/dev/ttyAMA0", 19200);
	socketErrorCount = 0;
	lastTry.Reset();
	socketConnected = false;
	pingDone = true;
	pingTime.Reset();

	if(portUART == -1){
		running = false;
		std::cout << "Error with uart" << std::endl;
		return;
	}

	socketConnect(true);
	//socket_get();

	//unsigned long prevTime = clock();

	while(running){
		UString line = getLine();
		UString socketGet = socket_get();

		if(line.size() > 0 || socketGet.size() > 0){
			processLine(line);
			processSocket(socketGet);
		}else usleep(50000);

		/*if(abs(clock() - prevTime) > 2000 && socketData.size() > 0){
			prevTime = clock();
			socket_send(socketData);
			socketData = "";
		}*/

		if(!socketClient.IsValid())
			socketConnected = false;

		if(pingDone){
			if(pingTime.GetElapsedTime() > 5.f){
				pingTime.Reset();
				pingDone = false;
				std::cout << "ping server" << std::endl;
				socket_send("PING\r\n");
			}
		}else{
			if(pingTime.GetElapsedTime() > 2.f){
				socketConnected = false;
				pingDone = true;
				pingTime.Reset();
				socketClient.Close();
				std::cout << "ping timed out" << std::endl;
			}
		}

		if(!socketConnected)
			socketConnect();
	}
}

void UartThread::processSocket(UString socketGet){
	cout << "SOCKET: " << socketGet << endl;
	if(socketGet.size() > 0){
		if(socketGet.left(4) == "PING" || socketGet.left(4) == "ping"){
			socket_send(UString("PONG\r\n"));
		}else if(socketGet.left(4) == "PONG" || socketGet.left(4) == "pong"){
			if(!pingDone){
				pingDone = true;
				//std::cout << "PONG responded after: " << (clock()-pingTime) << "ms" << std::endl; 
				pingTime.Reset();
			}
		}else{
			char cmd = socketGet.jsonData("cmd")[0];
			int id;
			UString key;
			switch(cmd){
				case 'n':
					id = atoi(socketGet.jsonData("id").c_str());
					if(sensorsList[id]->type == SENSOR_NONE)
						sensorsList[id]->name = socketGet.jsonData("name");
					break;
				default:
				break;
			}
		}
	}
}

void UartThread::processLine(UString line){
	if(line.size() > 0){
		char cmd = line.jsonData("cmd")[0];
		cout << "UART: " << line << endl;
		int id;
		UString key;
		switch(cmd){
			case 'g':
				id = atoi(line.jsonData("id").c_str());

				if(sensorsList[id]->status != atoi(line.jsonData("st").c_str()))
					updatedData = true;

				sensorsList[id]->status = atoi(line.jsonData("st").c_str());
				if(sensorsList[id]->type == SENSOR_NONE)
					sensorsList[id]->type = atoi(line.jsonData("ty").c_str());
				
				socket_send(line);

				break;
			case 'c':
				key = line.jsonData("key");

				cout << key << endl;
				break;

			case 'l':
				av->enableAlarm();
				socket_send(line);
				break;
			case 'u':
				av->disableAlarm();
				socket_send(line);
				break;
			default:
				break;
		}
	}
}

UString UartThread::getLine(){
	UString line;
	char c;
	if(serialDataAvail(portUART)){
		do{
			while(!serialDataAvail(portUART)) usleep(10);
			c = serialGetchar(portUART);
			line << c;
		}while(c != '\n');
	}
	return line;
}

void UartThread::socket_send(UString data){
	if(!socketConnected) return;
	int status;
	socketClient.SetBlocking(true);
	if((status = socketClient.Send(data.c_str(), data.size())) != sf::Socket::Done){
	    if(status == sf::Socket::Disconnected || status == sf::Socket::Error){
	    	socketConnected = false;
			socketConnect();
		}else if(status == sf::Socket::NotReady){

		}
	}else{

	}
	socketClient.SetBlocking(false);
}

UString UartThread::socket_get(){
	UString data;
	if(!socketConnected) return data;
	char buffer[1024];
	int status;
	std::size_t Received;
	if((status = socketClient.Receive(buffer, sizeof(buffer), Received)) != sf::Socket::Done){
	    if(status == sf::Socket::Disconnected || status == sf::Socket::Error){
	    	socketConnected = false;
			socketConnect();
		}else if(status == sf::Socket::NotReady){
			
		}
	}else{
		data << buffer;
	}
	return data;
}


void UartThread::socketConnect(bool force){
	if(lastTry.GetElapsedTime() > 10.f || force){
		lastTry.Reset();
		if(socketClient.Connect(SERVER_PORT, SERVER_HOST) != sf::Socket::Done){
			socketConnected = false;
			socketErrorCount++;
			if(socketErrorCount == 2){
				/* RAISE ERROR INTERNET FLAG */
				socketErrorCount = 0;
			}
		}else{
			socketConnected = true;
			socketErrorCount = 0;
			/* LOWER ERROR INTERNET FLAG */
			socket_send("{\"cmd\":\"connect\",\"sigfox\":\"123456789\"}");
			socketClient.SetBlocking(false);
		}
	}
}