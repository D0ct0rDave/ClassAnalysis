//---------------------------------------------------------------------------
//
// Name:        GASViewerApp.h
// Author:      David Márquez de la Cruz
// Created:
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __CClassViewerApp_h__
#define __CClassViewerApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class CClassViewerApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
