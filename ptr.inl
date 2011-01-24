#if !defined(__ptr_inl_include__)
#error "ptr.inl may only be included from ptr.h"
#endif // !defined(__ptr_inl_include__)



#ifndef __ptr_inl__
#define __ptr_inl__



#include <cassert>



struct ptr_counter
{
	ptr_counter() : _count(0) { /* empty */ };
	void inc() { _count++; }
	void dec() { _count--; }
	unsigned _count;
};



//
// default construction
//
template <typename X>
inline ptr<X>::ptr() : _ptr(0), _counter(0)
{
	// empty
}



//
// copying
//
template <typename X>
inline ptr<X>::ptr(const ptr<X>& other) : _ptr(0), _counter(0)
{
	// defer to copy assignment operator
	*this = other;
}

template <typename X>
inline ptr<X>& ptr<X>::operator=(const ptr<X>& other)
{
	// make certain it's not trying to copy assign itself to itself
	if ( this != &other )
	{
		// take the pointer and counter from the other and increment the count
		// TODO should we check that the other counter is non-zero?
		grab(other._ptr, other._counter);
	}

	// send back a reference to this object
	return *this;
}



//
// copying from a normal pointer
//
template <typename X>
inline ptr<X>::ptr(X* normal_ptr) : _ptr(0), _counter(0)
{
	// defer to operator
	*this = normal_ptr;
}

template <typename X>
inline ptr<X>& ptr<X>::operator=(X* normal_ptr)
{
	// initialize pointer and create new reference counter
	grab(normal_ptr, 0);

	// send back a reference to this object
	return *this;
}



//
// destructor
//
template <typename X>
inline ptr<X>::~ptr()
{
	// decrement count and possibly release pointer
	drop();
}



//
// take a pointer as ours and increment reference count
//
template <typename X>
inline void ptr<X>::grab(X* normal_ptr, ptr_counter* counter)
{
	// drop any pointer+counter we may already have
	drop();

	// if the pointer is 0, we're finished
	if ( !normal_ptr )
	{
		// zeroed out, nothing to do
		return;
	}

	// copy pointer
	_ptr = normal_ptr;

	// copy or create a new counter
	_counter = counter ? counter : new ptr_counter;
	assert(_counter);

	// increment reference count
	_counter->inc();
}



//
// reset our pointer and counter to zero and decrement reference count
// and, possibly, delete the pointer (if reference count goes to zero)
//
template <typename X>
inline void ptr<X>::drop()
{
	// check to see if we have anything to drop
	if ( valid() )
	{
		// decrement the count
		_counter->dec();

		// check if this is the last reference
		if ( unreferenced() )
		{
			// this is the last reference, delete the pointer and counter
			delete _ptr;
			delete _counter;
		}

		// now reset ("drop") the pointer and the counter
		_ptr     = 0;
		_counter = 0;
	}
}



// 
// comparison
//
template <typename X>
inline bool ptr<X>::operator==(const ptr<X>& other) const
{
	return _ptr == other._ptr;
}

template <typename X>
inline bool ptr<X>::operator!=(const ptr<X>& other) const
{
	return _ptr != other._ptr;
}

template <typename X>
inline bool ptr<X>::operator<(const ptr<X>& other) const
{
	return _ptr < other._ptr;
}

template <typename X>
inline bool ptr<X>::operator<=(const ptr<X>& other) const
{
	return _ptr <= other._ptr;
}

template <typename X>
inline bool ptr<X>::operator>(const ptr<X>& other) const
{
	return _ptr > other._ptr;
}

template <typename X>
inline bool ptr<X>::operator>=(const ptr<X>& other) const
{
	return _ptr >= other._ptr;
}



// 
// use the pointer
//
template <typename X>
inline X* ptr<X>::operator->() const
{
	assert(_ptr);
	return _ptr;
}

template <typename X>
inline X& ptr<X>::operator*() const
{
	assert(_ptr);
	return *_ptr;
}

template <typename X>
inline X& ptr<X>::operator[](size_t i)
{
	assert(_ptr);
	return _ptr[i];
}

template <typename X>
inline const X& ptr<X>::operator[](size_t i) const
{
	assert(_ptr);
	return _ptr[i];
}



//
// check whether pointer is valid
//
template <typename X>
inline ptr<X>::operator bool() const
{
	return valid();
}

template <typename X>
inline bool ptr<X>::valid() const
{
	return _ptr && _counter;
}



//
// miscellaneous utility methods
//
template <typename X>
inline unsigned ptr<X>::copies() const
{
	return valid() ? _counter->_count : 0;
}

template <typename X>
inline bool ptr<X>::shared() const
{
	return copies() > 1;
}

template <typename X>
inline bool ptr<X>::unique() const
{
	return copies() == 1;
}

