#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <conio.h>

#pragma warning(disable: 4996)

SOCKET Connection;

void esc() // выход по клавише esc
{
	char symb;
	int code_symb;

	symb = getch();
	code_symb = symb;

	if (code_symb == 27)
	{
		exit(1);
		std::cout << "the server was stopped" << std::endl;

	}
}

void ClientHandler() {

	int msg_size;

	while (true) {

		recv(Connection, (char*)&msg_size, sizeof(int), NULL);

		char* msg = new char[msg_size + 1];

		msg[msg_size] = '\0';

		recv(Connection, msg, msg_size, NULL);

		std::cout << msg << std::endl;

		delete[] msg;
	}
}

int main(int argc, char* argv[]) {


	WSAData wsaData;

	WORD DLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DLLVersion, &wsaData) != 0) {

		std::cout << "Error to load library" << std::endl;

		exit(1);
	}

	SOCKADDR_IN addr;

	int sizeofaddr = sizeof(addr);

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	addr.sin_port = htons(1111);

	addr.sin_family = AF_INET; // семейство протоколов (TCP/UPD)

	Connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {

		std::cout << "Error: failed connect to server.\n";

		return 1;
	}
	std::cout << "Connection to server succed!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)esc, NULL, NULL, NULL);

	std::string msg1;

	while (true) { // ожидаем ввода с клавиатуры
		std::getline(std::cin, msg1);

		int msg_size = msg1.size();

		send(Connection, (char*)&msg_size, sizeof(int), NULL);

		send(Connection, msg1.c_str(), msg_size, NULL);

		Sleep(10);
	}

	system("pause");

	return 0;
}