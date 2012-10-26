#include "stdafx.h"

//########  CHANGE TEAM NAME########## 
#define TEAMNAME L"무명의 달인(unknown)"
//####################################


int recvData (SOCKET &socket, char *_pData, int _size);
int sendData(SOCKET &socket, const char *_pData,int _size);

int _tmain()
{	
	SOCKADDR_IN		clientAddress;
	SOCKET			clientSocket;
	WSADATA			wsadata;
	int				result=0;	
	char			readBuffer[8192];
	srand(GetTickCount());
	

	//####### MUST EXECUTE########## 
	CBoard::init();
	//##############################

	
	char inetAddress[1024];
	int portNumber;
	printf("Enter server address:");
	gets_s(inetAddress,1024);
	printf("Enter port number:");
	scanf_s("%d",&portNumber);

	if ( WSAStartup( MAKEWORD(2, 2), &wsadata ) != 0 )
	{
		printf("WSAStartUp Error\n");
		return 0;
	}

	clientSocket = socket( AF_INET, SOCK_STREAM, 0 );

	ZeroMemory( &clientAddress, sizeof( clientAddress ) );
	clientAddress.sin_port = htons( portNumber );
	clientAddress.sin_addr.s_addr = inet_addr(inetAddress);
	clientAddress.sin_family = AF_INET;

	result = connect( clientSocket, 
					(SOCKADDR *)(&clientAddress), 
					sizeof( clientAddress ) );
	if ( result != 0 )
	{
		printf("Connection error\n");
		return 0;
	}
	unsigned long ul = 1;  
	if( ioctlsocket( clientSocket, FIONBIO, (unsigned long *)&ul ) == SOCKET_ERROR )  
	{  
	    printf("Can't set non-blocking mode\n");		
	} 
	//Send team name	
	char teamName[256];
	memset(teamName,0,sizeof(teamName));
	memcpy(teamName,(char*)TEAMNAME,sizeof(TEAMNAME));
	result= sendData(clientSocket,(const char*)teamName,sizeof(teamName));
	
	if(result==0)
	{
		printf("Send message error\n");
		closesocket( clientSocket );
		return 0;
	}
	

	//####### AI module########## 	
	AI	ai;
	//##############################
	while(true)
	{
		//receive message
		result= recvData(clientSocket, readBuffer, sizeof(GAME_INFO));
		GAME_INFO gameInfo;
		memcpy(&gameInfo,readBuffer,sizeof(gameInfo));
		
#ifdef _DEBUG				
		printf("Received message\n");
#endif

		//#############calculate#################
		DECISION_INFO decisionInfo=ai.decision(gameInfo);
		//##############################


		//send message
		result= sendData(clientSocket,(const char*)&decisionInfo,sizeof(DECISION_INFO));		
#ifdef _DEBUG				
		printf("Sended message\n");
#endif
		if(result==0)
		{
			printf("Send message error\n");
			break;
		}
	}

	closesocket( clientSocket );

	char ch;	
	scanf_s("%c",&ch);	
	return 1;
}


int recvData (SOCKET &socket, char *_pData, int _size)
{
	int ret, receiveSize = 0;
	int totalSize = _size;
	char *ptr;
	ptr =_pData;
	while (1)
	{
		ret = recv( socket, ptr, totalSize - receiveSize, 0);
		if (ret == 0)
		{	
			return 0;
		}
		if(ret==SOCKET_ERROR)
		{
			if(WSAGetLastError()!=WSAEWOULDBLOCK)
			{						
				return 0;
			}
			Sleep(5);
			continue;
		}

		receiveSize += ret;
		

		if (receiveSize >= totalSize) break;

		ptr = &_pData[receiveSize];
	}

	return 1;

}
int sendData(SOCKET &socket, const char *_pData,int _size)
{
	int sendSize = _size;
	int sendedSize = 0;

	int ret;
	char *ptr;	

	while (sendSize != 0)
	{
		ptr = (char *)&_pData [sendedSize ];

		ret = send( socket, ptr, sendSize, 0);
		if (ret == 0) //if disconnected
		{
			closesocket(socket);
			socket=INVALID_SOCKET;
	
			return 0;
		}
		if(ret==SOCKET_ERROR)
		{
			if(WSAGetLastError()!=WSAEWOULDBLOCK)
			{	
				return 0;
			}
			Sleep(5);
			continue;
		}
		sendedSize += ret;
		sendSize -= ret;
	}
	return ret;
}

