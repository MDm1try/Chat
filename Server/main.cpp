#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h> 
#include <iostream>
#include <WS2tcpip.h>


using namespace std;
SOCKET Connect;
SOCKET *Connections;
SOCKET Listen;
int ClientCount = 0;

void SendMessageToClient(int ID)
{
	char * buffer = new char[1024];
	for (;; Sleep(75)) {
		memset(buffer, 0, sizeof(buffer)); // ������ ������ ����������, ��� ������ 
		if (recv(Connections[ID], buffer, 1024 ,NULL)){ // �������� ���������� ������� ����� ������� recv
			std::cout << (buffer) << "\n";
			for (int i = 0; i <= ClientCount; i++) {

				send(Connections[i], buffer, strlen(buffer), NULL);
			}
		}
	}
	ClientCount--;
	cout << "����������";
	delete buffer;
}

int main(){

	setlocale(LC_ALL,"RUS");
	WSADATA data;
	WORD version = MAKEWORD(2, 2); // ������ �������
	int res = WSAStartup(version, &data); // c���� ������������� ���������� ������� ���������
	if (res != 0){
		return 0;
	}

	
	struct addrinfo *result = NULL;
	struct addrinfo hints; // ��������� ������ 

	//Connections = (SOCKET*)calloc(64, sizeof(SOCKET));
	Connections = new SOCKET[64];

	ZeroMemory(&hints, sizeof(hints)); // ���������� ������ 
	
	hints.ai_family = AF_INET; // ��� ������  (Internet ���������)
	hints.ai_flags = AI_PASSIVE;  // ����� �������� �� �����, ����� ��������� �������� ����������
	hints.ai_socktype = SOCK_STREAM; // ������������� ����������;
	hints.ai_protocol = IPPROTO_TCP; 

	getaddrinfo(NULL, "666", &hints, &result); // ������������� �������� ����� ������ - addr

	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // �������� �����
	bind(Listen, result->ai_addr, result->ai_addrlen); // ��� ���������� ������ � ������� � ������� ����
	//Listen ���������� ����� � �������� �������� ���������� �� �������
	listen(Listen, SOMAXCONN); //  ��������� SOMAXCONN �������� ����������� ��������� ����� ������������� TCP-����������

	freeaddrinfo(result); // ����������� ����� 

	std::cout << "Start server....";
	char m_connect[] = "Connect...........................;;;5";
	for (;;Sleep(75)) {

		if (Connect = accept(Listen, NULL, NULL)){ //  accept ������� ������ �� ��������� TCP-���������� �� ���������� ����

			std::cout << "Client connect...\n";
			Connections[ClientCount] = Connect;
			send(Connections[ClientCount], m_connect, strlen(m_connect), NULL); // ���������� ����� ������� � ������� ������� send
	
			ClientCount++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(ClientCount - 1), NULL, NULL);
		}

	}

	return 1;
}