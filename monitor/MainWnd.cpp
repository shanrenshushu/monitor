#include "stdafx.h"
#include "MainWnd.h"

#include "../Cchart/Plot/PlotInterface.h"
#include "ChartUI.h"
#include "Setting.h"
#include "Data.h"
#include "Manager.h"
#include "Comm.h"
//#include "Tool.h"
#include "Control.h"
#include "DlgForAddEmp.h"
#include "DlgForSelEmp.h"
#include "DlgForDelEmp.h"
#include "DlgForAddDevice.h"
#include "DlgForDelDevice.h"
#include "DlgForAddCom.h"
#include "DlgForDelCom.h"
#include "DlgForChangeDevice.h"
#include "DlgForHelp.h"




#include  <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <time.h>
static long t = time(0);
//bool MainWnd::mgExit = false;
//HWND MainWnd::mgHwnd = NULL;

#define DELAY_TIME 4000 //监控间隔延迟时间
#define BACKTRACE 3600	//当前时间往前回溯秒数
#define MAXDEVICE 50    //最大设备数

//通讯相关缓存大小
#define SEDN_LEN 8
#define RECEIVE_LEN_MAX 25
#define RECEIVE_LEN_X 25
#define RECEIVE_LEN_Z 25
#define RECEIVE_LEN_T 9

////自定义post消息
//#define WM_RECVDATA_MSG			(WM_USER  + 100)

//坐标轴刻度相关
#define  X_BIG_TICK_NUM	10		//x轴大刻度数
#define	 X_SMALL_TICK_NUM	5	//x轴小刻度数
#define  X_FONT		0.6
#define  Y_BIG_TICK_NUM	10		//x轴大刻度数
#define	 Y_SMALL_TICK_NUM	5	//x轴小刻度数
#define  Y_FONT		0.6


#define SYS_TAB			_T("tab")
#define SYS_OPTION_A	_T("option_a")
#define SYS_OPTION_B	_T("option_b")
#define SYS_OPTION_C	_T("option_c")
#define SYS_OPTION_D	_T("option_d")
#define SYS_OPTION_E	_T("option_e")
#define SYS_BTN_CHANGE_ACT_A _T("btn_changeact_a")
#define SYS_BTN_CHANGE_A  _T("btn_change_a")
#define AUTO_BTN_MODIFY _T("btnmodify")
#define SYS_BTN_CLEAR_TIME_A _T("btn_cleartime_a")

DUI_BEGIN_MESSAGE_MAP(MainWnd, WindowImplBase)
	DUI_ON_CLICK_CTRNAME( L"btn_help" , OnBtnHelp )
	//DUI_ON_CLICK_CTRNAME( SYS_BTN_CHANGE_A , OnBtnChangeA )
	//DUI_ON_CLICK_CTRNAME( SYS_BTN_CLEAR_TIME_A , OnBtnClearTimeA )
DUI_END_MESSAGE_MAP()
MainWnd::MainWnd()
{
	lightArr.push_back(L"green.png");
	lightArr.push_back(L"blue.png");
	lightArr.push_back(L"yellow.png");
	lightArr.push_back(L"red.png");
	lightArr.push_back(L"nolink.png");
	mHandleNum = 0;
	mInCurrent=false;
	mTodayEnd = GetTodayEndTime();
}
MainWnd::~MainWnd()
{
	//mgExit = true;
	//DWORD dw = WaitForMultipleObjects(mHandleNum, mHandleArr, true, DELAY_TIME+1000);
}

CControlUI* MainWnd::CreateControl( LPCTSTR pstrClassName )
{
	if (_tcsicmp(pstrClassName, _T("Wnd")) == 0)  
	{  
		return new CCharTUI;  
	}  
	return NULL;  
}
LRESULT MainWnd::OnDestroy( UINT , WPARAM , LPARAM , BOOL& bHandled )
{
	PostQuitMessage(0);
	return 0;
}

void MainWnd::Notify(TNotifyUI& msg){
	if ( _tcscmp( msg.pSender->GetName(), AUTO_BTN_MODIFY ) == 0 )
	{
		msg.pSender->SetAttribute( _T("font"), _T("3") );
	}
	else if (mpListView==msg.pSender && msg.sType==DUI_MSGTYPE_ITEMSELECT)
	{
		CControlUI* item = mpListView->GetItemAt(mpListView->GetCurSel());
		DBG_PUT(L"%d",item->GetTag());

		int tag = item->GetTag();
		mSelectDeviceID = tag;
		InitChartUI();
	}
	return WindowImplBase::Notify(msg);
}

void MainWnd::InitWindow(){
	//SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	::ShowWindow(this->GetHWND(),SW_MAXIMIZE);

	MAN->SetMainWnd(GetHWND());
	GetControlByName(CTabLayoutUI,mpClientTabLayout,L"client");
	mpClientTabLayout->SelectItem(0);
	InitPrimaryMenuPage();
	//GetControlByName(CTreeViewUI,mpTreeView,L"history_treeview");
	GetControlByName(DuiLib::CListUI,mpListView,L"ListView");
	GetControlByName(CCharTUI,mpUI1,L"wnd1");
	mpUI1->OnNotify += MakeDelegate(this, &MainWnd::HandleEvent);
	GetControlByName(CCharTUI,mpUI2,L"wnd2");
	mpUI2->OnNotify += MakeDelegate(this, &MainWnd::HandleEvent);
	GetControlByName(CCharTUI,mpUI3,L"wnd3");
	mpUI3->OnNotify += MakeDelegate(this, &MainWnd::HandleEvent);
	GetControlByName(CCharTUI,mpUI4,L"wnd4");
	mpUI4->OnNotify += MakeDelegate(this, &MainWnd::HandleEvent);
	GetControlByName(CCharTUI,mpUI5,L"wnd5");
	mpUI5->OnNotify += MakeDelegate(this, &MainWnd::HandleEvent);

	GetControlByName(CButtonUI,mpBtnCompareX,L"CompareX");
	mpBtnCompareX->OnNotify += MakeDelegate(this, &MainWnd::HandleEvent);
	GetControlByName(CButtonUI,mpBtnCompareZ,L"CompareZ");
	mpBtnCompareZ->OnNotify += MakeDelegate(this, &MainWnd::HandleEvent);
	GetControlByName(CButtonUI,mpBtnCompareT,L"CompareT");
	mpBtnCompareT->OnNotify += MakeDelegate(this, &MainWnd::HandleEvent);
	GetControlByName(CButtonUI,mpBtnReturn,L"Return");
	mpBtnReturn->OnNotify += MakeDelegate(this, &MainWnd::HandleEvent);
	GetControlByName(CDateTimeUI,mpStartTime,L"starttime");
	GetControlByName(CDateTimeUI,mpEndTime,L"endtime");

	/*vector<int> ve;
	for(int i=0;i<30;i++)
	{
	ve.push_back(i+1);
	}*/
	//从数据库查询链接的设备

	vector<sDeviceInfo> ve;
	MAN->GetDeviceDB()->GetAllDevice(ve);
	mHandleArr = new HANDLE[MAXDEVICE];
	if (!ve.empty())
	{
		InitDevice(ve);
	}
	mpListView->SelectItem(0);
	mpListView->HomeUp();
}

