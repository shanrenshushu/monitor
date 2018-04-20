#pragma once

class Comm  
{
public:
	Comm();
	virtual ~Comm();
	HANDLE m_pComm;//句柄
	wstring m_sPort;//端口号
	DCB m_dDcb;
	bool m_bOpened;
public:
	bool OpenConnect(const wstring &port);//打开连接
	bool IsOpened(){return  m_bOpened;}
	bool CloseComm();//关闭端口

	BOOL SetBaut(DWORD baud);//设置波特率

	void GetCommInfo(DCB &dcb);
	BOOL SetCommInfo(DCB *dcb);//设置端口信息
	BOOL SetCommInfo(DWORD ByteSize,DWORD Parity,DWORD StopBits,DWORD baud);//设置数据

	bool SetTimeOut(COMMTIMEOUTS timeout);//设置超时时间
	BOOL SetTimeOut(int RInterval,int RTotalC,int RTotalM,int WTotalC,int WTotalM);//设置时间

	int SendBytesWait(unsigned char *p,int Len);//发送字节并等待
	int ReadBytesNotwait(unsigned char *p,int Len);//读字节不等待
	int ReadBytesWait(unsigned char *p,int Len);//读字节等待

	bool IsDataCome();//判断是否有数据
	bool ClearBuf();
	double Delay(double ms);
};
