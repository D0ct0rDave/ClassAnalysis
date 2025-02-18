// ---------------------------------------------------------------------------
//
// Name:        FGASViewerGUI.h
// Author:      David Márquez de la Cruz
// Created:		28-Nov-2009
// Description: CClassAnalyzer class definition
//
// ---------------------------------------------------------------------------
#include "CClassAnalyzer.h"
#include "CAUtils.h"
#include "CATypes.h"
#include "CCAClassDB.h"
#include "deelx.h"

#include <stdlib.h>
#include <stdio.h>

// ----------------------------------------------------------------------------
typedef struct TClassAnalyzerGlobals{

    uint        m_uiCurrentLine;
    CCAString   m_sFilename;

}TClassAnalyzerGlobals;

static TClassAnalyzerGlobals globals;
// ----------------------------------------------------------------------------
// takes out the things that we don't need before starting the class analysis.
// ----------------------------------------------------------------------------
void PreprocessStream(char* _szStream)
{
    // Take out comments replacing them by spaces
    PreprocessComments(_szStream);

	// ------------------------------------------
	// Skip "using" keyword
	// ------------------------------------------
	PreprocessRegExp("using.*",_szStream);

	// ------------------------------------------
	// skip preprocessor definitions
	// ------------------------------------------
	PreprocessRegExp("#.*",_szStream);
}
// ----------------------------------------------------------------------------
/// Checks whether a given string is a number or not
// ----------------------------------------------------------------------------
bool bIsNumber(const CCAString& _sString)
{
	CRegexpT<char> myNum("((0x|0X)[a-fA-F0-9]+|[0-9]+)");
	MatchResult result = myNum.Match( ((CCAString&)_sString).szString() );
	return ( result.IsMatched() > 0 );
}
// ----------------------------------------------------------------------------
// Checks if the given char is a "word" character (including numbers and "_" character)
// ----------------------------------------------------------------------------
inline bool bIsWordChar(char c)
{
	return( 	((c >= 'a') && (c <= 'z'))
			  ||((c >= 'A') && (c <= 'Z'))
			  ||((c >= '0') && (c <= '9'))
			  ||(c=='_')
		);
}
// ----------------------------------------------------------------------------
/// Checks whether the given string is a full word or not.
// ----------------------------------------------------------------------------
bool bIsWord(const CCAString& _sString)
{
	/*
	CRegexpT<char> myNum("\A\w*\z");
	MatchResult result = myNum.Match( _sString.szString() );
	return ( result.IsMatched() );
    */

	for (uint i=0;i<_sString.size();i++)
		if (! bIsWordChar(_sString[i])) return(false);

	return(true);
}
// ----------------------------------------------------------------------------
/// Retrieves the next token in the stream skipping spaces, newline and carriage
/// returns.
// ----------------------------------------------------------------------------
CCAString sGetNextToken(char* &_szStream)
{
	char* szToken = _szStream;
	uint uiLen = 0;
	bool bWordInProgress = false;

	do{
		 if (
				(
					(*_szStream == ',')
				 || (*_szStream == ';')
				 || (*_szStream == ':')
				 || (*_szStream == '(')
				 || (*_szStream == ')')
				 || (*_szStream == '<')
				 || (*_szStream == '>')
				 || (*_szStream == '{')
				 || (*_szStream == '}')
				 || (*_szStream == '*')
				 || (*_szStream == '&')
				 || (*_szStream == '~')
				 || (*_szStream == '=')
				)
			)
		{
			if (bWordInProgress)
			{
				// we were defining a word and we've found it's end so return it
				return (sBuildToken(szToken,0,uiLen));
			}
			else
			{
				// advance stream
				_szStream++;
				return (sBuildToken(szToken,0,1));
			}			
		}

        // word character
	else if ( bIsWordChar(*_szStream) )
		{
			bWordInProgress = true;
			uiLen++;
		}

	    // any other char including spaces / tabs / new lines / carriage returns / etc ...
	else
		{
		    if (*_szStream == '\n')
                globals.m_uiCurrentLine++;

			if (bWordInProgress)
			{
				// we were defining a word and we've found it's end so return it
				return (sBuildToken(szToken,0,uiLen));
			}
			else
				// advance the token pointer
				szToken = (_szStream+1);
		}

		_szStream++;		
	} while ( (*_szStream != 0));

	return (sBuildToken(szToken,0,uiLen));
}
// ----------------------------------------------------------------------------
/// Creates a class node and initializes it with some data
// ----------------------------------------------------------------------------
CClassNode* poCreateClass(CClassNode* _poParent,ENodeType _eType,const CCAString& _sType)
{
    CClassNode* poClass = new CClassNode;

    poClass->m_uiLine = globals.m_uiCurrentLine;
    poClass->m_sFilename = globals.m_sFilename;
    poClass->m_eType = _eType;
    poClass->m_sType = _sType;
    
    // add this new type to the database
    CCAClassDB::Add(_sType,poClass);

    return(poClass);
}
// ----------------------------------------------------------------------------
/// Creates a class node and initializes it. This node is has name.
// ----------------------------------------------------------------------------
CClassNode* poCreateClassMember(CClassNode* _poParent,ENodeType _eType,const CCAString& _sType,const CCAString& _sName)
{
    CClassNode* pClass = new CClassNode;

    pClass->m_uiLine = globals.m_uiCurrentLine;
    pClass->m_sFilename = globals.m_sFilename;
    pClass->m_eType = _eType;
    pClass->m_sType = _sType;
    pClass->m_sName = _sName;
    pClass->m_poParent = _poParent;

    return(pClass);
}
// ----------------------------------------------------------------------------
/// Skips inlined definitions inside the stream.
// ----------------------------------------------------------------------------
void SkipInlineDefinition(char* &_szStream,bool _bInlineToken)
{
    if (_bInlineToken)
    {
        // go to the first "{" token
	    CCAString sToken = "";	    
	    do{
		    sToken = sGetNextToken(_szStream);

	    }while ((*_szStream) && (sToken!="{"));

	    if (*_szStream == 0)
	        // error ???
	        return;
    }

	CCAString sFuncToken = "";
	uint uiNumBrackets = 1;
	do{
		sFuncToken = sGetNextToken(_szStream);

		if (sFuncToken == "{")
			uiNumBrackets++;
		else if (sFuncToken == "}")
			uiNumBrackets--;

	}while ((*_szStream) && (uiNumBrackets > 0));
}
// ----------------------------------------------------------------------------
/// Retrieves whether the given string represents a Plain Old Data type or not.
// ----------------------------------------------------------------------------
bool bIsPODType(const CCAString& _sString)
{
    return(	 
           (_sString == "void")
		|| (_sString == "int")
        || (_sString == "char")
	    || (_sString == "short")
	    || (_sString == "long")
		|| (_sString == "float")
		|| (_sString == "double")
		|| (_sString == "bool")
		);
}
// ----------------------------------------------------------------------------
enum EParseState {

