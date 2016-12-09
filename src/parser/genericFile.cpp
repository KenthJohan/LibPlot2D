/*===================================================================================
                                    DataPlotter
                          Copyright Kerry R. Loux 2011-2016

                   This code is licensed under the GPLv2 License
                     (http://opensource.org/licenses/GPL-2.0).

===================================================================================*/

// File:  genericFile.cpp
// Created:  10/3/2012
// Author:  K. Loux
// Description:  Generic delimited data file.
// History:

// Local headers
#include "lp2d/parser/genericFile.h"

namespace LibPlot2D
{

//==========================================================================
// Class:			GenericFile
// Function:		IsType
//
// Description:		Method for determining if the specified file is this
//					type of file.
//
// Input Arguments:
//		fileName	= const wxString&
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, true if this object can process the specified file, false otherwise
//
//==========================================================================
bool GenericFile::IsType(const wxString& WXUNUSED(fileName))
{
	return true;
}

}// namespace LibPlot2D
