#pragma once

#ifdef LOG_EXPORTS
#ifndef YZG_LOG
#define YZG_LOG __declspec(dllexport)
#endif
#else
#ifndef YZG_LOG
#define YZG_LOG __declspec(dllimport)
#endif
#endif

///////////////////////////////////
void YZG_LOG DebugPrintA(const char* fileName,size_t line,const char* pfmt, ...);
void YZG_LOG DebugPrintW(const char* fileName,size_t line,const wchar_t* pfmt, ...);
void YZG_LOG SetScannerHook(HWND hwnd);
void YZG_LOG SetHookEnable(bool enable);
void YZG_LOG UnhookScanner();
#define WM_SCANNER_MESSAGE            WM_USER+400
//////////////////////////////////////////////////

/*******************************************
* 1.��־��3�У��ֱ��Ӧinfo.txt,warning.txt,error.txt
* 2.һ�����־������Ҫ��¼����Ϣ����ѯ����Ϣ����info�ӿڣ���try catch���쳣����Ϣ��Ҫ�������warning�ӿڣ������κεط��п��ܳ��ֵı�����Ҫ������쳣��־����error��
* 3.��Ex�Ľӿ�д��־ʱ��ѵ�ǰ�ļ����к�д��ȥ��error��������־�����Ex�ӿ�
* 2.����path����־����logger.dllģ���Ŀ¼�£���Ҫ�Զ���path���ô�path�Ľӿ�
********************************************/
#ifndef LOG
#define LOG

#define LogInfo(f,...)  Logger::Trace(0,Logger::eInfo,f,__VA_ARGS__)
#define LogInfoEx(f,...)  Logger::Trace(0,Logger::eInfo,__FILE__,__LINE__,f,__VA_ARGS__)
#define LogWarning(f,...)  Logger::Trace(0,Logger::eWarning,f,__VA_ARGS__)
#define LogWarningEx(f,...)  Logger::Trace(0,Logger::eWarning,__FILE__,__LINE__,f,__VA_ARGS__)
#define LogError(f,...)  Logger::Trace(0,Logger::eError,f,__VA_ARGS__)
#define LogErrorEx(f,...)  Logger::Trace(0,Logger::eError,__FILE__,__LINE__,f,__VA_ARGS__)

#define LogInfoPath(path,f,...)  Logger::Trace(path,Logger::eInfo,f,__VA_ARGS__)
#define LogInfoPathEx(path,f,...)  Logger::Trace(path,Logger::eInfo,__FILE__,__LINE__,f,__VA_ARGS__)
#define LogWarningPath(path,f,...)  Logger::Trace(path,Logger::eWarning,f,__VA_ARGS__)
#define LogWarningPathEx(path,f,...)  Logger::Trace(path,Logger::eWarning,__FILE__,__LINE__,f,__VA_ARGS__)
#define LogErrorPath(path,f,...)  Logger::Trace(path,Logger::eError,f,__VA_ARGS__)
#define LogErrorPathEx(path,f,...)  Logger::Trace(path,Logger::eError,__FILE__,__LINE__,f,__VA_ARGS__)

///������ڵ�����Ϣ�ӿڣ���debugģʽ��ʹ��
#ifdef _DEBUG
#ifndef DBG_PUTA
#define DBG_PUTA(f,...) DebugPrintA(__FILE__,__LINE__,f,__VA_ARGS__)
#endif
#ifndef DBG_PUT
#define DBG_PUT(f,...) DebugPrintW(__FILE__,__LINE__,f,__VA_ARGS__)
#endif
#else
#ifndef DBG_PUTA
#define DBG_PUTA
#endif
#ifndef DBG_PUT
#define DBG_PUT
#endif
#endif

#endif

class YZG_LOG Logger 
{
public:
	enum eLoggerType
	{
		eInfo,
		eWarning,
		eError
	};
	static std::wstring GetCurrWorkingDir();

	static void WriteLog(LPCTSTR path,LPCTSTR content,eLoggerType type);

	static void Trace(LPCTSTR path,eLoggerType type,LPCTSTR pstrFormat, ...);
	static void Trace(LPCTSTR path,eLoggerType type,const char* fileName,size_t line,LPCTSTR pstrFormat, ...);
};

