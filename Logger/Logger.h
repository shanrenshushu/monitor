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
* 1.日志分3中，分别对应info.txt,warning.txt,error.txt
* 2.一般的日志（如需要记录的信息，查询的信息都用info接口），try catch中异常的信息需要警告的用warning接口，对于任何地方有可能出现的必须需要解决的异常日志放在error中
* 3.带Ex的接口写日志时会把当前文件和行号写进去，error这样的日志最好用Ex接口
* 2.不带path的日志打在logger.dll模块的目录下，需要自定义path的用带path的接口
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

///输出窗口调试信息接口，在debug模式下使用
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

