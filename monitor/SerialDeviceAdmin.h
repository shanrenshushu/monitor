#pragma once
#include "stdafx.h"
#include "SerialComm.h"
#include "Device.h"

class SerialDeviceAdmin{
private:                                                                                                                                                                                                            
	map<wstring,SerialComm*>mMap;
public:
	SerialDeviceAdmin();
	~SerialDeviceAdmin();
	void Init();
	void AddDevice(const sDeviceInfo &dev);
	void DelDevice(const sDeviceInfo &dev);

	void Run();
};