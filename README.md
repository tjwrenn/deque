# deque
Deque datatype implemented in C++ using the stl

Description
   This project implements a functional deque container similar to the STL deque. The description below explains some highlights of our implementation. 

1) The private ensureCapacity() function grows the capacity by the same number of elements on each end. 

2) The __instances variables monitors deque allocation and deallocation. By termination, __instances should be zero. Otherwise, a memory leak has occured. 

3) Similar to begin() and end(), middle() is defined to speed up the scoot when inserting into the middle. For instance, if you insert into a position in the deque in the first half, it's much easier to scoot the elements down from index 0 up to index followed by inserting the new element at the index'th position than from index up to end(). By symmetry, inserting into a position in the second half has similar complexity. Even though this is still O(N), the max number of adjustments is O(N/2) as explained by here. A slightly modified version of the same argument works for erase as well.
