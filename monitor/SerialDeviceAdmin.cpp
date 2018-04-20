#include "stdafx.h"
#include "Device.h"
#include "SerialDeviceAdmin.h"

SerialDeviceAdmin::SerialDeviceAdmin()
{
	
}
SerialDeviceAdmin::~SerialDeviceAdmin()
{
	for (map<wstring,SerialComm*>::iterator it = mMap.begin();it!=mMap.end();++it)
	{
		it->second->Exit();
	}
}

void SerialDeviceAdmin::Init()
{
	vector<sDeviceInfo> mDeviceVe;
	MAN->GetDeviceDB()->GetAllEffectDevice(mDeviceVe);
	for (int i=0;i<mDeviceVe.size();i++)
	{
		map<wstring,SerialComm*>::iterator it =mMap.find(mDeviceVe[i].com);
		if( mMap.end()!= it)
		{
			SerialComm *ser = it->second;
			ser->AddDecive(mDeviceVe[i]);
		}
		else
		{
			SerialComm *ser = new SerialComm(mDeviceVe[i].com);
			ser->AddDecive(mDeviceVe[i]);
			mMap.insert(pair<wstring ,SerialComm*>(mDeviceVe[i].com,ser));
		}
	}
}

void SerialDeviceAdmin::AddDevice( const sDeviceInfo &dev )
{
	map<wstring,SerialComm*>::iterator it =mMap.find(dev.com);
	if( mMap.end()!= it)
	{
		SerialComm *ser = it->second;
		ser->AddDecive(dev);
	}
	else
	{
		SerialComm *ser= new SerialComm(dev.com);
		ser->AddDecive(dev);
		ser->Run();
		mMap.insert(pair<wstring ,SerialComm*>(dev.com,ser));
	}
}

void SerialDeviceAdmin::DelDevice( const sDeviceInfo &dev )
{
	map<wstring,SerialComm*>::iterator it =mMap.find(dev.com);
	if( mMap.end()!= it)
	{
		SerialComm *ser = it->second;
		ser->DelDecive(dev);
		if (ser->IsEmpty())
		{
			ser->Exit();
			mMap.erase(it);
		}
	}
}

void SerialDeviceAdmin::Run()
{
	for (map<wstring,SerialComm*>::iterator it = mMap.begin();it!=mMap.end();++it)
	{
		it->second->Run();
	}
}
