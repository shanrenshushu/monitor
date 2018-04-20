#pragma once
#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

#include "Device.h"

using namespace std;
using namespace boost;

class aaa
{
public:
	aaa()
	{
	}
private:
	aaa(const aaa&); // not defined
	aaa& operator=(const aaa&); // not defined
};
class SerialComm{

private:
	wstring  m_Com;
	bool mExit;
	vector<sDeviceInfo>mVeDevice;
	vector<sDeviceInfo>mVeDeviceAdd;
	vector<sDeviceInfo>mVeDeviceDel;
	HANDLE mHandle;
	//std::mutex mMutex;
	std::shared_ptr<std::mutex> mMutex;
public:
	SerialComm( wstring com );
	~SerialComm();
	void AddDecive(const sDeviceInfo &info);
	void DelDecive(const sDeviceInfo &info);
	void Run();
	void Exit();

	wstring GetCom(){return m_Com;}
	bool IsExit(){return mExit;}
	vector<sDeviceInfo> GetAddDecive();
	vector<sDeviceInfo> GetDelDecive();
	void ClearAddDeciveBuf();
	void ClearDelDeciveBuf();
	bool IsEmpty();
	static DWORD WINAPI RealTimeMonitoring(LPVOID pvoid);
};