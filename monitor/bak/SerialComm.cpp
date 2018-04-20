#include "stdafx.h"
#include "SerialComm.h"
#include "common.h"


SerialComm::SerialComm(){

}
SerialComm::SerialComm( wstring wstrSerialNum):m_hCom(NULL)
{
	m_wstrComNum = wstrSerialNum;
}

SerialComm::SerialComm( CallBackFunc callback_, int a_or_b ):m_hCom(NULL){
	callback_fun = callback_;

}

SerialComm::~SerialComm(){
	CloseSerial();
}
#define JOBNUM_MAX				17
#define SENDDATA_LENGTH			4
#define RECVDATA_LENGTH			7
BOOL SerialComm::OpenSerial(){

	if( m_wstrComNum.size()>=5 ){
		m_wstrComNum = (_T("\\\\.\\") + m_wstrComNum);
	}

	m_hCom = CreateFile( m_wstrComNum.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_hCom == INVALID_HANDLE_VALUE)
	{

		MessageBox(NULL,L"打开串口COM1失败！", L"提示", MB_OK);

		return FALSE;
	}
	COMMTIMEOUTS	 CommTimeouts;
	memset(&CommTimeouts, 0, sizeof(CommTimeouts));
	CommTimeouts.ReadIntervalTimeout = MAXDWORD;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 100;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;

	if (!SetCommTimeouts(m_hCom, &CommTimeouts))
		return FALSE;
	DCB dcb;
	if (!GetCommState(m_hCom, &dcb))
		return FALSE;
	dcb.BaudRate = 9600; //波特率为9600  
	dcb.ByteSize = 8;
	dcb.StopBits = 0;
	dcb.Parity=0;


	if (!SetCommState(m_hCom, &dcb))
		return FALSE;
	PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);        //清空缓冲区

	thread( [](SerialComm *pCSerial)
	{
		unsigned char buf;
		unsigned char m_RecvBuf[512];
		int recvIndex = 0;
		DWORD wCount;
		static int nCount = 1;
		while (true)
		{

			if (nCount <= JOBNUM_MAX)
			{
	
				pCSerial->CircleSendMessage(nCount);
				nCount ++;
			}
			else
			{
				nCount = 1;
			}

			recvIndex = 0;
			const int recvLength = RECVDATA_LENGTH;
			while( true )
			{
				ReadFile(pCSerial->m_hCom, &buf, 1, &wCount, NULL);
				if ( wCount>0 )
				{
					m_RecvBuf[ recvIndex++ ] = buf;
					if( recvIndex >= recvLength )
					{   //收到一帧数据
						pCSerial->RecvData(m_RecvBuf, recvLength);
						recvIndex = 0;
					}
				} 
				else 
				{
					recvIndex = 0;
					break;
				}								
			}	
			Sleep(500);
		}},this ).detach();
	return TRUE;
}

BOOL SerialComm::WriteData(char* buff){
	DWORD wCount = 0;
	WriteFile(m_hCom, buff, strlen(buff), &wCount, NULL);//发送数据
	return TRUE;
}

BOOL SerialComm::WriteData(unsigned char* buff, int intLength){
	DWORD wCount = 0;
	WriteFile(m_hCom, buff, intLength, &wCount, NULL);//发送数据	
	return TRUE;
}

void  SerialComm::CircleSendMessage(int i)
{
	static unsigned char sendBuf[SENDDATA_LENGTH] = {0};
	sendBuf[0] = 0x97;
	sendBuf[1] = i;
	UCHAR* crcRes = CRC16( sendBuf, 2);
	sendBuf[2] = crcRes[0] & 0x7F;
	sendBuf[3] = crcRes[1] & 0x7F;
	//static unsigned char sendBuf[9] = {0};
	//sendBuf[0] = 0x99;
	//sendBuf[1] = 0x01;
	//sendBuf[2] = 0x00;
	//sendBuf[3] = 0x01;
	//sendBuf[4] = 0x01;

	//UCHAR* crcRes = CRC16_0xFF( sendBuf, 5);
	//sendBuf[7] = crcRes[0];
	//sendBuf[8] = crcRes[1];
	//int i = crcRes[0];
	//int j = crcRes[1];
	WriteData(sendBuf, SENDDATA_LENGTH);
}

wstring SerialComm::GetSerialNumb()
{
	return m_wstrComNum;
}

void SerialComm::SetSerial( wstring strcom)
{
	m_wstrComNum = strcom;
}

BOOL SerialComm::ReadData(char* buff,int MaxLen){
	DWORD wCount = 0;
	ReadFile(m_hCom, buff, MaxLen, &wCount, NULL);//读取数据
	return TRUE;
}

BOOL SerialComm::GetComNum(std::vector<wstring> &vecAllComName){
	HKEY hKey;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Hardware\\DeviceMap\\SerialComm"),NULL,KEY_READ, &hKey) == ERROR_SUCCESS){
		int i = 0;
		TCHAR portName[256] = _T("\0"), commName[256] = _T("\0");
		DWORD dwLong, dwSize;
		while (TRUE){
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValue(hKey,i,portName,&dwLong,NULL,NULL,(PUCHAR)commName,&dwSize) == ERROR_NO_MORE_ITEMS)
				break;
			i++;
			vecAllComName.push_back(commName);
		}
		RegCloseKey(hKey);
	}

	return TRUE;
}

