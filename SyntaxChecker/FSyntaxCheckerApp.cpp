//---------------------------------------------------------------------------
//
// Name:        GASViewerApp.cpp
// Author:      David Márquez de la Cruz
// Created:
// Description: 
//
//---------------------------------------------------------------------------

#include "FSyntaxCheckerApp.h"
#include "FSyntaxCheckerGUI.h"

IMPLEMENT_APP(CSyntaxCheckerApp)

bool CSyntaxCheckerApp::OnInit()
{
	// Install all possible image handlers
    wxInitAllImageHandlers();

    CSyntaxCheckerGUI* frame = new CSyntaxCheckerGUI(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int CSyntaxCheckerApp::OnExit()
{
	return 0;
}
