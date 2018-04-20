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
		//MessageBox(NULL,L"加载GsmDll失败！",L"",NULL);
		return 1;
	}
	if( ConnectGsmModel()==false ){
		//MessageBox(NULL,L"连接短信设备失败!",L"",NULL);
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
		MessageBox( NULL,_T("Can not find sms.dll"),_T("提示"),NULL);
		return false;
	}

	Sms_Connection = (Fun_Sms_Connection)GetProcAddress(hinstDLL, "Sms_Connection");
	Sms_Send = (Fun_Sms_Send)GetProcAddress(hinstDLL, "Sms_Send");
	//Sms_Receive = (Fun_Sms_Receive)GetProcAddress(hinstDLL, "Sms_Receive");

	if (!Sms_Connection || !Sms_Send /*|| !Sms_Receive*/ ){
		MessageBox( NULL,_T("Can not find sms.dll function"),_T("提示"),NULL);
		return false;
	}
	return true;
}

bool GsmModel::ConnectGsmModel(){
	// 连接短信猫
	static char* CopyRight= "//上海迅赛信息技术有限公司,网址www.xunsai.com//";
	static string MobileType;
	              //   拷贝权限 端口              波特率 返回终端型号
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

		for( int i=0;i<4;i++ ){ //总共发4遍
			try{
				m_muForMsgBuf.lock();
				for( vector<OneMsg>::iterator it=m_vectorMsgBuf.begin() ; it!=m_vectorMsgBuf.end() ;  ){
					//发送sms
					if( true==SendOneSms( (char*)(it->strNum.c_str()) , (char*)(it->strContent.c_str()) )){
						it = m_vectorMsgBuf.erase(it);
					} else {
						it++;
					}			
				}
				m_muForMsgBuf.unlock();
				Sleep(200);  //1、休息  2、防止互斥量一直占有
			} catch(...) {
		
			}
		}
	}
}

//启动线程发送短信
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
		MessageBox( NULL,_T("Sms_Send fail"),_T("提示"),NULL);
		return false;
	}
	return true;
}

void GsmModel::UnloadLibrary(){
	FreeLibrary( hinstDLL );
}


//插入一条信息
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