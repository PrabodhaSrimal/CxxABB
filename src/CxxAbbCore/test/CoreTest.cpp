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
 * CoreTest.cpp
 *
 * FileId      : $Id: CoreTest.cpp 20 2012-11-22 07:46:58Z prabodar $
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Sep 9, 2012
 * Edited by   : $Author: prabodar $
 * Edited date : $Date: 2012-11-22 13:16:58 +0530 (Thu, 22 Nov 2012) $
 * Version     : $Revision: 20 $
 *
 * Library     : <Library Name>
 * Module      : <Module Name>
 * Comment     : <General Comment>
 *
 */

#include <CxxAbb/Core.h>
#include <CxxAbb/Debug.h>
#include <CxxAbb/SourceLineInfo.h>
#include <CxxAbb/SwapByteOrder.h>
#include <CxxAbb/TypeInfo.h>
#include <CxxAbb/BitOps.h>
#include <CxxAbb/Buffer.h>
#include <CxxAbb/NullType.h>

#include <cstdlib>
#include <typeinfo>
#include <gtest/gtest.h>

TEST(CoreTest, Compiler)
{
	COUT_LOG() << "__cplusplus : " << __cplusplus;
}

TEST(CoreTest, Platform)
{
	COUT_LOG() << "Platform OS        : " << CXXABB_OS << " - " << CxxAbb::PlatformOsName(CXXABB_OS);
	COUT_LOG() << "Platform OS Family : " << CXXABB_OS_FAMILY << " - " << CxxAbb::PlatformOsFamilyName(CXXABB_OS_FAMILY);
	COUT_LOG() << "Platform Arch      : " << CXXABB_ARCH << " - " << CxxAbb::PlatformArchName(CXXABB_ARCH);
#ifdef CXXABB_ARCH_LITTLE_ENDIAN
	COUT_LOG() << "Is LittleEndian    : Yes";
#else
	COUT_LOG() << "Is LittleEndian    : No";
#endif
}

TEST(CoreTest, Types)
{
	ASSERT_EQ(1u, sizeof(CxxAbb::Int8));
	ASSERT_EQ(2u, sizeof(CxxAbb::Int16));
	ASSERT_EQ(4u, sizeof(CxxAbb::Int32));
	ASSERT_EQ(8u, sizeof(CxxAbb::Int64));

	ASSERT_EQ(1u, sizeof(CxxAbb::UInt8));
	ASSERT_EQ(2u, sizeof(CxxAbb::UInt16));
	ASSERT_EQ(4u, sizeof(CxxAbb::UInt32));
	ASSERT_EQ(8u, sizeof(CxxAbb::UInt64));

	ASSERT_EQ(sizeof(void*), sizeof(CxxAbb::PtrT));
	ASSERT_EQ(sizeof(void*), sizeof(CxxAbb::UPtrT));

	//COUT_LOG() << "std::size_t : " << typeid(std::size_t).name();

	ASSERT_EQ(4u, sizeof(float));
	ASSERT_EQ(8u, sizeof(double));

	ASSERT_TRUE( std::numeric_limits<float>::has_infinity );
	ASSERT_TRUE( std::numeric_limits<double>::has_infinity );

	ASSERT_TRUE( std::numeric_limits<float>::has_quiet_NaN );
	ASSERT_TRUE( std::numeric_limits<double>::has_quiet_NaN );
}

TEST(CoreTest, Macros)
{
#define TEST_HELLO___ Hello_
#define TEST_WORLD___ World_
#define Hello_World_ Hello World

	COUT_LOG() << CXXABB_TOSTRING(int i = 10);
	COUT_LOG() << CXXABB_TOSTRING(CXXABB_JOIN(CurrentLine_, __LINE__));
	COUT_LOG() << CXXABB_TOSTRING(CXXABB_JOIN(__LINE__, _CurrentLine));
	COUT_LOG() << CXXABB_TOSTRING(CXXABB_JOIN(TEST_HELLO___, TEST_WORLD___));

#undef TEST_HELLO___
#undef TEST_WORLD___
#undef Hello_World_
}

TEST(CoreTest, Exceptions)
{

}