	PS_NONE,
	
	PS_BEGIN_NAMESPACE_DECL,
	
	PS_BEGINENUM,
	PS_ENUMNAME,
	PS_ENUMMEMBER,
	PS_ENUMNEWMEMBER,
	PS_ENDENUM,
	
	PS_BEGINSCOPE,
	
	PS_BEGINCLASS,
	PS_BEGINCLASS2,
	PS_BEGINCLASSINHERITANCE,
	PS_ENDCLASSDECL,

	PS_BEGINDECL,
	PS_DECLRESOLUTION,
	
	PS_FUNCDECL,
	PS_BEGINFUNCPARAM,
	PS_FUNCPARAMNAME,
	PS_ENDFUNCPARAM,
	PS_ENDFUNCDECL,

	PS_NUMPSs
};
// ----------------------------------------------------------------------------
/// Process the given stream analyzing its contents and storing the parsed data
/// into the given class node.
// ----------------------------------------------------------------------------
void ProcessStream(CClassNode* _poClass,char* &_szStream,EParseState _eFirstParseState = PS_NONE);
void ProcessStream(CClassNode* _poClass,char* &_szStream,EParseState _eFirstParseState)
{	
	ENodeType	eMT;
	CCAString	sDataName		= "";
	CCAString	sDataType		= "";
	CCAString	sScope			= "";	// public, private, protected
	CCAString	sTypeSpecifier	= "";	// unsigned
	CCAString	sTypeModifier	= "";	// * or &

	EParseState eState = _eFirstParseState;

	CCAString sToken = "";
	CCAString sPrevToken = "";

	do {
        
        sPrevToken = sToken;
		sToken = sGetNextToken(_szStream);

		switch (eState)
		{
			case PS_NONE:
			{
				if (sToken == "")
				{
					// nothing
				}
			else if (sToken == ";")
				{
					// just skip this :)
				}
			else if (sToken == "{")
				{
				    // Start new namespace/class/anything
					ProcessStream(_poClass,_szStream);
				}
			else if (sToken == "}")
				{
				    // End the namespace/class/anything
					return;
				}
			else if (sToken == "namespace")
				{
				    eState = PS_BEGIN_NAMESPACE_DECL;
				}
			else if (sToken == "inline")
				{
				    SkipInlineDefinition(_szStream,true);
				}
			else if (sToken == "typedef")
				{
				}
			else if (sToken == "enum")
				{
					eState = PS_BEGINENUM;
				}
			else if (
					(sToken == "public")
					||(sToken == "private")
					||(sToken == "protected")
					)
				{
					eState = PS_BEGINSCOPE;
					sScope = sToken;
				}
			else if (sToken == "struct")
				{
					eMT	   = NT_STRUCT;
					eState = PS_BEGINCLASS;
				}
			else if (sToken == "class")
				{
					eMT	   = NT_CLASS;
					eState = PS_BEGINCLASS;
				}
			else if (sToken == "unsigned")
				{
					sTypeSpecifier = sToken + " ";
				}
			else if (sToken == "const")
				{
					// sTypeSpecifier = sToken;
				}
			else if (sToken == "virtual")
				{
					// sTypeSpecifier = sToken;
				}
			else if (sToken == "static")
				{
					// sTypeSpecifier = sToken;
				}
			else if (sToken == "~")
				{
					// this should be the destructor of a class
				}
			else if (bIsPODType(sToken))
				{
					eMT = NT_POD;
					sDataType = sTypeSpecifier + sToken;
					sTypeSpecifier = "";
                    sTypeModifier = "";

					eState = PS_BEGINDECL;
				}
			else if (bIsWord(sToken))
				{
				    if (sTypeSpecifier != "")
				    {
				        // error ???
				        sTypeSpecifier = "";
				    }
                    sTypeModifier = "";

                    // Look if the class is already defined
				    CClassNode* poClass = CCAClassDB::poFind( sToken );
				    if (poClass)
				    {
					    eMT = poClass->m_eType;
					}
                    else
                    {
                        eMT = NT_UNKNOWN;
                    }

                    // destructor early detection
                    if (sPrevToken == "~")
				        sDataType = CCAString("~") + sToken;
				    else
                        sDataType = sToken;

					eState    = PS_BEGINDECL;
				}
			}
			break;
			

			// begins the declaration of something that can be a function, or a member variable
			case PS_BEGINDECL:
			{
				if (bIsWord(sToken))
				{
					// the data name
					sDataName = sToken;
					eState = PS_DECLRESOLUTION;
				}
			else if (
						(sToken == "*")
					 || (sToken == "&")
					)
				{
					sTypeModifier += sToken;
				}
			else if (sToken == "(")
				{
					// this can be the constructor/destructor of the class
					CCAString sFuncName;
					int iPos = sDataType.iPos("~");

					if (iPos>-1)
						// take the "~" and get just the function name
                       sFuncName = sDataType.SubString(iPos + 1,(int)sDataType.size() - iPos);
                    else
                       sFuncName = sDataType;

					if (sFuncName == _poClass->m_sType)
					{
						CClassNode* poClass  = NULL;
						if (iPos>-1)
							// this was a destructor
						    poClass = poCreateClassMember(_poClass,NT_DESTRUCTOR,"",sDataType);
						else
                            poClass = poCreateClassMember(_poClass,NT_CONSTRUCTOR,"",sDataType);

						_poClass->m_oMembers.push_back(poClass);

						ProcessStream(poClass,_szStream,PS_BEGINFUNCPARAM);

						// it was a function
						eState = PS_NONE;
					}
				}
			else{
					// error ??
					eState = PS_NONE;
				}				
			}
			break;

			case PS_DECLRESOLUTION:
			{
				if (sToken == ";")
				{
					// it was a member
					CClassNode* poClass  = poCreateClassMember(_poClass,eMT,sTypeSpecifier + sDataType + sTypeModifier,sDataName);
					_poClass->m_oMembers.push_back(poClass);					
					eState    = PS_NONE;
				}
			else if (sToken == "(")
				{
					CClassNode* poClass  = poCreateClassMember(_poClass,NT_METHFUNC,sTypeSpecifier + sDataType + sTypeModifier,sDataName);
					_poClass->m_oMembers.push_back(poClass);

					ProcessStream(poClass,_szStream,PS_BEGINFUNCPARAM);

					// it was a function
					eState = PS_NONE;
				}
			else
				{
					// error???
					eState = PS_NONE;
				}
			}
			break;

			// Namespace definition states
			case PS_BEGIN_NAMESPACE_DECL:
			{
			    if (bIsWord(sToken))
			    {
			        CClassNode* poClass = poCreateClassMember(_poClass,NT_NAMESPACE,sToken,"");
					_poClass->m_oMembers.push_back(poClass);

                    ProcessStream(poClass,_szStream);
                    eState = PS_NONE;
			    }
			    else
			    {
			        // error?   
			    }
			}
			break;
			
			// --------------------------
			// function definition states
			// --------------------------
			case PS_BEGINFUNCPARAM:
			{
				if (sToken == ")")
				{
					// end of function definition
					eState = PS_ENDFUNCDECL;
				}
			else if (sToken == "unsigned")
				{
					sTypeSpecifier = sToken+" ";
				}
			else if (sToken == "const")
				{
					// sTypeSpecifier = sToken;
				}
			else if ( bIsPODType(sToken))
				{
					eMT = NT_POD;
					sDataType = sTypeSpecifier + sToken;
					sTypeSpecifier = "";
                    sTypeModifier = "";

					eState = PS_FUNCPARAMNAME;
				}
			else if (bIsWord(sToken))
				{
				    CClassNode* poClass = CCAClassDB::poFind(sToken);
				    if (poClass)
				    {
					    eMT = poClass->m_eType;
					}
                    else
                    {
                        eMT = NT_UNKNOWN;
                    }

				    sDataType = sToken;
				    sTypeSpecifier = "";
                    sTypeModifier = "";

					eState    = PS_FUNCPARAMNAME;
				}
			}
			break;
			
			// we're defining now the parameter name
			case PS_FUNCPARAMNAME:
			{
				if (bIsWord(sToken))
				{
					CClassNode* poClass  = poCreateClassMember(_poClass,eMT,sTypeSpecifier + sDataType + sTypeModifier,sToken);
					_poClass->m_oMembers.push_back(poClass);

					eState    = PS_ENDFUNCPARAM;
				}
			else if (
						(sToken == "*")
					 || (sToken == "&")
					)
				{
					sTypeModifier += sToken;
				}
			}
			break;

            // after the name there can be default values etc ...
            case PS_ENDFUNCPARAM:
            {
                //
                if (sToken == "=")
                {
                    // expect a value right after... skip everything
                    CCAString sParamToken = "";
                    uint uiNumParenth = 0;
                    do {

                        sParamToken = sGetNextToken(_szStream);

                        // There can be constructors as default values!
                        if (sParamToken == "(") uiNumParenth++;
                        else if (sParamToken == ")") uiNumParenth--;

                    }while( !(((sParamToken == ",") && (uiNumParenth == 0)) || (uiNumParenth==-1)) );
                    
                    if (uiNumParenth == -1)
                        eState = PS_ENDFUNCDECL;
                    else
                        eState = PS_BEGINFUNCPARAM;
                }
            else if (sToken == ",")
                {
                    // a new parameter comes here
                    eState = PS_BEGINFUNCPARAM;
                }
            else if (sToken == ")")
                {
                    // the function is ending its definition                    
                    eState = PS_ENDFUNCDECL;
                }
            else
                {
                    // error ???
                }
            }
            break;

			case PS_ENDFUNCDECL:			
			{
				if (sToken == ";")
				{
					// end of function definition
					return;
				}
			else if (sToken == "const")
				{
					// nothing to do still
				}
			else if (sToken == "{")
				{
					// inline function implementation: skip it
					SkipInlineDefinition(_szStream,false);
					return;
				}
			}
			break;

			// --------------------------
			// enum definition states
			// --------------------------
			case PS_BEGINENUM:
			{
				if (bIsWord(sToken))
				{
				    CClassNode* poClass  = poCreateClass(_poClass,NT_ENUM,sToken);
                
					// add the enum to the current class
					_poClass->m_oMembers.push_back(poClass);

					ProcessStream(poClass,_szStream,PS_ENUMNAME);
					eState = PS_NONE;
				}
			else if (sToken == "{")
				{
					// enum starts without defining yet the name
					CClassNode* poClass  = poCreateClass(_poClass,NT_ENUM,"undefined enum");

					// add the enum to the current class
					_poClass->m_oMembers.push_back(poClass);

					ProcessStream(poClass,_szStream,PS_ENUMMEMBER);
					eState = PS_NONE;
				}
			else{
					// error ??
				}
			}
			break;
			
			case PS_ENUMNAME:
			{
				if (sToken == "{" )
				{
					// ok
					eState = PS_ENUMMEMBER;
				}				
			}
			break;
			
			case PS_ENUMMEMBER:
			{
				if (bIsWord(sToken))
				{
					CClassNode* poClassMember = poCreateClassMember(_poClass,NT_ENUM_MEMBER,"",sToken);
					_poClass->m_oMembers.push_back( poClassMember );

					// set the current member being edited
					eState = PS_ENUMNEWMEMBER;
				}
				else
				{
					// error ???
				}
			}
			break;

			case PS_ENUMNEWMEMBER:
			{
				if (sToken == ",")
				{
					eState = PS_ENUMMEMBER;
				}
			else if (sToken == "=")
				{
					// nothing
				}
			else if (bIsNumber(sToken))
				{
					// nothing
				}
			else if (sToken == "}")
				{
					// end of enum
					eState = PS_ENDENUM;
				}
			}
			break;
			
			case PS_ENDENUM:
			{				
				if (bIsWord(sToken))					
				{
					// enum name at the end of the definition rigth after the "}"
					_poClass->m_sType = sToken;
					return;
				}
			else if (sToken == ";")
				{
					// just finish
					return;
				}
			else
				{
					// error ?
					return;
				}
			}
			break;
			
			// --------------------------
			// class definition states
			// --------------------------
			case PS_BEGINSCOPE:
			{
				if (sToken == ":")
				{
					eState = PS_NONE;
				}
				else
				{
					// error
				}
			}
			break;
			
			case PS_BEGINCLASS:
			{
				if (bIsWord(sToken))
				{
					sDataName = sToken;
					eState = PS_BEGINCLASS2;
				}
			}
			break;

			case PS_BEGINCLASS2:
			{
				if (sToken == ":")
				{
					eState = PS_BEGINCLASSINHERITANCE;
				}
				else if (sToken == ";")
				{
					// just a forward definition: do nothing
					eState = PS_NONE;
				}
				else if (sToken == "{")
				{
					// Class definition begins here
					CClassNode *poClass = poCreateClass(_poClass,eMT,sDataName);

                    // add to the current class
					_poClass->m_oMembers.push_back(poClass);

					ProcessStream(poClass,_szStream);

					// expect exit from the class with an ending }
					eState = PS_NONE;
				}
			}
			break;

			case PS_BEGINCLASSINHERITANCE:
			{
				// Can be scope or just the name
				if (bIsWord(sToken))
				{
					// 
				}
			else if (sToken == ",")
				{
					// multiple inheritance??
				}
			else if (sToken == "{")
				{
					// Class definition begins here
					CClassNode* poClass  = poCreateClass(_poClass,NT_CLASS,sDataName);
					_poClass->m_oMembers.push_back(poClass);

					ProcessStream(poClass,_szStream,PS_NONE);

					// expect exit from the class with an ending }
					eState = PS_NONE;
				}
			}
			break;

			case PS_ENDCLASSDECL:
			{
				if (sToken == "}")
					eState = PS_NONE;
			}
			break;
		}
	}while (*_szStream != 0);
}
// ----------------------------------------------------------------------------
/// Extracts the filename(+ext) of a given string representing a full path
// ----------------------------------------------------------------------------
CCAString sShortFilename(const CCAString& _sPath)
{
	// convert the filename to forward slashes
	CCAString sPath = _sPath;
	char* pucPath = (char*)sPath.szString();
	for (uint i=0;i<sPath.size();i++)
		if (pucPath[i]= '\\') pucPath[i]='/';
	
	// 
	MatchResult result;
	CRegexpT<char> oRegExp("/[^/]+$");
	result = oRegExp.Match( sPath.szString() );

	if (result.IsMatched())
		return( sPath.SubString(result.GetStart()+1,result.GetEnd() - (result.GetStart() + 1)) );
	else
		return(_sPath);
}
// ----------------------------------------------------------------------------
/// Generates a class hierarchy with the definitions found in the given file.
// ----------------------------------------------------------------------------
CClassNode* CClassAnalyzer::poLoadClass(const CCAString& _sFilename)
{
	// Setup some global information
    globals.m_uiCurrentLine = 1;
    globals.m_sFilename     = _sFilename;

	// Read the file contents
    char* szFileDef = pucReadFile( ((CCAString&)_sFilename).szString() );

    // Check for correct file definition
    if (szFileDef == NULL)
        return(NULL);
	
	// Assign the file contents to the "Stream"
	char* szStream = szFileDef;

	// Preprocess the file contents to eliminate unnecessary info	
    PreprocessStream(szStream);

	// Create a class of nodetype FILE which will hold all the data defined in this file
    CClassNode* poClass		= new CClassNode;
    poClass->m_sFilename= _sFilename;
    poClass->m_eType	= NT_FILE;
    poClass->m_sName	= sShortFilename(_sFilename);    

	// Start the class analysis
	ProcessStream(poClass,szStream);

	// free the buffer
	free(szFileDef);
	
	// return the results
	return(poClass);
}
// ----------------------------------------------------------------------------