template <typename X>
inline bool ptr<X>::unreferenced() const
{
	return copies() == 0;
}



//
// default construction
//
template <typename X>
inline array_ptr<X>::array_ptr() : _ptr(0), _counter(0)
{
	// empty
}



//
// copying
//
template <typename X>
inline array_ptr<X>::array_ptr(const array_ptr<X>& other) : _ptr(0), _counter(0)
{
	// defer to copy assignment operator
	*this = other;
}

template <typename X>
inline array_ptr<X>& array_ptr<X>::operator=(const array_ptr<X>& other)
{
	// make certain it's not trying to copy assign itself to itself
	if ( this != &other )
	{
		// take the pointer and counter from the other and increment the count
		// TODO should we check that the other counter is non-zero?
		grab(other._ptr, other._counter);
	}

	// send back a reference to this object
	return *this;
}



//
// copying from a normal pointer
//
template <typename X>
inline array_ptr<X>::array_ptr(X* normal_ptr) : _ptr(0), _counter(0)
{
	// defer to operator
	*this = normal_ptr;
}

template <typename X>
inline array_ptr<X>& array_ptr<X>::operator=(X* normal_ptr)
{
	// initialize pointer and create new reference counter
	grab(normal_ptr, 0);

	// send back a reference to this object
	return *this;
}



//
// destructor
//
template <typename X>
inline array_ptr<X>::~array_ptr()
{
	// decrement count and possibly release pointer
	drop();
}



//
// take a pointer as ours and increment reference count
//
template <typename X>
inline void array_ptr<X>::grab(X* normal_ptr, ptr_counter* counter)
{
	// drop any pointer+counter we may already have
	drop();

	// if the pointer is 0, we're finished
	if ( !normal_ptr )
	{
		// zeroed out, nothing to do
		return;
	}

	// copy pointer
	_ptr = normal_ptr;

	// copy or create a new counter
	_counter = counter ? counter : new ptr_counter;
	assert(_counter);

	// increment reference count
	_counter->inc();
}



//
// reset our pointer and counter to zero and decrement reference count
// and, possibly, delete the pointer (if reference count goes to zero)
//
template <typename X>
inline void array_ptr<X>::drop()
{
	// check to see if we have anything to drop
	if ( valid() )
	{
		// decrement the count
		_counter->dec();

		// check if this is the last reference
		if ( unreferenced() )
		{
			// this is the last reference, delete the pointer and counter
			delete[] _ptr;
			delete _counter;
		}

		// now reset ("drop") the pointer and the counter
		_ptr     = 0;
		_counter = 0;
	}
}



// 
// comparison
//
template <typename X>
inline bool array_ptr<X>::operator==(const array_ptr<X>& other) const
{
	return _ptr == other._ptr;
}

template <typename X>
inline bool array_ptr<X>::operator!=(const array_ptr<X>& other) const
{
	return _ptr != other._ptr;
}

template <typename X>
inline bool array_ptr<X>::operator<(const array_ptr<X>& other) const
{
	return _ptr < other._ptr;
}

template <typename X>
inline bool array_ptr<X>::operator<=(const array_ptr<X>& other) const
{
	return _ptr <= other._ptr;
}

template <typename X>
inline bool array_ptr<X>::operator>(const array_ptr<X>& other) const
{
	return _ptr > other._ptr;
}

template <typename X>
inline bool array_ptr<X>::operator>=(const array_ptr<X>& other) const
{
	return _ptr >= other._ptr;
}



// 
// use the pointer
//
template <typename X>
inline X* array_ptr<X>::operator->() const
{
	assert(_ptr);
	return _ptr;
}

template <typename X>
inline X& array_ptr<X>::operator*() const
{
	assert(_ptr);
	return *_ptr;
}

template <typename X>
inline X& array_ptr<X>::operator[](size_t i)
{
	assert(_ptr);
	return _ptr[i];
}

template <typename X>
inline const X& array_ptr<X>::operator[](size_t i) const
{
	assert(_ptr);
	return _ptr[i];
}



//
// check whether pointer is valid
//
template <typename X>
inline array_ptr<X>::operator bool() const
{
	return valid();
}

template <typename X>
inline bool array_ptr<X>::valid() const
{
	return _ptr && _counter;
}



//
// miscellaneous utility methods
//
template <typename X>
inline unsigned array_ptr<X>::copies() const
{
	return valid() ? _counter->_count : 0;
}

template <typename X>
inline bool array_ptr<X>::shared() const
{
	return copies() > 1;
}

template <typename X>
inline bool array_ptr<X>::unique() const
{
	return copies() == 1;
}

template <typename X>
inline bool array_ptr<X>::unreferenced() const
{
	return copies() == 0;
}



#endif // __ptr_inl__

