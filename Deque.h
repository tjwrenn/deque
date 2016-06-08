// ------------------
// prog/deque/Deque.h
// Tj Wrenn
// ------------------

#ifndef Deque_h
#define Deque_h

// --------
// includes
// --------

#include <algorithm> // equal, lexicographical_compare
#include <iterator> // random_access_iterator_tag
#include <memory> // allocator
#include <stdexcept> // out_of_range
#include <cassert> //assert
#include <cmath> // ceil

using namespace std;

// ----------
// namespaces
// ----------

namespace dt{
namespace prog{
namespace deque{

// -----
// Deque
// -----

template < typename T, typename A = std::allocator<T> >
class Deque{
public:
// --------
// typedefs
// --------

typedef A allocator_type;
typedef typename allocator_type::value_type value_type;

typedef typename allocator_type::size_type size_type;
typedef typename allocator_type::difference_type difference_type;

typedef typename allocator_type::pointer pointer;
typedef typename allocator_type::const_pointer const_pointer;

typedef typename allocator_type::reference reference;
typedef typename allocator_type::const_reference const_reference;

// -------
// friends
// -------

/**
* O(n)
* M(1)
* @param  lhs a deque
* @param  rhs a deque
* @return true if lhs equals rhs
*/
friend bool operator == (const Deque& lhs, const Deque& rhs){
	return
		(lhs.size() == rhs.size()) &&
		std::equal(lhs.begin(), lhs.end(), rhs.begin());}

/**
* O(n)
* M(1)
* @param  lhs a deque
* @param  rhs a deque
* @return true if lhs does not equals rhs
*/
friend bool operator != (const Deque& lhs, const Deque& rhs){
	return !(lhs == rhs);}

/**
* O(n)
* M(1)
* @param  lhs a deque
* @param  rhs a deque
* @return true if lhs is less than rhs
*/
friend bool operator < (const Deque& lhs, const Deque& rhs){
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());}

/**
* O(n)
* M(1)
* @param  lhs a deque
* @param  rhs a deque
* @return true if lhs is less than or equal to rhs
*/
friend bool operator <= (const Deque& lhs, const Deque& rhs){
	return !(rhs < lhs);}

/**
* O(n)
* M(1)
* @param  lhs a deque
* @param  rhs a deque
* @return true if lhs is greater than rhs
*/
friend bool operator > (const Deque& lhs, const Deque& rhs){
	return (rhs < lhs);}

/**
* O(n)
* M(1)
* @param  lhs a deque
* @param  rhs a deque
* @return true if lhs is greater than or equal to rhs
*/
friend bool operator >= (const Deque& lhs, const Deque& rhs){
	return !(lhs < rhs);}

private:
// -------------
// static consts
// -------------

/**
* Length of the outer array
*/
static const size_type block_size = 10;

private:
// ----
// data
// ----


allocator_type a;


/**
* This is a count of how many heap allocated instances of T are in deque.
* this value grows and shrinks and should be zero upon termination.
*/
#ifndef NDEBUG
int __instances;
#endif

/**
* defines upper bound of number of Ts accessible to "at" / "[]"
*/
size_type s;

/**
* defines the capacity of the Deque instance
*/
size_type c;

/**
* array storing references to the outer array objects.
*/
pointer* outer;

/**
* number of elements of the outerArray
*/
size_type outerSize;

/**
* index of first element in Deque for internal purposes
*/
size_type f;

/**
* index of last element in Deque for internal purposes
*/
size_type l;

// -----
// valid
// -----

/**
* O(1)
* M(1)
* @return true if deque is in valid state
*/
bool valid ()const {
#ifndef NDEBUG
	if( __instances < 0) return false;
#endif

	return (outer != NULL && outerSize > 0 && block_size > 0 && l >= 0 && f >= 0 && c >= s && c >= 0);}

public:
// --------
// iterator
// --------

class iterator{
	friend class Deque;
	
public:
	// --------
	// typedefs
	// --------

