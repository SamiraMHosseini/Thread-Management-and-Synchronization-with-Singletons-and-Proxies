# Thread Management and Synchronization with Singletons and ThreadCounterRAII


This code demonstrates thread management and synchronization using singleton patterns, ThreadCounterRAII class, and condition variables in C++. The key components are:

CtrlSingleton class: A singleton class to manage a mutex and a condition variable for detecting a key press event in the controller thread.

ThrdCounter class: A singleton class that manages a thread counter, mutexes, and a condition variable to track the number of active threads and provide synchronization among them.

ThreadCounterRAII class: A simple RAII class that increments and decrements the thread counter managed by the ThrdCounter singleton when its instances are created and destroyed, respectively.

A, B, C, and D classes: Derived from BannerBase, these classes each represent a separate thread with their own specific tasks. They create a ThreadCounterRAII object in their operator() method, which will manage the thread counter.

Controller class: Derived from BannerBase, this class represents the controller thread. It waits for a key press event and then signals all other threads to terminate. It also waits for all threads to complete their execution.

main function: The main function creates instances of the A, B, C, D, and Controller classes, and spawns separate threads for each of them. It also handles the key press event and waits for the controller thread to finish.

# The key points of the code are:

The use of singleton patterns (CtrlSingleton and ThrdCounter) to manage shared resources across multiple threads.
The ThreadCounterRAII class to handle the RAII mechanism for incrementing and decrementing the thread counter.
The use of condition variables to synchronize and communicate between threads.
Properly detaching threads and using std::future and std::promise to ensure the main thread waits for the controller thread to finish.
The code demonstrates a multi-threaded C++ program that performs various tasks concurrently and terminates them gracefully upon receiving a key press event.

The ThreadCounterRAII class in the provided code serves as a simple RAII (Resource Acquisition Is Initialization) mechanism 
to increment and decrement the thread counter in the ThrdCounter singleton. 
It does this by calling ThrdCounter::TCUP() in its constructor and ThrdCounter::TCDown() in its destructor.

When an instance of the ThreadCounterRAII class is created in the scope of a thread function 
(e.g., within the operator() method of classes A, B, C, and D), 
it increments the thread counter by calling ThrdCounter::TCUP(). 
When the scope ends and the ThreadCounterRAII instance is destroyed, its destructor automatically calls ThrdCounter::TCDown() to decrement the thread counter.
This ensures that the thread counter is correctly updated as threads start and finish their tasks.

The main purpose of the ThreadCounterRAII class in this code is to help manage the lifetime of threads and ensure that
the ThrdCounter singleton keeps an accurate count of active threads.
