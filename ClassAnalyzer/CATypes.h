//---------------------------------------------------------------------------
//
// Name:        CATypes.h
// Author:      David Márquez de la Cruz
// Created:		01-Dec-2009
// Description: Basic types used by the class analyzer
//
//---------------------------------------------------------------------------
#ifndef CCATypesH
#define CCATypesH
// ----------------------------------------------------------------------------
#include <vector>
#include <map>
#include <string>

// ----------------------------------------------------------------------------
/// Simple types
// ----------------------------------------------------------------------------
typedef unsigned int uint;

// ----------------------------------------------------------------------------
/// Dynamic array class
// ----------------------------------------------------------------------------
template <class T>
class CCAArray : public std::vector<T> { };

// ----------------------------------------------------------------------------
/// String class wrapper
// ----------------------------------------------------------------------------
class CCAString : public std::basic_string<char>
{
	public:

		CCAString()
		{

		}

		CCAString(const char* _szStr)
		{
			this->assign(_szStr);
			// *this = std::string( _szStr );
			// return(*this);
		}

		CCAString(unsigned int _uiVal)
		{
			char szValue[32];
			sprintf_s(szValue,"%d",_uiVal);
	        
			this->assign(szValue);
		}

		int iPos(const CCAString &_sStr)
		{
			return(iPos(_sStr.c_str()));
		};

		int iPos(const char *_sStr)
		{
			size_t iPos = find(_sStr);

			if (iPos == npos)
				return (-1);
			else
				return ((int)iPos);
		};

		CCAString SubString(uint _uiPos,uint _uiLen)
		{
			return( * static_cast<CCAString*>( &substr(_uiPos,_uiLen) ) );
		};

		CCAString operator + (const CCAString& _sStr) const
		{
		  CCAString sRes(*this);
		  sRes.append(_sStr);
		  return sRes;
		}

		CCAString operator + (const char* _szStr) const
		{
		  CCAString sRes(*this);
		  sRes.append( CCAString(_szStr) );
		  return sRes;
		}
		
		char* szString() const
		{
		    return( const_cast<char*> (this->c_str()) );
		}
};
// ----------------------------------------------------------------------------
/// Look up array, search items using a variable name.
// ----------------------------------------------------------------------------
template <class T>
class CCALookupArray : public std::map<CCAString,T>
{
    public:
        void Add(const CCAString& _sVar,T _poValue)
        {
            insert ( std::pair<CCAString,T>(_sVar,_poValue) );
        }

        T* poFind(const CCAString& _sVar)
        {
			if (size() == 0) return(NULL);

            std::map<CCAString,T>::iterator it = find(_sVar);

            if (it != end())
				return( &((*it).second) );
            else
				return(NULL);
        }
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
