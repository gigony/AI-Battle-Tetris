#pragma once
class PSInstance
{
public:
	friend class PSFrameWork;
	PSInstance(void);
	~PSInstance(void);	
	int closeConnection();
	inline BOOL isSameSocket(SOCKET socket){return (socket==m_socket);}//인수로 받은 소켓핸들과 같은 소켓인가?
	//int setWSAAsyncSelect(HWND hWnd,u_int wMsg,long IEvent);
	SOCKET& getSocket(){return m_socket;}
	int sendData(const char *_pData,int _size);
	int recvData (char *_pData, int _size);	
	int sendData(SOCKET &socket, const char *_pData,int _size);
	int recvData (SOCKET &socket, char *_pData, int _size);	

protected:	
	int makeConnection(const char *ip,unsigned short port);		//특정 ip와 port로 접속을 시도한다.
	int acceptConnection(SOCKET clientSocket);					//인수로 받은 clientSocket을 멤버변수에 설정한다.	
	SOCKET m_socket;	

};
