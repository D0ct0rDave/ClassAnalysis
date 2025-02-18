//---------------------------------------------------------------------------
//
// Name:        FGASViewerGUI.cpp
// Author:      David Márquez de la Cruz
// Created:
// Description: CSyntaxCheckerGUI class implementation
//
// TODO P2: Automatic Search/Replace in files
// TODO P1 <30-Nov-2009>: Add system log info to get all the info about failing clases in a textual way.
// DONE<30-Nov-2009>
// TODO P1 <30-Nov-2009>: Eliminate any reference to GammaE internal to this file.
// DONE<02-Dic-2009>
// TODO P1 <01-Dic-2009>: Review the class viewer application
// TODO P1 <01-Dic-2009>: Read config from file correctly
// DONE<02-Dic-2009>
//---------------------------------------------------------------------------
#include "FSyntaxCheckerGUI.h"

#include "CClassAnalyzer.h"
#include "CATypes.h"
#include "CAUtils.h"
#include "CCAClassDB.h"
#include "deelx.h"

#include <wx/dirdlg.h>
#include <vector>
#include <ctype.h>

#include "images/icons.xpm"

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End
//---------------------------------------------------------------------------
class CAppGlobals
{
    public:
        CCALookupArray<CCAString> m_oSyntaxRules;
};

