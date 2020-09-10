// ������ �������� TCP-���-�������
using namespace std;
#include <stdio.h>
#include <winsock2.h> // Wincosk2.h ������ ���� ������ windows!
#include <windows.h>
#include <locale.h>
#include <vector>
#include <string>
#pragma comment(lib, "ws2_32.lib")

#define MY_PORT 8666 // ����, ������� ������� ������ 666

// ������ ��� ������ ���������� �������� �������������
#define PRINTNUSERS if (nclients) printf("%d user on-line\n", nclients); \
		else printf("No User on line\n");

// �������� �������, ������������� �������������� �������������
DWORD WINAPI SexToClient(LPVOID client_socket);

// ���������� ���������� - ���������� �������� �������������
int nclients = 0;

int main()
{
	setlocale(0, "");
	WSADATA wsa;
	char buff[1024]; // ����� ��� ��������� ����
	vector<SOCKET> my_socker_list;
	printf("TCP SERVER \n");
	// ��� 1 - ������������� ���������� �������
	// �.�. ������������ �������� ���������� �� ������������
	// �� ���������� ��������� �� ������� �����, ������������� � ���������
	// �� ��������� WSADATA.
	// ����� ����� ��������� ���������� ���� ����������, ������, �����
	// ������ ���� �� ����� ������������ �������� (��������� WSADATA
	// �������� 400 ����)
	if (WSAStartup(0x0202, &wsa))
	{
		// ������!
		printf("Error WSAStartup %d\n", WSAGetLastError());
		return -1;
	}

	// ��� 2 - �������� ������
	SOCKET mysocket;
	// AF_INET - ����� ���������
	// SOCK_STREAM - ��������� ����� (� ���������� ����������)
	// 0 - �� ��������� ���������� TCP ��������
	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		// ������!
		printf("Error socket %d\n", WSAGetLastError());
		WSACleanup(); // �������������� ���������� Winsock
		return -1;
	}

	// ��� 3 - ���������� ������ � ��������� �������
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(MY_PORT); // �� �������� � ������� �������!!!
	local_addr.sin_addr.s_addr = 0; // ������ ��������� �����������
	// �� ��� ���� IP-������

	// �������� bind ��� ����������
	if (bind(mysocket, (sockaddr *)&local_addr, sizeof(local_addr)))
	{
		// ������
		printf("Error bind %d\n", WSAGetLastError());
		closesocket(mysocket); // ��������� �����!
		WSACleanup();
		return -1;
	}

	// ��� 4 - �������� �����������
	// ������ ������� - 0x100
	if (listen(mysocket, 0x100))
	{
		// ������
		printf("Error listen %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	printf("�������� �����������...\n");

	// ��� 5 - ��������� ��������� �� �������
	SOCKET client_socket; // ����� ��� �������
	sockaddr_in client_addr; // ����� ������� (����������� ��������)
	// ������� accept ���������� �������� ������ ���������
	int client_addr_size = sizeof(client_addr);
	
	
	// ���� ���������� �������� �� ����������� �� �������
	while ((client_socket = accept(mysocket, (sockaddr *)&client_addr, &client_addr_size)))
	{
		nclients++; // ����������� ������� �������������� ��������

		// �������� �������� ��� �����
		HOSTENT *hst;
		hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);
		char cur[1024];
		
		
		
		my_socker_list.push_back(client_socket);
		// ����� �������� � �������
		printf("+%s [%s] new connect!\n", (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
		PRINTNUSERS

			// ����� ������ ������ ��� ����������� �������
			// ��, ��� ����� ������������� ������������ _beginthreadex
			// ��, ��������� ������� ������� ������� ����������� �� ����������
			// ����� �� ������, ����� �������� � CreateThread
			DWORD thID;
		CreateThread(NULL, NULL, SexToClient, &my_socker_list, NULL, &thID);
	}
	return 0;
}

// ��� ������� ��������� � ��������� ������
// � ���������� ���������� ��������������� ������� ���������� �� ���������
DWORD WINAPI SexToClient(LPVOID client_socket)
{
	vector<SOCKET> my_vector_input=((vector<SOCKET> *)client_socket)[0];
	SOCKET my_sock;
	
	my_sock = my_vector_input[nclients-1];
	char buff[20 * 1024];
	#define sHELLO "Hello, Sailor\r\n"
	
	// ���������� ������� �����������
	send(my_sock, sHELLO, sizeof(sHELLO), 0);
 	vector<SOCKET>::iterator ptr; 
	// ���� ���-�������: ����� ������ �� ������� � ����������� �� �������
	int bytes_recv;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) {
		vector<SOCKET> my_vector_input=((vector<SOCKET> *)client_socket)[0];
		for (int i =0;i<my_vector_input.size();i++)
			if (my_sock!=my_vector_input[i])
			{
				send(my_vector_input[i], &buff[0], bytes_recv, 0);
			}
	}

	// ���� �� �����, �� ��������� ����� �� ����� �� �������
	// ���������� �������� recv ������ - ���������� � �������� ���������
	nclients--; // ��������� ������� �������� ��������
	printf("-disconnect\n"); PRINTNUSERS

		// ��������� �����
		closesocket(my_sock);
	return 0;
}
