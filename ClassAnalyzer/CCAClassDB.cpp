// ---------------------------------------------------------------------------
//
// Name:        CAClassDB.cpp
// Author:      David Márquez de la Cruz
// Created:     01-Dec-2009
// Description: Class database for CClassAnalyzer
//
// ---------------------------------------------------------------------------
#include "CCAClassDB.h"
#include "CCAClassNode.h"
#include "CATypes.h"
//---------------------------------------------------------------------------
/// A database which will contain all the parsed entities in the process of parsing files
//---------------------------------------------------------------------------
CCALookupArray<CClassNode*> CCAClassDB::m_oDB;
// ----------------------------------------------------------------------------
CClassNode* CCAClassDB::poFind(const CCAString& _sVar)
{
	CClassNode** poClass = m_oDB.poFind(_sVar);

	if (poClass != NULL)
		return( *poClass );
	else
		return(NULL);
}
// ----------------------------------------------------------------------------
void CCAClassDB::Add(const CCAString& _sVar,CClassNode* _poClass)
{
    m_oDB.Add(_sVar,_poClass);
}
// ----------------------------------------------------------------------------