	typedef std::random_access_iterator_tag iterator_category;
	typedef typename Deque::value_type value_type;
	typedef typename Deque::difference_type difference_type;
	typedef typename Deque::pointer pointer;
	typedef typename Deque::reference reference;

private:
	// ----
	// data
	// ----

	Deque *thedeque;
	difference_type cur;

public:
	Deque* getDequePtr() const { return thedeque; };
	difference_type getCur() const { return cur; };

private:

	// -----
	// valid
	// -----

	/**
	* O(1)
	* M(1)
	* @return true
	*/
	bool valid ()const {
		//who knows what the user is going to do to invalidate this iterator...
		return true;}

public:
	// -----------
	// constructor
	// -----------

	/**
	* O(1)
	* M(1)
	*/
	iterator (){
		cur = 0;
		thedeque = NULL;
		assert(valid());}

	// destructor
	// ~const_iterator ();

	iterator (const iterator& that) {
		cur = that.cur;
		thedeque = that.thedeque;
	}

	iterator (const typename Deque::const_iterator& that) {
		cur = that.getCur();
		thedeque = that.getDequePtr();
	}

	// ----------
	// operator *
	// ----------

	/**
	* O(1)
	* M(1)
	* @return value at current iterator position
	*/
	reference operator * ()const {
		return (*thedeque)[cur];}

	// -----------
	// operator []
	// -----------

	/**
	* O(1)
	* M(1)
	* @param i index
	* @return reference to value at index i
	*/
	template <typename index_type>
		reference operator [] (const index_type &i )const {
			return (*thedeque)[i];}

		// -----------
		// operator ->
		// -----------

		/**
		* O(1)
		* M(1)
		* @return reference to value at current iterator position
		*/
		pointer operator -> ()const {
			return &**this;}

		// -----------
		// operator ++
		// -----------

		/**
		* O(1)
		* M(1)
		* @return prefix incremented iterator *this
		*/
		iterator& operator ++ (){
			++cur;
			assert(valid());
			return *this;}
	
		/**
		* O(1)
		* M(1)
		* @return copy of iterator *this before it is incremented.  (postfix)
		*/
		iterator operator ++ (int){
			iterator x = *this;
			++(*this);
			assert(valid());
			return x;}

		// -----------
		// operator +=
		// -----------

		/**
		* O(1)
		* M(1)
		* @param v iterator offset
		* @return current iterator as current iterator + v
		*/
		template <typename value>
			iterator& operator += (const value &v){
				cur += v;
				assert(valid());
				return *this;}

			// -----------
			// operator --
			// -----------
			
			/**
			* O(1)
			* M(1)
			* @return prefix decremented iterator *this
			*/
			iterator& operator -- (){
				--cur;
				assert(valid());
				return *this;}

			/**
			* O(1)
			* M(1)
			* @return copy of iterator *this before it is decremented.  (postfix)
			*/
			iterator operator -- (int){
				iterator x = *this;
				--(*this);
				assert(valid());
				return x;}

			// -----------
			// operator -=
			// -----------

			/**
			* O(1)
			* M(1)
			* @param v iterator offset
			* @return current iterator as current iterator - v
			*/			
			template <typename value>
				iterator& operator -= (const value &v){
					cur -= v;
					assert(valid());
					return *this;}

				// -----------
				// operator +
				// -----------

				/**
				* O(1)
				* M(1)
				* @param that an iterator
				* @return an iterator index as current iterator's index + that's index
				*/
				difference_type operator + (const iterator& that){
					assert(valid());
					return cur + that.cur;}
					
				/**
				* O(1)
				* M(1)
				* @param that iterator offset
				* @return an new iterator as current iterator + that
				*/
				template <typename value>
					iterator operator + (const value& that){
					iterator r;
					r.cur = cur+that;
					r.thedeque = thedeque;
					return r;}

					// -----------
					// operator -
					// -----------
					/**
					* O(1)
					* M(1)
					* @param that an iterator
					* @return an iterator index as current iterator's index - that's index
					*/
					difference_type operator - (const iterator& that){
						assert(valid());
						return cur - that.cur;}
					
