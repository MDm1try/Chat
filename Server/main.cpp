#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h> 
#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <Commctrl.h>
#include "TCPServer.h"

using namespace std;


int main(int argc, char* argv[])
{

	TCPServer *tCPServer = new TCPServer(MAKEWORD(2, 0));
	tCPServer->startServer();

	delete tCPServer;
	return 0;
}






























//SOCKET Connect;
//SOCKET *Connections;
//SOCKET Listen;
//int ClientCount = 0;
//#define SOME_SIZE 256
//char inFile[80]; // ������������ ����
//
//
//void sendMessageToClient(int ID);
//string receiveMessageToClient(int ID);
//bool acceptFileToClient(int ID);
//bool sendFileToClient(int ID);
//
//int main(){
//
//	WSADATA data;
//	WORD version = MAKEWORD(2, 2);						// ������ �������
//	int res = WSAStartup(version, &data);				// c���� ������������� ���������� ������� ���������
//	if (res != 0){
//		return 0;
//	}
//
//	
//	struct addrinfo *result = NULL;
//	struct addrinfo hints;								// ��������� ������ 
//
//	Connections = new SOCKET[64];
//
//	ZeroMemory(&hints, sizeof(hints));					// ���������� ������ 
//	
//	hints.ai_family = AF_INET;							// ��� ������  (Internet ���������)
//	hints.ai_flags = AI_PASSIVE;						// ����� �������� �� �����, ����� ��������� �������� ����������
//	hints.ai_socktype = SOCK_STREAM;					// ������������� ����������;
//	hints.ai_protocol = IPPROTO_TCP; 
//
//	getaddrinfo(NULL, "666", &hints, &result);			// ������������� �������� ����� ������ - addr
//
//	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // �������� �����
//	bind(Listen, result->ai_addr, result->ai_addrlen);  // ��� ���������� ������ � ������� � ������� ����
//	//Listen ���������� ����� � �������� �������� ���������� �� �������
//	listen(Listen, SOMAXCONN);							//  ��������� SOMAXCONN �������� ����������� ��������� ����� ������������� TCP-����������
//
//	freeaddrinfo(result);								// ����������� ����� 
//
//	std::cout << "Start server....";
//	char m_connect[] = "Connect....";
//	for (;;) {
//
//		if (Connect = accept(Listen, NULL, NULL)){		//  accept ������� ������ �� ��������� TCP-���������� �� ���������� ����
//
//			std::cout << "\nClient connect...\n";
//			Connections[ClientCount] = Connect;
//			send(Connections[ClientCount], m_connect, strlen(m_connect), NULL); // ���������� ����� ������� � ������� ������� send
//	
//			ClientCount++;
//			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)sendMessageToClient, (LPVOID)(ClientCount - 1), NULL, NULL);
//		}
//
//	}
//	delete Connections;
//	return 1;
//}
//
//
//void sendMessageToClient(int ID)
//{
//	int  nselect = 0;
//	FD_SET ReadSet;
//	string receivedData;
//	for (;;) {
//		
//		FD_ZERO(&ReadSet);
//		FD_SET(Connections[ID], &ReadSet);
//		nselect = select(NULL, &ReadSet, NULL, NULL, NULL);
//		if (nselect == SOCKET_ERROR){
//			cout << "SOCKET_ERROR " << endl;
//			return;
//		}
//
//		if (!nselect){
//			continue;
//		}
//		receivedData = receiveMessageToClient(ID);
//		if (FD_ISSET(Connections[ID], &ReadSet)) {
//			if (strcmp("Disconnection", receivedData.data())) {
//				for (auto ch : receivedData)
//					cout << ch;
//				cout << "\n";
//			}
//			if (!strcmp("Sendfile", receivedData.data())) {
//				if (acceptFileToClient(ID)) {
//					sendFileToClient(ID);
//				}
//			}
//
//
//			for (int i = 0; i < ClientCount; i++) {
//				if (send(Connections[i], receivedData.data(), receivedData.size(), NULL) == SOCKET_ERROR){
//					cout << "\nDisconnection" << endl;
//					ClientCount--;
//					return;
//				}
//				
//			}
//		}
//
//
//	}
//}
//
//string receiveMessageToClient(int ID)
//{
//	char buffer[SOME_SIZE];
//	int bytesRead  = 0;
//	string receivedData;
//	
//
//	FD_SET ReadSet;
//	struct timeval tv;
//	tv.tv_sec = 0;
//	tv.tv_usec = 5000;
//
//	do{
//		FD_ZERO(&ReadSet);
//		FD_SET(Connections[ID], &ReadSet);
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
//		if (FD_ISSET(Connections[ID], &ReadSet))
//		{
//			bytesRead = recv(Connections[ID], buffer, SOME_SIZE - 2, 0);
//			if (bytesRead == SOCKET_ERROR)
//			{
//				return "Disconnection";
//			}
//			buffer[bytesRead] = '\0';
//			for (int i = 0; i < bytesRead; i++)
//				receivedData += buffer[i];
//		}
//		
//
//	} while (bytesRead != 0 && bytesRead != SOCKET_ERROR);
//
//
//	return receivedData;
//}
//
//
//
//bool acceptFileToClient(int ID) {
//
//	char text[256] = "";
//	
//	memset(inFile, 0, sizeof(inFile)); //Clear the buffer
//	char rec[50] = "";
//	recv(Connections[ID], inFile, sizeof(inFile)-1, NULL); // �������� ��� �����
//	cout << "Name file: " << inFile << endl;
//
//	send(Connections[ID], "OK", strlen("OK"), NULL); // ���������� �����, ��� ��� ������� ������
//
//	FILE * fw = fopen(inFile, "wb");
//	int recs = recv(Connections[ID], rec, 32, 0);
//	send(Connections[ID], "OK", strlen("OK"), 0);
//
//	rec[recs + 1] = '\0';
//	int size = atoi(rec);
//	cout << "Size file: " << size << endl;
//	while (size > 0){
//		char buffer[1030];
//
//		if (size >= 1024){
//			recv(Connections[ID], buffer, 1024, 0);
//			send(Connections[ID], "OK", strlen("OK"), 0);
//			fwrite(buffer, 1024, 1, fw);
//		}
//		else {
//			recv(Connections[ID], buffer, size, 0);
//			send(Connections[ID], "OK", strlen("OK"), 0);
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
//
//
//bool sendFileToClient(int ID)
//{
//	string str= "AcceptFile";
////	if (send(Connections[ID], str.data(), str.size(), NULL) != SOCKET_ERROR){ // �������� �������
////		cout << "Comanda " << str.data() << endl;
////	}
//	cout << ("Name file C<=S: ") << inFile << endl;
//	std::ifstream myFile(inFile, std::ios::in | std::ios::binary | std::ios::ate);
//	int size = (int)myFile.tellg();
//	myFile.close();
//	cout << "Size file: " << size << endl;
//	char filesize[10];
//	itoa(size, filesize, 10);
//
//	if (send(Connections[ID], inFile, sizeof(inFile), NULL) != SOCKET_ERROR){ // �������� ����� �����
//
//		//SetDlgItemText(hDlg, IDC_LBL_STATUS, (LPCTSTR)str.data());
//		cout << "Name file " <<  inFile << endl;
//	}
//	char rec[32] = "";
//	recv(Connections[ID], rec, 32, 0); // �������� ����� � ��������� ���������
//	//SetDlgItemText(hDlg, IDC_LBL_STATUS, (LPCTSTR)rec);
//	cout << "Name file " << rec << endl;
//
//	send(Connections[ID], filesize, strlen(filesize), 0); // ���������� ���������� � ������� �����
//	recv(Connections[ID], rec, 32, 0); // �������� ����� � ������� ��������
//	cout << "Answer " << rec << endl;
//
//	FILE * fr = fopen(inFile, "rb");
//
//	while (size > 0){
//		char buffer[1030];
//
//		if (size >= 1024){
//			fread(buffer, 1024, 1, fr);
//			send(Connections[ID], buffer, 1024, 0);
//			recv(Connections[ID], rec, 32, 0);
//		}
//		else {
//			fread(buffer, size, 1, fr);
//			buffer[size] = '\0';
//			send(Connections[ID], buffer, size, 0);
//			recv(Connections[ID], rec, 32, 0);
//		}
//
//		size -= 1024;
//	}
//
//	fclose(fr);
//
//	return true;
//}
//
////void sendMessageToClient(int ID)
////{
////
////	vector<char> msg(SOME_SIZE);
////	int  nsize = 0;
////	for (;;) {
////		nsize = recv(Connections[ID], msg.data(), msg.size() + SOME_SIZE, NULL);  // �������� ���������� ������� ����� ������� recv
////		if (nsize != SOCKET_ERROR){
////			msg.resize(nsize);
////			for (int i = 0; i < ClientCount; i++) {
////				send(Connections[i], msg.data(), msg.size(), NULL);
////			}
////
////			for (auto ch : msg)
////				cout << ch;
////			cout << "\n";
////
////			msg.resize(SOME_SIZE);
////		}
////		else{
////			ClientCount--;
////			cout << "\nDisconnection\n";
////			return;
////		}
////
////	}
////}