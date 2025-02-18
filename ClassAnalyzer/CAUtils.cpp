// ---------------------------------------------------------------------------
//
// Name:        CAUtils.cpp
// Author:      David Márquez de la Cruz
// Created:     01-Dec-2009
// Description: Utility functions for the ClassAnalyzer
//
// ---------------------------------------------------------------------------
#include "CAUtils.h"
#include "CATypes.h"
#include <stdlib.h>
#include <stdio.h>

#include "deelx.h"
// ---------------------------------------------------------------------------
/// Reads a given file from disk and returns in the the form of a memory buffer
// ---------------------------------------------------------------------------
char* pucReadFile(char* _szFilename)
{
	// FILE* fd = fopen(_szFilename,"rb");	
	FILE* fd = NULL;
	fopen_s(&fd,_szFilename,"rb");	

	if (fd != NULL)
	{
		// Retrieve the file size
		fseek(fd,0,SEEK_END);
		uint uiLen = ftell(fd);
		fseek(fd,0,SEEK_SET);

		// Create a buffer to hold the complete file contents
		char* pBuffer = (char*)malloc(uiLen+1);
		fread(pBuffer,uiLen,1,fd);
		pBuffer[uiLen] = 0;

		// Close the file
		fclose(fd);

		// Return the file contents
		return((char*)pBuffer);
	}

	return(NULL);
}
// ----------------------------------------------------------------------------
/// Preprocess the given stream looking for expressions defined by the given RegExp
// ----------------------------------------------------------------------------
void PreprocessRegExp(char* _szRegExp,char* _szStream)
{
	// ------------------------------------------
	// Find regexp and take them out (substitute by spaces)
	// ------------------------------------------
	MatchResult result;
	CRegexpT<char> oRegExp(_szRegExp);
	result = oRegExp.Match( _szStream );

	while (result.IsMatched())
	{
		// fill with spaces
		uint uiLen = result.GetEnd() - result.GetStart();
		char* pucChar = _szStream + result.GetStart();
		for (uint i=0;i<uiLen;i++)
		{
			// in order to properly count the line numbers we need the new line characters
			if ((pucChar[i]!='\r') && (pucChar[i] != '\n'))
				pucChar[i] = ' ';
		}

		// find next
		result = oRegExp.Match(_szStream, result.GetEnd());
	}
}
// ---------------------------------------------------------------------------
/// Preprocess a stream of data replacing comments by space chars
// ---------------------------------------------------------------------------
void PreprocessComments(char* _szStream)
{
	// ------------------------------------------
    // Find multiline comments and take them out
    // ------------------------------------------
    PreprocessRegExp("/\\*([^*]|[\\r\\n]|(\\*+([^*/]|[\\r\\n])))*\\*+/",_szStream);

	// ------------------------------------------
	// Find single line comments and take them out
	// ------------------------------------------
	PreprocessRegExp("//.*",_szStream);
}
// ----------------------------------------------------------------------------
/// Builds a CCAString object from basic fields.
// ----------------------------------------------------------------------------
CCAString sBuildToken(char* _szString,uint _uiStart,uint _uiLen)
{
	char* szStr = (char*)malloc(_uiLen + 1);
	memcpy(szStr,_szString + _uiStart,_uiLen);
	szStr[_uiLen] = 0;
	CCAString oStr = CCAString(szStr);
	free(szStr);

	return (oStr);
}
// ----------------------------------------------------------------------------
