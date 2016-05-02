#include "StdAfx.h"
#include <exdisp.h>
#include <comdef.h>
#include "ControlEx.h"
#include "resource.h"
#include <ShellAPI.h>
#include "SkinFrame.h"

#include "MainWnd.h"
#include "PopWnd.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	
	HRESULT hRes = ::OleInitialize(NULL);

	AllocConsole();
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);

	CDemoFrame* pFrame = new CDemoFrame();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("compareTool"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	pFrame->ShowModal();
	//::ShowWindow(*pFrame, SW_SHOW);
	//CPaintManagerUI::MessageLoop();


	CResourceManager::GetInstance()->Release();

	OleUninitialize();
	::CoUninitialize();
	return 0;
}