#pragma once
#include "PopWnd.h"
#include "ShlObj.h"
#include "MsgWnd.h"
#include "ControlEx.h"

#include <iostream>
#include <algorithm>
#include "ReportReader.h"
#include "ReportEntity.h"
#include "ReportReader.h"
#include "ReportCompare.h"

class CDemoFrame : public WindowImplBase, public CWebBrowserEventHandler, public SkinChangedReceiver, public IListCallbackUI
{
public:
	CDemoFrame() 
	{
		m_pMenu = NULL;
	}

public:
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if(lstrcmpi(pstrClass, _T("CircleProgress" )) == 0) {
			return new CCircleProgressUI();
		}
		return NULL;
	}
	// 初始化资源管理器
	void InitResource()
	{
		// 多语言接口
		CResourceManager::GetInstance()->SetTextQueryInterface(this);
		// 注册控件
		REGIST_DUICONTROL(CCircleProgressUI);
		REGIST_DUICONTROL(CChartViewUI);
		// 加载资源
		if (GetResourceType() == UILIB_RESOURCE)
		{
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
		}
		else {
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
		}	
	}
	void InitWindow() 
	{
		CSkinManager::GetSkinManager()->AddReceiver(this);

		m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
		m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
		m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
		m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
		m_pSkinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("skinbtn")));
		m_pList = static_cast<CListUI*>(m_pm.FindControl(_T("listview")));
		m_edit1 = static_cast<CEditUI*>(m_pm.FindControl(_T("edit1")));
		m_edit2 = static_cast<CEditUI*>(m_pm.FindControl(_T("edit2")));
		
		m_pList->SetTextCallback(this);