					/**
					* O(1)
					* M(1)
					* @param that iterator offset
					* @return an new iterator as current iterator - that
					*/
					template <typename value>
						iterator operator - (const value& that){
						iterator r;
						r.cur = cur-that;
						r.thedeque = thedeque;
						return r;}

					// -----------
					// operator =
					// -----------
					
					/**
					* O(1)
					* M(1)
					* @param that an iterator
					* @return current iterator equal to that iterator
					*/
					iterator& operator = (const iterator& that){
						cur = that.cur;
						thedeque = that.thedeque;
						assert(valid());
						return *this;}

					/**
					* O(1)
					* M(1)
					* @param that a const_iterator
					* @return constant current iterator equal to that const_iterator
					*/					
					iterator& operator = (const typename Deque::const_iterator& that){
						cur = that.getCur();
						thedeque = that.getDequePtr();
						assert(valid());
						return *this;}


						// -----------
						// operator ==
						// -----------
						/**
						* O(1)
						* M(1)
						* @param that an iterator
						* @return true if that iterator is equal to current iterator
						*/
						bool operator == (const iterator& that)const {
							return (that.cur == this->cur) && (that.thedeque == this->thedeque);}

						/**
						* O(1)
						* M(1)
						* @param that index
						* @return true if index is equal to current index
						*/
						template <typename value>
							bool operator == (const value& that)const {
								return (that == this->cur);}

							// -----------
							// operator !=
							// -----------

							/**
							* O(1)
							* M(1)
							* @param that index if one wishes to compare the iterator to an ordinal value
							* @return true if the position of the iterator is not at the value'th position
							*/
							template <typename value>
								bool operator != (const value& that)const {
									return !(*this == that);}

								/**
								* O(1)
								* M(1)
								* @param that index
								* @return true if current index is less than that
								*/								
								template <typename value>
									bool operator < (const value& that)const {
										return (this->cur < that);}

									/**
									* O(1)
									* M(1)
									* @param that iterator
									* @return true if current index is less than that's index
									*/
									bool operator < (const iterator& that)const {
										return this->cur < that.cur;
									}

									/**
									* O(1)
									* M(1)
									* @param that index
									* @return true if current index is greater than that
									*/
									template <typename value>
										bool operator > (const value& that)const {
											return (this->cur > that);}
										
										/**
										* O(1)
										* M(1)
										* @param that iterator
										* @return true if current index is greater than that's index
										*/										
										bool operator > (const iterator& that)const {
											return that < *this;
										}

										/**
										* O(1)
										* M(1)
										* @param that index
										* @return true if current index is greater than or equal to that
										*/
										template <typename value>
											bool operator >= (const value& that)const {
												return (this->cur >= that);}
											
											/**
											* O(1)
											* M(1)
											* @param that iterator
											* @return true if current index is greater than or equal to that's index
											*/										
											bool operator >= (const iterator& that)const {
												return !(*this < that);
											}

											/**
											* O(1)
											* M(1)
											* @param that index
											* @return true if current index is less than or equal to that
											*/
											template <typename value>
												bool operator <= (const value& that)const {
													return (this->cur <= that);}
												
												/**
												* O(1)
												* M(1)
												* @param that iterator
												* @return true if current index is less than or equal to that's index
												*/										
												bool operator <= (const iterator& that)const {
													return !(*this > that);
												}};

public:
// --------------
// const_iterator
// --------------

class const_iterator{
	friend class Deque;

public:
	// --------
	// typedefs
	// --------

	typedef std::random_access_iterator_tag iterator_category;
	typedef typename Deque::value_type value_type;
	typedef typename Deque::difference_type difference_type;
	typedef typename Deque::const_pointer pointer;
	typedef typename Deque::const_reference reference;

private:
	// ----
	// data
	// ----

	const Deque* thedeque;
	difference_type cur;

public:
	Deque* getDequePtr() const { return thedeque; };
	difference_type getCur() const { return cur; };

private:
	// -----
	// valid
	// -----
	
