#include"exit.h"
#include"system.h"

#include<cstdlib>
#include<iostream>
#include<sstream>

#ifdef _WIN32
#include<windows.h>
#endif

namespace godefv{

bool error_checking_exit_has_popup=true;

NORETURN void exit(std::string const& msg) noexcept {
	std::stringstream msg_stream;
	msg_stream<<msg<<"\n";

	auto error_system=godefv::os::last_error();
	if(error_system.code){
		msg_stream<<"Last system error ("<<error_system.code<<"): "<<error_system.string()<<"\n";
	}

	msg_stream<<std::flush;
	auto msg_string=msg_stream.str();

	std::cerr<<msg_string<<std::flush;
#ifdef _WIN32
	if(error_checking_exit_has_popup){
		MessageBox(GetDesktopWindow(),msg_string.c_str(),"fatal error",MB_OK | MB_ICONERROR | MB_SETFOREGROUND | MB_TOPMOST);
	}
#endif

	std::exit(1);
}

void exit_if_false(bool condition, std::string const& msg){
	if(condition==false) exit(msg);
}

void exit_if_true(bool condition, std::string const& msg){
	if(condition==true) exit(msg);
}


}

