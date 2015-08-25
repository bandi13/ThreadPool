ThreadPool
==========

A simple C++11 Thread Pool implementation which returns a Future. I have expanded
the original version to have another version using the io_service in the ASIO Boost
library. In my limited testing (refer to example.cpp), I have found that with Boost 1.56,
the Thread Pool using the std library outperforms the Boost version.
