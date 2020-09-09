#ifndef GODEFV_ERROR_CHECKING_CHECK_EQUAL_CONTAINER_H
#define GODEFV_ERROR_CHECKING_CHECK_EQUAL_CONTAINER_H 

#include"fundamental_types.h"
#include"tuple.h"
#include"../return_status.h"

#include<map>
#include<string>
#include<unordered_map>
#include<vector>

namespace godefv{

template<class ContainerT> //TODO: put constraints on ContainerT
return_status_t check_equal_container(ContainerT const& a, ContainerT const& b){
	if(a.size()!=b.size()){
		return return_status_t{"different sizes (respectively "+std::to_string(a.size())+" and "+std::to_string(b.size())+")"};
	}
	using ::check_equal;
	auto a_it=a.begin();
	auto b_it=b.begin();
	auto i=0u;
	while(a_it!=a.end()){
		auto is_equal=check_equal(*a_it++, *b_it++);
		if(!is_equal) return return_status_t{+"element "+std::to_string(i)+" differs ("+is_equal.msg+")"};
		++i;
	}
	return return_status_t{};
}

} /* namespace godefv */

namespace std{
	template<class T>           godefv::return_status_t check_equal(std::vector<T>          const& a, std::vector<T>          const& b){return godefv::check_equal_container(a,b);}
	template<class K, class V>  godefv::return_status_t check_equal(std::map<K,V>           const& a, std::map<K,V>           const& b){return godefv::check_equal_container(a,b);}
	template<class K, class V>  godefv::return_status_t check_equal(std::unordered_map<K,V> const& a, std::unordered_map<K,V> const& b){return godefv::check_equal_container(a,b);}
	template<class Char>        godefv::return_status_t check_equal(std::basic_string<Char> const& a, std::basic_string<Char> const& b){return godefv::check_equal_container(a,b);}
}

#endif /* GODEFV_ERROR_CHECKING_CHECK_EQUAL_CONTAINER_H */
