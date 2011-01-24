#ifndef __ptr_h__
#define __ptr_h__



//
//
//
// smart pointer - a.k.a. "FORGET ABOUT delete FOREVER"
//
//
// This pointer wrapper class makes it so that you don't have to
// delete objects on your own.  It's a reference counting implementation
// and should be safe to use in STL (or similar) containers.
//
// However, please use caution when passing them around as parameters.
// See the note at the bottom of this comment for details.
//
//
// First, a simple example:
//
//   ptr<int> p = new int; // notice "ptr<int>" instead of the usual "int*"?
// 
// when p goes out of scope (or it's owning object is destroyed), the int will
// get deleted as well.
//
//
// This works exactly the same for non trivial classes:
//
//   ptr<SomeClass> p = new SomeClass;
//
//
// You may also initialize the ptr directly instead of using assignment
// (this may even be preferred!). Here's an example:
//
//   ptr<SomeClass> p(new SomeClass);
//
//
// You can check the ptr's validity in a few ways:
//
//   ptr<SomeClass> p(new SomeClass);
//   if ( p ) // most elegant!
//     ...
//   if ( p.valid() ) // pretty good...
//     ...
//   if ( p != 0 ) // ERROR! ugly anyway, stop doing this forever please
//     ...
//   if ( p != NULL ) // ERROR! the *ugliest*, stop it now!
//
//
// Of course, you may use the ptr just about exactly in the same ways
// you can use a normal C++ pointer:
//
//   ptr<int> p(new int);
//   (*p) = 8675309; // note parens
//   printf("*p = %d\n",*p); // by using the '*' operator
//
//   ptr<SomeClass> p(new SomeClass);
//   p->SomeMethod(); // or by using the '->' operator
//
//   array_ptr<SomeClass> p(new SomeClass[10]); // note "array_ptr"
//   p[3].SomeMethod(); // or by using the array [] operator
//
//
// When you're finished with the object, you may simply let its ptr<>
// go out of scope, or you may remove your reference to it by assigning
// the pointer to 0.
//
//   {
//     ptr<SomeClass> p(new SomeClass);
//     ...
//   } // p automatically deleted here (if no references remain!)
//
//   ptr<SomeClass> p(new SomeClass);
//   p = 0; // p automatically deleted here (if no references remain!)
//
//
// Note that because of this, there's no problem in assigning one ptr<>
// to another.  For example:
//
//   ptr<SomeClass> pA = new SomeClass; // construct ONE object
//   ptr<SomeClass> pB;
//
//   pB = pA; // now our object is referenced by both pA and pB
//   pA = 0;  // now it's only referenced by pB
//   pB = 0;  // that was the last reference, so the object is destroyed
//
//
// This may be particularly useful in the case where you might want to
// maintain a list of pointers to a base class.  For example:
//
//   class Base { ... };
//   class Derived1 : public Base { ... };
//   class Derived2 : public Base { ... };
//
//   std::list< ptr<Base> > objects;
//
//   objects.push_back(new Base);
//   objects.push_back(new Derived1);
//   objects.push_back(new Derived2);
//
//   // at this point you've new'd off new objects, and can forget about them.
//   // when the list is cleared() or destroyed (by going out of scope in
//   // this case), your objects will be automatically cleared.
//   //
//   // note that you may also copy the list, or transfer it to another
//   // sort of container, as such
//
//   std::vector< ptr<Base> > objects_vec;
//   for ( std::list< ptr<Base> >::iterator it = objects.begin(); it != objects.end(); ++it )
//   {
//     objects_vec.push_back( *it );
//   }
//
//   // now we could even wipe out the original list
//   objects.clear();
//
//   // none of our original objects are deleted because they are still referenced
//   // in the objects_vec vector
//
//   // this would actually cause the objects to be deleted
//   objects_vec.clear();
//
//
// One caveat, you will not be able to call delete!  If you want to force the deletion
// of an object, you can assign the pointer to 0.  It'll be deleted if that pointer
// was the last reference to the object.
//
//   ptr<int> p(new int);
//
//   delete p; // ERROR!  IMPOSSIBLE.. I said to "FORGET ABOUT delete FOREVER" :)
//   p = 0; // Good! will actually delete the int (as long as p's the only reference to it)
//
//
// It's hard to really call this a "caveat" with a straight face.
//
//
// Ok, on a more serious note, this could really bite you if you aren't careful.
// For example:
//
//   void foo(ptr<int> pInt) { ... }
//
//   void bar()
//   {
//     int* x = new int;
//     int y = 0;
//
//     foo(x); // ERROR! foo()'s ptr<int> stack object deletes x!
//     foo(&y); // ERROR! foo()'s ptr<int> stack object deletes &y!
//   }
//
// So, be safe when you're passing these things around.
//
// Enjoy!  --Steve
//
//
//



#include <cstddef>



struct ptr_counter;

template <typename T>
class ptr
{
public:

	// default constructor
	ptr();

	// copying construction and assignment
	ptr(const ptr<T>& other);
	ptr& operator=(const ptr<T>& other);

	// copy from a normal pointer, construction and assignment
	ptr(T* normal_ptr);
	ptr& operator=(T* normal_ptr);

	// destruction
	~ptr();

	// comparison
	bool operator== (const ptr<T>& other) const;
	bool operator!= (const ptr<T>& other) const;
	bool operator<  (const ptr<T>& other) const;
	bool operator<= (const ptr<T>& other) const;
	bool operator>  (const ptr<T>& other) const;
	bool operator>= (const ptr<T>& other) const;

	// use the pointer
	T* operator->() const;
	T& operator*() const;
	T& operator[](size_t i);
	const T& operator[](size_t i) const;

	// check whether pointer is valid
	operator bool() const;
	bool valid() const;

private:

	// these do the work of taking a pointer in, updating reference count, etc.
	void grab(T* normal_ptr, ptr_counter* counter);
	void drop();

	// private utilities
	unsigned copies() const;
	bool shared() const;
	bool unique() const;
	bool unreferenced() const;

	// data
	T*           _ptr;
	ptr_counter* _counter;

};



//
// array_ptr<> is an exact copy of ptr<> except it uses 'delete[]' instead of 'delete'
//

template <typename X>
class array_ptr
{
public:

	// default constructor
	array_ptr();

	// copying construction and assignment
	array_ptr(const array_ptr<X>& other);
	array_ptr& operator=(const array_ptr<X>& other);

	// copy from a normal pointer, construction and assignment
	array_ptr(X* normal_ptr);
	array_ptr& operator=(X* normal_ptr);

	// destruction
	~array_ptr();

	// comparison
	bool operator==(const array_ptr<X>& other) const;
	bool operator!=(const array_ptr<X>& other) const;
	bool operator<(const array_ptr<X>& other) const;
	bool operator<=(const array_ptr<X>& other) const;
	bool operator>(const array_ptr<X>& other) const;
	bool operator>=(const array_ptr<X>& other) const;

	// use the pointer
	X* operator->() const;
	X& operator*() const;
	X& operator[](size_t i);
	const X& operator[](size_t i) const;

	// check whether pointer is valid
	operator bool() const;
	bool valid() const;

private:

	// these do the work of taking a pointer in, updating reference count, etc.
	void grab(X* normal_ptr, ptr_counter* counter);
	void drop();

	// private utilities
	unsigned copies() const;
	bool shared() const;
	bool unique() const;
	bool unreferenced() const;

	// data
	X*           _ptr;
	ptr_counter* _counter;

};



#define __ptr_inl_include__
#include "ptr.inl"
#undef __ptr_inl_include__



#endif // __ptr_h__