TEST(CoreTest, Debugger)
{
	COUT_LOG() << "A debug message should print now!!!";
	DEBUGLOG("Test debug message");
#ifdef CXXABB_DEBUG
	EXPECT_THROW(DEBUGBREAK("This will break the flow"), CxxAbb::DebuggerException);
	EXPECT_THROW(ASSERT(false), CxxAbb::AssertionViolationException);
	EXPECT_NO_THROW(ASSERT(true));

	int * ip = NULL;

	//CHECKNULL(ip);
	EXPECT_THROW(CHECKNULL(ip), CxxAbb::NullPointerException);

	ip = new int;
	*ip = 10;

	EXPECT_NO_THROW(CHECKNULL(ip));

#endif

	//ASSERT(false);

	// this should compile without error
	STATIC_ASSERT(sizeof(CxxAbb::Int32) == 4);
	STATIC_ASSERT(sizeof(CxxAbb::Int64) == 8);

	COUT_LOG() << "Source Info : " << CXXABB_SOURCEINFO;
	COUT_LOG() << "Dummy Message" + CXXABB_SOURCEINFO;
	COUT_LOG() << std::string("Dummy Message") + CXXABB_SOURCEINFO;
	COUT_LOG() << CXXABB_SOURCEINFO + "Dummy Message";
	COUT_LOG() << CXXABB_SOURCEINFO + std::string("Dummy Message");

}

