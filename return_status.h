#ifndef GODEFV_ERROR_CHECKING_RETURN_STATUS_H
#define GODEFV_ERROR_CHECKING_RETURN_STATUS_H 

#include<functional>
#include<iostream>
#include<string>

namespace godefv{

//! Return status type that includes an error flag and potential error information. See return_status.md for examples.
struct return_status_t{
	bool no_error=true; //!< The error flag. 
	std::string msg;    //!< The error message (optional). 
	int code;           //!< The error code (optional). 

	return_status_t()=default;
	return_status_t(return_status_t const&)=default;
	return_status_t(return_status_t &&)=default;
	explicit return_status_t(std::string const& error_msg, int code_=0); //!< An error occured and the error message is set to error_msg 

	return_status_t& operator=(return_status_t const&)=default;
	return_status_t& operator=(return_status_t &&    )=default;

	template<class F> void on_error  (F const& fun) const; //!< Convenient member function for handling error cases. 
	template<class F> void on_success(F const& fun) const; //!< Convenient member function for handling success cases. 

	operator bool() const;    //!< Returns true if no error occured. 
	
	//! Error handler class that just prints the error message, and which you can pass to the on_error() member function. 
	class print{
		std::ostream& out;
	public:
		print(std::ostream& o);
		void operator()(return_status_t const& e) const;
	};
	//! Error handler class that just throws the error message, and which you can pass to the on_error() member function. 
	struct throw_{
		void operator()(return_status_t const& e) const;
	};
	//! Error handler class that exits the program displaying the error message, and which you can pass to the on_error() member function. 
	struct exit{
		void operator()(return_status_t const& e) const;
	};
};

//! Return value with a return status to pass errors. See return_status.md for examples. 
//! The return value is default constructed on error and this construction should be optimized out in branches in which the error is catched and the value not used. 
template<class T>
struct return_value_and_status_t{
	T value;
	return_status_t status;

	return_value_and_status_t()=default;
	return_value_and_status_t(return_value_and_status_t<T> const&)=default;
	return_value_and_status_t(return_value_and_status_t<T> &&    )=default;
	return_value_and_status_t(return_status_t const& s); //!< An error occured with status s. (implicit conversion allowed) 
	return_value_and_status_t(T const& v);               //!< No error, return value is v.    (implicit conversion allowed) 

	return_value_and_status_t<T>& operator=(return_value_and_status_t<T> const&)=default;
	return_value_and_status_t<T>& operator=(return_value_and_status_t<T> &&    )=default;

	template<class F> T               on_error  (F const& fun) const; //!< Convenient member function for handling error cases. 
	template<class F> return_status_t on_success(F const& fun) const; //!< Convenient member function for handling success cases. 

	operator T() const; //!< returns value if no error occured, or a default constructed T otherwise 
};


// return_status_t implementation 
template<class F> void return_status_t::on_error  (F const& fun) const {
	if(!no_error) fun(*this);
}

template<class F> void return_status_t::on_success(F const& fun) const {
	if(no_error) fun();
}

// return_value_and_status_t implementation 
template<class T> return_value_and_status_t<T>::return_value_and_status_t(return_status_t const& s):status(s){}
template<class T> return_value_and_status_t<T>::return_value_and_status_t(T const& v):value(v){}

template<class T> template<class F> T    return_value_and_status_t<T>::on_error  (F const& fun) const {
	status.on_error(fun);
	return static_cast<T const>(*this);
}
template<class T> template<class F> return_status_t return_value_and_status_t<T>::on_success(F const& fun) const {
	if(status.no_error) fun(value);
	return status;
}

template<class T> return_value_and_status_t<T>::operator T() const{return value;}

} // namespace godefv 

#endif // GODEFV_ERROR_CHECKING_RETURN_STATUS_H 