	/**
	* O(1)
	* M(1)
	* @return true
	*/
	bool valid ()const {
		//who knows what the user is going to do to invalidate this iterator...
		return true;}

public:
	// -----------
	// constructor
	// -----------

	/**
	* O(1)
	* M(1)
	*/
	const_iterator (){
		cur = 0;
		thedeque = NULL;
		assert(valid());}

	// destructor
	// ~const_iterator ();

	const_iterator (const const_iterator& that) {
		cur = that.cur;
		thedeque = that.thedeque;
	}

	const_iterator (const iterator& that) {
		cur = that.getCur();
		thedeque = that.getDequePtr();
	}

	// ----------
	// operator *
	// ----------

	/**
	* O(1)
	* M(1)
	* @return value at current iterator position
	*/
	const_reference operator * ()const {
		return (*thedeque)[cur];}

	// -----------
	// operator []
	// -----------

	/**
	* O(1)
	* M(1)
	* @param i index
	* @return reference to value at index i	
	*/
	template <typename index_type>
		const_reference operator [] (const index_type &i )const {
			return (*thedeque)[i];}

		// -----------
		// operator ->
		// -----------

		/**
		* O(1)
		* M(1)
		* @return reference to value at current iterator position
		*/
		const_pointer operator -> ()const {
			return &**this;}

		// -----------
		// operator ++
		// -----------

		/**
		* O(1)
		* M(1)
		* @return prefix incremented iterator *this
		*/
		const_iterator& operator ++ (){
			++cur;
			assert(valid());
			return *this;}

		/**
		* O(1)
		* M(1)
		* @return copy of iterator *this before it is incremented.  (postfi                *
		*/
		const_iterator operator ++ (int){
			const_iterator x = *this;
			++(*this);
			assert(valid());
			return x;}

		// -----------
		// operator +=
		// -----------

		/**
		* O(1)
		* M(1)
		* @param v iterator offset
		* @return constant current iterator as current iterator + v
		*/
		template <typename value>
			const_iterator& operator += (const value &v){
				cur += v;
				assert(valid());
				return *this;}

			// -----------
			// operator --
			// -----------

			/**
			* O(1)
			* M(1)
			* @return decremented constant iterator *this
			*/
			const_iterator& operator -- (){
				--cur;
				assert(valid());
				return *this;}

			/**
			* O(1)
			* M(1)
			* @return copy of iterator *this before it is decremented.
			*/
			const_iterator operator -- (int){
				const_iterator x = *this;
				--(*this);
				assert(valid());
				return x;}

			// -----------
			// operator -=
			// -----------

			/**
			* O(1)
			* M(1)
			* @param v iterator offset
			* @return constant current iterator as current iterator - v
			*/			
			template <typename value>
				const_iterator& operator -= (const value &v){
					cur -= v;
					assert(valid());
					return *this;}

				// -----------
				// operator +
				// -----------

				/**
				* O(1)
				* M(1)
				* @param that a constant iterator
				* @return an iterator index as current iterator's index + that's index
				*/
				difference_type operator + (const const_iterator& that){
					assert(valid());
					return cur + that.cur;}
					
				/**
				* O(1)
				* M(1)
				* @param that iterator offset
				* @return an new constant iterator as current iterator + that
				*/					
				template <typename value>
					const_iterator operator + (const value& that){
					const_iterator r;
					r.cur = cur+that;
					r.thedeque = thedeque;
					return r;}

					// -----------
					// operator -
					// -----------

					/**
					* O(1)
					* M(1)
					* @param that a constant iterator
					* @return an iterator index as current iterator's index - that's index
					*/
					difference_type operator - (const const_iterator& that){
						assert(valid());
						return cur - that.cur;}
					
					/**
					* O(1)
					* M(1)
					* @param that iterator offset
					* @return a new constant iterator as current iterator - that
					*/						
					template <typename value>
						const_iterator operator - (const value& that){
						const_iterator r;
						r.cur = cur-that;
						r.thedeque = thedeque;
						return r;}

