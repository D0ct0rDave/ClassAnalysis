//---------------------------------------------------------------------------
//
// Name:        CClassNode.h
// Author:      David Márquez de la Cruz
// Created:     01-Dec-2009
// Description: CClassNode class Declaration
//
//---------------------------------------------------------------------------
#ifndef CClassNodeH
#define CClassNodeH
//---------------------------------------------------------------------------
#include "CATypes.h"

//---------------------------------------------------------------------------
/// Defines the types of nodes contained in the CClassNode class.
//---------------------------------------------------------------------------
typedef enum ENodeType
{
	NT_NONE,
	
	NT_DIRECTORY,
	NT_FILE,

    NT_NAMESPACE,
	NT_CLASS,
	NT_STRUCT,
	NT_ENUM,

	NT_ENUM_MEMBER,
	NT_METHFUNC,
	NT_CONSTRUCTOR,
	NT_DESTRUCTOR,
	NT_POD,

	NT_UNKNOWN,
	NT_NUMMTs
};
//---------------------------------------------------------------------------
/// The CClassNode node. A container for all the information belonging to a node.
//---------------------------------------------------------------------------
class CClassNode
{   
    public:

        /// Filename where the class is defined (probably we can get rid of this)
        CCAString			m_sFilename;

        /// Line of the file where the class is defined
        uint				m_uiLine;

        /// Type: class/type/file/...
        ENodeType			m_eType;

        /// Class type (uint,unsigned char,void*,class XXX,...)
        CCAString			m_sType;

        /// List of members for this class/type/file
        CCAArray<CClassNode*> m_oMembers;
       
        /// The parent node of this node
        CClassNode*			m_poParent;

    // Class Members when belonging to a class, function, enum, ...

        /// The node name 
        CCAString			m_sName;

        /// The member scope
        CCAString			m_sScope;

        /// Default initialization for this class
        CClassNode() :

			m_sFilename(""),
            m_sType(""),
            m_eType(NT_NONE),
            m_sName(""),
            m_sScope(""),
            m_uiLine(0),
            m_poParent(NULL)
        {
		};
};
//---------------------------------------------------------------------------
#endif // CClassNodeH
//---------------------------------------------------------------------------