TEST(CoreTest, SwapByteOrder)
{
	COUT_LOG() << "ByteOrder LittleEndian : " << CxxAbb::ByteOrder::LittleEndian();
	COUT_LOG() << "ByteOrder BigEndian    : " << CxxAbb::ByteOrder::BigEndian();

#ifdef CXXABB_ARCH_LITTLE_ENDIAN
	ASSERT_TRUE(CxxAbb::ByteOrder::LittleEndian());
	ASSERT_FALSE(CxxAbb::ByteOrder::BigEndian());
#else
	ASSERT_FALSE(CxxAbb::ByteOrder::LittleEndian());
	ASSERT_TRUE(CxxAbb::ByteOrder::BigEndian());
#endif

	{
		CxxAbb::Int8 iUnSwap = 0x02;
		CxxAbb::Int8 iSwap = CxxAbb::Swab(iUnSwap); // No swapping
		ASSERT_EQ(iUnSwap, iSwap);
		iSwap = CxxAbb::Swab(iSwap);
		ASSERT_EQ(iUnSwap, iSwap);
	}

	{
		CxxAbb::UInt8 iUnSwap = 0x02;
		CxxAbb::UInt8 iSwap = CxxAbb::Swab(iUnSwap); // No swapping
		ASSERT_EQ(iUnSwap, iSwap);
		iSwap = CxxAbb::Swab(iSwap);
		ASSERT_EQ(iUnSwap, iSwap);
	}

	{
		CxxAbb::Int16 iUnSwap = 0x1122;
		CxxAbb::Int16 iSwap = CxxAbb::Swab(iUnSwap); // watchout for data range overflow
		ASSERT_EQ(0x2211, iSwap);
		iSwap = CxxAbb::Swab(iSwap);
		ASSERT_EQ(iUnSwap, iSwap);
	}

	{
		CxxAbb::UInt16 iUnSwap = 0x1122;
		CxxAbb::UInt16 iSwap = CxxAbb::Swab(iUnSwap); // watchout for data range overflow
		ASSERT_EQ(0x2211, iSwap);
		iSwap = CxxAbb::Swab(iSwap);
		ASSERT_EQ(iUnSwap, iSwap);
	}

	{
		CxxAbb::Int32 iUnSwap = 0x11223344;
		CxxAbb::Int32 iSwap = CxxAbb::Swab(iUnSwap); // watchout for data range overflow
		ASSERT_EQ(0x44332211, iSwap);
		iSwap = CxxAbb::Swab(iSwap);
		ASSERT_EQ(iUnSwap, iSwap);
	}

	{
		CxxAbb::UInt32 iUnSwap = 0x11223344;
		CxxAbb::UInt32 iSwap = CxxAbb::Swab(iUnSwap); // watchout for data range overflow
		ASSERT_EQ(CxxAbb::UInt32(0x44332211), iSwap);
		iSwap = CxxAbb::Swab(iSwap);
		ASSERT_EQ(iUnSwap, iSwap);
	}

	{
		CxxAbb::Int64 iUnSwap = (CxxAbb::Int64(0x11223344) << 32) + 0x55667788;
		CxxAbb::Int64 iSwap = CxxAbb::Swab(iUnSwap); // watchout for data range overflow
		ASSERT_EQ((CxxAbb::Int64(0x88776655) << 32) + 0x44332211, iSwap);
		iSwap = CxxAbb::Swab(iSwap);
		ASSERT_EQ(iUnSwap, iSwap);
	}

	{
		CxxAbb::UInt64 iUnSwap = (CxxAbb::Int64(0x11223344) << 32) + 0x55667788;
		CxxAbb::UInt64 iSwap = CxxAbb::Swab(iUnSwap); // watchout for data range overflow
		ASSERT_EQ((CxxAbb::UInt64(0x88776655) << 32) + 0x44332211, iSwap);
		iSwap = CxxAbb::Swab(iSwap);
		ASSERT_EQ(iUnSwap, iSwap);
	}

	if(CxxAbb::ByteOrder::LittleEndian())
	{
		{
			CxxAbb::Int16 iLe = 0x20;
			CxxAbb::Int16 iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::Int16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::Int16>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::Int16>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::Int16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::Int16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::Int16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);
		}

		{
			CxxAbb::UInt16 iLe = 0x20;
			CxxAbb::UInt16 iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::UInt16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::UInt16>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::UInt16>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::UInt16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::UInt16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::UInt16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);
		}

		{
			CxxAbb::Int32 iLe = 0x20;
			CxxAbb::Int32 iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::Int32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::Int32>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::Int32>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::Int32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::Int32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::Int32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);
		}

		{
			CxxAbb::UInt32 iLe = 0x20;
			CxxAbb::UInt32 iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::UInt32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::UInt32>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::UInt32>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::UInt32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::UInt32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::UInt32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);
		}

		{
			CxxAbb::Int64 iLe = 0x20;
			CxxAbb::Int64 iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::Int64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::Int64>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::Int64>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::Int64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::Int64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::Int64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);
		}

		{
			CxxAbb::UInt64 iLe = 0x20;
			CxxAbb::UInt64 iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::UInt64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::UInt64>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::UInt64>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::UInt64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::UInt64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::UInt64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);
		}

	}
	else
	{
		{
			CxxAbb::Int16 iLe = 0x20;
			CxxAbb::Int16 iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::Int16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::Int16>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::Int16>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::Int16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::Int16>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::Int16>(iLe);
			ASSERT_EQ(iLe, iBe);
		}

		{
			CxxAbb::UInt16 iLe = 0x20;
			CxxAbb::UInt16 iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::UInt16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::UInt16>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::UInt16>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::UInt16>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::UInt16>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::UInt16>(iLe);
			ASSERT_EQ(iLe, iBe);
		}

		{
			CxxAbb::Int32 iLe = 0x20;
			CxxAbb::Int32 iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::Int32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::Int32>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::Int32>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::Int32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::Int32>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::Int32>(iLe);
			ASSERT_EQ(iLe, iBe);
		}

		{
			CxxAbb::UInt32 iLe = 0x20;
			CxxAbb::UInt32 iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::UInt32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::UInt32>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::UInt32>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::UInt32>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::UInt32>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::UInt32>(iLe);
			ASSERT_EQ(iLe, iBe);
		}

		{
			CxxAbb::Int64 iLe = 0x20;
			CxxAbb::Int64 iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::Int64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::Int64>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::Int64>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::Int64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::Int64>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::Int64>(iLe);
			ASSERT_EQ(iLe, iBe);
		}

		{
			CxxAbb::UInt64 iLe = 0x20;
			CxxAbb::UInt64 iBe = CxxAbb::ByteOrder::HostToLe<CxxAbb::UInt64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToBe<CxxAbb::UInt64>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::BeToHost<CxxAbb::UInt64>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::LeToHost<CxxAbb::UInt64>(iLe);
			ASSERT_NE(iLe, iBe);
			iBe = CxxAbb::Swab(iBe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::NetworkToHost<CxxAbb::UInt64>(iLe);
			ASSERT_EQ(iLe, iBe);

			iBe = CxxAbb::ByteOrder::HostToNetwork<CxxAbb::UInt64>(iLe);
			ASSERT_EQ(iLe, iBe);
		}
	}

}