					// -----------
					// operator =
					// -----------
					
					/**
					* O(1)
					* M(1)
					* @param that a constant iterator
					* @return constant current iterator equal to that constant iterator
					*/					
					const_iterator& operator = (const const_iterator& that){
						cur = that.cur;
						thedeque = that.thedeque;
						assert(valid());
						return *this;}

					/**
					* O(1)
					* M(1)
					* @param that a iterator
					* @return constant current iterator equal to that iterator
					*/					
					const_iterator& operator = (const iterator& that){
						cur = that.getCur();
						thedeque = that.getDequePtr();
						assert(valid());
						return *this;}


						// -----------
						// operator ==
						// -----------
						/**
						* O(1)
						* M(1)
						* @param that a constant iterator
						* @return true if that constant iterator is equal to current iterator
						*/
						bool operator == (const const_iterator& that)const {
							return (that.cur == this->cur) && (that.thedeque == this->thedeque);}

						/**
						* O(1)
						* M(1)
						* @param that index
						* @return true if index is equal to current index
						*/						
						template <typename value>
							bool operator == (const value& that)const {
								return (that == this->cur);}

							// -----------
							// operator !=
							// -----------

							/**
							* O(1)
							* M(1)
							* @param that index if one wishes to compare the iterator to an ordinal value
							* @return true if the position of the iterator is not at the value'th position
							*/
							template <typename value>
								bool operator != (const value& that)const {
									return !(*this == that);}

								/**
								* O(1)
								* M(1)
								* @param that index
								* @return true if current index is less than that
								*/
								template <typename value>
									bool operator < (const value& that)const {
										return (this->cur < that);}

									/**
									* O(1)
									* M(1)
									* @param that constant iterator
									* @return true if current index is less than that's index
									*/
									bool operator < (const const_iterator& that)const {
										return this->cur < that.cur;
									}

									/**
									* O(1)
									* M(1)
									* @param that index
									* @return true if current index is greater than that
									*/
									template <typename value>
										bool operator > (const value& that)const {
											return (this->cur > that);}

										/**
										* O(1)
										* M(1)
										* @param that constant iterator
										* @return true if current index is greater than that's index
										*/										
										bool operator > (const const_iterator& that)const {
											return that < *this;
										}

										/**
										* O(1)
										* M(1)
										* @param that index
										* @return true if current index is greater than or equal to that
										*/
										template <typename value>
											bool operator >= (const value& that)const {
												return (this->cur >= that);}

											/**
											* O(1)
											* M(1)
											* @param that constant iterator
											* @return true if current index is greater than or equal to that's index
											*/										
											bool operator >= (const const_iterator& that)const {
												return !(*this < that);
											}

											/**
											* O(1)
											* M(1)
											* @param that index
											* @return true if current index is less than or equal to that
											*/
											template <typename value>
												bool operator <= (const value& that)const {
													return (this->cur <= that);}

