#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h> 
#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <Commctrl.h>

using namespace std;

#define SOME_SIZE 256
#define PORT "666"

class TCPServer
{
public:
	TCPServer(WORD version) :result_(NULL), Connect_(0){
		WSADATA data;
		version_ = version;
		if (WSAStartup(version_, &data)){						// cтарт использования библиотеки сокетов процессом
			cout << "Erorr WSAStartn" << (WSAGetLastError());
			return;
		}
		ZeroMemory(&hints_, sizeof(hints_));					// заполненин нулями
		Connections_ = new SOCKET[64];
		hints_.ai_family = AF_INET;								// тип сокета  (Internet протоколы)
		hints_.ai_flags = AI_PASSIVE;							// Сокет биндится на адрес, чтобы принимать входящие соединения
		hints_.ai_socktype = SOCK_STREAM;						// установлением соединения;
		hints_.ai_protocol = IPPROTO_TCP;
		getaddrinfo(NULL, PORT, &hints_, &result_);				// инициализация хранящую адрес сокета - addr

		Listen_ = socket(result_->ai_family, result_->ai_socktype, result_->ai_protocol); // Создадим сокет
		bind(Listen_, result_->ai_addr, result_->ai_addrlen);	// Для связывания сокета с адресом и номером порт
																//Listen Подготовим сокет к принятию входящих соединений от клиенто
		listen(Listen_, SOMAXCONN);								//  константе SOMAXCONN хранится максимально возможное число одновременных TCP-соединений

		freeaddrinfo(result_);									// освобождает вызов 
	}
	TCPServer(){
		//delete Connections_;
	}
	void startServer(){
		cout << "Start server....";
		char m_connect[] = "Connect....";
		for (;;){

			if (Connect_ = accept(Listen_, NULL, NULL)){		//  accept ожидает запрос на установку TCP-соединения от удаленного хост

				std::cout << "\nClient connect...\n";
				Connections_[ClientCount_] = Connect_;
				send(Connections_[ClientCount_], m_connect, strlen(m_connect), NULL); // Отправляем ответ клиенту с помощью функции send

				ClientCount_++;
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)sendMessageToClient, (LPVOID)(ClientCount_ - 1), NULL, NULL);

			}

		}
	}

	static void sendMessageToClient(int ID)
	{
		int  nselect = 0;
		TCPServer tCPServer;
		FD_SET ReadSet;
		string receivedData;
		for (;;) {

			FD_ZERO(&ReadSet);
			FD_SET(Connections_[ID], &ReadSet);
			nselect = select(NULL, &ReadSet, NULL, NULL, NULL);
			if (nselect == SOCKET_ERROR){
				cout << "SOCKET_ERROR " << endl;
				return;
			}

			if (!nselect){
				continue;
			}
			receivedData = tCPServer.receiveMessageToClient(ID);
			if (FD_ISSET(Connections_[ID], &ReadSet)) {
				if (strcmp("Disconnection", receivedData.data())) {
					for (auto ch : receivedData)
						cout << ch;
					cout << "\n";
				}
				if (!strcmp("Sendfile", receivedData.data())) {
					if (tCPServer.acceptFileToClient(ID)) {
						tCPServer.sendFileToClient(ID);
					}
				}


				for (int i = 0; i < ClientCount_; i++) {
					if (send(Connections_[i], receivedData.data(), receivedData.size(), NULL) == SOCKET_ERROR){
						cout << "\nDisconnection" << endl;
						ClientCount_--;
						return;
					}

				}
			}


		}
	}

	string receiveMessageToClient(int ID)
	{
		char buffer[SOME_SIZE];
		int bytesRead = 0;
		string receivedData;


		FD_SET ReadSet;
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 5000;

		do{
			FD_ZERO(&ReadSet);
			FD_SET(Connections_[ID], &ReadSet);
			int rv = select(0, &ReadSet, NULL, NULL, &tv);
			if (rv == SOCKET_ERROR){
				cout << "SOCKET_ERROR .. \n";
				break;
			}

			if (!rv){
				bytesRead = 0;
			}

			if (FD_ISSET(Connections_[ID], &ReadSet))
			{
				bytesRead = recv(Connections_[ID], buffer, SOME_SIZE - 2, 0);
				if (bytesRead == SOCKET_ERROR)
				{
					return "Disconnection";
				}
				buffer[bytesRead] = '\0';
				for (int i = 0; i < bytesRead; i++)
					receivedData += buffer[i];
			}


		} while (bytesRead != 0 && bytesRead != SOCKET_ERROR);


		return receivedData;
	}

	bool acceptFileToClient(int ID) {

		inFile_.resize(SOME_SIZE);
		int sizeFileName = recv(Connections_[ID], inFile_.data(), SOME_SIZE - 2, NULL); // получаем имя файла
		if (sizeFileName > (SOME_SIZE - 2)){
			cout << "File size is too large!" << endl;
			return false;
		}
		inFile_.resize(sizeFileName + 1);
		cout << "Name file: " << inFile_.data() << endl;

		send(Connections_[ID], "OK", strlen("OK"), NULL);		  // отправляем ответ, что все принято хорошо

		FILE * fw = fopen(inFile_.data(), "wb");
		//char rec[50] = "";
		vector <char> rec(SOME_SIZE);

		int recSize = recv(Connections_[ID], rec.data(), SOME_SIZE - 2, 0);
		cout << "			 rec.data()" << rec.data() << endl;
		send(Connections_[ID], "OK", strlen("OK"), 0);
		if (recSize > (SOME_SIZE - 2)){
			cout << "File size is too large!" << endl;
			return false;
		}
		//rec[recSize + 1] = '\0';
		rec.resize(recSize);


		int size = atoi(rec.data());
		cout << "Size file: " << size << endl;
		while (size > 0){
			char buffer[1030];

			if (size >= 1024){
				recv(Connections_[ID], buffer, 1024, 0);
				send(Connections_[ID], "OK", strlen("OK"), 0);
				fwrite(buffer, 1024, 1, fw);
			}
			else {
				recv(Connections_[ID], buffer, size, 0);
				send(Connections_[ID], "OK", strlen("OK"), 0);
				buffer[size] = '\0';
				fwrite(buffer, size, 1, fw);
			}
			size -= 1024;
		}
		fclose(fw);

		cout << "File accpet " << endl;
		
		return true;
	}

	bool sendFileToClient(int ID){
		//string str = "AcceptFile";
		//	if (send(Connections[ID], str.data(), str.size(), NULL) != SOCKET_ERROR){ // отправка команды
		//		cout << "Comanda " << str.data() << endl;
		//	}
		cout << ("Name file C<=S: ") << inFile_.data() << endl;
		std::ifstream myFile(inFile_.data(), std::ios::in | std::ios::binary | std::ios::ate);
		if (!myFile.is_open()){
			cout << "Cannot open file" << endl;
			return false;
		}
		int size = (int)myFile.tellg();
		myFile.close();
		cout << "Size file: " << size << endl;
		char filesize[10];
		itoa(size, filesize, 10);

		if (send(Connections_[ID], inFile_.data(), inFile_.size(), NULL) != SOCKET_ERROR){ // отправка имени файла

			cout << "Name file " << inFile_.data() << endl;
		}
		char rec[32];
		memset(rec, 0, 32);
		recv(Connections_[ID], rec, 32, 0);									// получаем ответ о состоянии получения
		cout << "Answer " << rec << endl;

		send(Connections_[ID], filesize, strlen(filesize), 0);				// отправляем информацию о размере файла
		recv(Connections_[ID], rec, 32, 0);									// получаем ответ о статусе отправки
		cout << "Answer " << rec << endl;

		FILE * fp = NULL;
		if ((fp = fopen(inFile_.data(), "rb")) == NULL){
			cout << "Cannot open file" << endl;
			return false;
		}
		
		while (size > 0){
			char buffer[1030];

			if (size >= 1024){
				fread(buffer, 1024, 1, fp);
				send(Connections_[ID], buffer, 1024, 0);
				recv(Connections_[ID], rec, 32, 0);
			}
			else {
				fread(buffer, size, 1, fp);
				buffer[size] = '\0';
				send(Connections_[ID], buffer, size, 0);
				recv(Connections_[ID], rec, 32, 0);
			}

			size -= 1024;
		}

		fclose(fp);
		inFile_.clear();
		return true;
	}



	~TCPServer(){
		//delete Connections_;
		//Connections_ = NULL;
	}
private:
	WORD version_;												// версия сокетов
	struct addrinfo *result_;
	struct addrinfo hints_;										// настройки сокета 
	SOCKET Connect_;
	static SOCKET *Connections_;
	SOCKET Listen_;
	static int ClientCount_;
	vector<char> inFile_;
};

unsigned int* TCPServer::Connections_ = NULL;
int TCPServer::ClientCount_ = 0;