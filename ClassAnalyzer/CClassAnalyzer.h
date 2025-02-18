//---------------------------------------------------------------------------
//
// Name:        CClassAnalyzer.h
// Author:      David Márquez de la Cruz
// Created:		28-Nov-2009
// Description: CClassAnalyzer class Declaration
//
//---------------------------------------------------------------------------
#ifndef CClassAnalyzerH
#define CClassAnalyzerH
//---------------------------------------------------------------------------

#include "CATypes.h"
#include "CCAClassNode.h"

//---------------------------------------------------------------------------
class CClassAnalyzer
{
	public:
		static CClassNode* poLoadClass(const CCAString& _sFilename);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
