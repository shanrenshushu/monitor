#include "stdafx.h"
#include "SerialComm.h"
#include "Comm.h"


#define DELAY_TIME 1000 //监控间隔延迟时间
#define BACKTRACE 3600	//当前时间往前回溯秒数
#define MAXDEVICE 50    //最大设备数

//通讯相关缓存大小
#define SEDN_LEN 8
#define RECEIVE_LEN_MAX 25
#define RECEIVE_LEN_X 25
#define RECEIVE_LEN_Z 25
#define RECEIVE_LEN_T 9

static std::mutex mMutex;
SerialComm::SerialComm( wstring com )
{
	m_Com = com;
	mExit = false;
	mHandle= NULL;
	mMutex =std::make_shared<std::mutex>();
}

SerialComm::~SerialComm()
{

}

void SerialComm::AddDecive( const sDeviceInfo &info )
{
	std::lock_guard<std::mutex> lock(*mMutex); 
	mVeDevice.push_back(info);
	mVeDeviceAdd.push_back(info);
	for (vector<sDeviceInfo>::iterator be=mVeDeviceDel.begin();be!=mVeDeviceDel.end();++be)
	{
		if (info.deviceID == (*be).deviceID)
		{
			mVeDeviceDel.erase(be);
			 break;
		}
	}
}
void SerialComm::DelDecive( const sDeviceInfo &info )
{
	std::lock_guard<std::mutex> lock(*mMutex); 
	mVeDeviceDel.push_back(info);
	for (vector<sDeviceInfo>::iterator be=mVeDeviceAdd.begin();be!=mVeDeviceAdd.end();++be)
	{
		if (info.deviceID == (*be).deviceID)
		{
			mVeDeviceAdd.erase(be);
			break;
		}
	}
	for (vector<sDeviceInfo>::iterator be=mVeDevice.begin();be!=mVeDevice.end();++be)
	{
		if (info.deviceID == (*be).deviceID)
		{
			mVeDevice.erase(be);
			break;
		}
	}
}
vector<sDeviceInfo> SerialComm::GetAddDecive()
{
	std::lock_guard<std::mutex> lock(*mMutex); 
	return mVeDeviceAdd;
}

vector<sDeviceInfo> SerialComm::GetDelDecive()
{
	std::lock_guard<std::mutex> lock(*mMutex); 
	return mVeDeviceDel;
}

void SerialComm::ClearAddDeciveBuf()
{
	std::lock_guard<std::mutex> lock(*mMutex); 
	mVeDeviceAdd.clear();
}

void SerialComm::ClearDelDeciveBuf()
{
	std::lock_guard<std::mutex> lock(*mMutex); 
	mVeDeviceDel.clear();
}

void SerialComm::Exit()
{
	mExit=true;
	if(mHandle!=NULL)
	{
		if (WaitForSingleObject(mHandle, 1000) != WAIT_OBJECT_0)
		{
			DWORD exitCode;
			GetExitCodeThread(mHandle, &exitCode);
			if (exitCode == STILL_ACTIVE)
			{
				TerminateThread(mHandle, 0);
			}
		}
		CloseHandle(mHandle);
		mHandle=0;
	}
}

bool SerialComm::IsEmpty()
{
	std::lock_guard<std::mutex> lock(*mMutex); 
	bool emp = mVeDevice.empty();
	return emp;
}


void SerialComm::Run()
{
	DWORD threadid;
	mHandle = CreateThread(NULL, 0, &SerialComm::RealTimeMonitoring, (LPVOID)this, 0, &threadid);
}


