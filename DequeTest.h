// ----------------------
// prog/deque/DequeTest.h
// Tj Wrenn
// ----------------------

#ifndef DequeTest_h
#define DequeTest_h

// --------
// includes
// --------

#include <cassert>   // assert
#include <stdexcept> // out_of_range
#include <string>    // string

// ----------
// namespaces
// ----------

namespace dt {
namespace prog  {
namespace deque  {

// ----------
// deque_test
// ----------

/**
 * function deque_test is a tester of class Deque
 */
template <typename Deque>
void deque_test () {
	// ----------
    // your tests
    // ----------
    
	typedef typename Deque::value_type		value_type;
	        
	typedef typename Deque::size_type		size_type;
	typedef typename Deque::difference_type	difference_type;

	typedef typename Deque::pointer         pointer;
	typedef typename Deque::const_pointer   const_pointer;

	typedef typename Deque::reference       reference;
	typedef typename Deque::const_reference const_reference;
	
	{
	//default constructor
	Deque a;
	assert(a.size() == 0);
	assert(a.empty());
	
	//deque with 5 elements
	Deque b(5);
	assert(b.size() == 5);
	assert(!b.empty());
		
	//copy constructor
	Deque c(b);
	assert(c.size() == 5);
	assert(b.size() == 5);
	assert(!c.empty());
	assert(!b.empty());
	
	}
	{
	//large deque construction all initialized to -1
	Deque bigb(4000000, -1);
	assert(bigb.size() == 4000000);
	assert(!bigb.empty());
	
	//make sure all values are indeed initialized to -1
	for(int i=0;i<bigb.size();++i)
		assert(bigb[i] == -1);	
	}
	
	{
	//Max num of possible elements
	Deque a(5, 10);
	
	// comparing equal deques
	Deque b(5, 10);
	assert(a == b);
	assert(!(a != b));
	assert(!(a < b));
	assert(!(a > b));
	assert(a <= b);
	assert(a >= b);
	
	// comparing deques with different number of elements
	Deque c(10, 10);
	assert(b != c); // should just compare sizes
	assert(!(b == c));
	assert(b < c);
	assert(!(b > c));
	assert(b <= c);
	assert(!(b >= c));
	
	// comparing deques with different elements
	Deque d(10, 9);
	assert(d != c);
	assert(!(d == c));
	assert(d < c);
	assert(!(d > c));
	assert(d <= c);
	assert(!(d >= c));
	
	// comparing deques with a different number of elements and different elements
	Deque e(9, 10);
	assert(e != c); // should just compare sizes
	e.push_back(9);
	assert(e != c); // should compare all elements until a difference is found
	assert(!(e == c));
	assert(e < c);
	assert(!(e > c));
	assert(e <= c);
	assert(!(e >= c));
	}
	
	{
	// at, []
	Deque a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	
	assert(a.at(0) == 1);
	assert(a.at(2) == 3);
	assert(a[0] == 1);
	assert(a[2] == 3);
		
	try {
		a.at(3);
		assert(false);
	} catch (const std::out_of_range& e) {
		assert(std::string(e.what()) == "deque [] access out of range");
	}

	try {
		a.at(-1);
		assert(false);
	} catch (const std::out_of_range& e) {
        assert(std::string(e.what()) == "deque [] access out of range");
	
	}
	
	Deque b;
	try {
		b.at(0);
		assert(false);
	} catch (const std::out_of_range& e) {
        assert(std::string(e.what()) == "deque [] access out of range");
	}
	
	Deque c(3);
	assert(c.at(0) == 0);
	assert(c.at(1) == 0);
	assert(c.at(2) == 0);
	}
	
	{
	// front(), back()
	Deque a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	
	assert(a.front() == 1);
	assert(a.back() == 3);
	
	a.pop_back();
	assert(a.back() == 2);
	a.pop_front();
	assert(a.front() == 2);
	}
	
	{
	// begin(), end(), rbegin(), rend()
	Deque a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	
	Deque b(a.size());
	copy(a.begin(), a.end(), b.begin());
	assert(b[0] == 1);
	assert(b[1] == 2);
	assert(b[2] == 3);
	assert(b.size() == 3);  
	assert(b.at(0) == 1); 
		
	Deque d(a.size());
	copy(++a.begin(), --a.end(), d.begin());
	assert(d[0] == 2);

	}
		
	{
	// Test iterators: ++x, x++, --x, x--, *x, ->x, x == y, x != y, and const versions
	Deque a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);
	
	typename Deque::iterator pos = a.begin();        //1
	typename Deque::iterator pos2 = (++a.begin());   //2
	pos2++; //3
	
	typename Deque::const_iterator const_pos =  a.begin();        //1
	typename Deque::const_iterator const_pos2 = (++a.begin());    //2
	const_pos2++;  //3
		
	assert(pos != pos2);
	++pos;
	--pos2;
	assert(pos == pos2);

	assert(const_pos != const_pos2);
	
	pos--;
	pos2++;
	assert(*pos == 1);
	assert(*pos2 == 3);
	
	assert(*const_pos == 1);
	assert(*const_pos2 == 3);	
	}
	