												/**
												* O(1)
												* M(1)
												* @param that constant iterator
												* @return true if current index is less than or equal to that's index
												*/										
												bool operator <= (const const_iterator& that)const {
													return !(*this > that);
												}};

private:
// --------------
// ensureCapacity
// --------------
/**
* ensures the requested capacity in the Deque
* O(n), where n is the capacity / block_size.
* M(n), where n is the requested capacity
* @param capacity the requested capacity to which to adjust the Deque
*/
void ensureCapacity(size_type capacity){
	if(c >= capacity)
		return; //nothing to do
	size_type n = (c*2)+1;
	if(capacity > n) n = capacity; //make sure new capacity is enough

	n = (size_type)ceil((double)n / block_size);

	if(n % 2) ++n; //make sure we add as many new arrays to bottom as we do to top
	if(n == 2) ++n; //add at least one on bottom and one on top

	c = n * block_size; //increase capacity

	typename A::template rebind<pointer>::other x;
	pointer* newOuter = x.allocate(n);

	size_type& oldOuterSize = outerSize;
	size_type& newOuterSize = n;
	size_type h = (newOuterSize - oldOuterSize) / 2;

	f += (h * block_size);
	l += (h * block_size);

	//top
	for(difference_type i = 0; i < h; ++i){
		newOuter[i] = this->a.allocate(block_size);
	}

	//middle
	for(difference_type i = 0; i < oldOuterSize; ++i){
		newOuter[i+h] = outer[i];
	}

	//bottom
	for(difference_type i = oldOuterSize + h; i < n; ++i){
		newOuter[i] = this->a.allocate(block_size);
	}

	x.deallocate(outer, oldOuterSize);
	outer = newOuter;
	outerSize = newOuterSize;

	assert(valid());
}

// ------
// middle
// ------
/**
* O(1)
* M(1)
* @return iterator positioned in the middle of the deque
*/
iterator middle (){
	typename Deque::iterator i;
	i.thedeque = this;
	i.cur = size() / 2;
	return i;}

/**
* allocation and initialization of deque
* O(1)
* M(block size)
*/
void init(){
	typename A::template rebind<pointer>::other x;
	outer = x.allocate(1);
	outer[0] = this->a.allocate(block_size);
	f = (block_size) / 2;
	l = f - 1;
	c = block_size;
	s = 0;
	outerSize = 1;

#ifndef NDEBUG
	__instances = 0;
#endif
}

/**
* returns the capacity of the top portion of the Deque.  useful in determining reallocation requirement
* O(1)
* M(1)
* @return the capacity of the top portion of the deque
*/
size_type topCapacity()const {
	return f;
}

/**
* returns the capacity of the bottom portion of the Deque.  useful in determining reallocation requirement
* O(1)
* M(1)
* @return the capacity of the bottom portion of the deque
*/
size_type bottomCapacity()const {
	return c - size() - topCapacity();
}

public:
// -----
// Deque
// -----

/**
* initialize the local data
* O(1)
* M(n), where n is the block_size
* @param a allocator
*/
Deque (const allocator_type &a = allocator_type())
	: a(a) {
		init();
		assert(valid());}

    /**
 	 * initialize the local data
 	 * O(n), where n is the requested size
     * M(n), where n is the requested size
     * @param s initial size of the deque
 	 * @param v value to fill deque with
	 * @param a allocator
	 */
	Deque (size_type s, const_reference v = value_type(), const allocator_type &a = allocator_type())
		: a(a) {
			init();
			resize(s, v);
			assert(valid());}

		/**
		* copies deque that to a new deque
		* O(n), where n is the size of that
		* M(n), where n is the size of that
		* @param that a deque
		*/
		Deque (const Deque &that): a(that.a) {
			init();
			for(size_type i=0;i<that.size();++i)
				this->push_back(that[i]);

			assert(valid());}

		// ------
		// ~Deque
		// ------
		/**
		* O(outerSize)
		* M(1)
		*/
		~Deque (){
			resize(0);
			for(difference_type i = 0; i<outerSize;++i)
				this->a.deallocate(outer[i], block_size);

			typename A::template rebind<pointer>::other x;
			x.deallocate(outer, outerSize);

			assert(__instances == 0);
			assert(valid());}

		// ----------
		// operator =
		// ----------
		
		/**
		* O(n), where n is the size of that
		* M(n), where n is the size of that
		* @param that a deque
		* @return current deque equal to that deque
		*/
		Deque& operator = (const Deque& that){
			if(this == &that) 
				return *this;
				
			this->~Deque();
			
			init();
			for(size_type i=0;i<that.size();++i)
				this->push_back(that[i]);

			assert(valid());
			return *this;}

		// -----------
		// operator []
		// -----------
		
		/**
		* O(1)
		* M(1)
		* @param index element index in deque
		* @return reference to value at the index'th position
		*/
		reference operator [] (size_type index){
			size_type n = f + index;
			return (outer[n / block_size])[n % block_size];}

