#pragma once
#include "string"
#include "stdafx.h"
#include <boost/thread/mutex.hpp>
#include <queue>


using namespace std;
using namespace boost;

typedef int(_stdcall *Fun_Sms_Connection)(char* CopyRight, int Com_Port, int Com_BaudRate, string* Mobile_Type);	// connection device
typedef int(_stdcall *Fun_Sms_Send)(char* Sms_TelNum, char* Sms_Text);											    // Send Data
typedef int(_stdcall *Sms_Disconnection)();
//typedef int(_stdcall *Fun_Sms_Receive)(BSTR Sms_Type, BSTR *Sms_Text);											// Receive Data

typedef struct OneMsg{
	string strNum;
	string strContent;
}OneMsg;

class GsmModel{

private:
	static int m_intSerialNumb;

	static map<wstring,wstring> m_map_SendFaild;
	static vector<string> m_vectorAllPhones;
	static string         m_strContent;

	static HINSTANCE            hinstDLL;
	static Fun_Sms_Connection   Sms_Connection;
	static Fun_Sms_Send         Sms_Send;
	//Fun_Sms_Receive Sms_Receive;

	static boost::mutex          m_muForMsgBuf;
	static vector<OneMsg> m_vectorMsgBuf;
	static queue<OneMsg>  m_queMsgBuf;

	

	//���͵�������
	static bool SendOneSms( char* wstrPhone,char* wstrContent );
	//�������̺߳���
	//static unsigned __stdcall SendMsgsThread(LPVOID pParam);
	static void SendMsgsThreadFunc();
	//���ض�̬��
	static bool LoadGsmDll();
	//����GSM
	static bool ConnectGsmModel();
	//�����̷߳��Ͷ���
	static void StartThreadToSendSms();


public:
	GsmModel();
	~GsmModel();

	//��ʼ��
	static int  StartGsm( int intSeriaoNumb );
	//ģ���Ƿ����
	static bool GsmModelExit();
	//�Ͽ�GSM
	static int  DisconnectGsmModel();
	//�ͷŶ�̬��
	static void UnloadLibrary();
	//����һ����Ϣ
	static void InsertOneMsg( string strPhone,string strContent );
};