void MainWnd::InitPrimaryMenuPage(){
	//GetControlByName(DuiLib::CListUI,mpListView,L"ListView");

	m_pBtnAddEmp     = static_cast<CButtonUI*>( this->m_PaintManager.FindControl(L"btn_add_emp") );
	m_pBtnAddEmp->OnEvent+=MakeDelegate( this,&MainWnd::OnBtnPrimaryMenu );

	m_pBtnDelEmp     = static_cast<CButtonUI*>( this->m_PaintManager.FindControl(L"btn_del_emp") );
	m_pBtnDelEmp->OnEvent+=MakeDelegate( this,&MainWnd::OnBtnPrimaryMenu );

	m_pBtnSelEmp     = static_cast<CButtonUI*>( this->m_PaintManager.FindControl(L"btn_sel_emp") );
	m_pBtnSelEmp->OnEvent+=MakeDelegate( this,&MainWnd::OnBtnPrimaryMenu );

	m_pBtnAddDevice     = static_cast<CButtonUI*>( this->m_PaintManager.FindControl(L"btn_add_device") ); 
	m_pBtnAddDevice->OnEvent+=MakeDelegate( this,&MainWnd::OnBtnPrimaryMenu );

	m_pBtnDelDevice     = static_cast<CButtonUI*>( this->m_PaintManager.FindControl(L"btn_del_device") );
	m_pBtnDelDevice->OnEvent+=MakeDelegate( this,&MainWnd::OnBtnPrimaryMenu );

	m_pBtnChangeDevice  = static_cast<CButtonUI*>( this->m_PaintManager.FindControl(L"btn_change_device") );
	m_pBtnChangeDevice->OnEvent+=MakeDelegate( this,&MainWnd::OnBtnPrimaryMenu );

	m_pBtnAddCom = static_cast<CButtonUI*>( this->m_PaintManager.FindControl(L"btn_add_com") );
	m_pBtnAddCom->OnEvent+=MakeDelegate( this,&MainWnd::OnBtnPrimaryMenu );

	m_pBtnDelCom = static_cast<CButtonUI*>( this->m_PaintManager.FindControl(L"btn_delete_com") );
	m_pBtnDelCom->OnEvent+=MakeDelegate( this,&MainWnd::OnBtnPrimaryMenu );

	/*m_pBtnSelCom = static_cast<CButtonUI*>( this->m_PaintManager.FindControl(L"btn_sel_com") );	
	m_pBtnSelCom->OnEvent+=MakeDelegate( this,&MainWnd::OnBtnPrimaryMenu );*/
	
	m_pBtndetail = static_cast<CButtonUI*>( this->m_PaintManager.FindControl(L"btn_detail") );	
	m_pBtndetail->OnEvent+=MakeDelegate( this,&MainWnd::OnBtnPrimaryMenu );
	
}

LRESULT MainWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	LRESULT lRes = 0;
	switch( uMsg ) 
	{
	case WM_LBUTTONDOWN:	
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:	
	case WM_CONTEXTMENU:
		{
			//XYChartID * pxy= (XYChartID *)mpUI->GetChart();
			//pxy->Interactive(m_hWnd, uMsg, wParam, lParam);
		}
		break;
	case WM_RECVDATA_MSG:
		{
			//sRecordInfo* pData = (sRecordInfo*)wParam;
			sDataMsg *pData = (sDataMsg*)wParam;
			sRecordInfo &info = pData->data; 
			RefreshListView(pData->state,info);
			int deviceid = pData->data.deviceID;
			if(pData->state !=0 && mpClientTabLayout->GetCurSel() ==0 && info.deviceID == mSelectDeviceID)
			{
				AddData(&info);
			}
			delete pData;
		}
		break;
	case WM_GETMINMAXINFO:
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

		LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
		lpMMI->ptMaxPosition.x	= rcWork.left;
		lpMMI->ptMaxPosition.y	= rcWork.top;
		lpMMI->ptMaxSize.x		= rcWork.right;
		lpMMI->ptMaxSize.y		= rcWork.bottom;

		bHandled = FALSE;
		break;
	}
	return lRes;
}
bool MainWnd::HandleEvent( void* param )
{
	TNotifyUI* pMsg = (TNotifyUI*)param;

	if (pMsg->sType == DUI_MSGTYPE_CLICK)
	{
		if (pMsg->pSender == mpBtnCompareX)
		{
			CompareChartUI(XAxis);
		}
		else if (pMsg->pSender == mpBtnCompareZ)
		{
			CompareChartUI(ZAxis);
		}
		else if (pMsg->pSender == mpBtnCompareT)
		{
			CompareChartUI(TAxis);
		}
		else if (pMsg->pSender == mpBtnReturn)
		{
			ChangePage(0);
		}
	}
	//else if (pMsg->sType == DUI_MSGTYPE_DBCLICK)
	//{
	//	XYChartID * pxy= (XYChartID *)(((CCharTUI *)pMsg->pSender)->GetChart());

	//	if(pxy)
	//		pxy->Interactive(m_hWnd, WM_LBUTTONDBLCLK, pMsg->wParam, pMsg->lParam);
	//}

	return true;
}
bool MainWnd::OnBtnPrimaryMenu( void *param ){

	//保存选中的Btn初始化为计划菜单
	static CButtonUI* selectedBtn;// = m_pBtnFactoryAB;

	TEventUI *pEvent = (TEventUI*)param;
	if(pEvent == NULL) {
		return false;
	}
	CButtonUI* pBtnModify = (CButtonUI*)(pEvent->pSender);
	if( pBtnModify==NULL ){
		return false;
	}

	if( pEvent->Type==UIEVENT_MOUSEENTER ){
		pBtnModify->SetBorderSize( 1 );
	}else if( pEvent->Type==UIEVENT_MOUSELEAVE ){
		if( selectedBtn!=pBtnModify ){
			pBtnModify->SetBorderSize( 0 );
		}
	} else if( pEvent->Type==UIEVENT_BUTTONUP){
		if(pBtnModify->GetName()==L"btn_add_emp"){
			ShowDlgForAddEmp();
		} else if(pBtnModify->GetName()==L"btn_sel_emp" ){
			ShowDlgForSelEmp();
		} else if(pBtnModify->GetName()==L"btn_del_emp" ){
			ShowDlgForDelEmp();
		} else if(pBtnModify->GetName()==L"btn_add_device" ){
			ShowDlgForAddDevice();
		} else if(pBtnModify->GetName()==L"btn_del_device" ){
			ShowDlgForDelDevice();
		} else if(pBtnModify->GetName()==L"btn_change_device" ){
			ShowDlgForChangeDevice();
		} else if(pBtnModify->GetName()==L"btn_add_com" ){
			ShowDlgForAddCom();
		} else if(pBtnModify->GetName()==L"btn_delete_com" ){
			ShowDlgForDelCom();
		}else if(pBtnModify->GetName()==L"btn_detail" ){
			ChangePage(1);
			//SearchData();
			//InitChartUI();
		}
	}
	return true;
}