		/**
		* O(1)
		* M(1)
		* @param index element index in deque
		* @return constant reference to value at the index'th position
		*/
		const_reference operator [] (size_type index)const {
			return const_cast<Deque*>(this)->operator[](index);}

		// --
		// at
		// --

		/**
		* O(1)
		* M(1)
		* @param index element index
		* @throw std::out_of_range
		* @return reference to value at the index'th position
		*/
		reference at (size_type index)throw (std::out_of_range) {
			if (index >= size())
				throw std::out_of_range("deque [] access out of range");
			return (*this)[index];
		}

		/**
		* O(1)
		* M(1)
		* @param index element index
		* @throws std::out_of_range
		* @return constant reference to value at the index'th position
		*/
		const_reference at (size_type index)const throw (std::out_of_range) {
			return const_cast<Deque*>(this)->at(index);}

		// ----
		// back
		// ----

		/**
		* O(1)
		* M(1)
		* @return reference to value of last element
		*/
		reference back (){
			return (*this)[size()-1];}

		/**
		* O(1)
		* M(1)
		* @return constant reference to value of last element
		*/
		const_reference back ()const {
			return const_cast<Deque*>(this)->back();}

		// -----
		// begin
		// -----
		/**
		* O(1)
		* M(1)
		* @return an iterator for the beginning of the deque
		*/
		iterator begin (){
			typename Deque::iterator i;
			i.thedeque = this;
			i.cur = 0;
			return i;}

		/**
		* O(1)
		* M(1)
		* @return a constant iterator for the beginning of the deque
		*/
		const_iterator begin ()const {
			typename Deque::const_iterator i;
			i.thedeque = this;
			i.cur = 0;
			return i;
		}


		// -----
		// clear
		// -----
		/**
		* removes all elements and makes the container empty
		* O(n)
		* M(1)
		*/
		void clear (){
			resize(0);
			assert(valid());}

		// -----
		// empty
		// -----
		/**
		* O(1)
		* M(1)
		* @return true if size is equal to 0
		*/
		bool empty ()const {
			return !size();}

		// ---
		// end
		// ---
		/**
		* O(1)
		* M(1)
		* @return an iterator for the end (one past the last element) of the deque
		*/
		iterator end (){
			typename Deque::iterator i;
			i.thedeque = this;
			i.cur = size();
			return i;}

		/**
		* O(1)
		* M(1)
		* @return a constant iterator for the end (one past the last element) of the deque
		*/
		const_iterator end ()const {
			typename Deque::const_iterator i;
			i.thedeque = this;
			i.cur = size();
			return i;
		}
		// -----
		// erase
		// -----
		/**
		* O(1)/O(n) constant if removing from front/back, linear if removing from middle.
		* M(1)
		* @param i iterator position
		* @return iterator position of the next element
		*/
		iterator erase (iterator i){
			if(i == end()-1){ //remove from the end
				--l; // decrement last position marker by 1 if removing from the back
				--s; // decrement size
			}else if (i == begin()){
				++f; // increment front position marker by 1 if removing from the front
				--s; // decrement size
			}else{ // removing from the middle
#ifndef NDEBUG
				++__instances;
#endif
				if(i < middle()){ // easier to reposition from middle towards front
					for(difference_type j=i.cur; j>0; --j){
						std::swap((*this)[j], (*this)[j-1]);
					}
					pop_front();
				}else{ // easier to reposition from the middle towards back
					for(difference_type j=i.cur; j<size(); ++j){
						std::swap((*this)[j], (*this)[j+1]);
					}
					pop_back();
				}
			}
			a.destroy(&(*i));
#ifndef NDEBUG
			--__instances;
#endif

			return i;}

		// -----
		// front
		// -----
		/**
		* O(1)
		* M(1)
		* @return reference to value of first element
		*/
		reference front (){
			return (*this)[0];}

		/**
		* O(1)
		* M(1)
		* @return constant reference to value of first element
		*/
		const_reference front ()const {
			return const_cast<Deque*>(this)->front();}

