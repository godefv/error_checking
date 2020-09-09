#include"return_status.h"
#include"exit.h"
#include"check_equal/container.h"
#include"check_equal/fundamental_types.h"

#include<sstream>
#include<iostream>

int main(){
	{
	auto ok=godefv::return_status_t{};
	auto ko=godefv::return_status_t{"message"};

	godefv::exit_if_false(ok, "ok should be true");
	godefv::exit_if_true (ko, "ko should be false");

	ok.on_error  ([](auto const&){godefv::exit("ok.on_error was executed");});
	ko.on_success([](){godefv::exit("ko.on_success was executed");});

	std::ostringstream error_stream;
	ok.on_success([&](){error_stream<<"ok.on_success ";});
	ko.on_error(godefv::return_status_t::print(error_stream));
	check_equal(error_stream.str(), std::string{"ok.on_success message"}).on_error(godefv::return_status_t::exit{});
	}
	{
	auto ok=godefv::return_value_and_status_t<int>{4};
	auto ko=godefv::return_value_and_status_t<int>{godefv::return_status_t{"message"}};

	check_equal<int>(ok,4).on_error(godefv::return_status_t::exit{});
	//value in ko is undefined

	std::ostringstream error_stream;
	ok.on_success([&](int a){error_stream<<a;})
	  .on_error([](auto const&){godefv::exit("ok.on_error was executed");});

	ko.on_success([](auto const&){godefv::exit("ko.on_success was executed");})
	  .on_error(godefv::return_status_t::print(error_stream));
	
	check_equal(error_stream.str(), std::string{"4message"}).on_error(godefv::return_status_t::exit{});
	}

	std::cout<<"success"<<std::endl;
	return 0;
}

