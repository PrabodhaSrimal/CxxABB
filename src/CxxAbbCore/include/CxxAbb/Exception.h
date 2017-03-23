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
 * Exception.h
 *
 * FileId      : $Id: Exception.h 20 2012-11-22 07:46:58Z prabodar $
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

#ifndef CXXABB_CORE_EXCEPTION_H_
#define CXXABB_CORE_EXCEPTION_H_

#include "CxxAbb/Core.h"
#include <exception>
#include <stdexcept>

/// STD Exception hierarchy
///
/// * std::exception			<-- Base of all
///  * std::bad_alloc
///  * std::bad_cast
///  * std::bad_exception
///  * std::bad_typeid
///  * std::ios_base::failure
///  * std::logic_error
///    * std::domain_error
///    * std::invalid_argument
///    * std::length_error
///    * std::out_of_range
///  * std::runtime_error
///    * std::overflow_error
///    * std::range_error
///    * std::underflow_error

namespace CxxAbb
{

/** @brief Base class for all exceptions in CxxAbb
 *
 * supports nested exceptions and rethrow
 *
 */
class CXXABB_API Exception: public std::exception
{
public:
	Exception(const std::string& _sMsg, int _iCode = 0);

	Exception(const std::string& _sMsg, const Exception& _mNested, int _iCode = 0);

	Exception(const Exception& _mExc);

	~Exception() throw ();

	Exception& operator =(const Exception& _mExc);

	virtual const char* Name() const throw ();

	virtual const char* ClassName() const throw ();

	virtual const char* what() const throw ();

	const Exception* Nested() const
	{
		return p_Nested;
	}

	const std::string& Message() const
	{
		return s_Msg;
	}

	int Code() const
	{
		return i_Code;
	}

	virtual Exception* Clone() const;

	virtual void Rethrow() const;

protected:
	Exception(int _iCode = 0);
	/// Standard constructor.

	void Message(const std::string& _sMsg)
	{
		s_Msg = _sMsg;
	}

	void ExtendedMessage(const std::string& _sArg);

private:
	std::string s_Msg;
	Exception* p_Nested;
	int i_Code;
};

/// Helper macros to define and implement new exception classes

#define CXXABB_DEFINE_EXCEPTION(API, CLS, BASE, CODE) \
	class API CLS: public BASE														\
	{																				\
	public:																			\
		CLS(int _iCode = CODE);														\
		CLS(const std::string& s_Msg, int _iCode = CODE);								\
		CLS(const std::string& s_Msg, const CxxAbb::Exception& _mExc, int _iCode = CODE);	\
		CLS(const CLS& _mExc);														\
		~CLS() throw();																\
		CLS& operator = (const CLS& _mExc);											\
		const char* Name() const throw();										\
		const char* ClassName() const throw();										\
		CxxAbb::Exception* Clone() const;												\
		void Rethrow() const;														\
	};

#define CXXABB_DEFINE_EXCEPTION_BASE(API, CLS, BASE) \
	CXXABB_DEFINE_EXCEPTION(API, CLS, BASE, 0)

#define CXXABB_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													\
	CLS::CLS(int _iCode): BASE(_iCode)																	\
	{																								\
	}																								\
	CLS::CLS(const std::string& _sMsg, int _iCode): BASE(_sMsg, _iCode)										\
	{																								\
	}																								\
	CLS::CLS(const std::string& _sMsg, const CxxAbb::Exception& _mExc, int _iCode): BASE(_sMsg, _mExc, _iCode)	\
	{																								\
	}																								\
	CLS::CLS(const CLS& _mExc): BASE(_mExc)																\
	{																								\
	}																								\
	CLS::~CLS() throw()																				\
	{																								\
	}																								\
	CLS& CLS::operator = (const CLS& _mExc)															\
	{																								\
		BASE::operator = (_mExc);																		\
		return *this;																				\
	}																								\
	const char* CLS::Name() const throw()															\
	{																								\
		return NAME;																				\
	}																								\
	const char* CLS::ClassName() const throw()														\
	{																								\
		return typeid(*this).name();																\
	}																								\
	CxxAbb::Exception* CLS::Clone() const																\
	{																								\
		return new CLS(*this);																		\
	}																								\
	void CLS::Rethrow() const																		\
	{																								\
		throw *this;																				\
	}

/// Library defined exception classes

/// Logic Error
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, LogicException, Exception)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, AssertionViolationException, LogicException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, NullPointerException, LogicException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, NullValueException, LogicException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, DebuggerException, LogicException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, InvalidArgumentException, LogicException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, NotImplementedException, LogicException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, RangeException, LogicException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, IllegalStateException, LogicException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, InvalidAccessException, LogicException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, SignalException, LogicException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, UnhandledException, LogicException)

/// Runtime Error
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, RuntimeException, Exception)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, NotFoundException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, ExistsException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, TimeoutException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, SystemException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, RegularExpressionException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, LibraryLoadException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, LibraryAlreadyLoadedException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, NoThreadAvailableException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, PropertyNotSupportedException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, PoolOverflowException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, NoPermissionException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, OutOfMemoryException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, DataException, RuntimeException)

/// Misc Errors
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, DataFormatException, DataException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, SyntaxException, DataException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, CircularReferenceException, DataException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, PathSyntaxException, SyntaxException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, IOException, RuntimeException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, ProtocolException, IOException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, FileException, IOException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, FileExistsException, FileException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, FileNotFoundException, FileException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, PathNotFoundException, FileException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, FileReadOnlyException, FileException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, FileAccessDeniedException, FileException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, CreateFileException, FileException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, OpenFileException, FileException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, WriteFileException, FileException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, ReadFileException, FileException)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, UnknownURISchemeException, RuntimeException)

CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, ApplicationException, Exception)
CXXABB_DEFINE_EXCEPTION_BASE(CXXABB_API, BadCastException, RuntimeException)

}

#endif /* CXXABB_CORE_EXCEPTION_H_ */