void SerialComm::RecvData(unsigned char* buff, int length)
{
	/*if (buff[0] == 0x99)
	{
		tRecord* tdata = new tRecord;
		tdata->jobnum = buff[1];
		tdata->procduct_ = (buff[2] & 0xf0) >> 4;
		tdata->quality_ = (buff[2] & 0x0f);
		tdata->technology_ = (buff[3] & 0xf0) >> 4;
		tdata->equipment_ = (buff[3] & 0x0f);
		tdata->director_ = (buff[4] & 0xf0) >> 4;

		manager()->GetUiUpdater()->postIntMessage(RECVDATA_MSG,(WPARAM)tdata);
	}*/
}

BOOL SerialComm::CloseSerial()
{
	CloseHandle(m_hCom);
	m_hCom = NULL;
	return TRUE;
}

void SerialComm::SendSound(int jobnum, vector<int> vec_partment)
{
	int nlength = 0;
	unsigned char buff[300] = {0};
	buff[0] = 0xFD;

	unsigned char str1[] ={0x01,0x01,0x73,0x6F,0x75,0x6E,0x64,0x79,0xA3,0xAC};
	memcpy(buff + 3, str1, sizeof(str1)/sizeof(unsigned char));


	if (jobnum < 10)
	{
		buff[13] = jobnum + 30;
		nlength = 14;
	}
	else
	{
		buff[13] = jobnum / 10 + 30;
		buff[14] = jobnum % 10 + 30;
		nlength = 15;
	}
	unsigned char str2[] ={0xBA,0xC5,0xB9,0xA4,0xCE,0xBB,0xBA,0xF4,0xBD,0xD0,0xA3,0xAC};
	memcpy(buff + nlength, str2, sizeof(str2)/sizeof(unsigned char));
	nlength += sizeof(str2)/sizeof(unsigned char);

	vector<unsigned char> vec_tmp;
	//for (auto it : vec_partment)
	//{
	//	switch (it)
	//	{
	//	case PROCDUCT:
	//		{
	//			if (vec_tmp.size() != 0)
	//			{
	//				vec_tmp.push_back(0xA3);
	//				vec_tmp.push_back(0xAC);
	//			}
	//			unsigned char proc[] ={0xC9, 0xFA, 0xB2, 0xFA, 0xB2, 0xBF };
	//			vec_tmp.insert(vec_tmp.begin() + vec_tmp.size(), proc, proc + 6);

	//		}
	//		break;
	//	case QUALITY:
	//		{
	//			if (vec_tmp.size() != 0)
	//			{
	//				vec_tmp.push_back(0xA3);
	//				vec_tmp.push_back(0xAC);
	//			}
	//			unsigned char qua[] ={0xD6, 0xCA, 0xC1, 0xBF, 0xB2, 0xBF };
	//			vec_tmp.insert(vec_tmp.begin() + vec_tmp.size(), qua, qua + 6);
	//		}
	//		break;
	//	case TECHNOLOGY1:
	//		{
	//			if (vec_tmp.size() != 0)
	//			{
	//				vec_tmp.push_back(0xA3);
	//				vec_tmp.push_back(0xAC);
	//			}
	//			unsigned char tech[] ={0xB9, 0xA4, 0xD2, 0xD5, 0xB2, 0xBF };
	//			vec_tmp.insert(vec_tmp.begin() + vec_tmp.size(), tech, tech + 6);
	//		}
	//		break;
	//	case EQUIPMENT:
	//		{
	//			if (vec_tmp.size() != 0)
	//			{
	//				vec_tmp.push_back(0xA3);
	//				vec_tmp.push_back(0xAC);
	//			}
	//			unsigned char equip[] ={0xC9, 0xEB, 0xB1, 0xBB, 0xB2, 0xBF };
	//			vec_tmp.insert(vec_tmp.begin() + vec_tmp.size(), equip, equip + 6);
	//		}
	//		break;
	//	case DIRECTOR:
	//		{
	//			if (vec_tmp.size() != 0)
	//			{
	//				vec_tmp.push_back(0xA3);
	//				vec_tmp.push_back(0xAC);
	//			}
	//			unsigned char director[] ={0xD4, 0xCB, 0xD3, 0xAA, 0xD7, 0xDC, 0xBC, 0xE0 };
	//			vec_tmp.insert(vec_tmp.begin() + vec_tmp.size(), director, director + 8);
	//		}
	//		break;
	//	default:
	//		break;
	//	}
	//}

	int sum = nlength + vec_tmp.size();
	buff[1] = ((sum - 3) & 0xf0) >> 4;
	buff[2] = ((sum - 3) & 0x0f);

	if (!vec_tmp.empty())
	{
		memcpy(buff + nlength, &vec_tmp[0], vec_tmp.size() * sizeof(unsigned char));
	}
	
	WriteData(buff, sum);

}

void SerialComm::SetSoundFlag(bool bFlag)
{
	m_bSoundFlag = bFlag;
}

bool SerialComm::GetSoundFlag()
{
	return m_bSoundFlag;
}

void SetSoundData()
{

}
