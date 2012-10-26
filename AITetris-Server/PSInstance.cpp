/*
 Copyright (c) 2005-2012 Gigon Bae

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE, DAMMIT.
*/
#include "dxstdafx.h"


PSInstance::PSInstance(void):m_socket(INVALID_SOCKET)
{
	
}

PSInstance::~PSInstance(void)
{
}




int PSInstance::makeConnection(const char *ip,unsigned short port)
{	
	SOCKADDR_IN serverAddress;		
	if(m_socket!=INVALID_SOCKET)				//���� ������ �����ִٸ� ������ �ݴ´�.
	{
		MYSYSTRACE("������ �̹� �����־ �����ִ� ������ �ݽ��ϴ�");
		closesocket (m_socket);
	}
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( m_socket== INVALID_SOCKET)				//�߸��� �����̶�� 0�� ����
	{
		MYSYSTRACE("������ �����Ҷ� ������ �߻��Ͽ����ϴ� ERROR CODE:%d",WSAGetLastError());
		return 0;
	}
	ZeroMemory( &serverAddress, sizeof( serverAddress) );
	serverAddress.sin_addr.s_addr = inet_addr(ip);	
	if(serverAddress.sin_addr.s_addr==INADDR_NONE)	//�߸��� ip�ּҶ�� 0�� ����
	{
		MYSYSTRACE("�߸��� ������ �ּҸ� �Է��Ͽ����ϴ�.");		
		return 0;
	}
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	//delay���� �������� ���� 
	int nodelay=1;
	int sockOptSize=sizeof(int);
	setsockopt(m_socket,IPPROTO_TCP,TCP_NODELAY,(char *)&nodelay,sockOptSize);

	//fail connecting!
	if ( connect(m_socket, (SOCKADDR *) &serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		MYSYSTRACE("connect �������� ������ �߻��Ͽ����ϴ� ERROR CODE:%d",WSAGetLastError());
		return 0;
	}
	return 1;

}
int PSInstance::acceptConnection(SOCKET clientSocket)
{	
	m_socket=clientSocket;
	if(m_socket==INVALID_SOCKET)
	{
		MYSYSTRACE("Ŭ���̾�Ʈ�� ������Accept�ϴµ� �����Ͽ����ϴ�. ERROR CODE:%d",WSAGetLastError());
		return 0;
	}
	return 1;
}
/*int PSInstance::setWSAAsyncSelect(HWND hWnd,u_int wMsg,long IEvent)
{
	if(m_socket==INVALID_SOCKET) return 0;
	if(WSAAsyncSelect(m_socket,hWnd,wMsg,IEvent)==SOCKET_ERROR)
	{
		MYSYSTRACE("WSAAsyncSelect�Լ��� ȣ���� �� ������ �߻��Ͽ����ϴ� ERROR CODE:%d",WSAGetLastError());
		return 0;
	}
	//IEvent���� ���̶�� ���ŷ ���� ��ȯ�Ѵ�.
	if(IEvent==NULL)
	{
		u_long value=0;
		ioctlsocket(m_socket,FIONBIO,&value);
	}	
	return 1;
}
*/



int PSInstance::closeConnection()
{
	if(m_socket!=INVALID_SOCKET)
	{
		MYSYSDETAILTRACE("���� �ݱ⸦ �õ��մϴ�");
        if(closesocket(m_socket)==SOCKET_ERROR)
		{
			MYSYSTRACE("������ �������� ������ �߻��Ͽ����ϴ�. ERROR CODE:%d",WSAGetLastError());
			m_socket=INVALID_SOCKET;
			return 0;
		}
		m_socket=INVALID_SOCKET;
		MYSYSDETAILTRACE("���� �ݱ⿡ �����Ͽ����ϴ�");
	}
	return 1;
}

int PSInstance::sendData(const char *_pData,int _size)
{
	return sendData(m_socket,_pData,_size);
}
int PSInstance::sendData(SOCKET &socket, const char *_pData,int _size)
{
	int sendSize = _size;
	int sendedSize = 0;

	int ret;
	char *ptr;	

	while (sendSize != 0)
	{
		ptr = (char *)&_pData [sendedSize ];

		ret = send( socket, ptr, sendSize, 0);
		if (ret == 0) //������ ����Ǿ��ٸ�
		{
			closesocket(socket);
			socket=INVALID_SOCKET;
			MYSYSTRACE("������� ������ �������ϴ�.");
			return 0;
		}
		if(ret==SOCKET_ERROR)
		{
			if(WSAGetLastError()!=WSAEWOULDBLOCK)
			{
				MYSYSTRACE("�����͸� ���� ��(sendData) ���� ������ �߻��Ͽ����ϴ� ERROR CODE:%d \n",WSAGetLastError());
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
int PSInstance::recvData (char *_pData, int _size)
{
	return recvData(m_socket,_pData,_size);
}

int PSInstance::recvData (SOCKET &socket, char *_pData, int _size)
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
			MYSYSTRACE("������� ������ �������ϴ�.");
			return 0;
		}
		if(ret==SOCKET_ERROR)
		{
			if(WSAGetLastError()!=WSAEWOULDBLOCK)
			{				
				MYSYSTRACE("�����͸� ���� ��(recvData) ���� ������ �߻��Ͽ����ϴ� ERROR CODE:%d \n",WSAGetLastError());
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


