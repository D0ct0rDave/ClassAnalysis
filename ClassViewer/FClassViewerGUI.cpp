//---------------------------------------------------------------------------
//
// Name:        FGASViewerGUI.cpp
// Author:      David Márquez de la Cruz
// Created:
// Description: CClassViewerGUI class implementation
//
//---------------------------------------------------------------------------

#include "FClassViewerGUI.h"
#include "CClassAnalyzer.h"
#include <wx/dirdlg.h>
#include <vector>
#include <ctype.h>

#include "images/icons.xpm"
#include "CCAClassDB.h"

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
void CClassViewerGUI::CreateImageList()
{
	const uint ICON_WIDTH  = 16;
	const uint ICON_HEIGHT = 16;
	//
	// Make an image list containing small icons
    wxImageList *images = new wxImageList(ICON_WIDTH,ICON_HEIGHT,true);

    // should correspond to TreeCtrlIcon_xxx enum
    wxBusyCursor wait;
    wxIcon icons;
    icons = wxIcon(icons_xpm);

    uint uiTotalIcons = icons.GetWidth() / ICON_WIDTH;

    for ( uint i = 0; i < uiTotalIcons; i++ )
    {
		// Get a rect of the whole image
		wxRect oRect;
		oRect.SetTop(0);
		oRect.SetLeft(i*ICON_WIDTH);
		oRect.SetSize(wxSize(ICON_WIDTH,ICON_HEIGHT));

        images->Add(wxBitmap( wxBitmap(icons).ConvertToImage().GetSubImage(oRect) ) );
    }

    tv_ClassView->AssignImageList(images);
}
//----------------------------------------------------------------------------
// CSyntaxCheckerGUI
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(CClassViewerGUI,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(CClassViewerGUI::OnClose)
	EVT_MENU(ID_MNU_OPENHEADERFILE_1005, CClassViewerGUI::Mnuopengtsfile1005Click0)
	EVT_MENU(ID_MNU_SELECTDIR_1010, CClassViewerGUI::Click)
	EVT_MENU(ID_MNU_QUIT_1004, CClassViewerGUI::Mnuquit1004Click)
END_EVENT_TABLE()
////Event Table End
//---------------------------------------------------------------------------
CClassViewerGUI::CClassViewerGUI(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	tv_ClassView = NULL;
	CreateGUIControls();
}
//---------------------------------------------------------------------------
CClassViewerGUI::~CClassViewerGUI()
{
    tv_ClassView->DeleteAllItems();	
}
//---------------------------------------------------------------------------
void CClassViewerGUI::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxMenuBar1 = new wxMenuBar();
	wxMenu *ID_MNU_FILEMENU_1001_Mnu_Obj = new wxMenu(0);
	ID_MNU_FILEMENU_1001_Mnu_Obj->Append(ID_MNU_OPENHEADERFILE_1005, wxT("Open Header File"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILEMENU_1001_Mnu_Obj->Append(ID_MNU_SELECTDIR_1010, wxT("Select Directory to Analyze"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILEMENU_1001_Mnu_Obj->AppendSeparator();
	ID_MNU_FILEMENU_1001_Mnu_Obj->Append(ID_MNU_QUIT_1004, wxT("Quit"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_FILEMENU_1001_Mnu_Obj, wxT("File"));
	SetMenuBar(WxMenuBar1);

	dlg_OpenH =  new wxFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.h"), wxOPEN);

	tv_ClassView = new wxTreeCtrl(this, ID_TV_CLASSVIEW, wxPoint(48, 24), wxSize(513, 473), wxTR_HAS_BUTTONS, wxDefaultValidator, wxT("tv_ClassView"));

	WxStaticBox3 = new wxStaticBox(this, ID_WXSTATICBOX3, wxT("WxStaticBox3"), wxPoint(728, -132), wxSize(223, 126));

	SetTitle(wxT("Class Analyzer"));
	SetIcon(wxNullIcon);
	SetSize(8,8,611,560);
	Center();


	////GUI Items Creation End
	// This is debug code, just to test the app.		

	/*
	CClass* poClass = CClassAnalyzer::poLoadClass("CClassAnalyzer.h");
	if (poClass) 
	    ShowInfo(poClass);
    */

    // Creates the image list we will use for the class viewer
    CreateImageList();

	dlg_SelectDir = new wxDirDialog(this,wxT("Select a directory to parse files:"));
}
//---------------------------------------------------------------------------
void CClassViewerGUI::OnClose(wxCloseEvent& event)
{
	Destroy();
}
//---------------------------------------------------------------------------
/*
 * Mnuquit1004Click
 */
void CClassViewerGUI::Mnuquit1004Click(wxCommandEvent& event)
{
	// insert your code here
	exit(0);
}
//---------------------------------------------------------------------------
CCAString sGetType(CClassNode* _poClass)
{
	switch (_poClass->m_eType)
	{
		case NT_DIRECTORY:
		    return("directory");
		break;

		case NT_FILE:
		    return("file");
		break;

		case NT_NAMESPACE:
		    return("namespace");
		break;

		case NT_CLASS:
		    return("class");
		break;

		case NT_ENUM:
		    return("enum");
		break;

		case NT_STRUCT:
		    return("struct");
		break;

		case NT_POD:
		    return("POD");
		break;

		case NT_CONSTRUCTOR:
		    return("constructor");
		break;

		case NT_DESTRUCTOR:
		    return("destructor");
		break;

		case NT_METHFUNC:
		    return("method");
		break;

		case NT_UNKNOWN:
		    return("unknown");
		break;

		default:
			return("");
	    break;
	}
	
	return("");
}
//---------------------------------------------------------------------------
wxTreeItemId CClassViewerGUI::AppendItem(wxTreeItemId _oItemId,CClassNode* _poClass)
{
	const int IMAGE_IDX[NT_NUMMTs] = {
			10, // NT_NONE,
			
			7,	// NT_DIRECTORY,
			6,	// NT_FILE,

			8,	// NT_NAMESPACE,
			2,	// NT_CLASS,
			3,	// NT_STRUCT,
			4,	// NT_ENUM,

			5,	// NT_ENUM_MEMBER,
			0,	// NT_METHFUNC,
			0,	// NT_CONSTRUCTOR,
			0,	// NT_DESTRUCTOR,
			1,	// NT_POD,

			9, // NT_UNKNOWN,
	};

	CCAString sStr = "";
	CCAString sDataType = sGetType(_poClass);

	if (_poClass->m_sScope != "") sStr = _poClass->m_sScope;
	if (sDataType != "") sStr = sStr + " " + sDataType;
	if (_poClass->m_sType != "") sStr = sStr + " " + _poClass->m_sType;
	if (_poClass->m_sName != "") sStr = sStr + " " + _poClass->m_sName;

	wxString str( sStr.szString(), wxConvUTF8 );

	return( tv_ClassView->AppendItem(_oItemId,str,IMAGE_IDX[_poClass->m_eType] ) );
}
//---------------------------------------------------------------------------
void CClassViewerGUI::ShowClassMembers(wxTreeItemId _oItemId,CClassNode* _poClass)
{
	// Build this object description
	wxTreeItemId oItemId = AppendItem(_oItemId,_poClass);

	for (uint i=0;i<_poClass->m_oMembers.size();i++)
	{
		CClassNode* poMember = _poClass->m_oMembers[i];
		ShowClassMembers(oItemId,poMember);
	};
}
//---------------------------------------------------------------------------
void CClassViewerGUI::ShowInfo(CClassNode* _poClass)
{
	char szStr[1024];

	sprintf(szStr,_poClass->m_sFilename.szString());
	wxString str( szStr, wxConvUTF8 );
    tv_ClassView->DeleteAllItems();
	wxTreeItemId oFileItemId = tv_ClassView->AppendItem(tv_ClassView->GetRootItem(), str);
	
	ShowClassMembers(oFileItemId,_poClass);
}
//---------------------------------------------------------------------------
/*
 * Mnuopengtsfile1005Click0
 */
void CClassViewerGUI::Mnuopengtsfile1005Click0(wxCommandEvent& event)
{
	// insert your code here
	if (dlg_OpenH->ShowModal() == wxID_OK)
	{
		wxString wxsFilename = dlg_OpenH->GetFilename();
		wxString wxsDirectory = dlg_OpenH->GetDirectory();
		wxString wxsFullFilename = wxsDirectory + wxT("\\") + wxsFilename;
		
		CClassNode* poClass = CClassAnalyzer::poLoadClass( CCAString(wxsFullFilename.char_str()) );

		if (poClass != NULL)
		    ShowInfo(poClass);
	}
}
// ----------------------------------------------------------------------------
bool bIsHeaderFile(const CCAString& _sFilename)
{
	// TODO P2: add support for hpp,hxx, etc...
    CCAString sStr = ((CCAString)_sFilename).SubString( _sFilename.size()-2,2);
    return( stricmp(sStr.szString(),".h") == 0 );

//    return(false);
}
/*
char* w2sz(WCHAR* str)
{
    static wxString wxS;
    wxS.copy((wxChar*)str,wcslen(str));
    return(wxS.char_str());
}
*/
// ----------------------------------------------------------------------------
// Resolve possible types that at the time of parsing were unknown, but possibly
// at the end of the process should not be unknown: For instance when a directory
// is searched recursively
// ----------------------------------------------------------------------------
void ResolveUnknowns(CClassNode* _poClass)
{
    if (_poClass->m_eType == NT_UNKNOWN)
    {
        CClassNode* poClass = CCAClassDB::poFind( _poClass->m_sType );
        if (poClass != NULL)
        {
            _poClass->m_eType = poClass->m_eType;
        }
        
        // go to members
        for (uint i=0;i<_poClass->m_oMembers.size();i++)
            ResolveUnknowns(_poClass->m_oMembers[i]);
    }
}
// ----------------------------------------------------------------------------
// Adds classes from header files files from a given directory recursively.
// In:      asRelDir, the relative directory from which files belong
// In/Out: _poClass parent class to append recursively the found classes 
// ----------------------------------------------------------------------------
void ScanRecursivelyDirectory(const wxString& _wxsRelDir,CClassNode* _poClass)
{
	struct TSearchRec
	{
		WIN32_FIND_DATA			FindData;
		HANDLE					hFind;	
	} sr;

	std::vector <wxString>		oSubDirs;

	// ------------------------------------
	// Paser files inside the current dir
	// ------------------------------------
	oSubDirs.clear();

	if ((sr.hFind = FindFirstFile(_T("*.*"), &sr.FindData)) != 0)
	{
		do{
		    wxString wxsFilename = sr.FindData.cFileName;
            CCAString sFilename = CCAString( wxsFilename.char_str() );

	    	CCAString oLog = CCAString("Processing file ") + sFilename;
	    	
	    	#ifdef WIN32
    		OutputDebugStringA(oLog.szString());
    		#endif

			if (! (sr.FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (bIsHeaderFile( sFilename ))
				{
				    CClassNode* poClass = CClassAnalyzer::poLoadClass( sFilename );
					_poClass->m_oMembers.push_back(poClass);
					poClass->m_poParent = _poClass;
				}
			}
			else
			{
				if ((sFilename != ".") && (sFilename != ".."))
					oSubDirs.push_back(sr.FindData.cFileName);
			}

		}while (FindNextFile(sr.hFind,&sr.FindData) != 0);

		FindClose(sr.hFind);
	}

	// --------------------------------
	// For each subdirectory, recurse
	// --------------------------------
	for (uint i=0;i<oSubDirs.size();i++)
	{
	    CClassNode* poClass	= new CClassNode;
	    poClass->m_sFilename= CCAString(oSubDirs[i].char_str());
		poClass->m_sName    = poClass->m_sFilename;
	    poClass->m_eType    = NT_DIRECTORY;
	    poClass->m_poParent = _poClass;
	    
	    // add current directory to parent class
	    _poClass->m_oMembers.push_back(poClass);

		CCAString oLog = CCAString("Recursing ") + CCAString(oSubDirs[i].char_str()) + " directory\n";

		#ifdef WIN32
		OutputDebugStringA(oLog.szString());
		#endif

		SetCurrentDirectory( oSubDirs[i] );
		ScanRecursivelyDirectory(_wxsRelDir + oSubDirs[i] + wxT("/") ,poClass);
		SetCurrentDirectory(wxT(".."));
	}
}
//---------------------------------------------------------------------------
/*
 * Click
 */
void CClassViewerGUI::Click(wxCommandEvent& event)
{
	// insert your code here
	// Recursively scan all header files from withing a given directory
	if (dlg_SelectDir->ShowModal() == wxID_OK)
	{
	    wxString wxsDirectory = dlg_SelectDir->GetPath();		

	    // insert your code here
	    WCHAR szCurDir[1024];
	    GetCurrentDirectory(1024,szCurDir);
	    SetCurrentDirectory( wxsDirectory );

	    CClassNode* poClass		 = new CClassNode;   
	    poClass->m_sFilename = CCAString(wxsDirectory.char_str());
	    poClass->m_sName     = poClass->m_sFilename;
	    poClass->m_eType     = NT_DIRECTORY;
        
        ScanRecursivelyDirectory(wxsDirectory,poClass);

        // Resolve unknown references in case they've been resolved somewhere
        ResolveUnknowns(poClass);

        ShowInfo(poClass);
        
        SetCurrentDirectory( szCurDir );
	}
}
//---------------------------------------------------------------------------
