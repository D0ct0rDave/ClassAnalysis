//---------------------------------------------------------------------------
//
// Name:        GASViewerApp.h
// Author:      David Márquez de la Cruz
// Created:
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __CSyntaxCheckerApp_h__
#define __CSyntaxCheckerApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class CSyntaxCheckerApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
