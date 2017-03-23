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
 * EnvironmentImpl.cpp
 *
 * FileId      : $Id$
 *
 * Created by  : Prabodha Srimal <prabodha007@gmail.com> - Jan 2, 2013
 * Edited by   : $Author$
 * Edited date : $Date$
 * Version     : $Revision$
 *
 * Library     : CxxAbbCore
 * Module      : System
 * Comment     : Application Running Environment - POSIX implimentation
 *
 */

#include "EnvironmentImpl.h"
#include <CxxAbb/Exception.h>
#include <CxxAbb/Sys/ScopedLock.h>

#include <cstdlib> // for get set env
#include <unistd.h> // for sysconf
#include <sys/utsname.h> // for utsname
#include <sys/param.h>
#include <sys/sysctl.h>
#include <string.h>

#if (CXXABB_OS == CXXABB_OS_LINUX) || (CXXABB_OS == CXXABB_OS_CYGWIN)

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if.h>
#if (CXXABB_OS != CXXABB_OS_CYGWIN)
#include <net/if_arp.h>
#else // workaround for Cygwin, which does not have if_arp.h
#define ARPHRD_ETHER 1 /* Ethernet 10Mbps */
#endif
#include <arpa/inet.h>
#include <errno.h>

#elif CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_BSD

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if_dl.h>

#elif CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_UNIX

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <net/if_arp.h>

#endif

namespace CxxAbb
{

namespace Sys
{

CxxAbb::Sys::FastMutex EnvironmentImpl::m_Mutex;

std::string EnvironmentImpl::GetImpl(const std::string & _key)
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(m_Mutex);

	const char* val = ::getenv(_key.c_str());
	if (val)
		return std::string(val);
	else
		throw CxxAbb::NotFoundException(_key);
}

std::string EnvironmentImpl::GetImpl(const std::string & _key, const std::string & _default)
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(m_Mutex);

	const char* val = ::getenv(_key.c_str());
	if (val)
		return std::string(val);
	else
		return _default;
}

bool EnvironmentImpl::HasImpl(const std::string & _key)
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(m_Mutex);

	return (::getenv(_key.c_str()) != 0);
}

void EnvironmentImpl::SetImpl(const std::string & _key, const std::string & _value)
{
	CxxAbb::Sys::FastMutex::ScopedLock lock(m_Mutex);


	std::string val = _key + "=" + _value;
	int ret = ::putenv(const_cast<char*>(val.c_str()));
	if (ret)
	{
		std::string msg = "Setting Environment variable failed : " + val;
		throw CxxAbb::SystemException(msg, ret);
	}
}

std::string EnvironmentImpl::OsNameImpl()
{
	struct utsname uts;
	::uname(&uts);
	return uts.sysname;
}

std::string EnvironmentImpl::HostNameImpl()
{
	struct utsname uts;
	::uname(&uts);
	return uts.nodename;
}

void EnvironmentImpl::EthernetAddressImpl(EnvironmentImpl::NodeId & _address)
{
#if (CXXABB_OS == CXXABB_OS_LINUX) || (CXXABB_OS == CXXABB_OS_CYGWIN)

	// SO link - http://stackoverflow.com/a/1779758/1822994

	std::memset(&_address, 0, sizeof(_address));
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1) return;

	// the following code is loosely based
	// on W. Richard Stevens, UNIX Network Programming, pp 434ff.
	int lastlen = 0;
	int len = 100*sizeof(struct ifreq);
	struct ifconf ifc;
	char* buf = 0;
	for (;;)
	{
		buf = new char[len];
		ifc.ifc_len = len;
		ifc.ifc_buf = buf;
		if (::ioctl(sock, SIOCGIFCONF, &ifc) < 0)
		{
			if (errno != EINVAL || lastlen != 0)
			{
				close(sock);
				delete [] buf;
				return;
			}
		}
		else
		{
			if (ifc.ifc_len == lastlen)
				break;
			lastlen = ifc.ifc_len;
		}
		len += 10*sizeof(struct ifreq);
		delete [] buf;
	}
	for (const char* ptr = buf; ptr < buf + ifc.ifc_len;)
	{
		const struct ifreq* ifr = reinterpret_cast<const struct ifreq*>(ptr);
		int rc = ioctl(sock, SIOCGIFHWADDR, ifr);
		if (rc != -1)
		{
			const struct sockaddr* sa =
				reinterpret_cast<const struct sockaddr*>(&ifr->ifr_hwaddr);
			if (sa->sa_family == ARPHRD_ETHER)
			{
				std::memcpy(&_address, sa->sa_data, sizeof(_address));
				break;
			}
		}
		ptr += sizeof(struct ifreq);
	}
	close(sock);
	delete [] buf;

#elif CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_BSD

	std::memset(&_address, 0, sizeof(_address));
	struct ifaddrs* ifaphead;
	int rc = getifaddrs(&ifaphead);
	if (rc) return;

	for (struct ifaddrs* ifap = ifaphead; ifap; ifap = ifap->ifa_next)
	{
		if (ifap->ifa_addr && ifap->ifa_addr->sa_family == AF_LINK)
		{
			struct sockaddr_dl* sdl = reinterpret_cast<struct sockaddr_dl*>(ifap->ifa_addr);
			caddr_t ap = LLADDR(sdl);
			int alen = sdl->sdl_alen;
			if (ap && alen > 0)
			{
				std::memcpy(&_address, ap, sizeof(_address));
				break;
			}
		}
	}
	freeifaddrs(ifaphead);

#elif CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_UNIX

	std::memset(&_address, 0, sizeof(_address));
	char name[MAXHOSTNAMELEN];
	if (gethostname(name, sizeof(name)))
		return;

	struct hostent* pHost = gethostbyname(name);
	if (!pHost) return;

	int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == -1) return;

	struct arpreq ar;
	std::memset(&ar, 0, sizeof(ar));
	struct sockaddr_in* pAddr = reinterpret_cast<struct sockaddr_in*>(&ar.arp_pa);
	pAddr->sin_family = AF_INET;
	std::memcpy(&pAddr->sin_addr, *pHost->h_addr_list, sizeof(struct in_addr));
	int rc = ioctl(s, SIOCGARP, &ar);
	close(s);
	if (rc < 0) return;
	std::memcpy(&_address, ar.arp_ha.sa_data, sizeof(_address));

#endif
}

std::string EnvironmentImpl::OsVersionImpl()
{
	struct utsname uts;
	::uname(&uts);
	return uts.release;
}

std::string EnvironmentImpl::OsArchitectureImpl()
{
	struct utsname uts;
	::uname(&uts);
	return uts.machine;
}

unsigned int EnvironmentImpl::ProcessorCountImpl()
{
#if (CXXABB_OS_FAMILY == CXXABB_OS_FAMILY_BSD)
	unsigned count;
	std::size_t size = sizeof(count);
	if (sysctlbyname("hw.ncpu", &count, &size, 0, 0))
		return 1;
	else
		return count;
#elif defined(_SC_NPROCESSORS_ONLN)
	int count = sysconf(_SC_NPROCESSORS_ONLN);
	if (count <= 0) count = 1;
	return static_cast<int>(count);
#else
	return 1;
#endif
}

}  /* namespace Sys */

}  /* namespace CxxAbb */



 
