#pragma once

typedef void (*CallBackFunc)( unsigned char*,int);
class SerialComm{

private:
	HANDLE  m_hCom;
	wstring m_wstrComNum;
	//void (*CallBackFunc)( unsigned char*,int);
	//fun Recv_callback;
	CallBackFunc callback_fun;

	bool m_bSoundFlag;

private:
	BOOL ReadData(char* buff, int MaxLen);
	BOOL WriteData(char* buff);
	BOOL WriteData(unsigned char* buff, int intLength);

public:
	SerialComm();
	SerialComm( wstring wstrSerialNum);
	SerialComm( CallBackFunc callback_, int a_or_b );
	~SerialComm();

	void SetName(wstring strName) { m_wstrComNum = strName; }

	BOOL OpenSerial();
	void SetSerial( wstring strcom);
	BOOL CloseSerial();
	void CircleSendMessage(int i);
	void RecvData(unsigned char* buff, int length);
	void SendSound(int jobnum, vector<int> vec_partment);

	void SetSoundFlag(bool bFlag);

	bool GetSoundFlag();
	
	wstring GetSerialNumb();
	BOOL    GetComNum(std::vector<wstring> &vecComName);

};