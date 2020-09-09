#ifndef GODEFV_ERROR_CHECKING_EXIT_H
#define GODEFV_ERROR_CHECKING_EXIT_H

#include<string>

#ifdef _MSC_VER
#define NORETURN __declspec(noreturn)
#else
#define NORETURN [[noreturn]]
#endif

namespace godefv{

//! If true (default), calling godefv::exit will open a popup window displaying the error message.
//! In this case, the calling process terminates only when the user closes this popup window.
extern bool error_checking_exit_has_popup;

//! Exits the calling process, displaying given message and last os error.
NORETURN void exit(std::string const& msg) noexcept;
void exit_if_false(bool condition, std::string const& msg);
void exit_if_true (bool condition, std::string const& msg);

}

#endif /* GODEFV_ERROR_CHECKING_EXIT_H */
