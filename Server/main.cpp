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
		memset(buffer, 0, sizeof(buffer)); // память любыми значениями, тут нулями 
		if (recv(Connections[ID], buffer, 1024 ,NULL)){ // получаем содержимое запроса через функцию recv
			std::cout << (buffer) << "\n";
			for (int i = 0; i <= ClientCount; i++) {

				send(Connections[i], buffer, strlen(buffer), NULL);
			}
		}
	}
	ClientCount--;
	cout << "Отключение";
	delete buffer;
}

int main(){

	setlocale(LC_ALL,"RUS");
	WSADATA data;
	WORD version = MAKEWORD(2, 2); // версия сокетов
	int res = WSAStartup(version, &data); // cтарт использования библиотеки сокетов процессом
	if (res != 0){
		return 0;
	}

	
	struct addrinfo *result = NULL;
	struct addrinfo hints; // настройки сокета 

	//Connections = (SOCKET*)calloc(64, sizeof(SOCKET));
	Connections = new SOCKET[64];

	ZeroMemory(&hints, sizeof(hints)); // заполненин нулями 
	
	hints.ai_family = AF_INET; // тип сокета  (Internet протоколы)
	hints.ai_flags = AI_PASSIVE;  // Сокет биндится на адрес, чтобы принимать входящие соединения
	hints.ai_socktype = SOCK_STREAM; // установлением соединения;
	hints.ai_protocol = IPPROTO_TCP; 

	getaddrinfo(NULL, "666", &hints, &result); // инициализация хранящую адрес сокета - addr

	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // Создадим сокет
	bind(Listen, result->ai_addr, result->ai_addrlen); // Для связывания сокета с адресом и номером порт
	//Listen Подготовим сокет к принятию входящих соединений от клиенто
	listen(Listen, SOMAXCONN); //  константе SOMAXCONN хранится максимально возможное число одновременных TCP-соединений

	freeaddrinfo(result); // освобождает вызов 

	std::cout << "Start server....";
	char m_connect[] = "Connect...........................;;;5";
	for (;;Sleep(75)) {

		if (Connect = accept(Listen, NULL, NULL)){ //  accept ожидает запрос на установку TCP-соединения от удаленного хост

			std::cout << "Client connect...\n";
			Connections[ClientCount] = Connect;
			send(Connections[ClientCount], m_connect, strlen(m_connect), NULL); // Отправляем ответ клиенту с помощью функции send
	
			ClientCount++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(ClientCount - 1), NULL, NULL);
		}

	}

	return 1;
}