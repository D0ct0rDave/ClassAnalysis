// ---------------------------------------------------------------------------
//
// Name:        CAUtils.h
// Author:      David Márquez de la Cruz
// Created:     01-Dec-2009
// Description: Utility functions for the ClassAnalyzer
//
// ---------------------------------------------------------------------------
#ifndef CAUtilsH
#define CAUtilsH
// ---------------------------------------------------------------------------
#include "CATypes.h"

/// Reads a given file from disk and returns in the the form of a memory buffer
char* pucReadFile(char* _szFilename);

/// Preprocess a stream of data replacing comments by space chars
void PreprocessComments(char* _szStream);

/// Preprocess a stream of data searching for a given regular expression and replaces
/// the matching strings with spaces
void PreprocessRegExp(char* _szRegExp,char* _szStream);

/// Builds a CCAString object from basic fields.
CCAString sBuildToken(char* _szString,uint _uiStart,uint _uiLen);

// ----------------------------------------------------------------------------
#endif // CAUtilsH
// ---------------------------------------------------------------------------
