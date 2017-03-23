/**
 *                                                             _|        _|
 *                       _|_|_|  _|    _|  _|    _|    _|_|_|  _|_|_|    _|_|_|
 *                     _|          _|_|      _|_|    _|    _|  _|    _|  _|    _|
 *                     _|        _|    _|  _|    _|  _|    _|  _|    _|  _|    _|
 *                       _|_|_|  _|    _|  _|    _|    _|_|_|  _|_|_|    _|_|_|
 *
 *                              CxxABB - C++ Application Building Blocks
 *
 *                     Copyright (C) 2017 Prabodha Srimal <prabodha007@gmail.com>
 *
 *
 * SourceLineInfo.h
 *
 * FileId      : $Id: SourceLineInfo.h 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Dec 7, 2011
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCode
 * Module      : Core
 * Comment     : Holds information about source code line
 *
 */

#ifndef _SOURCELINEINFO_H_
#define _SOURCELINEINFO_H_

#include <CxxAbb/Core.h>
#include <string>

// GNU C++ compiler
#ifdef __GNUC__
#define CXXABB_FUNCTION __PRETTY_FUNCTION__
// Borland C++
#elif defined(__BORLANDC__)
#define CXXABB_FUNCTION __FUNC__
// Microsoft C++ compiler
#elif defined(_MSC_VER)
// .NET 2003 support's demangled function names
#if _MSC_VER >= 1300
#define CXXABB_FUNCTION __FUNCDNAME__
#else
#define CXXABB_FUNCTION __FUNCTION__
#endif
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#define CXXABB_FUNCTION __func__
// otherwise use standard macro
#else
#define CXXABB_FUNCTION "unknown symbol"
#endif


#define CXXABB_SOURCEINFO CxxAbb::SourceLineInfo(__FILE__, CXXABB_TOSTRING(__LINE__), CXXABB_FUNCTION)

namespace CxxAbb
{

/** @brief Source code info class

 This class is used to store information about a location in the source
 code. The CXXABB_SOURCEINFO macro can be used to construct a CxxAbb::SourceLineInfo
 object conveniently.

 @code
 CxxAbb::SourceLineInfo si(CXXABB_SOURCEINFO);

 // print file, line and function
 std::cout << si.file() << std::endl;
 std::cout << si.line() << std::endl;
 std::cout << si.func() << std::endl;

 // print combined string
 std::cout << si.where() << std::endl;
 @endcode
 */
class CXXABB_API SourceLineInfo
{
public:

	inline SourceLineInfo(const char* _file, const char* _line, const char* _func)
		: z_File(_file), z_Line(_line), z_Func(_func)
	{}

	inline const char* file() const
	{
		return z_File;
	}

	inline const char* line() const
	{
		return z_Line;
	}

	inline const char* func() const
	{
		return z_Func;
	}

private:
	const char* z_File;
	const char* z_Line;
	const char* z_Func;
};

inline std::string operator+(const std::string& what, const SourceLineInfo& info)
{
	return std::string(info.file()) + ':' + info.line() + ": " += what;
}

inline std::string operator+(const char* what, const SourceLineInfo& info)
{
	return std::string(info.file()) + ':' + info.line() + ": " += what;
}

inline std::string operator+(const SourceLineInfo& info, const std::string& what)
{
	return std::string(info.file()) + ':' + info.line() + ": " += what;
}

inline std::string operator+(const SourceLineInfo& info, const char* what)
{
	return std::string(info.file()) + ':' + info.line() + ": " += what;
}

inline std::ostream& operator<< (std::ostream& stream, const SourceLineInfo& info)
{
	stream << "[" << info.file() << ':' << info.line() << "] " << info.func();
	return stream;
}


} /* namespace CxxAbb */

#endif /* _SOURCELINEINFO_H_ */