TEST(CoreTest, TypeInfo)
{
	CxxAbb::Int32 i1 = 10;
	CxxAbb::Int32 i2 = 2000;
	CxxAbb::UInt32 ui3 = 32423000;
	CxxAbb::UInt64 ui4 = 0xAABBCCDDEEFF;

	CxxAbb::TypeInfo<CxxAbb::Int32> iType1(i1);
	CxxAbb::TypeInfo<CxxAbb::Int32> iType2(i2);
	CxxAbb::TypeInfo<CxxAbb::UInt32> uiType3(ui3);

	EXPECT_TRUE(iType1 == iType2);

	EXPECT_FALSE(iType1.IsEqual(uiType3));

	EXPECT_FALSE(iType1.IsEqual(ui4));

//	COUT_LOG() << iType1.Name();
//	COUT_LOG() << iType2.Name();
//	COUT_LOG() << uiType3.Name();

}

TEST(CoreTest, BitOps)
{
	CxxAbb::UInt32 flag = 0x0000;

	BITOP_SET_MASK(flag, 0x0010);
	ASSERT_EQ(CxxAbb::UInt32(0x0010), flag);

	BITOP_CLEAR_MASK(flag, 0x0010);
	ASSERT_EQ(CxxAbb::UInt32(0x0000), flag);

	BITOP_SET_MASK(flag, 0x1010);
	BITOP_CLEAR_MASK(flag, 0x0010);
	ASSERT_EQ(CxxAbb::UInt32(0x1000), flag);

	BITOP_TOGGLE_MASK(flag, 0x0011);
	ASSERT_EQ(CxxAbb::UInt32(0x1011), flag);
	BITOP_TOGGLE_MASK(flag, 0x0110);
	ASSERT_EQ(CxxAbb::UInt32(0x1101), flag);

	ASSERT_TRUE(BITOP_IS_ANY_SET_MASK(flag, 0x1000));
	ASSERT_FALSE(BITOP_IS_ANY_SET_MASK(flag, 0x0010));
	ASSERT_TRUE(BITOP_IS_ANY_SET_MASK(flag, 0x1001));
	ASSERT_TRUE(BITOP_IS_ANY_SET_MASK(flag, 0x0110));

	ASSERT_EQ(20, B8(00010100));
	ASSERT_EQ(0, B8(00000000));
	ASSERT_EQ(255, B8(11111111));
	ASSERT_EQ(85, B8(01010101));

	ASSERT_TRUE(BITOP_IS_EVEN(24));
	ASSERT_TRUE(BITOP_IS_EVEN(0x14));
	ASSERT_FALSE(BITOP_IS_EVEN(13));
	ASSERT_FALSE(BITOP_IS_EVEN(B8(01010101)));

	ASSERT_FALSE(BITOP_IS_ODD(24));
	ASSERT_FALSE(BITOP_IS_ODD(0x14));
	ASSERT_TRUE(BITOP_IS_ODD(13));
	ASSERT_TRUE(BITOP_IS_ODD(B8(01010101)));

	ASSERT_FALSE(BITOP_IS_SET(B8(0), 0));
	ASSERT_TRUE(BITOP_IS_SET(B8(1), 0));
	ASSERT_TRUE(BITOP_IS_SET(B8(01010101), 2));

	CxxAbb::UInt8 uiByte = B8(00000000);

	ASSERT_EQ(BITOP_SET(uiByte, 0), B8(00000001));
	ASSERT_EQ(BITOP_SET(uiByte, 1), B8(00000011));
	ASSERT_EQ(BITOP_SET(uiByte, 2), B8(00000111));
	ASSERT_EQ(BITOP_SET(uiByte, 3), B8(00001111));
	ASSERT_EQ(BITOP_SET(uiByte, 4), B8(00011111));
	ASSERT_EQ(BITOP_SET(uiByte, 5), B8(00111111));
	ASSERT_EQ(BITOP_SET(uiByte, 6), B8(01111111));
	ASSERT_EQ(BITOP_SET(uiByte, 7), B8(11111111));

	ASSERT_EQ(BITOP_SET(uiByte, 5), B8(11111111));
	ASSERT_EQ(BITOP_SET(uiByte, 1), B8(11111111));
	ASSERT_EQ(BITOP_SET(uiByte, 0), B8(11111111));

	uiByte = B8(11111111);
	ASSERT_EQ(BITOP_UNSET(uiByte, 0), B8(11111110));
	ASSERT_EQ(BITOP_UNSET(uiByte, 1), B8(11111100));
	ASSERT_EQ(BITOP_UNSET(uiByte, 2), B8(11111000));
	ASSERT_EQ(BITOP_UNSET(uiByte, 3), B8(11110000));
	ASSERT_EQ(BITOP_UNSET(uiByte, 4), B8(11100000));
	ASSERT_EQ(BITOP_UNSET(uiByte, 5), B8(11000000));
	ASSERT_EQ(BITOP_UNSET(uiByte, 6), B8(10000000));
	ASSERT_EQ(BITOP_UNSET(uiByte, 7), B8(00000000));

	ASSERT_EQ(BITOP_UNSET(uiByte, 2), B8(00000000));

	uiByte = B8(11111111);
	ASSERT_EQ(BITOP_TOGGLE(uiByte, 0), B8(11111110));
	ASSERT_EQ(BITOP_TOGGLE(uiByte, 0), B8(11111111));
	ASSERT_EQ(BITOP_TOGGLE(uiByte, 3), B8(11110111));
	ASSERT_EQ(BITOP_TOGGLE(uiByte, 3), B8(11111111));
	ASSERT_EQ(BITOP_TOGGLE(uiByte, 4), B8(11101111));
	ASSERT_EQ(BITOP_TOGGLE(uiByte, 4), B8(11111111));

	std::stringstream os;
	CxxAbb::Int8 i8 = 7;
	CxxAbb::Int8ToBinary(i8, os);
	COUT_LOG() << os.str();

	os.str("");
	CxxAbb::Int16 i16 = 258;
	CxxAbb::Int16ToBinary(i16, os);
	COUT_LOG() << os.str();

	os.str("");
	CxxAbb::Int32 i32 = 2346365;
	CxxAbb::Int32ToBinary(i32, os);
	COUT_LOG() << os.str();

}