void MainWnd::ShowDlgForChangeDevice(){
	DlgForChangeDevice* pChangeDevice = new DlgForChangeDevice( this,mSelectDeviceID );
	pChangeDevice->CenterWindow();
	pChangeDevice->ShowModal();
}
void MainWnd::ShowDlgForAddDevice(){
	DlgForAddDevice* pAddDevice = new DlgForAddDevice( this );
	pAddDevice->CenterWindow();
	pAddDevice->ShowModal();
}
void MainWnd::ShowDlgForDelDevice(){
	DlgForDelDevice* pDelDevice = new DlgForDelDevice( this );
	pDelDevice->CenterWindow();
	pDelDevice->ShowModal();
}

void MainWnd::ShowDlgForAddEmp(){
	DlgForAddEmp* pAddEmp = new DlgForAddEmp( this->GetHWND() );
	pAddEmp->CenterWindow();
	pAddEmp->ShowModal();
}
void MainWnd::ShowDlgForDelEmp(){
	DlgForDelEmp* pDelEmp = new DlgForDelEmp( this->GetHWND() );
	pDelEmp->CenterWindow();
	pDelEmp->ShowModal();
}
void MainWnd::ShowDlgForSelEmp(){
	DlgForSelEmp* pSelEmp = new DlgForSelEmp( this->GetHWND() );
	pSelEmp->CenterWindow();
	pSelEmp->ShowModal();
}

void MainWnd::ShowDlgForAddCom(){
	DlgForAddCom* pAddCom = new DlgForAddCom( this->GetHWND() );
	pAddCom->CenterWindow();
	pAddCom->ShowModal();
}

void MainWnd::ShowDlgForDelCom(){
	DlgForDelCom* pDelCom = new DlgForDelCom( this->GetHWND() );
	pDelCom->CenterWindow();
	pDelCom->ShowModal();
}

void MainWnd::ChangePage(int page)
{
	if (mpClientTabLayout->GetCurSel()!=page)
	{
		mpClientTabLayout->SelectItem(page);
	}
}
void MainWnd::OnBtnHelp(TNotifyUI& msg){
	DlgForHelp* pDlgForHelp = new DlgForHelp( this->GetHWND() );
	pDlgForHelp->CenterWindow();
	pDlgForHelp->ShowModal();
}
void MainWnd::InitDevice( vector<sDeviceInfo> ve )
{
	if (!ve.empty())
	{
		for (int i=0;i<ve.size();i++)
		{
			CListContainerElementUI *pListItem;
			CDialogBuilder builder;
			pListItem = static_cast<CListContainerElementUI*>(builder.Create(_T("listitemNew.xml"), 0, NULL, &m_PaintManager));
			if(pListItem)
			{
				mpListView->Add(pListItem);
				pListItem->SetTag(ve[i].deviceID);
				CLabelUI* pLight = (CLabelUI *)pListItem->FindSubControl(L"light");
				pLight->SetBkImage(L"nolink.png");
				pLight->SetToolTip(L"未连接");
				CLabelUI* pName = (CLabelUI *)pListItem->FindSubControl(L"name");
				pName->SetText(ve[i].name.c_str());
				//if (mHandleNum<MAXDEVICE)
				//{
				//DWORD threadid;
				//mHandleArr[mHandleNum++] = CreateThread(NULL, 0, &MainWnd::RealTimeMonitoring, (LPVOID)pListItem, 0, &threadid);
				//}
			}
		}
	}
	mMAN.Init();
	mMAN.Run();
}


