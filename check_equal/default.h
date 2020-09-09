#ifndef GODEFV_ERROR_CHECKING_CHECK_EQUAL_DEFAULT_H
#define GODEFV_ERROR_CHECKING_CHECK_EQUAL_DEFAULT_H 

#include<error_checking/return_status.h>

template<class T>
godefv::return_status_t check_equal(T const& a, T const& b){
	if(a!=b) return godefv::return_status_t{"not equal"};
	return {};
}

#endif /* GODEFV_ERROR_CHECKING_CHECK_EQUAL_DEFAULT_H */
