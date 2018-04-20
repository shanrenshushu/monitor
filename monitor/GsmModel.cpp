#include "stdafx.h"
#include "GsmModel.h"
#include <process.h>
#include "Control.h"



#define BOOST_DATE_TIME_SOURCE
#define BOOST_THREAD_NO_LIB
#include <boost/thread.hpp>


vector<string> GsmModel::m_vectorAllPhones;
string         GsmModel::m_strContent;

HINSTANCE            GsmModel::hinstDLL;
Fun_Sms_Connection   GsmModel::Sms_Connection;
Fun_Sms_Send         GsmModel::Sms_Send;
map<wstring,wstring> GsmModel::m_map_SendFaild;
vector<OneMsg>       GsmModel::m_vectorMsgBuf;
boost::mutex         GsmModel::m_muForMsgBuf;
int                  GsmModel::m_intSerialNumb;

GsmModel::GsmModel(){

}
GsmModel::~GsmModel(){
	UnloadLibrary();
}

int  GsmModel::StartGsm( int intSeriaoNumb ){

	m_intSerialNumb = intSeriaoNumb;

	if( LoadGsmDll()==false ){
		//MessageBox(NULL,L"����GsmDllʧ�ܣ�",L"",NULL);
		return 1;
	}
	if( ConnectGsmModel()==false ){
		//MessageBox(NULL,L"���Ӷ����豸ʧ��!",L"",NULL);
		return 2;
	} else {
		StartThreadToSendSms();
	}
	return 0;
}

bool GsmModel::LoadGsmDll()
{
	hinstDLL = LoadLibrary( _T("sms.dll") );
	if (hinstDLL == NULL){
		MessageBox( NULL,_T("Can not find sms.dll"),_T("��ʾ"),NULL);
		return false;
	}

	Sms_Connection = (Fun_Sms_Connection)GetProcAddress(hinstDLL, "Sms_Connection");
	Sms_Send = (Fun_Sms_Send)GetProcAddress(hinstDLL, "Sms_Send");
	//Sms_Receive = (Fun_Sms_Receive)GetProcAddress(hinstDLL, "Sms_Receive");

	if (!Sms_Connection || !Sms_Send /*|| !Sms_Receive*/ ){
		MessageBox( NULL,_T("Can not find sms.dll function"),_T("��ʾ"),NULL);
		return false;
	}
	return true;
}

bool GsmModel::ConnectGsmModel(){
	// ���Ӷ���è
	static char* CopyRight= "//�Ϻ�Ѹ����Ϣ�������޹�˾,��ַwww.xunsai.com//";
	static string MobileType;
	              //   ����Ȩ�� �˿�              ������ �����ն��ͺ�
	if ( Sms_Connection(CopyRight,m_intSerialNumb, 9600, &MobileType) == 0 ){
		return false;
	}
	return true;
}

void GsmModel::SendMsgsThreadFunc(){
	while( true ){
		if( m_vectorMsgBuf.size()==0 ){
			continue;
		}

		for( int i=0;i<4;i++ ){ //�ܹ���4��
			try{
				m_muForMsgBuf.lock();
				for( vector<OneMsg>::iterator it=m_vectorMsgBuf.begin() ; it!=m_vectorMsgBuf.end() ;  ){
					//����sms
					if( true==SendOneSms( (char*)(it->strNum.c_str()) , (char*)(it->strContent.c_str()) )){
						it = m_vectorMsgBuf.erase(it);
					} else {
						it++;
					}			
				}
				m_muForMsgBuf.unlock();
				Sleep(200);  //1����Ϣ  2����ֹ������һֱռ��
			} catch(...) {
		
			}
		}
	}
}

//�����̷߳��Ͷ���
void GsmModel::StartThreadToSendSms( ){
	boost::thread t(SendMsgsThreadFunc);
}

int  GsmModel::DisconnectGsmModel(){
	Sms_Disconnection Proc;
	Proc = (Sms_Disconnection)GetProcAddress(hinstDLL,"Sms_Disconnection");
	int iValue = Proc();
	return iValue;
}

bool GsmModel::SendOneSms( char* phone,char *message ){
	if (Sms_Send(phone, message) == 0){
		OutputDebugString(_T("Sms_Send fail"));
		MessageBox( NULL,_T("Sms_Send fail"),_T("��ʾ"),NULL);
		return false;
	}
	return true;
}

void GsmModel::UnloadLibrary(){
	FreeLibrary( hinstDLL );
}


//����һ����Ϣ
void GsmModel::InsertOneMsg( string strPhone,string strContent ){

	OneMsg oneMsg;
	oneMsg.strNum     = strPhone;
	oneMsg.strContent = strContent;

	boost::thread( []( OneMsg oneMsg ){
											GsmModel::m_muForMsgBuf.lock();
											GsmModel::m_vectorMsgBuf.push_back( oneMsg );
											GsmModel::m_muForMsgBuf.unlock();		
									  },oneMsg );
}