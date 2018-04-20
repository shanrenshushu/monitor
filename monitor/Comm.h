#pragma once

class Comm  
{
public:
	Comm();
	virtual ~Comm();
	HANDLE m_pComm;//���
	wstring m_sPort;//�˿ں�
	DCB m_dDcb;
	bool m_bOpened;
public:
	bool OpenConnect(const wstring &port);//������
	bool IsOpened(){return  m_bOpened;}
	bool CloseComm();//�رն˿�

	BOOL SetBaut(DWORD baud);//���ò�����

	void GetCommInfo(DCB &dcb);
	BOOL SetCommInfo(DCB *dcb);//���ö˿���Ϣ
	BOOL SetCommInfo(DWORD ByteSize,DWORD Parity,DWORD StopBits,DWORD baud);//��������

	bool SetTimeOut(COMMTIMEOUTS timeout);//���ó�ʱʱ��
	BOOL SetTimeOut(int RInterval,int RTotalC,int RTotalM,int WTotalC,int WTotalM);//����ʱ��

	int SendBytesWait(unsigned char *p,int Len);//�����ֽڲ��ȴ�
	int ReadBytesNotwait(unsigned char *p,int Len);//���ֽڲ��ȴ�
	int ReadBytesWait(unsigned char *p,int Len);//���ֽڵȴ�

	bool IsDataCome();//�ж��Ƿ�������
	bool ClearBuf();
	double Delay(double ms);
};
