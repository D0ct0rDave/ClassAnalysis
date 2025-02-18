// ---------------------------------------------------------------------------
//
// Name:        CCAClassDB.h
// Author:      David Márquez de la Cruz
// Created:     01-Dec-2009
// Description: Utility functions for the ClassAnalyzer
//
// ---------------------------------------------------------------------------
#ifndef CClassDBH
#define CClassDBH
// ---------------------------------------------------------------------------
#include "CATypes.h"
#include "CCAClassNode.h"
// ---------------------------------------------------------------------------
/// A database which will contain all the parsed entities in the process of parsing files
// ---------------------------------------------------------------------------
class CCAClassDB
{
    public:
        static CClassNode* poFind(const CCAString& _sVar);
        static void Add(const CCAString& _sVar,CClassNode* _poClass);

    protected:
        static CCALookupArray<CClassNode*> m_oDB;
};

// ----------------------------------------------------------------------------
#endif // CAUtilsH
// ---------------------------------------------------------------------------
