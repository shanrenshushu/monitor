#pragma once

struct sFaviconTask
{
	bool directDown;
	int *isDel;
	DuiLib::CEventSource *onEvent;
	std::wstring url;
	std::wstring path;
};

class cFaviconManager
{
public:
	static cFaviconManager* Instance;

	static cFaviconManager* CreateInstance() { Instance = new cFaviconManager; return Instance;}
	static void ReleaseInstance() { if(Instance) { delete Instance; Instance = NULL; } }
	static cFaviconManager* GetInstance() {return Instance;}

public:
	cFaviconManager(void);
	~cFaviconManager(void);

	void Begin();
	void End();
	bool Exit();
	static DWORD WINAPI run(LPVOID param);

	bool IsEmpty();
	void PushTask(sFaviconTask &task);
	bool GetTask(sFaviconTask &task);
	bool FaviconTaskRun(sFaviconTask &task);
	void Invok(sFaviconTask &task);

public:
	CRITICAL_SECTION mCs;
	HANDLE mShareHandle;
	bool mIsExit;
	std::queue<sFaviconTask> mTaskQueue;
};

#define  FAVMAN cFaviconManager::GetInstance()
