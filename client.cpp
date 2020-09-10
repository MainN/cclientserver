// ������ �������� TCP-�������
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#define PORT 8666
#define SERVERADDR "127.0.0.1"
DWORD WINAPI CexToServer(LPVOID client_socket);
int main(int argc, char* argv[])
{
	char buff[1024];
	WSADATA wsa;
	printf("TCP CLIENT\n");

	// ��� 1 - ������������� ���������� Winsock
	if (WSAStartup(0x202, &wsa))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		return -1;
	}

	// ��� 2 - �������� ������
	SOCKET my_sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
	{
		printf("Socket() error %d\n", WSAGetLastError());
		return -1;
	}

	// ��� 3 - ��������� ����������
	// ���������� ��������� sockaddr_in - �������� ������ � ����� �������
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	HOSTENT *hst;

	// �������������� IP ������ �� ����������� � ������� ������
	if (inet_addr(SERVERADDR) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	else
	{
		printf("Invalid address %s\n", SERVERADDR);
		closesocket(my_sock);
		WSACleanup();
		return -1;
	}

	// ����� ������� ������� - �������� ���������� ����������
	if (connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr)))
	{
		printf("Connect error %d\n", WSAGetLastError());
		return -1;
	}
	DWORD thID;
	CreateThread(NULL, NULL, CexToServer, &my_sock, NULL, &thID);
	printf("���������� � %s ������� �����������\nType quit for quit\n\n", SERVERADDR);

	// ��� 4 - ������ � �������� ���������
	int nsize;
	while ((nsize = recv(my_sock, buff, sizeof(buff)-1, 0)) != SOCKET_ERROR)
	{
		// ������ ����������� ���� � ����� ������
		buff[nsize] = 0;

		// ������� �� �����
		printf("S=>C:%s\n", buff);

		// ������ ���������������� ���� � ����������
		printf("S<=C:"); 
	}
	printf("Recv error %d\n", WSAGetLastError());
	closesocket(my_sock);
	WSACleanup();
	return -1;
}
DWORD WINAPI CexToServer(LPVOID client_socket){
	
	SOCKET my_sock = ((SOCKET*)client_socket)[0];
	char buff[1024];
	do
	{
	gets(buff);

	// �������� �� "quit"
	
		// ���������� �����
		
	
	// �������� ������ ������� �������
	send(my_sock, buff, strlen(buff), 0);
	} while (strcmp(buff, "quit\n"));
	printf("Exit...");

	closesocket(my_sock);
	WSACleanup();
	return 0;
};