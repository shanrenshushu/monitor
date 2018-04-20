// Comm.cpp: implementation of the Comm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Comm.h"

Comm::Comm()
{
	m_bOpened = false;
	m_pComm =NULL;
}

Comm::~Comm()
{
	if (m_pComm)
	{
		CloseComm();
	}
}

bool Comm::OpenConnect(const wstring &port)
{
	m_sPort=port;
	m_pComm = ::CreateFile(m_sPort.c_str(),GENERIC_READ | GENERIC_WRITE, 0 ,NULL,OPEN_EXISTING,NULL,NULL);
	if(m_pComm == INVALID_HANDLE_VALUE)//判断是否创建成功
	{
		return false;
	}
	else
	{
		GetCommState(m_pComm,&m_dDcb);//获得端口状态
		SetupComm(m_pComm,1024,1024);//set in buffers and out buffers
		//SetCommMask(m_pComm,EV_RXCHAR);//设置端口掩码
		ClearBuf();
		m_bOpened = true;
		return (true);
	}
}

bool Comm::CloseComm()
{
	SetCommMask(m_pComm,0);//设置端口掩码
	PurgeComm(m_pComm,PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);//
	CloseHandle(m_pComm);//关闭端口
	m_pComm = NULL;
	return (true);
}

BOOL Comm::SetBaut(DWORD baud)
{
	m_dDcb.BaudRate=baud;
	return(	SetCommState(m_pComm,&m_dDcb));
}



BOOL Comm::SetCommInfo(DWORD byteSize, DWORD Parity, DWORD StopBits, DWORD baud)
{
	m_dDcb.ByteSize = (BYTE)byteSize;       //8                                               
	m_dDcb.Parity = (BYTE)Parity;			//0:none;1:odd;2:even;3:mark;4:space  
	m_dDcb.StopBits = (BYTE)StopBits;		//0 :01 stop bit ;1:1.5;2:2 
	m_dDcb.BaudRate = baud;         
	return(SetCommState(m_pComm,&m_dDcb));
}

void  Comm::GetCommInfo(DCB & dcb)
{
	GetCommState(m_pComm,&dcb);
}

BOOL Comm::SetCommInfo(DCB *dcb)//设置端口信息
{
	m_dDcb.BaudRate=dcb->BaudRate;
	m_dDcb.ByteSize=dcb->ByteSize;
	m_dDcb.StopBits=dcb->StopBits;
	m_dDcb.Parity=dcb->Parity;

//	m_dDcb.fParity = true;
//	m_dDcb.fNull = false;

	return (SetCommState(m_pComm,&m_dDcb));
}


bool Comm::SetTimeOut(COMMTIMEOUTS timeout)//设置超时时间
{
	if (SetCommTimeouts(m_pComm,&timeout))
		return (true);
	else 
		return (false);
}
BOOL Comm::SetTimeOut(int RInterval,int RTotalC,int RTotalM,int WTotalC,int WTotalM)
{
	COMMTIMEOUTS M_cCommTimeOuts;
	M_cCommTimeOuts.ReadIntervalTimeout = RInterval;//MAXDWORD
	M_cCommTimeOuts.ReadTotalTimeoutConstant = RTotalC;//40
	M_cCommTimeOuts.ReadTotalTimeoutMultiplier = RTotalM;//(DWORD)(2*9600/2000);//11*1000/m_dDcb.BaudRate+1
	M_cCommTimeOuts.WriteTotalTimeoutConstant = WTotalC;//0
	M_cCommTimeOuts.WriteTotalTimeoutMultiplier = WTotalM;//11*1000/m_dDcb.BaudRate+1
	return(SetCommTimeouts(m_pComm,&M_cCommTimeOuts));
}

int Comm::ReadBytesNotwait(unsigned char *p, int Len)
{
	//read the bytes in inbuf and retrun right now;
	DWORD nBytesRead,dwErrorMask,nToRead;
	COMSTAT  comstat;
	ClearCommError(m_pComm,&dwErrorMask,&comstat);
	if(dwErrorMask)
		return (0);
	if (comstat.cbInQue > 0)
	{
		if(comstat.cbInQue>(DWORD)Len)
			nToRead=(DWORD)Len;
		else
			nToRead=comstat.cbInQue;
		if(ReadFile(m_pComm,p,nToRead,&nBytesRead,NULL))
			return nBytesRead;
	}
	return (0);
}

int Comm::ReadBytesWait(unsigned char *p, int Len)//读多个字节并等待
{
	//read the bytes in inbuf and retrun right now;
	/*DWORD nBytesRead;
	if(!ReadFile(m_pComm,p,Len,&nBytesRead,NULL))
	return (-1);
	else
	return nBytesRead;*/

	int recvIndex = 0;
	DWORD wCount;
	unsigned char buf;
	
	while( true )
	{
		ReadFile(m_pComm,&buf,1,&wCount,NULL);
		if ( wCount>0 )
		{
			p[ recvIndex++ ] = buf;
			if( recvIndex >= Len )
			{ 
				break;
			}
		} 
		else 
		{
			recvIndex = 0;
			break;
		}								
	}	
	return recvIndex;
}

int Comm::SendBytesWait(unsigned char *p, int Len)
{
	DWORD nByteWrite;
 	if(p[0]!=0)
	{
		if(WriteFile(m_pComm,p,(DWORD)Len,&nByteWrite,NULL))//写入多个字节
		{
			return nByteWrite;
		}
		else
			return -1;
	}
 	else return -1;
}

bool Comm::ClearBuf( )
{
	//return PurgeComm(m_pComm, PURGE_RXCLEAR|PURGE_RXABORT) ;  
	return(PurgeComm(m_pComm, PURGE_TXABORT | PURGE_TXCLEAR)&&PurgeComm(m_pComm, PURGE_RXCLEAR|PURGE_RXABORT)) ;  
}
double  Comm::Delay(double ms)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart; // 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart; // 获得初始值
	do{
		QueryPerformanceCounter(&litmp); 
		QPart2 = litmp.QuadPart;// 获得中止值 
		dfMinus = (double)(QPart2-QPart1); 
		dfTim = dfMinus *1000 / dfFreq ; // 获得对应的时间值，单位为毫秒
	}while(dfTim<=ms);
	return dfTim;
}

bool Comm::IsDataCome()//判断是否有数据
{
	DWORD dwErrorMask;
	COMSTAT comstat;
	ClearCommError(m_pComm,&dwErrorMask,&comstat);
	if(comstat.cbInQue > 0)
		return (true);
	else
		return (false);
}