static CAppGlobals globals;
//----------------------------------------------------------------------------
// Read and Parse a file with Hungarian Notation rules
//----------------------------------------------------------------------------
void ReadSyntaxRules(const CCAString& _sFilename)
{
    //
    char* szFileDef = pucReadFile( _sFilename.szString() );
    
    // Check for correct file definition
    if (szFileDef == NULL)
        return;
    
    char* szStream = szFileDef;
    char* szLine   = szStream;

    // Clear comments
    PreprocessComments(szStream);

	// Parse variables
	MatchResult result;
	CRegexpT<char> oRegExp("^[\\t ]*(\\w+|[\\w\\t ]+)[\\t ]*=[\\t ]*(\\w*)[\\t ]*$",MULTILINE);
	result = oRegExp.Match( szStream );

	while (result.IsMatched())
	{
		int iStart,iEnd;

		// Group 0 is the full parsed sentence

		// Retrieve variable name
        CCAString sVar;
		iStart = result.GetGroupStart(1);
		iEnd   = result.GetGroupEnd(1);
        sVar = sBuildToken(szStream, iStart, iEnd-iStart);

		// Retrieve value
		iStart = result.GetGroupStart(2);
		iEnd   = result.GetGroupEnd(2);

		if (iStart > -1)
		{
		    CCAString sValue = sBuildToken(szStream, iStart, iEnd-iStart);
			globals.m_oSyntaxRules.Add(sVar,sValue);
		}
		else
			globals.m_oSyntaxRules.Add(sVar,"");

		// find next
		result = oRegExp.Match(szStream, result.GetEnd());
	}

	// free the buffer
	free(szFileDef);
}
//----------------------------------------------------------------------------
void CSyntaxCheckerGUI::CreateImageList()
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
BEGIN_EVENT_TABLE(CSyntaxCheckerGUI,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(CSyntaxCheckerGUI::OnClose)
	EVT_SIZE(CSyntaxCheckerGUI::CSyntaxCheckerGUISize)
	EVT_MENU(ID_MNU_OPENHEADERFILE_1005, CSyntaxCheckerGUI::Mnuopengtsfile1005Click0)
	EVT_MENU(ID_MNU_SELECTDIR_1010, CSyntaxCheckerGUI::Click)
	EVT_MENU(ID_MNU_QUIT_1004, CSyntaxCheckerGUI::Mnuquit1004Click)
	EVT_MENU(ID_MNU_EXPANDTREE_1060, CSyntaxCheckerGUI::Mnuexpandtree1060Click)
END_EVENT_TABLE()
////Event Table End
//---------------------------------------------------------------------------
CSyntaxCheckerGUI::CSyntaxCheckerGUI(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	tv_ClassView = NULL;
	CreateGUIControls();
}
//---------------------------------------------------------------------------
CSyntaxCheckerGUI::~CSyntaxCheckerGUI()
{
    tv_ClassView->DeleteAllItems();	
}
//---------------------------------------------------------------------------
void CSyntaxCheckerGUI::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	mem_Log = new wxTextCtrl(this, ID_MEM_LOG, wxEmptyString, wxPoint(0, 464), wxSize(601, 185), wxTE_MULTILINE, wxDefaultValidator, wxT("mem_Log"));
	mem_Log->SetMaxLength(0);
	mem_Log->SetFocus();
	mem_Log->SetInsertionPointEnd();

	WxMenuBar1 = new wxMenuBar();
	wxMenu *ID_MNU_FILEMENU_1001_Mnu_Obj = new wxMenu(0);
	ID_MNU_FILEMENU_1001_Mnu_Obj->Append(ID_MNU_OPENHEADERFILE_1005, wxT("Open Header File"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILEMENU_1001_Mnu_Obj->Append(ID_MNU_SELECTDIR_1010, wxT("Select Directory to Analyze"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILEMENU_1001_Mnu_Obj->AppendSeparator();
	ID_MNU_FILEMENU_1001_Mnu_Obj->Append(ID_MNU_QUIT_1004, wxT("Quit"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_FILEMENU_1001_Mnu_Obj, wxT("File"));
	
	wxMenu *ID_MNU_OPTIONS_1059_Mnu_Obj = new wxMenu(0);
	ID_MNU_OPTIONS_1059_Mnu_Obj->Append(ID_MNU_EXPANDTREE_1060, wxT("Expand Tree"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_OPTIONS_1059_Mnu_Obj, wxT("Options"));
	SetMenuBar(WxMenuBar1);

	dlg_OpenH =  new wxFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.h"), wxOPEN);

	tv_ClassView = new wxTreeCtrl(this, ID_TV_CLASSVIEW, wxPoint(0, 0), wxSize(601, 457), wxTR_HAS_BUTTONS, wxDefaultValidator, wxT("tv_ClassView"));

	WxStaticBox3 = new wxStaticBox(this, ID_WXSTATICBOX3, wxT("WxStaticBox3"), wxPoint(728, -132), wxSize(223, 126));

	SetTitle(wxT("Class Analyzer"));
	SetIcon(wxNullIcon);
	SetSize(8,8,611,687);
	Center();
	
	////GUI Items Creation End
	// This is debug code, just to test the app.		

	/*
	CClassNode* poClass = CClassAnalyzer::poLoadClass("../ClassAnalyzer/CClassAnalyzer.h");
	if (poClass) 
	    ShowInfo(poClass);
    */

    // Creates the image list we will use for the class viewer
    CreateImageList();

	dlg_SelectDir = new wxDirDialog(this,wxT("Select a directory to parse files:"));
	ReadSyntaxRules("notation.txt");
}
//---------------------------------------------------------------------------
void CSyntaxCheckerGUI::OnClose(wxCloseEvent& event)
{
	Destroy();
}
//---------------------------------------------------------------------------
/*
 * Mnuquit1004Click
 */
void CSyntaxCheckerGUI::Mnuquit1004Click(wxCommandEvent& event)
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
wxTreeItemId CSyntaxCheckerGUI::AppendItem(wxTreeItemId _oItemId,CClassNode* _poClass)
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
void CSyntaxCheckerGUI::LogClass(CClassNode* _poClass)
{	
	if (_poClass)
	{
		CCAString sStr = _poClass->m_sFilename + " (line: " + CCAString(_poClass->m_uiLine) +"): ";
		
		if (_poClass->m_poParent) 
		{
			if (_poClass->m_poParent->m_poParent)
			{
				switch (_poClass->m_poParent->m_poParent->m_eType)
				{
					case NT_CLASS:
						sStr += CCAString("In class ") + _poClass->m_poParent->m_poParent->m_sType + CCAString(", ");
					break;
					
					case NT_STRUCT:
						sStr += CCAString("In struct ") + _poClass->m_poParent->m_poParent->m_sType + CCAString(", ");
					break;
					
					case NT_NAMESPACE:
						sStr += CCAString("In namespace ") + _poClass->m_poParent->m_poParent->m_sType + CCAString(", ");
					break;
				}
			}

			switch (_poClass->m_poParent->m_eType)
			{
				case  NT_CLASS:
				{					
					if (_poClass->m_eType == NT_METHFUNC)
						sStr += CCAString("member method ") + _poClass->m_sName + CCAString(" is not correct.\n");
					else
						sStr += CCAString("member ")+ _poClass->m_sName + CCAString(" is not correct.\n");
				}
				break;

				case  NT_METHFUNC:
				{					
					sStr += CCAString("in method ") + _poClass->m_poParent->m_sName + CCAString(", parameter ") + _poClass->m_sName + CCAString(" is not correct.\n");
				}
				break;
				
				case  NT_CONSTRUCTOR:
				{					
					sStr += CCAString("in constructor, ") + _poClass->m_poParent->m_sName + CCAString(", parameter ") + _poClass->m_sName + CCAString(" is not correct.\n");
				}
				break;				
			}		
		}

		mem_Log->AppendText( wxString(sStr.szString(),wxConvUTF8) );
	}	
}
//---------------------------------------------------------------------------
void CSyntaxCheckerGUI::MarkItem(wxTreeItemId _oItemId,bool _bBold)
{
	tv_ClassView->SetItemTextColour(_oItemId, *wxRED);
	tv_ClassView->SetItemBold(_oItemId, _bBold);
}
//---------------------------------------------------------------------------
bool CSyntaxCheckerGUI::bShowClassMembers(wxTreeItemId _oItemId,CClassNode* _poClass)
{
	// Build this object description
	wxTreeItemId oItemId = AppendItem(_oItemId,_poClass);

	// Check the syntax of this object
	CCAString sPrefix = "";
	bool bNodeHasCorrectSyntax = true;

	if (_poClass->m_poParent)
	{   
		switch (_poClass->m_poParent->m_eType)
		{
			case NT_CONSTRUCTOR:
			case NT_METHFUNC:
			{
			    CCAString* psStr = globals.m_oSyntaxRules.poFind("function_param");
				if (psStr) sPrefix += *psStr;
			}
			break;

			case NT_CLASS:         
			{
				// if the member is an attribute it should contain the class attribute prefix
				if (
					(_poClass->m_sName != "")
				&&  (_poClass->m_eType != NT_METHFUNC) 
				&&  (_poClass->m_eType != NT_CONSTRUCTOR) 
				&&  (_poClass->m_eType != NT_DESTRUCTOR)
					)
				{ 
			       CCAString* psStr = globals.m_oSyntaxRules.poFind("class_attribute");
				   if (psStr) sPrefix += *psStr;
			    }
			}
			break;
		}
         
		if (_poClass->m_sName != "")
		{
			// check syntax
			CCAString* psStr = globals.m_oSyntaxRules.poFind(_poClass->m_sType);

			if (psStr)
			{
				// Get the prefix
				sPrefix += *psStr;
			}
			else
			// look if the type is a pointer to type
			{
				int iPos = _poClass->m_sType.iPos("*");				
				if ( iPos > -1)
				{
					sPrefix += "p";

					// Take out the "*" from the type
					CCAString sType = _poClass->m_sType.SubString(0,iPos);

					CCAString* psStr = globals.m_oSyntaxRules.poFind(_poClass->m_sType);
					if (psStr) sPrefix += *psStr;
				}
			}

			// if there is no prefix or if the prefix is reflected in the name ok else ...
			if ((sPrefix.szString() == NULL) || (_poClass->m_sName.iPos(sPrefix) == 0))
			{
				// ok
			}
			else
				bNodeHasCorrectSyntax = false;
		}
	}

	// Check the correctness of its members
	bool bAllMembersHaveCorrectSyntax = true;
	for (uint i=0;i<_poClass->m_oMembers.size();i++)
	{
		CClassNode* poMember = _poClass->m_oMembers[i];
		bAllMembersHaveCorrectSyntax = bAllMembersHaveCorrectSyntax & bShowClassMembers(oItemId,poMember);
	};

	// if the node itself has bad syntax
	if (! bNodeHasCorrectSyntax)
	{
		MarkItem(oItemId,true);
		LogClass(_poClass);
	}
	else 
		// if some of its members are not correct, mark this node as non syntax compliant
		if (!bAllMembersHaveCorrectSyntax)
			MarkItem(oItemId,false);
	
	// To be correct, the node itself and its children members should be correct
	return(bNodeHasCorrectSyntax && bAllMembersHaveCorrectSyntax);
}
//---------------------------------------------------------------------------
void CSyntaxCheckerGUI::ShowInfo(CClassNode* _poClass)
{
	char szStr[1024];

	sprintf(szStr,_poClass->m_sFilename.szString());
	wxString str( szStr, wxConvUTF8 );
	
	// Clear log
	mem_Log->Clear();

	// Delete everything possibly previously created
    tv_ClassView->DeleteAllItems();
	wxTreeItemId oFileItemId = AppendItem(tv_ClassView->GetRootItem(), _poClass);

	if (! bShowClassMembers(oFileItemId,_poClass))
	{
		// some member does not meet the specs
		MarkItem(oFileItemId,false);
	}
}
//---------------------------------------------------------------------------
/*
 * Mnuopengtsfile1005Click0
 */
void CSyntaxCheckerGUI::Mnuopengtsfile1005Click0(wxCommandEvent& event)
{
	// insert your code here
	if (dlg_OpenH->ShowModal() == wxID_OK)
	{
		wxString wxsFilename = dlg_OpenH->GetFilename();
		wxString wxsDirectory = dlg_OpenH->GetDirectory();
		
		wxString wxsFullFilename = wxsDirectory + wxT("\\") + wxsFilename;
		
		CClassNode* poClass = CClassAnalyzer::poLoadClass( CCAString(wxsFullFilename.char_str()) );

		// Delete everything possibly previously created
		tv_ClassView->DeleteAllItems();

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
	    CClassNode* poClass		= new CClassNode;
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
void CSyntaxCheckerGUI::Click(wxCommandEvent& event)
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
/*
 * Mnuexpandtree1060Click
 */
void CSyntaxCheckerGUI::Mnuexpandtree1060Click(wxCommandEvent& event)
{
	// insert your code here
	tv_ClassView->ExpandAll();
}
//---------------------------------------------------------------------------
/*
 * CSyntaxCheckerGUISize
 */
void CSyntaxCheckerGUI::CSyntaxCheckerGUISize(wxSizeEvent& event)
{
	// insert your code here
	if (tv_ClassView)
	{
		wxSize s = GetClientSize();
		wxPoint p;
		
		uint uiTVHeight = s.GetHeight() * 0.75f;
		uint uiRemainder = s.GetHeight() - uiTVHeight;
		
		p.x = 0;
		p.y = 0;
		s.SetHeight(uiTVHeight);

		tv_ClassView->SetPosition(p);
		tv_ClassView->SetSize(s);

		p.x = 0;
		p.y = uiTVHeight;
		s.SetHeight(uiRemainder);
		
		mem_Log->SetPosition(p);
		mem_Log->SetSize(s);
	}
}
//---------------------------------------------------------------------------
