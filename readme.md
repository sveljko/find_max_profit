For the definition of the problem to solve, see [](challenge.md).

The function to find the maximum profit is defined in
[source](find_max_profit.cpp) with the [header](find_max_profit.hpp).

The unit tests are in [](find_max_profit.t.cpp).

There's no Makefile or some such thing, since there's nothing
special here, just compile the unit test and the module under
test, like:

    c++ find_max_profit.cpp find_max_profit.t.cpp

Remarks:

1. The unit test contains two recursive implementations
   of the "challenge". The non-cached one is easy to
   reason about, thus is a good reference implementation.
   But, it's _very_ slow. So, there is a slightly less
   reasonable one which uses a cache; that one is used as the 
   reference for "stress testing".
2. Even the cached recursive implementation is more than 10 times
   slower than the actual algorithm, so, it's not supposed to be
   used in production, not to mention potential stack overflow issues.
3. MS Visual C++ is notorious for badly optimizing its own standard
   library, so, if performance is a concern, one could ditch `std::vector<>`
   and use arrays, for some loss of convenience (I measured a > 10%
   speedup).
4. On my machine, this can handle tens of thousands of elements of
   `prices` in a reasonable time (< 1 sec) - in the optimized build. 
   This should be enough, as few dozen thousand days is about the lifetime 
   of a stock broker.  If it is not enough, it's possible to apply 
   multithreading here, but that is tricky, naive implementation could 
   loose most of the speedup because of lock contention and overhead.
5. Since several tens of thousands is about as much as we expect the 
   `prices` to have elements, there is no handling of running out of memory 
   for the `cache` - any modern computer should have ~1MB heap memory 
   available. But, if that is a concern, say, if this should run on an 
   embedded system, then some handling should be involved, like returning 
   some error if there are too many elements.
6. Since tests are straight-forward and don't have any special needs, I did 
   not use any unit testing framework, which simplifies the setup.
