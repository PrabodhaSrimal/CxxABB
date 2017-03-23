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
 * Exception.cpp
 *
 * FileId      : $Id: Exception.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Dec 6, 2011
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : CxxAbbCore
 * Module      : Core
 * Comment     : Base of all the Exceptions
 *
 */

#include <CxxAbb/Exception.h>
#include <typeinfo>

namespace CxxAbb
{

Exception::Exception(int _iCode)
		: p_Nested(0), i_Code(_iCode)
{

}

Exception::Exception(const std::string& _sMsg, int _iCode)
		: s_Msg(_sMsg), p_Nested(0), i_Code(_iCode)
{

}

Exception::Exception(const std::string& _sMsg, const Exception& _mNested, int _iCode)
		: s_Msg(_sMsg), p_Nested(_mNested.Clone()), i_Code(_iCode)
{

}

Exception::Exception(const Exception& _mExc)
		: std::exception(_mExc), s_Msg(_mExc.Message()), i_Code(_mExc.Code())
{
	p_Nested = _mExc.Nested() ? _mExc.Nested()->Clone() : 0;
}

Exception::~Exception() throw ()
{
	if (p_Nested) delete p_Nested;
}

Exception& Exception::operator =(const Exception& _mExc)
{
	if (&_mExc != this)
	{
		if (p_Nested) delete p_Nested;
		i_Code = _mExc.Code();
		p_Nested = _mExc.Nested() ? _mExc.Nested()->Clone() : 0;
		s_Msg = _mExc.Message();
	}

	return *this;
}

const char* Exception::Name() const throw ()
{
	return "CxxAbbException";
}

const char* Exception::ClassName() const throw ()
{
	const char * z = typeid(*this).name();
	return z;
}

const char* Exception::what() const throw ()
{
	std::string sWhat = Name();
	if (!s_Msg.empty())
	{
		sWhat.append(": ");
		sWhat.append(s_Msg);
	}
	return sWhat.c_str();
}

Exception* Exception::Clone() const
{
	return new Exception(*this);
}

void Exception::Rethrow() const
{
	throw *this;
}

CXXABB_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
CXXABB_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
CXXABB_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
CXXABB_IMPLEMENT_EXCEPTION(NullValueException, LogicException, "Null value")
CXXABB_IMPLEMENT_EXCEPTION(DebuggerException, LogicException, "Debugger")
CXXABB_IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException, "Invalid argument")
CXXABB_IMPLEMENT_EXCEPTION(NotImplementedException, LogicException, "Not implemented")
CXXABB_IMPLEMENT_EXCEPTION(RangeException, LogicException, "Out of range")
CXXABB_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
CXXABB_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
CXXABB_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
CXXABB_IMPLEMENT_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

CXXABB_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
CXXABB_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
CXXABB_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
CXXABB_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
CXXABB_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
CXXABB_IMPLEMENT_EXCEPTION(RegularExpressionException, RuntimeException, "Error in regular expression")
CXXABB_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException, "Cannot load library")
CXXABB_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException, "Library already loaded")
CXXABB_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException, "No thread available")
CXXABB_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException, "Property not supported")
CXXABB_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
CXXABB_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException, "No permission")
CXXABB_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")
CXXABB_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")

CXXABB_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
CXXABB_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
CXXABB_IMPLEMENT_EXCEPTION(CircularReferenceException, DataException, "Circular reference")
CXXABB_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
CXXABB_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
CXXABB_IMPLEMENT_EXCEPTION(ProtocolException, IOException, "Protocol error")
CXXABB_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
CXXABB_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
CXXABB_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
CXXABB_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
CXXABB_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
CXXABB_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
CXXABB_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
CXXABB_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
CXXABB_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
CXXABB_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
CXXABB_IMPLEMENT_EXCEPTION(UnknownURISchemeException, RuntimeException, "Unknown URI scheme")

CXXABB_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
CXXABB_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException, "Bad cast exception")

} // namespace CxxAbb

