#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>

#pragma  comment(lib,"ws2_32.lib")

typedef struct sockaddr_in sockaddr_in;

int main(int argc, char* argv[])
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(7070);
	serAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.113");
	if (connect(sclient, (sockaddr_in*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	printf("connect ok\r\n");

	char sendData[1024] = {0};
	sprintf(sendData, "%s%s", sendData, "GET https://cloud.iotsquare.xyz/openapi/app/list HTTP/1.1\r\n");
	//sprintf(sendData, "%s%s", sendData, "Host: cloud.iotsquare.xyz\r\n");
	sprintf(sendData, "%s%s", sendData, "Connection: keep-alive\r\n");
	sprintf(sendData, "%s%s", sendData, "Content-Length: 79\r\n");
	//sprintf(sendData, "%s%s", sendData, "Token: NWH34U5dt4lSJlEeRTukJA==\r\n");
	sprintf(sendData, "%s%s", sendData, "\r\n{\"pageNo\":1,\"pageSize\":1}");

	printf("send data:\r\n%s\r\n", sendData);
	send(sclient, sendData, strlen(sendData), 0);

	/* ½ÓÊÕ */
	char recData[65535] = {0};
	while (1)
	{
		int ret = recv(sclient, recData, 65535, 0);
		if (ret > 0)
		{
			printf("------------------------------\r\n");
			recData[ret] = 0x00;
			printf(recData);
		}

	}
	return 0;
}

