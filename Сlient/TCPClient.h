#pragma once
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h> 
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>
#include <Winuser.h>
#include <conio.h>
using namespace std;

#define PORT 666
#define SERVERADDR "127.0.0.1"
#define SOME_SIZE 256


class  TCPClient
{
public:
	TCPClient(WORD version){
		WSADATA data;
		version_ = version;
		cout << "TCP Client\n";
		if (WSAStartup(version, &data)){
			cout << "Erorr WSAStartn" << (WSAGetLastError());
			_getch();
			return;
		}
		my_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (my_sock_ < 0)
		{
			cout << "Socket() error \n" << (WSAGetLastError()) << endl;
			_getch();
			return;
		}
		hos_addr_.sin_family = AF_INET;
		hos_addr_.sin_port = htons(PORT);
		hos_addr_.sin_addr.s_addr = inet_addr(SERVERADDR);

		if (connect(my_sock_, (sockaddr *)&hos_addr_, sizeof(hos_addr_)))
		{
			cout << "Connect error " << (WSAGetLastError()) << endl;
			_getch();
			return;
		}
		cout << "Connection established " << SERVERADDR;
	}

	void receiveSendMessageToServer(){
		int nsize = 0;
		for (;; Sleep(75)){
			str_ = acceptMessageToServer();
			if (nsize != SOCKET_ERROR){
				cout << ("S=>C: ");
				for (auto ch : str_)
					cout << ch;
				cout << endl;
				cout << ("S<=C: ");
				str_.clear();
				cin >> str_;

				if (!strcmp(str_.data(), "quit")){
					cout << ("Exit...");
					closesocket(my_sock_);
					WSACleanup();
					return;
				}
				if (!strcmp(str_.data(), "Sendfile")){
					if(sendFileToServer())
						acceptFileToClient();
				}
			}
			send(my_sock_, str_.data(), str_.size(), NULL); // для отправки сообщения
		}
	}

	string acceptMessageToServer() {

		char buffer[SOME_SIZE];
		int bytesRead = 0;
		string receivedData;

		FD_SET ReadSet;
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 5000;


		do{
			FD_ZERO(&ReadSet);
			FD_SET(my_sock_, &ReadSet);
			int rv = select(0, &ReadSet, NULL, NULL, &tv);
			if (rv == SOCKET_ERROR){
				cout << "SOCKET_ERROR .. \n";
				break;
			}

			if (!rv){
				bytesRead = 0;
			}

			if (FD_ISSET(my_sock_, &ReadSet))
			{
				bytesRead = recv(my_sock_, buffer, SOME_SIZE - 2, 0);
				if (bytesRead == SOCKET_ERROR){
					break;
				}
				buffer[bytesRead] = '\0';
				for (int i = 0; i < bytesRead; i++)
					receivedData += buffer[i];
			}


		} while (bytesRead != 0 && bytesRead != SOCKET_ERROR);

		return receivedData;
	}

	bool sendFileToServer() {

		if (send(my_sock_, str_.data(), str_.size(), NULL) != SOCKET_ERROR){ // отправка команды
			cout << "Сommand file " << str_.data() << endl;
		}

		string nameFile;
		cout << ("Name file S<=C: ");  cin >> nameFile;

		ifstream myFile(nameFile.data(), std::ios::in | std::ios::binary | std::ios::ate);
		if (!myFile.is_open()){
			cout << "Cannot open file" << endl;
			return false;
		}
		int size = (int)myFile.tellg();
		myFile.close();

		cout << "Size file elements: " << size << endl;
		char fileSize[10];
		itoa(size, fileSize, 10);
		
		if (send(my_sock_, nameFile.data(), nameFile.size(), NULL) != SOCKET_ERROR){ // отправка имени файла

			//SetDlgItemText(hDlg, IDC_LBL_STATUS, (LPCTSTR)str.data());
			cout << "Name file sended: " << nameFile.data() << endl;
		}
		char rec[32] = "";
		recv(my_sock_, rec, 32, 0); // получаем ответ о состоянии получения

		cout << "Answer: " << rec << endl;
		cout << "               fileSize " << fileSize << endl;
		send(my_sock_, fileSize, strlen(fileSize), 0); // отправляем информацию о размере файла
		recv(my_sock_, rec, 32, 0); // получаем ответ о статусе отправки
		cout << "Answer " << rec << endl;

	
		FILE * fp = NULL;
		if ((fp = fopen(nameFile.data(), "rb")) == NULL){
			cout << "Cannot open file" << endl;
			return false;
		}

		while (size > 0){
			char buffer[1030];

			if (size >= 1024){
				fread(buffer, 1024, 1, fp);
				send(my_sock_, buffer, 1024, 0);
				recv(my_sock_, rec, 32, 0);
			}
			else {
				fread(buffer, size, 1, fp);
				buffer[size] = '\0';
				send(my_sock_, buffer, size, 0);
				recv(my_sock_, rec, 32, 0);
			}

			size -= 1024;
		}

		fclose(fp);

		return true;
	}

	bool acceptFileToClient(){

	
		char inFile[80]; //  файл
		memset(inFile, 0, sizeof(inFile)); //Clear the buffer
		recv(my_sock_, inFile, sizeof(inFile)-1, NULL); // получаем имя файла
		cout << "Accepting name file: " << inFile << endl;

		send(my_sock_, "OK", strlen("OK"), NULL); // отправляем ответ, что все принято хорошо

		FILE * fw = fopen(inFile, "wb");
		char rec[50] = "";

		int recSize = recv(my_sock_, rec, 32, 0);
		send(my_sock_, "OK", strlen("OK"), 0);

		rec[recSize + 1] = '\0';
		int size = atoi(rec);
		cout << "Size file: " << size << endl;
		while (size > 0){
			char buffer[1030];

			if (size >= 1024){
				recv(my_sock_, buffer, 1024, 0);
				send(my_sock_, "OK", strlen("OK"), 0);
				fwrite(buffer, 1024, 1, fw);
			}
			else {
				recv(my_sock_, buffer, size, 0);
				send(my_sock_, "OK", strlen("OK"), 0);
				buffer[size] = '\0';
				fwrite(buffer, size, 1, fw);
			}
			size -= 1024;
		}
		fclose(fw);

		cout << "File accpet " << endl;
		str_.clear();
		return true;
	}

	~TCPClient(){
		cout << "Recv error " << (WSAGetLastError()) << endl;
		closesocket(my_sock_);
		str_.clear();
		WSACleanup();
	}

private:

	WORD version_;
	SOCKET my_sock_;
	sockaddr_in hos_addr_;
	string str_;

};
