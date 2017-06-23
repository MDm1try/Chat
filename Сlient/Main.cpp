#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h> 
#include <iostream>
#include <WS2tcpip.h>
#include <conio.h>
using namespace std;


#define PORT 666
#define SERVERADDR "127.0.0.1"

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RUS");
	char buff[1024];
	cout << ("TCP клиент\n");
	WSADATA data;					//данные о сокете
	WORD version = MAKEWORD(2, 0);  // запрашиваемая версия винсок и  задаем версию

	if (WSAStartup(version, &data))
	{
		cout << "Ошибка WSAStartn" << (WSAGetLastError());
		_getch();
		return -1;
	}

	SOCKET my_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (my_sock < 0)
	{
		cout << "Socket() error \n" << (WSAGetLastError());
		_getch();
		return -1;
	}
	
	// указываем куда будет подключаться 
	sockaddr_in hos_addr;
	hos_addr.sin_family = AF_INET;
	hos_addr.sin_port = htons(PORT);
	hos_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//HOSTENT *hostt;
	//if (inet_addr(SERVERADDR) != INADDR_NONE)//конвертирует строку в значение,инхандер нон это ошибка
	//	hos_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	//else
	//if (hostt = gethostbyname(SERVERADDR))
	//	((unsigned long *)&hos_addr.sin_addr)[0] = ((unsigned long **)hostt->h_addr_list)[0][0];//??????????????
	//else
	//{
	//	cout << "Неверный адресс \n" << (SERVERADDR);
	//	closesocket(my_sock);
	//	WSACleanup();
	//	return -1;
	//}

	// подключаемсЯ
	if (connect(my_sock, (sockaddr *)&hos_addr, sizeof(hos_addr)))
	{
		cout << "Connect error \n" << (WSAGetLastError());
		_getch();
		return -1;
	}

	cout  << "Сoединение  установленно " << SERVERADDR;

	int nsize = 0, sz;
	bool flag = true;
	for (;; Sleep(75))
	{

		nsize = recv(my_sock, buff, sizeof(buff) -1 , 0);
		if (nsize != SOCKET_ERROR){
			buff[nsize] = '\0';
			cout << ("S=>C: ") << buff << endl;

			if (!strcmp(buff, "q"))
				flag = false;
			if (!strcmp(buff, "t"))
				flag = true;

			if (flag == true){
				cout << ("S<=C: "); cin >> buff;
			}
			
			if (!strcmp(buff, "quit"))
			{
				cout << ("Exit...");
				closesocket(my_sock);
				WSACleanup();
				return 0;
			}
		}
		send(my_sock, buff, strlen(buff), 0);
	}

	cout << "Recv error %d\n" << (WSAGetLastError());
	closesocket(my_sock);
	WSACleanup();
	_getch();
	return 0;
}