// 		CListContainerElementUI* pListItem  = new CListContainerElementUI();
// 		pListItem->SetBorderColor(0xFF00FF00);
// 		pListItem->SetBorderSize(1);
// 		pListItem->SetBorderStyle(PS_DASH);
// 		pListItem->SetFixedHeight(20);
// 		pListItem->SetManager(&m_pm, NULL, false);
// 		m_pList->Add(pListItem);
	}

	void LoadFromFile(LPCTSTR pathI, LPCTSTR pathII)
	{
		
		ReportEntity e1, e0;
		ReportReader::read(pathI, e0);
		ReportReader::read(pathII, e1);

		m_entity.init();
		ReportCompare::compare(e0, e1, m_entity);
		//m_entity.print();

		CListHeaderUI* header = m_pList->GetHeader();		
		header->RemoveAll();	
		m_pList->RemoveAll();

		for(int i = 0; i < m_entity.getFieldCount(); ++i){
			CListHeaderItemUI* header_text = new CListHeaderItemUI();
			header_text->SetText(m_entity.getFieldName(i).c_str());
			header_text->SetAttribute("align", "center");
			header_text->SetPushedImage(_T("list_header_pushed.png"));
			header_text->SetSepImage(_T("list_header_sep.png"));
			header_text->SetHotImage(_T("list_header_hot.png"));
			header_text->SetSepWidth(1);
			header_text->SetFixedWidth(100);
			header_text->SetMinWidth(0);
			header_text->SetShowHtml();
			header_text->SetTag(i);
			header->Add(header_text);
		}

		for(int i=0; i< m_entity.getLineCount(); ++i){
			CListTextElementUI* pListElement = new CListTextElementUI;
			if (pListElement != NULL)
			{
				pListElement->SetTag(i);
				m_pList->Add(pListElement);
			}
		}

		m_pList->NeedUpdate();
	}

	LPCTSTR	GetItemText	(CControlUI* pControl, int iIndex, int iSubItem)
	{
		if (iIndex >= this->m_entity.getLineCount())
			return NULL;

		ReportEntity::ReportRow& row = this->m_entity.getContentRow(iIndex);
		if(iSubItem >= row.size())
			return NULL;

		const std::string showText = row.at(iSubItem)->toString();
		static TCHAR szBuf[MAX_PATH] = {0};
		memset(szBuf, 0, sizeof(szBuf));
		sprintf(szBuf, showText.c_str());
		return szBuf;
	}

	virtual BOOL Receive(SkinChangedParam param)
	{
		CControlUI* pRoot = m_pm.FindControl(_T("root"));
		if( pRoot != NULL ) {
			if( param.bColor ) {
				pRoot->SetBkColor(param.bkcolor);
				pRoot->SetBkImage(_T(""));
			}
			else {
				pRoot->SetBkColor(0);
				pRoot->SetBkImage(param.bgimage);
				m_pm.SetLayeredImage(param.bgimage);
			}
		}
		return TRUE;
	}

	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void)
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE GetHostInfo(CWebBrowserUI* pWeb, 
		/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
	{
		if (pInfo != NULL) {
			pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_NO3DOUTERBORDER;
		}
		return S_OK;
	}
public:

	DuiLib::CDuiString GetSkinFolder()
	{
#ifdef _DEBUG
		return _T("skin\\duidemo\\");
#else
		return _T("skin\\");
#endif
	}

	DuiLib::CDuiString GetSkinFile()
	{
		return _T("XML_MAIN");
	}

	UILIB_RESOURCETYPE GetResourceType() const
	{
#ifdef _DEBUG
		return UILIB_FILE;
#else
		return UILIB_ZIPRESOURCE;
#endif
	}

	LPCTSTR GetResourceID() const
	{
		return _T("IDR_ZIPRES");
	}

	DuiLib::CDuiString GetZIPFileName() const
	{
		return _T("skin.zip");
	}


	LPCTSTR GetWindowClassName() const
	{ 
		return _T("MainWnd");
	}

	UINT GetClassStyle() const
	{ 
		return CS_DBLCLKS; 
	}

	void OnFinalMessage(HWND hWnd)
	{ 
		delete this;
	}

	LPCTSTR QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType)
	{
		CDuiString sLanguage = CResourceManager::GetInstance()->GetLanguage();
		if(sLanguage == _T("en")){
			if(lstrcmpi(lpstrId, _T("lantext")) == 0) {
				return _T("Switch to Chinese");
			}
			else if(lstrcmpi(lpstrId, _T("titletext")) == 0) {
				return _T("Duilib Demo v1.1");
			}
			else if(lstrcmpi(lpstrId, _T("hometext")) == 0) {
				return _T("{a}Home Page{/a}");
			}
		}
		else{
			if(lstrcmpi(lpstrId, _T("lantext")) == 0) {
				return _T("切换到英文");
			}
			else if(lstrcmpi(lpstrId, _T("titletext")) == 0) {
				return _T("Duilib 使用演示 v1.1");
			}
			else if(lstrcmpi(lpstrId, _T("hometext")) == 0) {
				return _T("{a}演示官网{/a}");
			}
		}

		return NULL;
	}

	void Notify(TNotifyUI& msg)
	{
if( msg.sType == _T("click") )
		{
			if( msg.pSender == m_pCloseBtn ) 
			{
				if(IDYES == MessageBox(m_hWnd, _T("确定退出duidemo演示程序？"), _T("Duilib旗舰版"), MB_YESNO))
				{
					PostQuitMessage(0);
				}
				return; 
			}
			else if( msg.pSender == m_pMinBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
			else if( msg.pSender == m_pMaxBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
			else if( msg.pSender == m_pRestoreBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
			else if( msg.pSender == m_pSkinBtn ) {
				new CSkinFrame(m_hWnd, m_pSkinBtn);
			}
			// 按钮消息
			OnLClick(msg.pSender);
		}
		else if(msg.sType==_T("selectchanged"))
		{
			CDuiString name = msg.pSender->GetName();
			CTabLayoutUI* pTabSwitch = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_switch")));

			if(name.CompareNoCase(_T("basic_tab")) == 0) pTabSwitch->SelectItem(0);
			if(name.CompareNoCase(_T("rich_tab")) == 0) pTabSwitch->SelectItem(1);
			if(name.CompareNoCase(_T("ani_tab")) == 0) pTabSwitch->SelectItem(2);
			if(name.CompareNoCase(_T("split_tab")) == 0) pTabSwitch->SelectItem(3);
		}
		else if(msg.sType == _T("valuechanged"))
		{
			CProgressUI* pSlider = static_cast<CProgressUI*>(m_pm.FindControl(_T("slider")));
			CProgressUI* pPro1 = static_cast<CProgressUI*>(m_pm.FindControl(_T("progress")));
			CProgressUI* pPro2 = static_cast<CProgressUI*>(m_pm.FindControl(_T("circle_progress")));
			pPro1->SetValue(pSlider->GetValue());
			pPro2->SetValue(pSlider->GetValue());
		}
		else if(msg.sType == _T("headerclick"))
		{
			CDuiString name = msg.pSender->GetName();
			int tag = msg.pSender->GetTag();

			int sortTag;
			bool sortAsc;
			ReportEntity::getSortType(sortTag, sortAsc);

			if(tag != sortTag){
				sortAsc = true;
				CControlUI * pre_column = m_pList->GetHeader()->GetItemAt(sortTag);
				pre_column->SetText(m_entity.getFieldName(sortTag).c_str());
				
				std::string token = "<i arrowup.png>";
				msg.pSender->SetText((m_entity.getFieldName(tag)+token).c_str());				
			}
			else{
				sortAsc = !sortAsc;
				std::string token = sortAsc ? "<i arrowup.png>" : "<i arrowdown.png>";
				msg.pSender->SetText((m_entity.getFieldName(tag)+token).c_str());				
			}				

			ReportEntity::setSortType(tag, sortAsc);
			std::sort(m_entity.m_content.begin(), m_entity.m_content.end(), &ReportEntity::compareRow);

			m_pList->NeedUpdate();
		}
		



	}
	void OnLClick(CControlUI *pControl)
	{
		CDuiString sName = pControl->GetName();
		if(sName.CompareNoCase(_T("homepage_btn")) == 0)
		{
		}
		else if(pControl->GetName() == _T("Btn_Left"))
		{
			CButtonUI* btnLeft= static_cast<CButtonUI*>(m_pm.FindControl(_T("Btn_Left")));
			CButtonUI* btnRight= static_cast<CButtonUI*>(m_pm.FindControl(_T("Btn_Right")));
			CVerticalLayoutUI* config_panel = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("config_panel")));

			btnLeft->SetVisible(false);
			btnRight->SetVisible(true);
			config_panel->SetVisible(false);
		}

		else if(pControl->GetName() == _T("Btn_Right"))
		{
			CButtonUI* btnLeft= static_cast<CButtonUI*>(m_pm.FindControl(_T("Btn_Left")));
			CButtonUI* btnRight= static_cast<CButtonUI*>(m_pm.FindControl(_T("Btn_Right")));
			CVerticalLayoutUI* config_panel = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("config_panel")));

			btnLeft->SetVisible(true);
			btnRight->SetVisible(false);
			config_panel->SetVisible(true);
		}
		else if(pControl->GetName() == _T("fresh"))
		{
			LPCTSTR text1 = m_edit1->GetText().GetData();
			LPCTSTR text2 = m_edit2->GetText().GetData();

			LoadFromFile(text1, text2);
		}
	}

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) {
			if( !bZoomed ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 关闭窗口，退出程序
		if(uMsg == WM_DESTROY)
		{
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		else if (uMsg == WM_MENUCLICK)
		{
			MenuCmd* pMenuCmd = (MenuCmd*)wParam;
			if(pMenuCmd != NULL)
			{
				BOOL bChecked = pMenuCmd->bChecked;
				CDuiString sMenuName = pMenuCmd->szName;
				CDuiString sUserData = pMenuCmd->szUserData;
				CDuiString sText = pMenuCmd->szText;
				m_pm.DeletePtr(pMenuCmd);

				if(sMenuName.CompareNoCase(_T("lan")) == 0)
				{
					static bool bEn = false;
					if(!bEn) {
						CResourceManager::GetInstance()->SetLanguage(_T("en"));
					}
					else {
						CResourceManager::GetInstance()->SetLanguage(_T("cn_zh"));					
					}
					bEn = !bEn;
					CResourceManager::GetInstance()->ReloadText();
					InvalidateRect(m_hWnd, NULL, TRUE);
					m_pm.NeedUpdate();
				}
				else if (sMenuName == _T("qianting"))
				{
					if (bChecked)
					{
						CMsgWnd::MessageBox(m_hWnd, NULL, _T("你预定修潜艇服务"));
					} 
					else
					{
						CMsgWnd::MessageBox(m_hWnd, NULL, _T("你取消修潜艇服务"));
					}			 
				}
				else
				{
					CMsgWnd::MessageBox(m_hWnd, NULL, sText);
				}
			}
			if(m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			bHandled = TRUE;
			return 0;
		}
		bHandled = FALSE;
		return 0;
	}

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pSkinBtn;
	CMenuWnd* m_pMenu;
	CListUI* m_pList;
	CEditUI* m_edit1;
	CEditUI* m_edit2;

	std::map<CDuiString, bool> m_MenuInfos;
	ReportEntity m_entity;
	
};

