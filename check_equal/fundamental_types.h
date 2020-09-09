#ifndef GODEFV_ERROR_CHECKING_CHECK_EQUAL_FUNDAMENTAL_TYPES_H
#define GODEFV_ERROR_CHECKING_CHECK_EQUAL_FUNDAMENTAL_TYPES_H 

#include"../return_status.h"

#include<cmath>
#include<type_traits>

template<class Float
		,std::enable_if_t<std::is_floating_point<Float>::value, int> = 0
		> 
godefv::return_status_t check_equal(Float const& a, Float const& b){
	if(std::isnan(a) && std::isnan(b)) return {};
	if(a==b) return {};
	return godefv::return_status_t{std::to_string(a)+"!="+std::to_string(b)};
}

template<class Integer
		,std::enable_if_t<std::is_integral<Integer>::value, int> = 0
        >
godefv::return_status_t check_equal(Integer const& a, Integer const& b){
	if(a==b) return {};
	return godefv::return_status_t{std::to_string(a)+"!="+std::to_string(b)};
}

inline godefv::return_status_t check_equal(void const* const& a, void const* const& b){
	if(a==b) return {};
	return godefv::return_status_t{"not equal"};
}

#endif /* GODEFV_ERROR_CHECKING_CHECK_EQUAL_FUNDAMENTAL_TYPES_H */
