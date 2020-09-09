#ifndef GODEFV_OS_ERROR_H
#define GODEFV_OS_ERROR_H 

#include<string>

namespace godefv{namespace os{

/** A simple class that stores an error code from an OS error and retrieve the corresponding error string. */
struct error_t{
	error_t()=default;
	error_t(int errcode);
	int code=0;                 /**< The error code from an OS error. */
	std::string string() const; /**< Retrieves the error string corresponding to code form the OS. */
};

/** Gets the last OS error. */
error_t last_error() noexcept;

}} /* namespace godefv::os */

#endif /* GODEFV_OS_ERROR_H */
