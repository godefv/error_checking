#include"system.h"

#ifdef _WIN32
	#include<windows.h> 
#else
	#include<cerrno>
	#include<cstring>
#endif

namespace godefv{namespace os{

error_t::error_t(int errcode):code(errcode){}

#ifdef _WIN32
std::string error_t::string() const{
	if(!code) return std::string{"(no error)"};

	LPVOID lpMsgBuf;
	DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );
	if(!bufLen) return std::string{};

	LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
	std::string result(lpMsgStr, lpMsgStr+bufLen);

	LocalFree(lpMsgBuf);

	return result;
}
#else
std::string error_t::string() const{
	char * e = strerror(code);
	return e ? e : "";
}
#endif

error_t last_error() noexcept{
#ifdef _WIN32
	return error_t{static_cast<int>(GetLastError())};
#else
	return error_t{errno};
#endif
}

}} /* namespace godefv::os */
