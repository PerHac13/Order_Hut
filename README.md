Sure, here's the updated README.md file with an additional point about concurrency:

# Order Hut

The Order Hut is a C++ application that demonstrates the classic Reader-Writer problem using semaphores. It simulates a simple order processing system where readers (customers) can query the status of orders, and a writer (order preparer) can update the order status.

## File Structure

The project has the following files:

1. **CMakeLists.txt**: The CMake configuration file that builds the project.
2. **order_counter.h**: The header file for the `OrderCounter` class, which manages the order counter and synchronization primitives.
3. **order_counter.cpp**: The implementation file for the `OrderCounter` class.
4. **reader.h**: The header file for the `Reader` class, which represents a customer querying the order status.
5. **reader.cpp**: The implementation file for the `Reader` class.
6. **writer.h**: The header file for the `Writer` class, which represents the order preparer.
7. **writer.cpp**: The implementation file for the `Writer` class.
8. **main.cpp**: The entry point of the application, which creates and manages the readers and writers.

## Functionality

The Order Hut has the following functionality:

1. **Order Counter**: The `OrderCounter` class maintains a counter for the latest order number and provides methods to increment the order number and retrieve the latest order number.
2. **Readers (Customers)**: The `Reader` class represents customers who can query the status of a specific order number. Readers acquire a read lock to read the latest order number without interfering with the writer.
3. **Writer (Order Preparer)**: The `Writer` class represents the order preparer who can update the latest order number. The writer acquires a write lock to ensure exclusive access to the order counter.
4. **Synchronization**: The system uses semaphores (`sem_t`) to implement the Reader-Writer problem. Readers acquire a read lock, and the writer acquires a write lock to ensure the necessary synchronization.
5. **Concurrency**: The Order Hut supports concurrent reader and writer operations. Multiple readers can query the order status simultaneously, while the writer can exclusively update the order counter. The synchronization primitives ensure that readers and writers do not interfere with each other's operations.

## Building and Running the Application

1. Install a C++ compiler and CMake (version 3.10 or higher).
2. Clone the repository and navigate to the project directory.
3. Create a build directory and navigate to it:

   ```
   mkdir build
   cd build
   ```

4. Run CMake to generate the build files:

   ```
   cmake ..
   ```

5. Build the project:

   ```
   cmake --build .
   ```

6. Run the application:

   ```
   ./order_hut
   ```

7. The application will prompt you to enter commands. You can enter the following commands:
   - `R <reader_number> <order_number>`: Simulates a reader (customer) querying the status of a specific order.
   - `W`: Simulates a writer (order preparer) preparing a new order.
   - `Q`: Quits the application.

The output will show the timestamps and the status of the order processing.

## Terminating the Main Program

To terminate the main program, the user can enter the `Q` command, which will exit the program's main loop and join any remaining threads before the application exits.

## Conclusion

The Order Hut demonstrates the implementation of the Reader-Writer problem using semaphores in C++. It provides a simple example of how to use synchronization primitives to ensure the correct behavior of concurrent processes accessing a shared resource.