namespace
{

static int gi = 0;

struct C
{
	void func();
};

template<typename T>
void h( T*  )
{
	gi++;
	//COUT_LOG() << "Called T*";
}

template<typename T>
void h( T  )
{
	gi--;
	//COUT_LOG() << "Called T";
}

void func (double *)
{
	gi++;
}

void func (int)
{
	gi--;
}

}

using CxxAbb::NullPtr;
TEST(CoreTest, NullPtrTest)
{
	char * ch = NullPtr;        // ok
	ASSERT_TRUE(ch == NullPtr);
	func (NullPtr);             // Calls func(double *)
	ASSERT_EQ(gi, 1);
	func (0);                   // Calls func(int)
	ASSERT_EQ(gi, 0);

	void (C::*pmf2)() = 0;      // ok
	void (C::*pmf)() = NullPtr; // ok

	ASSERT_TRUE(pmf2 == pmf); // both points to same

	//ASSERT_TRUE((typeof(pmf))NullPtr == pmf);

	CxxAbb::NullPtrT n1, n2;
	n1 = n2;
	//NullPtrT *null = &n1;    // Address can't be taken.
//	//if (NullPtr == pmf) {}      // Valid statement; but fails on g++ 4.1.1-4.5 due to bug #33990
//	// for GCC 4: if ((typeof(pmf))nullptr == pmf) {}

	//	const int n = 0;
//	if (NullPtr == n) {}        // Should not compile; but only Comeau shows an error.
//	//int p = 0;
//	//if (nullptr == p) {}      // not ok
//	//g (nullptr);              // Can't deduce T
	int expr = 0;
	char* ch3 = expr ? NullPtr : NullPtr; // ch3 is the null pointer value
	ASSERT_TRUE(ch3 == NullPtr);
	//char* ch4 = expr ? 0 : NullPtr;     // error, types are not compatible
//	//int n3 = expr ? nullptr : nullptr;  // error, nullptr can’t be converted to int
//	//int n4 = expr ? 0 : nullptr;        // error, types are not compatible
//
	h( 0 );                // deduces T = int
	ASSERT_EQ(gi, -1);
	h( NullPtr );          // deduces T = nullptr_t
	ASSERT_EQ(gi, -2);
	h( (float*) NullPtr ); // deduces T = float*
	ASSERT_EQ(gi, -1);
//
//	//sizeof( NullPtr );     // ok
//	//typeid( NullPtr );     // ok
//	//ASSERT_THROW(throw NullPtr, NullPtr);         // ok
}
