# return status

Some functions may encounter errors, in which case it is desirable to communicate those errors to the caller. There are a few common ways to do it :

**1. Display an error message or warning, possibly in a file, stdout, stderr, or a gui.**

Sometimes, by simplicity and lack of good options, the error is just displayed somewhere. For instance, GTK is known to spam errors and warnings in its console. It is better than nothing but it is unpleasant to the end user who is flooded when he didn't ask for it, and unpleasant to the person looking for useful information in all those messages.

Moreover, the caller of the function does not know if an error has occurred.

It is not the responsibility of the function which encounters an error to decide what to do with the error, it needs to pass it back to its caller.

**2. Return an error code in an integer or enum.** 

This is the most common way in C. The main drawback is that if the function should return some result, it has to take a pointer to some preallocated memory and write the result in there. 
```
int f(T1 arg1, T2 arg2, result_t* result);

result_t result;
if(f(arg1,arg2,&result)!=success){/* handle error */}
//there is no strong guarantee that `result` is initialized, function called cannot be chained, it is harder to read than `result_t result=f(arg1,arg2);`
```

**3. Throw an exception.**

First, if somehow the exception is not handled, the exception will go through every function in the call stack and eventually terminate the program, sometimes without even telling what happened. In other words, it may cause the program to crash.

Then, catching exception can be very bulky for the caller :
```
try{
	do_something_which_may_throw();
}catch(std::exception const&){
	/* handle error */
}
//who wants to write this every time he calls a function which may throw ?
//if he doesn't the whole program may crash !!
```

Finally, the performance of throwing and catching exceptions is horrible. So it is unreasonable to use this mechanism for errors which are not exceptional.

The conclusion is that exceptions should be used only for errors which are known to be fatal.

**4. Put the error in a global variable (errno).**

In this case, there is no clue that the function might encounter an error. One has to know that the function might pass an error through `errno` and check this global variable every time such a function is called.

---

None of the above is satisfactory. The purpose of the `return_status_t` and `return_value_and_status_t<T>` classes is to fill this gap. 

Usage of `return_status_t` :
```
//define a function which may encounter an error
return_status_t do_stuff(){
	try_stuff(); 
	if(stuff_ok()) return return_status_t{}; 
	else           return return_status_t{"my error message"};
}

//simple case
if(do_stuff().no_error){process_stuff();} 

//.on_error with a lambda
do_stuff().on_error([](auto const& e){std::cerr<<"Error in do_stuff: "<<e.msg<<std::endl;});

//.on_error with predefined error handling
do_stuff().on_error(return_status_t::print{std::cerr});
do_stuff().on_error(return_status_t::exit{});
do_stuff().on_error(return_status_t::throw_{});
```

Usage of `return_value_and_status_t<T>` :
```
//define a function which may encounter an error or return a value
return_value_and_status_t<toto_t> do_stuff(){
	toto_t value=try_stuff(); 
	if(stuff_ok()) return value; 
	else           return return_status_t{"my error message"};
}

//if it is ok to get an error, but we want to do something in case of success
do_stuff().on_success([](toto_t const& toto){process_stuff(toto)});

//or if we also want to do something when an error occurs
do_stuff().on_success([](toto_t const& toto){process_stuff(toto)})
          .on_error(return_status_t::print{std::cerr});

//if error is fatal in our context
toto_t toto=do_stuff().on_error(return_status_t::exit);
process_stuff(toto);

//if the caller just wants to do a quick test without bothering about errors
toto_t toto=do_stuff(); //toto_t is default constructed if an error is returned
process_stuff(toto);

//using structured bindings
auto [toto,return_status]=do_stuff();
if(return_status.no_error){
	process_stuff(toto);
}else{
	//as we are not in a lambda, we can return from the current function
	//typically, we can pass the error, possibly adding details to it
	return return_status_t{"failed to make toto: "+return_status.message};
}
```


