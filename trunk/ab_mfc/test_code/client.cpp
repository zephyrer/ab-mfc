#include <Winsock2.h>
#include <iostream>
#include <time.h>
#pragma  comment(lib,"ws2_32.lib")
using namespace std;

int main()
{
	WSADATA wsaData;
	if( WSAStartup(MAKEWORD( 2, 2 ), &wsaData )!=0);
	
	struct hostent *host = NULL;
	char strIP[128];
	
	gethostname(strIP, 128);
	host = gethostbyname(strIP);

	SOCKET sockClient = socket(AF_INET,SOCK_DGRAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(*(struct in_addr*)(*host->h_addr_list)));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(1989);
	
	int addlen = sizeof(SOCKADDR);

	srand(time(NULL));
	cout << "Ñ¡Ôñ:\n0.ÊäÈë×Ö·û´®;\n1.Ëæ»ú×Ö·û\n";

	int flag = 0;
	
	cin >> flag;

	while(1)
	{
		char sendstr[800];
		memset(sendstr, 0, 800);

		if(flag)
		{
			sendstr[0] = rand() % 100;
			Sleep(500);
		}
		else cin >> sendstr;
		cout << sendstr << "  =  "<< (short)sendstr[0] << endl;

		sendto(sockClient, sendstr, strlen(sendstr) + 1, 0, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
		
		if(strlen(sendstr))
		cout << sendstr << endl;
	}

	closesocket(sockClient);
	WSACleanup();
 return 0;
}