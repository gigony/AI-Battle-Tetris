#pragma once
class PSInstance
{
public:
	friend class PSFrameWork;
	PSInstance(void);
	~PSInstance(void);	
	int closeConnection();
	inline BOOL isSameSocket(SOCKET socket){return (socket==m_socket);}//�μ��� ���� �����ڵ�� ���� �����ΰ�?
	//int setWSAAsyncSelect(HWND hWnd,u_int wMsg,long IEvent);
	SOCKET& getSocket(){return m_socket;}
	int sendData(const char *_pData,int _size);
	int recvData (char *_pData, int _size);	
	int sendData(SOCKET &socket, const char *_pData,int _size);
	int recvData (SOCKET &socket, char *_pData, int _size);	

protected:	
	int makeConnection(const char *ip,unsigned short port);		//Ư�� ip�� port�� ������ �õ��Ѵ�.
	int acceptConnection(SOCKET clientSocket);					//�μ��� ���� clientSocket�� ��������� �����Ѵ�.	
	SOCKET m_socket;	

};
