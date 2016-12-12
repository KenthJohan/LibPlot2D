/*===================================================================================
                                    DataPlotter
                          Copyright Kerry R. Loux 2011-2016

                   This code is licensed under the GPLv2 License
                     (http://opensource.org/licenses/GPL-2.0).

===================================================================================*/

// File:  machineDefinitions.h
// Created:  4/15/2010
// Author:  K. Loux
// Description:  Collection of functions for converting built-in types on systems that
//				 have different bits/byte (i.e. 32 vs. 64-bit).  This will require a
//				 preprocessor definition and will generate an error of one of the
//				 required definitions is not used.

#ifndef MACHINE_DEFS_H_
#define MACHINE_DEFS_H_

// Standard C++ headers
#include <stdint.h>

// NOTE:  See wxPlatformInfo for run-time determination of OS address width

#ifdef LIBPLOT2D_32_BIT_
#define DP_ULONG unsigned long
#elif LIBPLOT2D_64_BIT_
#define DP_ULONG unsigned int
#else
#error "Must define preprocessor flags LIBPLOT2D_32_BIT_ or LIBPLOT2D_64_BIT_"
#endif// TODO:  Is this necessary?  At the very least, can we deduce with preprocessor?

namespace LibPlot2D
{

namespace MachineDefinitions
{

// Conversion to signed 32-bit integers
inline int32_t ReadInt32(int64_t in)
{
	return in;
}

inline int32_t ReadInt32(int32_t in)
{
	return in;
}

// Conversion to unsigned 32-bit integers
inline uint32_t ReadUInt32(uint64_t in)
{
	return in;
}

inline uint32_t ReadUInt32(uint32_t in)
{
	return in;
}

// Conversion to signed 64-bit integers
inline int64_t ReadInt64(int64_t in)
{
	return in;
}

inline int64_t ReadInt64(int32_t in)
{
	return in;
}

// Conversion to unsigned 64-bit integers
inline uint64_t ReadUInt64(uint64_t in)
{
	return in;
}

inline uint64_t ReadUInt64(uint32_t in)
{
	return in;
}

}// namespace MachineDefinitions

}// namespace LibPlot2D

#endif// MACHINE_DEFS_H_
