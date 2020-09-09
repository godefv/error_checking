#ifndef GODEFV_ERROR_CHECKING_CHECK_EQUAL_TUPLE_H
#define GODEFV_ERROR_CHECKING_CHECK_EQUAL_TUPLE_H 

#include"fundamental_types.h"
#include"../return_status.h"

#include<array>
#include<utility> //pair
#include<tuple>

namespace godefv{

template<size_t I, class TupleT> struct check_equal_tuple_t{
return_status_t operator() (TupleT const& a, TupleT const& b){
	using ::check_equal;
	auto is_equal=check_equal(std::get<I-1>(a), std::get<I-1>(b));
	if(!is_equal) return return_status_t{"std::get<"+std::to_string(I-1)+">("+typeid(a).name()+") differs ("+is_equal.msg+")"};
	return check_equal_tuple_t<I-1, TupleT>{}(a,b);
}
};

template<class TupleT> struct check_equal_tuple_t<0,TupleT>{
	return_status_t operator() (TupleT const& a, TupleT const& b){return {};}
};

template<size_t I, class TupleT> inline auto check_equal_tuple(TupleT const& a, TupleT const& b){return check_equal_tuple_t<I,TupleT>{}(a,b);}

} /* namespace godefv */

namespace std{
	template<class T1, class T2> godefv::return_status_t check_equal(std::pair<T1,T2>  const& a, std::pair<T1,T2>  const& b){return godefv::check_equal_tuple<2>(a,b);}
	template<class T, size_t N>  godefv::return_status_t check_equal(std::array<T,N>   const& a, std::array<T,N>   const& b){return godefv::check_equal_tuple<N>(a,b);}
	template<class... Ts>        godefv::return_status_t check_equal(std::tuple<Ts...> const& a, std::tuple<Ts...> const& b){return godefv::check_equal_tuple<sizeof...(Ts)>(a,b);}
}

#endif /* GODEFV_ERROR_CHECKING_CHECK_EQUAL_TUPLE_H */
