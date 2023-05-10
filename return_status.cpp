#include"return_status.h"
#include"exit.h"

#include<stdexcept>

namespace godefv{

/* return_status_t:: */
return_status_t::return_status_t(std::string const& s, int code_):no_error(false),msg(s),code(code_){};

return_status_t::operator bool() const{
	return no_error;
}

/* return_status_t::print:: */
return_status_t::print::print(std::ostream& o):out(o){}

void return_status_t::print::operator()(return_status_t const& e) const{
	out<<e.msg<<std::endl;
}

/* return_status_t::throw_:: */
void return_status_t::throw_::operator()(return_status_t const& e) const{
	throw std::runtime_error{e.msg};
}

/* return_status_t::exit:: */
void return_status_t::exit::operator()(return_status_t const& e) const{
	godefv::exit(e.msg);
}

} /* namespace godefv */
