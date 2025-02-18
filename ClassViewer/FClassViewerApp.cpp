//---------------------------------------------------------------------------
//
// Name:        GASViewerApp.cpp
// Author:      David Márquez de la Cruz
// Created:
// Description: 
//
//---------------------------------------------------------------------------

#include "FClassViewerApp.h"
#include "FClassViewerGUI.h"

IMPLEMENT_APP(CClassViewerApp)

bool CClassViewerApp::OnInit()
{
	// Install all possible image handlers
    wxInitAllImageHandlers();

    CClassViewerGUI* frame = new CClassViewerGUI(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int CClassViewerApp::OnExit()
{
	return 0;
}