DWORD WINAPI SerialComm::RealTimeMonitoring( LPVOID pvoid )
{
	SerialComm *pSC = (SerialComm*)pvoid;
	vector<sDeviceInfo> mVeDevice ;
	unsigned char Xbuf[SEDN_LEN]={0x01,0x03,0x09,0x92,0x00,0x0A,0x67,0xBC};
	unsigned char Zbuf[SEDN_LEN]={0x01,0x03,0x09,0x60,0x00,0x0A,0xC6,0x4F};
	unsigned char Tbuf[SEDN_LEN]={0x01,0x03,0x00,0x28,0x00,0x02,0x44,0x03};
	unsigned char ReadBuf[RECEIVE_LEN_MAX]={0};

	int count = 10;
	Comm comm;
	while (!pSC->mExit)
	{
		count = 10;
		/////////////////////////////////////////////////
		if (!comm.IsOpened())
		{
			if (comm.OpenConnect(pSC->GetCom()))
			{
				comm.SetCommInfo(8,ONESTOPBIT,NOPARITY,9600);//ONESTOPBIT:1位停止位
				comm.SetTimeOut(MAXDWORD,0,1000,10,500);
			}
		}
		if (comm.IsOpened())//读写数据
		{
			vector<sDeviceInfo> veAdd = pSC->GetAddDecive();
			pSC->ClearAddDeciveBuf();
			for(int i=0;i<veAdd.size();i++)
			{
				mVeDevice.push_back(veAdd[i]);
			}
			vector<sDeviceInfo> veDel = pSC->GetDelDecive();
			pSC->ClearDelDeciveBuf();
			for(int i=0;i<veDel.size();i++)
			{
				for (vector<sDeviceInfo>::iterator be=mVeDevice.begin();be!=mVeDevice.end();++be)
				{
					if (veDel[i].deviceID == (*be).deviceID)
					{
						mVeDevice.erase(be);
						break;
					}
				}
			}
			for (int i=0;i<mVeDevice.size();i++)
			{
				int count2 = 10;
				sRecordInfo info;
				info.deviceID = mVeDevice[i].deviceID;
				info.createTime = A2U(GetCurTime());
				info.createTimeNum = time(0); 

				bool succ = false;
				AdjustBuf(Xbuf,SEDN_LEN,0,mVeDevice[i].address);
				comm.SendBytesWait(Xbuf,SEDN_LEN);
				if(RECEIVE_LEN_X == comm.ReadBytesWait(ReadBuf,RECEIVE_LEN_X) && CRC_Check(ReadBuf,RECEIVE_LEN_X))
				{
					//Log(ReadBuf,RECEIVE_LEN_X);
					info.XData = (ReadBuf[7]*0x100 + ReadBuf[8])/1000.0f;

					Sleep(500);
					AdjustBuf(Zbuf,SEDN_LEN,0,mVeDevice[i].address);
					comm.SendBytesWait(Zbuf,SEDN_LEN);
					if(RECEIVE_LEN_Z == comm.ReadBytesWait(ReadBuf,RECEIVE_LEN_Z) && CRC_Check(ReadBuf,RECEIVE_LEN_Z))
					{
						//Log(ReadBuf,RECEIVE_LEN_Z);
						info.ZData = (ReadBuf[7]*0x100 + ReadBuf[8])/1000.0f;
						AdjustBuf(Tbuf,SEDN_LEN,0,mVeDevice[i].address);
						comm.SendBytesWait(Tbuf,SEDN_LEN);
						if(RECEIVE_LEN_T == comm.ReadBytesWait(ReadBuf,RECEIVE_LEN_T) && CRC_Check(ReadBuf,RECEIVE_LEN_T))
						{
							//Log(ReadBuf,RECEIVE_LEN_T);
							succ = true;
							info.TData = (ReadBuf[3]*0x100 + ReadBuf[4])/20.0f;
						}
					}
				}
				//保存数据库
				sDataMsg *data = new sDataMsg;
				data->data = info;
				if (succ)
				{
					data->state =1;
					MAN->GetRecordDB()->Insert(info);
					//sRecordInfo *pInfo = new sRecordInfo;
					//*pInfo = info;
					//::PostMessage(MAN->GetMainWnd(), WM_RECVDATA_MSG,(WPARAM)pInfo,0);
				}
				::PostMessage(MAN->GetMainWnd(), WM_RECVDATA_MSG,(WPARAM)data,0);
				while (!pSC->IsExit()&& --count2 > 0)
				{
					Sleep(DELAY_TIME/count2);
				}
			}
			
		}
		/////////////////////////////////////////////////
		while (!pSC->IsExit()&& --count > 0)
		{
			Sleep(DELAY_TIME/count);
		}

	} 
	return 1;
}