void MainWnd::InitChartUI()
{
	vector<sRecordInfo*> infoArr;
	char buf[40]={0};
	if (1/*incurrent*/)
	{
		/*tm stime;  
		DWORD t = time(0)+8*3600-BACKTRACE;
		_gmtime32_s(&stime,(const __time32_t*)&t);
		stime.tm_year += 1900;
		stime.tm_mon++;
		sprintf_s(buf, "%04d-%02d-%02d %02d:%02d:%02d", stime.tm_year, stime.tm_mon, stime.tm_mday,stime.tm_hour,stime.tm_min,stime.tm_sec);
		string strStartTime =  buf;*/;
		SYSTEMTIME st;
		GetLocalTime(&st);
		sprintf_s(buf, "%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
		string strStartTime =  buf;
		strStartTime += " 00:00:01";
		string strEndTime = GetCurTime();
		MAN->GetRecordDB()->SearchDataByTime(infoArr,strStartTime,strEndTime,mSelectDeviceID);
		RefreshChartUI(infoArr);
	}
	SafeDeleteVector(infoArr);
	//mpClientTabLayout->SelectItem(0);
}

void MainWnd::AddData( sRecordInfo *pData )
{
	Chart * pxy= (mpUI1->GetChart());
	if (pxy && mTodayEnd>=pData->createTimeNum)
	{
		CPlotData<DataPoint2D>	*dataStruct = ((XYChartID *)pxy)->GetAbsoluteDataSet(0);
		vector<DataPoint2D>& dataArr = dataStruct->GetData();
		DataPoint2D pd;;
		pd.x=pData->createTimeNum;
		pd.y=pData->XData;
		dataArr.push_back(pd);
		MyVData2D arr;
		arr.push_back(pd);
		//((XYChartID*)pxy)->SetData(1,arr);
		((XYChartID*)pxy)->GetDataRanges();

		CAxis *ax  = pxy->GetAxis(0);
		double range[2];
		ax->GetRange(range);
		range[1] = GetTodayNowTime();
		((XYChartID*)pxy)->GetDataRanges();
		ax->SetRange(range[0],range[1]);
		//((XYChartID*)pxy)->GetDataRanges();
		HDC hDC = GetDC(m_hWnd);
		pxy->OnDraw(hDC, mpUI1->GetPos());
		ReleaseDC(m_hWnd,hDC);
	}
	else
	{
		mTodayEnd = GetTodayEndTime();
		vector<sRecordInfo*> arr;
		arr.push_back(pData);
		RefreshChartUI(arr);
		return;
	}

	pxy= (mpUI2->GetChart());
	if (pxy)
	{
		CPlotData<DataPoint2D>	*dataStruct = ((XYChartID *)pxy)->GetAbsoluteDataSet(0);
		vector<DataPoint2D>& dataArr = dataStruct->GetData();
		DataPoint2D pd;;
		pd.x=pData->createTimeNum;
		pd.y=pData->ZData;
		dataArr.push_back(pd);
		((XYChartID*)pxy)->GetDataRanges();

		CAxis *ax  = pxy->GetAxis(0);
		double range[2];
		ax->GetRange(range);
		range[1] = GetTodayNowTime();
		ax->SetRange(range[0],range[1]);
		//((XYChartID*)pxy)->GetDataRanges();
		HDC hDC = GetDC(m_hWnd);
		pxy->OnDraw(hDC, mpUI2->GetPos());
		ReleaseDC(m_hWnd,hDC);
	}

	pxy= (mpUI3->GetChart());
	if (pxy)
	{
		CPlotData<DataPoint2D>	*dataStruct = ((XYChartID *)pxy)->GetAbsoluteDataSet(0);
		vector<DataPoint2D>& dataArr = dataStruct->GetData();
		DataPoint2D pd;;
		pd.x=pData->createTimeNum;
		pd.y=pData->TData;
		dataArr.push_back(pd);
		((XYChartID*)pxy)->GetDataRanges();

		CAxis *ax  = pxy->GetAxis(0);
		double range[2];
		ax->GetRange(range);
		range[1] = GetTodayNowTime();
		ax->SetRange(range[0],range[1]);
		//((XYChartID*)pxy)->GetDataRanges();
		HDC hDC = GetDC(m_hWnd);
		pxy->OnDraw(hDC, mpUI3->GetPos());
		ReleaseDC(m_hWnd,hDC);
	}
}

void MainWnd::RefreshChartUI( vector<sRecordInfo*>& infoArr )
{
	mpUI1->Detach();
	mpUI2->Detach();
	mpUI3->Detach();
	if (infoArr.empty())
	{
		InvalidateRect( m_hWnd, &mpClientTabLayout->GetPos(), FALSE);
		return;
	}
	int nLen=infoArr.size();
	double *pX=new double[nLen];
	double *pY=new double[nLen];

	try
	{
		///////////X轴震动开始///////////
		if (1)
		{
			Chart *chart=new XYChartID;
			for(int i=0; i<nLen; i++)
			{
				pX[i]=infoArr[i]->createTimeNum;
				pY[i]=infoArr[i]->XData;
			}
			((XYChartID*)chart)->SetSwitchReverse(true);
			//((XYChartID*)chart)->SetKeepFormer(false);//在AddCurve时会重新设置Currve
			((XYChartID*)chart)->AddCurve(pX, pY, nLen);
			//((XYChartID*)chart)->AddCurve(pX+2, pY+2, nLen-2);
			//chart->SetTitle(_TEXT("Test Double Buffer Plotting"));

			chart->SetUseLegend(false);
			chart->SetBorderShow(false);
			chart->SetBkgndColor(TO_RGB(mpUI1->GetBkColor()));
			chart->SetUseMajorHGrids(true);
			chart->SetMajorGridLineStyle(PS_DOT);
			((XYChartID*)chart)->SetAutoDataColor(false);
			((XYChartID*)chart)->SetDataColor(0,TO_RGB(0xffffffff));
			///x坐标轴相关
			CAxis* AxisBottom=chart->AddAxis(kLocationBottom);
			//AxisBottom->SetTitle(_TEXT("时间"));
			//AxisBottom->SetRange(true,pX[0],);
			double range[2];
			//AxisBottom->GetRange(range);
			if (mInCurrent)
			{
				range[0] = GetTodayNowTime()-BACKTRACE;
				range[0] = range[0]>pX[0]?range[0]:pX[0];
				range[1] = GetTodayNowTime();
			}
			else
			{
				range[0] = pX[0];
				range[1] = pX[nLen-1];
			}
			AxisBottom->SetRange(range[0],range[1]);
			AxisBottom->SetAxisTimeFormat("%H:%M:%S");//"%Y/%m/%d\n%H:%M:%S"
			LOGFONT	&logfont = AxisBottom->GetLabelFont();
			logfont.lfHeight *= X_FONT;
			logfont.lfWidth *= X_FONT;
			//AxisBottom->SetSameColor(false);
			//AxisBottom->SetTickLabelColor(RGB(255,255,255));
			AxisBottom->SetAutoMinorTick(false);
			AxisBottom->SetAutoMajorTick(false);
			
			AxisBottom->SetMinorTickCount(X_SMALL_TICK_NUM);
			AxisBottom->SetTickCount(X_BIG_TICK_NUM);
			AxisBottom->SetAxisColor(TO_RGB(0xffffffff));

			///y坐标轴相关
			CAxis* AxisLeft=chart->AddAxis(kLocationLeft);
			AxisLeft->SetTitle(_TEXT("X轴震动mm/s"));
			LOGFONT	&LeftTitlefont = AxisLeft->GetTitleFont();
			LeftTitlefont.lfHeight *= X_FONT;
			LeftTitlefont.lfWidth *= X_FONT;
			LOGFONT	&LeftLablefont = AxisLeft->GetLabelFont();
			LeftLablefont.lfHeight *= X_FONT;
			LeftLablefont.lfWidth *= X_FONT;
			AxisLeft->SetAxisDataFormat(_TEXT("%.2f"));
			AxisLeft->SetAutoMinorTick(false);
			AxisLeft->SetMinorTickCount(Y_SMALL_TICK_NUM);
			//AxisLeft->SetAutoMajorTick(false);
			//AxisLeft->SetTickCount(5);
			mpUI1->Attach(m_hWnd,chart);
		}
		///////////X轴震动结束///////////

		///////////Z轴震动开始///////////
		if (1)
		{
			Chart *chart=new XYChartID;
			for(int i=0; i<nLen; i++)
			{
				pX[i]=infoArr[i]->createTimeNum;
				pY[i]=infoArr[i]->ZData;
			}
			((XYChartID*)chart)->SetSwitchReverse(true);
			((XYChartID*)chart)->AddCurve(pX, pY, nLen);
			chart->SetUseLegend(false);
			chart->SetBorderShow(false);
			chart->SetBkgndColor(TO_RGB(mpUI2->GetBkColor()));
			chart->SetUseMajorHGrids(true);
			chart->SetMajorGridLineStyle(PS_DOT);
			///x坐标轴相关
			CAxis* AxisBottom=chart->AddAxis(kLocationBottom);
			double range[2];
			if (mInCurrent)
			{
				range[0] = GetTodayNowTime()-BACKTRACE;
				range[0] = range[0]>pX[0]?range[0]:pX[0];
				range[1] = GetTodayNowTime();
			}
			else
			{
				range[0] = pX[0];
				range[1] = pX[nLen-1];
			}
			AxisBottom->SetRange(range[0],range[1]);
			AxisBottom->SetAxisTimeFormat("%H:%M:%S");
			LOGFONT	&logfont = AxisBottom->GetLabelFont();
			logfont.lfHeight *= X_FONT;
			logfont.lfWidth *= X_FONT;
			AxisBottom->SetAutoMinorTick(false);
			AxisBottom->SetAutoMajorTick(false);
			AxisBottom->SetMinorTickCount(X_SMALL_TICK_NUM);
			AxisBottom->SetTickCount(X_BIG_TICK_NUM);

			///y坐标轴相关
			CAxis* AxisLeft=chart->AddAxis(kLocationLeft);
			AxisLeft->SetTitle(_TEXT("Z轴震动mm/s"));
			LOGFONT	&LeftTitlefont = AxisLeft->GetTitleFont();
			LeftTitlefont.lfHeight *= X_FONT;
			LeftTitlefont.lfWidth *= X_FONT;
			LOGFONT	&LeftLablefont = AxisLeft->GetLabelFont();
			LeftLablefont.lfHeight *= X_FONT;
			LeftLablefont.lfWidth *= X_FONT;
			AxisLeft->SetAxisDataFormat(_TEXT("%.2f"));
			AxisLeft->SetAutoMinorTick(false);
			AxisLeft->SetMinorTickCount(Y_SMALL_TICK_NUM);
			//AxisLeft->SetAutoMajorTick(false);
			//AxisLeft->SetTickCount(4);
			//AxisLeft->SetRange(true,-5,20);
			mpUI2->Attach(m_hWnd,chart);
		}
		///////////Z轴震动结束///////////
		///////////温度开始/////////////
		if (1)
		{
			Chart *chart=new XYChartID;
			for(int i=0; i<nLen; i++)
			{
				pX[i]=infoArr[i]->createTimeNum;
				pY[i]=infoArr[i]->TData;
			}
			((XYChartID*)chart)->SetSwitchReverse(true);
			((XYChartID*)chart)->AddCurve(pX, pY, nLen);
			chart->SetUseLegend(false);
			chart->SetBorderShow(false);
			chart->SetBkgndColor(TO_RGB(mpUI3->GetBkColor()));
			chart->SetUseMajorHGrids(true);
			chart->SetMajorGridLineStyle(PS_DOT);
			///x坐标轴相关
			CAxis* AxisBottom=chart->AddAxis(kLocationBottom);
			double range[2];
			if (mInCurrent)
			{
				range[0] = GetTodayNowTime()-BACKTRACE;
				range[0] = range[0]>pX[0]?range[0]:pX[0];
				range[1] = GetTodayNowTime();
			}
			else
			{
				range[0] = pX[0];
				range[1] = pX[nLen-1];
			}
			AxisBottom->SetRange(range[0],range[1]);
			AxisBottom->SetAxisTimeFormat("%H:%M:%S");
			LOGFONT	&logfont = AxisBottom->GetLabelFont();
			logfont.lfHeight *= X_FONT;
			logfont.lfWidth *= X_FONT;
			AxisBottom->SetAutoMinorTick(false);
			AxisBottom->SetAutoMajorTick(false);
			AxisBottom->SetMinorTickCount(X_SMALL_TICK_NUM);
			AxisBottom->SetTickCount(X_BIG_TICK_NUM);

			///y坐标轴相关
			CAxis* AxisLeft=chart->AddAxis(kLocationLeft);
			AxisLeft->SetTitle(_TEXT("实时温度℃"));
			LOGFONT	&LeftTitlefont = AxisLeft->GetTitleFont();
			LeftTitlefont.lfHeight *= X_FONT;
			LeftTitlefont.lfWidth *= X_FONT;
			LOGFONT	&LeftLablefont = AxisLeft->GetLabelFont();
			LeftLablefont.lfHeight *= X_FONT;
			LeftLablefont.lfWidth *= X_FONT;
			AxisLeft->SetAxisDataFormat(_TEXT("%.1f"));
			AxisLeft->SetAutoMinorTick(false);
			AxisLeft->SetMinorTickCount(Y_SMALL_TICK_NUM);
			mpUI3->Attach(m_hWnd,chart);
		}
		///////////温度结束/////////////
	}
	catch (...)
	{
		//LogErrorEx(L"jdkfl");
		MessageBox(GetHWND(),L"errr",L"fdf",MB_OK);
	}

	InvalidateRect( m_hWnd, &mpClientTabLayout->GetPos(), FALSE);
	delete []pX;
	delete []pY;
}
void MainWnd::CompareChartUI( eAxis xzt )
{
	char buf[64];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(buf, "%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
	string strStartTime =  buf;
	strStartTime += " 00:00:00";
	string strEndTime = buf;
	strEndTime += " 23:59:59";
	vector<sRecordInfo*> infoArr1;
	MAN->GetRecordDB()->SearchDataByTime(infoArr1,strStartTime,strEndTime,mSelectDeviceID);

	wstring wstrStartTime = mpStartTime->GetText();
	wstrStartTime += L" 00:00:01";
	wstring wstrEndTime = mpEndTime->GetText();
	wstrEndTime += L" 23:59:59";
	vector<sRecordInfo*> infoArr2;
	MAN->GetRecordDB()->SearchDataByTime(infoArr2,U2A(wstrStartTime),U2A(wstrEndTime),mSelectDeviceID);
	if (infoArr2.empty() && infoArr1.empty())
	{
		MessageBox(GetHWND(),L"无数据",L"错误",MB_OK);
		return;
	}
	mpUI4->Detach();
	mpUI5->Detach();

	if(1)
	{
		int nLen=infoArr1.size();
		double *pX=new double[nLen];
		double *pY=new double[nLen];
		///////////X轴震动开始///////////
		if (xzt==XAxis)
		{
			Chart *chart=new XYChartID;
			for(int i=0; i<nLen; i++)
			{
				pX[i]=infoArr1[i]->createTimeNum;
				pY[i]=infoArr1[i]->XData;
			}
			((XYChartID*)chart)->SetSwitchReverse(true);
			//((XYChartID*)chart)->SetKeepFormer(false);//在AddCurve时会重新设置Currve
			double range[2];
			//AxisBottom->GetRange(range);
			if (infoArr1.empty())
			{
				range[0] = GetTodayBeginTime();
				range[1] = GetTodayNowTime();
			}
			else
			{
				((XYChartID*)chart)->AddCurve(pX, pY, nLen);
				range[0] = pX[0];
				range[1] = pX[nLen-1];
			}
			
			chart->SetTitle(_TEXT("Test Double Buffer Plotting"));

			chart->SetUseLegend(false);
			chart->SetBorderShow(false);
			chart->SetBkgndColor(TO_RGB(mpUI4->GetBkColor()));
			chart->SetUseMajorHGrids(true);
			chart->SetMajorGridLineStyle(PS_DOT);

			///x坐标轴相关
			CAxis* AxisBottom=chart->AddAxis(kLocationBottom);
			//AxisBottom->SetTitle(_TEXT("时间"));
			//AxisBottom->SetRange(true,pX[0],);

			AxisBottom->SetRange(range[0],range[1]);
			AxisBottom->SetAxisTimeFormat("%H:%M:%S");//"%Y/%m/%d\n%H:%M:%S"
			LOGFONT	&logfont = AxisBottom->GetLabelFont();
			logfont.lfHeight *= X_FONT;
			logfont.lfWidth *= X_FONT;
			//AxisBottom->SetSameColor(false);
			//AxisBottom->SetTickLabelColor(RGB(255,255,255));
			AxisBottom->SetAutoMinorTick(false);
			AxisBottom->SetAutoMajorTick(false);
			AxisBottom->SetMinorTickCount(X_SMALL_TICK_NUM);
			AxisBottom->SetTickCount(X_BIG_TICK_NUM);

			///y坐标轴相关
			CAxis* AxisLeft=chart->AddAxis(kLocationLeft);
			AxisLeft->SetTitle(_TEXT("X轴震动mm/s"));
			LOGFONT	&LeftTitlefont = AxisLeft->GetTitleFont();
			LeftTitlefont.lfHeight *= X_FONT;
			LeftTitlefont.lfWidth *= X_FONT;
			LOGFONT	&LeftLablefont = AxisLeft->GetLabelFont();
			LeftLablefont.lfHeight *= X_FONT;
			LeftLablefont.lfWidth *= X_FONT;
			AxisLeft->SetAxisDataFormat(_TEXT("%.2f"));
			AxisLeft->SetAutoMinorTick(false);
			AxisLeft->SetMinorTickCount(Y_SMALL_TICK_NUM);
			//AxisLeft->SetAutoMajorTick(false);
			//AxisLeft->SetTickCount(10);
			mpUI4->Attach(m_hWnd,chart);
		}
		///////////X轴震动结束///////////

		///////////Z轴震动开始///////////
		if (xzt==ZAxis)
		{
			Chart *chart=new XYChartID;
			for(int i=0; i<nLen; i++)
			{
				pX[i]=infoArr1[i]->createTimeNum;
				pY[i]=infoArr1[i]->ZData;
			}
			((XYChartID*)chart)->SetSwitchReverse(true);
			double range[2];
			if (infoArr1.empty())
			{
				range[0] = GetTodayBeginTime();
				range[1] = GetTodayNowTime();
			}
			else
			{
				((XYChartID*)chart)->AddCurve(pX, pY, nLen);
				range[0] = pX[0];
				range[1] = pX[nLen-1];
			}

			chart->SetUseLegend(false);
			chart->SetBorderShow(false);
			chart->SetBkgndColor(TO_RGB(mpUI4->GetBkColor()));
			chart->SetUseMajorHGrids(true);
			chart->SetMajorGridLineStyle(PS_DOT);

			///x坐标轴相关
			CAxis* AxisBottom=chart->AddAxis(kLocationBottom);
			AxisBottom->SetRange(range[0],range[1]);
			AxisBottom->SetAxisTimeFormat("%H:%M:%S");
			LOGFONT	&logfont = AxisBottom->GetLabelFont();
			logfont.lfHeight *= X_FONT;
			logfont.lfWidth *= X_FONT;
			AxisBottom->SetAutoMinorTick(false);
			AxisBottom->SetAutoMajorTick(false);
			AxisBottom->SetMinorTickCount(X_SMALL_TICK_NUM);
			AxisBottom->SetTickCount(X_BIG_TICK_NUM);

			///y坐标轴相关
			CAxis* AxisLeft=chart->AddAxis(kLocationLeft);
			AxisLeft->SetTitle(_TEXT("Z轴震动mm/s"));
			LOGFONT	&LeftTitlefont = AxisLeft->GetTitleFont();
			LeftTitlefont.lfHeight *= X_FONT;
			LeftTitlefont.lfWidth *= X_FONT;
			LOGFONT	&LeftLablefont = AxisLeft->GetLabelFont();
			LeftLablefont.lfHeight *= X_FONT;
			LeftLablefont.lfWidth *= X_FONT;
			AxisLeft->SetAxisDataFormat(_TEXT("%.2f"));
			AxisLeft->SetAutoMinorTick(false);
			AxisLeft->SetMinorTickCount(Y_SMALL_TICK_NUM);
			//AxisLeft->SetAutoMajorTick(false);
			//AxisLeft->SetTickCount(4);
			//AxisLeft->SetRange(true,-5,20);
			mpUI4->Attach(m_hWnd,chart);
		}
		///////////Z轴震动结束///////////
		///////////温度开始/////////////
		if (xzt==TAxis)
		{
			Chart *chart=new XYChartID;
			for(int i=0; i<nLen; i++)
			{
				pX[i]=infoArr1[i]->createTimeNum;
				pY[i]=infoArr1[i]->TData;
			}
			((XYChartID*)chart)->SetSwitchReverse(true);
			double range[2];
			if (infoArr1.empty())
			{
				range[0] = GetTodayBeginTime();
				range[1] = GetTodayNowTime();
			}
			else
			{
				((XYChartID*)chart)->AddCurve(pX, pY, nLen);
				range[0] = pX[0];
				range[1] = pX[nLen-1];
			}
			chart->SetUseLegend(false);
			chart->SetBorderShow(false);
			chart->SetBkgndColor(TO_RGB(mpUI4->GetBkColor()));
			chart->SetUseMajorHGrids(true);
			chart->SetMajorGridLineStyle(PS_DOT);

			///x坐标轴相关
			CAxis* AxisBottom=chart->AddAxis(kLocationBottom);
			AxisBottom->SetRange(range[0],range[1]);
			AxisBottom->SetAxisTimeFormat("%H:%M:%S");
			LOGFONT	&logfont = AxisBottom->GetLabelFont();
			logfont.lfHeight *= X_FONT;
			logfont.lfWidth *= X_FONT;
			AxisBottom->SetAutoMinorTick(false);
			AxisBottom->SetAutoMajorTick(false);
			AxisBottom->SetMinorTickCount(X_SMALL_TICK_NUM);
			AxisBottom->SetTickCount(X_BIG_TICK_NUM);

			///y坐标轴相关
			CAxis* AxisLeft=chart->AddAxis(kLocationLeft);
			AxisLeft->SetTitle(_TEXT("实时温度℃"));
			LOGFONT	&LeftTitlefont = AxisLeft->GetTitleFont();
			LeftTitlefont.lfHeight *= X_FONT;
			LeftTitlefont.lfWidth *= X_FONT;
			LOGFONT	&LeftLablefont = AxisLeft->GetLabelFont();
			LeftLablefont.lfHeight *= X_FONT;
			LeftLablefont.lfWidth *= X_FONT;
			AxisLeft->SetAxisDataFormat(_TEXT("%.1f"));
			AxisLeft->SetAutoMinorTick(false);
			AxisLeft->SetMinorTickCount(Y_SMALL_TICK_NUM);
			mpUI4->Attach(m_hWnd,chart);
		}
		///////////温度结束/////////////
		delete []pX;
		delete []pY;
	}
	//catch (...)
	//{
	//	//LogErrorEx(L"jdkfl");
	//	MessageBox(GetHWND(),L"11111",L"1111",MB_OK);
	//}
	if(1)
	{
		int nLen=infoArr2.size();
		double *pX=new double[nLen];
		double *pY=new double[nLen];
		///////////X轴震动开始///////////
		if (xzt==XAxis)
		{
			Chart *chart=new XYChartID;
			for(int i=0; i<nLen; i++)
			{
				pX[i]=infoArr2[i]->createTimeNum;
				pY[i]=infoArr2[i]->XData;
			}
			((XYChartID*)chart)->SetSwitchReverse(true);
			double range[2];
			if (infoArr2.empty())
			{
				range[0] = GetTodayBeginTime();
				range[1] = GetTodayNowTime();
			}
			else
			{
				((XYChartID*)chart)->AddCurve(pX, pY, nLen);
				range[0] = pX[0];
				range[1] = pX[nLen-1];
			}
			chart->SetTitle(_TEXT("Test Double Buffer Plotting"));

			chart->SetUseLegend(false);
			chart->SetBorderShow(false);
			chart->SetBkgndColor(TO_RGB(mpUI5->GetBkColor()));
			chart->SetUseMajorHGrids(true);
			chart->SetMajorGridLineStyle(PS_DOT);

			///x坐标轴相关
			CAxis* AxisBottom=chart->AddAxis(kLocationBottom);
			AxisBottom->SetRange(range[0],range[1]);
			AxisBottom->SetAxisTimeFormat("%H:%M:%S");//"%Y/%m/%d\n%H:%M:%S"
			LOGFONT	&logfont = AxisBottom->GetLabelFont();
			logfont.lfHeight *= X_FONT;
			logfont.lfWidth *= X_FONT;
			//AxisBottom->SetSameColor(false);
			//AxisBottom->SetTickLabelColor(RGB(255,255,255));
			AxisBottom->SetAutoMinorTick(false);
			AxisBottom->SetAutoMajorTick(false);
			AxisBottom->SetMinorTickCount(X_SMALL_TICK_NUM);
			AxisBottom->SetTickCount(X_BIG_TICK_NUM);

			///y坐标轴相关
			CAxis* AxisLeft=chart->AddAxis(kLocationLeft);
			AxisLeft->SetTitle(_TEXT("X轴震动mm/s"));
			LOGFONT	&LeftTitlefont = AxisLeft->GetTitleFont();
			LeftTitlefont.lfHeight *= X_FONT;
			LeftTitlefont.lfWidth *= X_FONT;
			LOGFONT	&LeftLablefont = AxisLeft->GetLabelFont();
			LeftLablefont.lfHeight *= X_FONT;
			LeftLablefont.lfWidth *= X_FONT;
			AxisLeft->SetAxisDataFormat(_TEXT("%.2f"));
			AxisLeft->SetAutoMinorTick(false);
			AxisLeft->SetMinorTickCount(Y_SMALL_TICK_NUM);
			//AxisLeft->SetAutoMajorTick(false);
			//AxisLeft->SetTickCount(10);
			mpUI5->Attach(m_hWnd,chart);
		}
		///////////X轴震动结束///////////

		///////////Z轴震动开始///////////
		if (xzt==ZAxis)
		{
			Chart *chart=new XYChartID;
			for(int i=0; i<nLen; i++)
			{
				pX[i]=infoArr2[i]->createTimeNum;
				pY[i]=infoArr2[i]->ZData;
			}
			((XYChartID*)chart)->SetSwitchReverse(true);
			double range[2];
			if (infoArr2.empty())
			{
				range[0] = GetTodayBeginTime();
				range[1] = GetTodayNowTime();
			}
			else
			{
				((XYChartID*)chart)->AddCurve(pX, pY, nLen);
				range[0] = pX[0];
				range[1] = pX[nLen-1];
			}
			chart->SetUseLegend(false);
			chart->SetBorderShow(false);
			chart->SetBkgndColor(TO_RGB(mpUI5->GetBkColor()));
			chart->SetUseMajorHGrids(true);
			chart->SetMajorGridLineStyle(PS_DOT);

			///x坐标轴相关
			CAxis* AxisBottom=chart->AddAxis(kLocationBottom);
			AxisBottom->SetRange(range[0],range[1]);
			AxisBottom->SetAxisTimeFormat("%H:%M:%S");
			LOGFONT	&logfont = AxisBottom->GetLabelFont();
			logfont.lfHeight *= X_FONT;
			logfont.lfWidth *= X_FONT;
			AxisBottom->SetAutoMinorTick(false);
			AxisBottom->SetAutoMajorTick(false);
			AxisBottom->SetMinorTickCount(X_SMALL_TICK_NUM);
			AxisBottom->SetTickCount(X_BIG_TICK_NUM);

			///y坐标轴相关
			CAxis* AxisLeft=chart->AddAxis(kLocationLeft);
			AxisLeft->SetTitle(_TEXT("Z轴震动mm/s"));
			LOGFONT	&LeftTitlefont = AxisLeft->GetTitleFont();
			LeftTitlefont.lfHeight *= X_FONT;
			LeftTitlefont.lfWidth *= X_FONT;
			LOGFONT	&LeftLablefont = AxisLeft->GetLabelFont();
			LeftLablefont.lfHeight *= X_FONT;
			LeftLablefont.lfWidth *= X_FONT;
			AxisLeft->SetAxisDataFormat(_TEXT("%.2f"));
			AxisLeft->SetAutoMinorTick(false);
			AxisLeft->SetMinorTickCount(Y_SMALL_TICK_NUM);
			mpUI5->Attach(m_hWnd,chart);
		}
		///////////Z轴震动结束///////////
		///////////温度开始/////////////
		if (xzt==TAxis)
		{
			Chart *chart=new XYChartID;
			for(int i=0; i<nLen; i++)
			{
				pX[i]=infoArr2[i]->createTimeNum;
				pY[i]=infoArr2[i]->TData;
			}
			((XYChartID*)chart)->SetSwitchReverse(true);
			double range[2];
			if (infoArr2.empty())
			{
				range[0] = GetTodayBeginTime();
				range[1] = GetTodayNowTime();
			}
			else
			{
				((XYChartID*)chart)->AddCurve(pX, pY, nLen);
				range[0] = pX[0];
				range[1] = pX[nLen-1];
			}
			chart->SetUseLegend(false);
			chart->SetBorderShow(false);
			chart->SetBkgndColor(TO_RGB(mpUI5->GetBkColor()));
			chart->SetUseMajorHGrids(true);
			chart->SetMajorGridLineStyle(PS_DOT);

			///x坐标轴相关
			CAxis* AxisBottom=chart->AddAxis(kLocationBottom);
			AxisBottom->SetRange(range[0],range[1]);
			AxisBottom->SetAxisTimeFormat("%H:%M:%S");
			LOGFONT	&logfont = AxisBottom->GetLabelFont();
			logfont.lfHeight *= X_FONT;
			logfont.lfWidth *= X_FONT;
			AxisBottom->SetAutoMinorTick(false);
			AxisBottom->SetAutoMajorTick(false);
			AxisBottom->SetMinorTickCount(X_SMALL_TICK_NUM);
			AxisBottom->SetTickCount(X_BIG_TICK_NUM);

			///y坐标轴相关
			CAxis* AxisLeft=chart->AddAxis(kLocationLeft);
			AxisLeft->SetTitle(_TEXT("实时温度℃"));
			LOGFONT	&LeftTitlefont = AxisLeft->GetTitleFont();
			LeftTitlefont.lfHeight *= X_FONT;
			LeftTitlefont.lfWidth *= X_FONT;
			LOGFONT	&LeftLablefont = AxisLeft->GetLabelFont();
			LeftLablefont.lfHeight *= X_FONT;
			LeftLablefont.lfWidth *= X_FONT;
			AxisLeft->SetAxisDataFormat(_TEXT("%.1f"));
			AxisLeft->SetAutoMinorTick(false);
			AxisLeft->SetMinorTickCount(Y_SMALL_TICK_NUM);
			mpUI5->Attach(m_hWnd,chart);
		}
		///////////温度结束/////////////
		delete []pX;
		delete []pY;
	}
	//catch (...)
	//{
	//	//LogErrorEx(L"jdkfl");
	//	MessageBox(GetHWND(),L"errr",L"fdf",MB_OK);
	//}
	InvalidateRect( m_hWnd, &mpClientTabLayout->GetPos(), FALSE);
	
	SafeDeleteVector(infoArr1);
	SafeDeleteVector(infoArr2);
}

void MainWnd::AddDevice( const sDeviceInfo &info )
{
	CListContainerElementUI *pListItem;
	CDialogBuilder builder;
	pListItem = static_cast<CListContainerElementUI*>(builder.Create(_T("listitemNew.xml"), 0, NULL, &m_PaintManager));
	if(pListItem)
	{
		mpListView->Add(pListItem);
		pListItem->SetTag(info.deviceID);
		CLabelUI* pLight = (CLabelUI *)pListItem->FindSubControl(L"light");
		pLight->SetBkImage(L"nolink.png");
		if (info.type.find(L"微") != wstring::npos)
		{
			pLight->SetTag(eTiny);
		}
		else if (info.type.find(L"微") != wstring::npos)
		{
			pLight->SetTag(eSmall);
		}
		else if (info.type.find(L"微") != wstring::npos)
		{
			pLight->SetTag(eMedium);
		}
		else
		{
			pLight->SetTag(eBig);
		}
		pLight->SetToolTip(L"未连接");
		CLabelUI* pName = (CLabelUI *)pListItem->FindSubControl(L"name");
		pName->SetText(info.name.c_str());
		if (info.address !=0 && !info.com.empty())
		{
			mMAN.AddDevice(info);
			if (mpListView->GetCount() ==1)
			{
				mpListView->SelectItem(0);
			}
		}
	}
}

void MainWnd::DelDevice( const sDeviceInfo &info )
{
	int count = mpListView->GetCount();
	for (int i=0;i<count;i++)
	{
		CControlUI *pListItem = (CControlUI *)mpListView->GetItemAt(i);
		if (pListItem->GetTag() == info.deviceID)
		{
			mpListView->Remove(pListItem);
			mMAN.DelDevice(info);
			break;
		}
	}
	if (mpListView->GetCount() == 0)
	{
		mpUI1->Detach();
		mpUI2->Detach();
		mpUI3->Detach();
		InvalidateRect( m_hWnd, &mpClientTabLayout->GetPos(), FALSE);
	}
}

void MainWnd::RefreshListView( int state,const sRecordInfo &info )
{
	for (int i=0;i<mpListView->GetCount();i++)
	{
		CListContainerElementUI *pListItem = (CListContainerElementUI *)mpListView->GetItemAt(i);
		if (info.deviceID == pListItem->GetTag())
		{
			CLabelUI* pLight = (CLabelUI *)pListItem->FindSubControl(L"light");
			if (state == 0)
			{
				pLight->SetBkImage(lightArr[eSafeLevel::eNolink].c_str());
			}
			else
			{
				eSafeLevel level = MAN->GetSafeLevel((eDeviceType)pLight->GetTag(),info);
				pLight->SetBkImage(lightArr[level].c_str());
			}
			break;
		}
	}
}