		// ------
		// insert
		// ------
		/**
		* ~O(1)/O(n)  amortized constant when inserting to the front/back, linear if inserting to middle.
		* M(1)
		* @param i iterator position
		* @param v value to insert
		* @return iterator position of the next element
		*/
		iterator insert (iterator i, const_reference v){
			assert(valid());
			if(i == end()){ //insert at the end
				if(bottomCapacity() == 0) ensureCapacity(c + 1);
				++l; // increment last position marker by 1 if adding to the back
				++s; // increment size
			}else if (i == begin()){
				if(topCapacity() == 0) ensureCapacity(c + 1);
				--f; // decrement front position marker by 1 if adding to the front
				++s; // increment size
			}else{ // inserting into the middle
#ifndef NDEBUG
				--__instances;
#endif
				if(i < middle()){ // easier to reposition from middle towards front
					if(topCapacity() == 0) ensureCapacity(c + 1);
					push_front(*begin());
					for(iterator j=begin(); j<=i; ++j){
						std::swap(*j, *(j+1));
					}
				}else{ // easier to reposition from the middle towards back
					if(bottomCapacity() == 0) ensureCapacity(c + 1);
					push_back(*(--end()));
					for(iterator j=--end(); j>i; --j){
						std::swap(*j, *(j-1));
					}
				}
			}

			a.construct(&(*i), v);
#ifndef NDEBUG
			++__instances;
#endif

			return i;}

		// ---
		// pop
		// ---
		/**
		* O(1)
		* M(1)
		*/
		void pop_back (){
			erase(end()-1);
			assert(valid());}

		/**
		* O(1)
		* M(1)
		*/
		void pop_front (){
			erase(begin());
			assert(valid());}

		// ----
		// push
		// ----
		/*
		* ~O(1), unless capacity must be increased
		* M(1)
		* @param v value to insert at back
		*/
		void push_back (const_reference v){
			insert(end(), v);
			assert(valid());}

		/*
		* ~O(1), unless capacity must be increased
		* M(1)
		* @param v value to insert at front
		*/
		void push_front (const_reference v){
			insert(begin(), v);
			assert(valid());}

		// ------
		// resize
		// ------
		/**
		* O(n), where n = abs(new size - current size) 
		* M(n), where n = new size - current size
		* @param s new size of deque
		* @param v value to fill deque with
		*/
		void resize (size_type s, const_reference v = value_type()){
			if(size() == s)
				return;
			if(s < size()){
				for(size_type i = s; i < size(); ++i){
					a.destroy(&(*this)[i]);
#ifndef NDEBUG
					--__instances;
#endif
				}
				l = f + s - 1;
				this->s = s;
			}else{
				for(size_type i = size(); i < s; ++i){
					if(bottomCapacity() == 0) ensureCapacity(c + 1);
					++l;
					a.construct(&(*this)[i], v);
#ifndef NDEBUG
					++__instances;
#endif
					++this->s;
				}
			}
			assert(valid());}

		// ----
		// size
		// ----
		/**
		* O(1)
		* M(1)
		* @return size of deque
		*/
		size_type size ()const {
			return s;}

		// ----
		// swap
		// ----
		/**
		* swaps the data of this deque and that deque
		* O(1)
		* M(1)
		* @param that a deque
		*/
		void swap (Deque& that){
			std::swap(this->s, that.s);
			std::swap(this->l, that.l);
			std::swap(this->f, that.f);
			std::swap(this->c, that.c);
#ifndef NDEBUG
			std::swap(this->__instances, that.__instances);
#endif
						
			std::swap(this->outerSize, that.outerSize);
			std::swap(this->outer, that.outer);
			std::swap(this->a, that.a);

			assert(valid());}};

			// ----
			// swap
			// ----

			template <typename T, typename A>
				/**
				* swaps the data of deque x and deque y
				* O(1)
				* M(1)
				* @param x a deque
				* @param y another deque
				*/		
				void swap (Deque<T, A>& x, Deque<T, A>& y){
					x.swap(y);}

} // deque
} // prog
} // dt

#endif // Deque_h