	{
	// copy assignment =
	const Deque x(10, 9);
	const Deque y(5, 4);
	Deque a(10, 9);
	Deque b(5, 4);
	
	a = a;  // Should make sure the Deques are not the same already first
	
	a = b;  // copy assign smaller into larger
	
	assert(a == b);
	
	assert(a.size() == 5);
	assert(a[4] == 4);
	
	b = x; // copy assign larger into smaller
	assert(b.size() == 10);
	assert(b[9] == 9);
	
	}
	
	{
	// swap
	Deque a(5, 4);
	Deque b(5, 3);
	Deque c(10, 9);
	
	a.swap(b);
	assert(a[4] == 3);
	assert(b[4] == 4);
	assert(a.size() == 5);
	assert(b.size() == 5);
	
	swap(a, b);
	assert(a[4] == 4);
	assert(b[4] == 3);
	assert(a.size() == 5);
	assert(b.size() == 5);
	
	b.swap(c);
	assert(b[9] == 9);
	assert(c[4] == 3);
	assert(b.size() == 10);
	assert(c.size() == 5);

	swap(b, c);
	assert(b[4] == 3);
	assert(c[9] == 9);
	assert(b.size() == 5);
	assert(c.size() == 10);
	}
	
	{
	// insert(pos, elem)
	Deque a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);
	assert(a.size() == 4);
	assert(a[0] == 1); 
	
	// Insertion as first element of deque
	a.insert(a.begin(), 0);
	assert(a[0] == 0);
	assert(a[1] == 1);
	assert(a.size() == 5);
	
	// Insertion as last element of deque
	a.insert(a.end(), 5);
	assert(a[5] == 5);
	assert(a[4] == 4);
	

	// Insertion in the middle of deque - invalidation of all pointers
	a.insert(a.begin() + 3, 6);
	assert(a[2] == 2);
	assert(a[3] == 6);
	assert(a[4] == 3);
	
	// insert(pos, n, elem)
	Deque b;
	b.push_back(1);
	b.push_back(2);
	b.push_back(3);
	b.push_back(4);
	
	// Insertion at beginning
	typename Deque::iterator pos = b.insert(b.begin(), 0);  // insert(pos, elem) returns position of new element as an iterator
	assert(*pos == 0);
	}
	
	{
	// push_back(elem), pop_back(), push_front(elem), pop_front()
	Deque a(5, 2);
	a.push_back(3);
	a.push_back(4);
	assert(a.size() == 7);
	assert(a[4] == 2);
	assert(a[5] == 3);
	assert(a[6] == 4);
	
	a.push_front(1);
	a.push_front(0);
	assert(a.size() == 9);
	assert(a[0] == 0);
	assert(a[1] == 1);
	assert(a[2] == 2);
	assert(a[8] == 4);
	
	a.pop_back();
	a.pop_front();
	assert(a.size() == 7);
	assert(a[6] == 3);
	assert(a[0] == 1);
	
	a.pop_back();
	a.pop_back();
	assert(a.size() == 5);
	assert(a[4] == 2);
	assert(a[3] == 2);
	assert(a[0] == 1);
	
	a.pop_front();
	a.pop_front();
	assert(a.size() == 3);
	assert(a[2] == 2);
	assert(a[1] == 2);
	assert(a[0] == 2);
	
	a.clear();
	a.push_front(1);
	a.push_front(2);
	a.push_front(3);
	assert(a.size() == 3);
	assert(a[0] == 3);
	assert(a[1] == 2);
	assert(a[2] == 1);
	
	a.clear();
	assert(a.size() == 0);
	}
	
	{
	// erase(pos),  clear()
	Deque a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);
	
	// erase first element
	typename Deque::iterator pos1 = a.erase(a.begin());
	assert(*pos1 == 2);
	assert(a[0] == 2);
	assert(a.size() == 3);
	
	// erase last element
	typename Deque::iterator pos2 = a.erase(a.end() - 1);
	assert(pos2 == a.end());
	assert(a[0] == 2);
	assert(a[1] == 3);
	assert(a.size() == 2);
	
	// erase middle element
	a.push_front(1);
	a.push_back(4);
	typename Deque::iterator pos3 = a.erase(a.begin() + 2);
	assert(a[0] == 1);
	assert(a[1] == 2);
	assert(*pos3 = 4);
	assert(a[2] == 4);
	assert(a.size() == 3);
	
	//reset Deque a
	a.insert(pos3, 3);
	a.push_back(5);
	assert(a.size() == 5);
	assert(a[0] == 1);
	assert(a[1] == 2);
	assert(a[2] == 3);
	assert(a[3] == 4);
	assert(a[4] == 5);
	
	// clear already extensively tested higher up
	}
	
	{
	// resize(num), resize(num, elem)
	Deque a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);
	
	a.resize(2);
	assert(a.size() == 2);
	assert(a[1] == 2);
	
	a.resize(4);
	assert(a.size() == 4);
	assert(a[0] == 1);
	assert(a[1] == 2);
	assert(a[2] == 0);
	assert(a[3] == 0);
	
	
	a[2] = 3;
	a[3] = 4;
	assert(a[2] == 3);
	assert(a[3] == 4);
	
	a.resize(2, 5);
	assert(a.size() == 2);
	assert(a[0] == 1);
	assert(a[1] == 2);
	
	a.resize(4, 5);
	assert(a.size() == 4);
	assert(a[2] == 5);
	assert(a[3] == 5); 
	}
	
} // deque_test

} // deque
} // prog
} // dt

#endif // DequeTest_h
