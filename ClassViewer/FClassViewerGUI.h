//---------------------------------------------------------------------------
//
// Name:        FGASViewerGUI.h
// Author:      David Márquez de la Cruz
// Created:
// Description: CClassViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef __CClassViewerGUI_h__
#define __CClassViewerGUI_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/treectrl.h>
#include <wx/statbox.h>
////Header Include End

////Dialog Style Start
#undef CClassViewerGUI_STYLE
#define CClassViewerGUI_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class CClassNode;
class CClassViewerGUI : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		CClassViewerGUI(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("GASViewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = CClassViewerGUI_STYLE);
		virtual ~CClassViewerGUI();
		void MnuopenGASfile1001Click(wxCommandEvent& event);
		void Mnusavegemfile1002Click(wxCommandEvent& event);
		void Mnuquit1004Click(wxCommandEvent& event);
		void Mnufile1005Click(wxCommandEvent& event);
		void MnuopenGTSfile1005Click(wxCommandEvent& event);
		void MnuopenGASfile1006Click(wxCommandEvent& event);
		void WxTimer1Timer(wxTimerEvent& event);
		void Mnuloadgemfile1009Click(wxCommandEvent& event);
		void cb_MaterialListSelected(wxCommandEvent& event );
		void WxstaticClick(wxCommandEvent& event);
		void WxButton1Click(wxCommandEvent& event);
		void WxstaticClick0(wxCommandEvent& event);
		void Mnuopengtsfile1005Click0(wxCommandEvent& event);
		void Click(wxCommandEvent& event);
		
	private:
		//Do not add custom control declarations between
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxMenuBar *WxMenuBar1;
		wxFileDialog *dlg_OpenH;
		wxTreeCtrl *tv_ClassView;
		wxStaticBox *WxStaticBox3;
		////GUI Control Declaration End
		
	private:
		// void OnPaint(wxPaintEvent& WXUNUSED(event) );
    
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_MNU_FILEMENU_1001 = 1001,
			ID_MNU_OPENHEADERFILE_1005 = 1005,
			ID_MNU_SELECTDIR_1010 = 1010,
			ID_MNU_QUIT_1004 = 1004,
			
			ID_TV_CLASSVIEW = 1058,
			ID_WXSTATICBOX3 = 1028,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
		
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();		
		void ShowInfo(CClassNode* _poClass);
		void ShowStruct(wxTreeItemId _oItemId,CClassNode* _poClass);
		void ShowClassMembers(wxTreeItemId _oItemId,CClassNode* _poClass);
		void ShowEnum(wxTreeItemId _oItemId,CClassNode* _poClass);
		void ShowClass(wxTreeItemId _oItemId,CClassNode* _poClass);	
		void ShowFunction(wxTreeItemId _oItemId,CClassNode* _poClass);
		wxTreeItemId AppendItem(wxTreeItemId _oItemId,CClassNode* _poClass);
		void CreateImageList();
		
		wxDirDialog* dlg_SelectDir;
};

#endif
