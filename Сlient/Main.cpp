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
#include "TCPClient.h"
using namespace std;


//#define PORT 666
//#define SERVERADDR "127.0.0.1"
//#define SOME_SIZE 256
//#define IDC_LBL_STATUS 1002




int main(int argc, char* argv[])
{
	TCPClient * tCPClient = new TCPClient(MAKEWORD(2, 0));
	tCPClient->receiveSendMessageToServer();


	delete tCPClient;
	return 0;
}






































//string acceptMessageToServer(SOCKET sock);
//bool acceptFileToClient(SOCKET sock);

//int main(int argc, char* argv[])
//{
//	setlocale(LC_ALL, "RUS");
//
//	cout << ("TCP клиент\n");
//	WSADATA data;								//данные о сокете
//	WORD version = MAKEWORD(2, 0);				// запрашиваемая версия винсок и  задаем версию
//	
//
//	if (WSAStartup(version, &data))
//	{
//		cout << "Ошибка WSAStartn" << (WSAGetLastError());
//		_getch();
//		return -1;
//	}
//
//	SOCKET my_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (my_sock < 0)
//	{
//		cout << "Socket() error \n" << (WSAGetLastError());
//		_getch();
//		return -1;
//	}
//	
//	// указываем куда будет подключаться 
//	sockaddr_in hos_addr;
//	hos_addr.sin_family = AF_INET;
//	hos_addr.sin_port = htons(PORT);
//	hos_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
//
//	// подключаемсЯ
//	if (connect(my_sock, (sockaddr *)&hos_addr, sizeof(hos_addr)))
//	{
//		cout << "Connect error \n" << (WSAGetLastError());
//		_getch();
//		return -1;
//	}
//
//	cout  << "Сoединение  установленно " << SERVERADDR;
//
//	int nsize = 0;
//	string str;
//
//	for (;; Sleep(75))
//	{
//		str = acceptMessageToServer(my_sock);
//		
//		if (nsize != SOCKET_ERROR){
//			cout << ("S=>C: ");
//			for (auto ch : str)
//				cout << ch;
//			cout << endl;
//			cout << ("S<=C: "); 
//			str.clear();
//			cin >> str;
//
//			if (!strcmp(str.data(), "quit")){
//				cout << ("Exit...");
//				closesocket(my_sock);
//				WSACleanup();
//				return 0;
//			}
//
//			if (!strcmp(str.data(), "Sendfile")){
//
//				if (send(my_sock, str.data(), str.size(), NULL) != SOCKET_ERROR){ // отправка команды
//					cout << "Команда " <<  str.data() << endl;
//				}
//				cout << ("Name file S<=C: ");  cin >> str;
//				std::ifstream myFile(str.data(), std::ios::in | std::ios::binary | std::ios::ate);
//				int size = (int)myFile.tellg();
//				myFile.close();
//				cout << "Size file: " << size << endl;
//				char filesize[10]; 
//				itoa(size, filesize, 10);
//
//				if (send(my_sock, str.data(), str.size(), NULL) != SOCKET_ERROR){ // отправка имени файла
//					
//					//SetDlgItemText(hDlg, IDC_LBL_STATUS, (LPCTSTR)str.data());
//					cout <<	"Name file send: " << str.data() << endl;
//				}
//				char rec[32] = ""; 
//				recv(my_sock, rec, 32, 0); // получаем ответ о состоянии получения
//				//SetDlgItemText(hDlg, IDC_LBL_STATUS, (LPCTSTR)rec);
//				cout << "Answer "<< rec << endl;
//
//				send(my_sock, filesize, strlen(filesize), 0); // отправляем информацию о размере файла
//				recv(my_sock, rec, 32, 0); // получаем ответ о статусе отправки
//				cout << "Answer " << rec << endl;
//
//				FILE * fr = fopen(str.data(), "rb");
//
//				while (size > 0){
//					char buffer[1030];
//
//					if (size >= 1024){
//						fread(buffer, 1024, 1, fr);
//						send(my_sock, buffer, 1024, 0);
//						recv(my_sock, rec, 32, 0);
//					}
//					else {
//						fread(buffer, size, 1, fr);
//						buffer[size] = '\0';
//						send(my_sock, buffer, size, 0);
//						recv(my_sock, rec, 32, 0);
//					}
//
//					size -= 1024;
//				}
//
//				fclose(fr);
//				acceptFileToClient(my_sock);
//			}
//
//			//if (!strcmp(str.data(), "AcceptFile")){
//
//				
//			//}
//
//		}
//		send(my_sock, str.data(), str.size(), NULL); // для отправки сообщения
//	}
//
//	cout << "Recv error " << (WSAGetLastError()) << endl;
//	closesocket(my_sock);
//	str.clear();
//	WSACleanup();
//	_getch();
//	return 0;
//}
//string acceptMessageToServer(SOCKET sock) {
//
//	char buffer[SOME_SIZE];
//	int bytesRead = 0;
//	string receivedData;
//
//	FD_SET ReadSet;
//	struct timeval tv;
//	tv.tv_sec = 0;
//	tv.tv_usec = 5000;
//
//
//	do{
//		FD_ZERO(&ReadSet);
//		FD_SET(sock, &ReadSet);
//		int rv = select(0, &ReadSet, NULL, NULL, &tv);
//		if (rv == SOCKET_ERROR){
//			cout << "SOCKET_ERROR .. \n";
//			break;
//		}
//
//		if (!rv){
//			bytesRead = 0;
//		}
//
//		if (FD_ISSET(sock, &ReadSet))
//		{
//			bytesRead = recv(sock, buffer, SOME_SIZE - 2, 0);
//			if (bytesRead == SOCKET_ERROR){
//				break;
//			}
//			buffer[bytesRead] = '\0';
//			for (int i = 0; i < bytesRead; i++)
//				receivedData += buffer[i];
//		}
//
//
//	} while (bytesRead != 0 && bytesRead != SOCKET_ERROR);
//
//	return receivedData;
//}
//
//
//bool acceptFileToClient(SOCKET sock) {
//
//	char text[256] = "";
//	char inFile[80]; // отправляемый файл
//	memset(inFile, 0, sizeof(inFile)); //Clear the buffer
//	char rec[50] = "";
//	recv(sock, inFile, sizeof(inFile)-1, NULL); // получаем имя файла
//	cout << "Name file: " << inFile << endl;
//
//	send(sock, "OK", strlen("OK"), NULL); // отправляем ответ, что все принято хорошо
//
//	FILE * fw = fopen(inFile, "wb");
//	int recs = recv(sock, rec, 32, 0);
//	send(sock, "OK", strlen("OK"), 0);
//
//	rec[recs + 1] = '\0';
//	int size = atoi(rec);
//	cout << "Size file: " << size << endl;
//	while (size > 0){
//		char buffer[1030];
//
//		if (size >= 1024){
//			recv(sock, buffer, 1024, 0);
//			send(sock, "OK", strlen("OK"), 0);
//			fwrite(buffer, 1024, 1, fw);
//		}
//		else {
//			recv(sock, buffer, size, 0);
//			send(sock, "OK", strlen("OK"), 0);
//			buffer[size] = '\0';
//			fwrite(buffer, size, 1, fw);
//		}
//		size -= 1024;
//	}
//	fclose(fw);
//
//	cout << "File accpet " << endl;
//
//	return true;
//}




