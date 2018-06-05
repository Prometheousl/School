# School

Contains all of Alex Lay's schoolwork at the University of Alabama

## Course List

1. Data Structures and Algorithms - CS201
2. Database Management Systems - CS557
3. Operating Systems - CS526
4. Computer Algorithms - CS570
5. Intro to Autonomous Robotics - CS460

### Data Structures and Algorithms

Desc: Written in C, this course contains data structures that utilize void pointers to ensure that they can be used with any data type. Type-casting files are also included to make sure that those data types can be passed as void pointers. In our class, we were told not to worry about freeing memory, so these projects do have memory leaks.

#### List of Projects

1. assign0: Create a dynamic array, circular dynamic array, stack, and queue
2. assign1: Reads in a series of variable declarations as well as a mathematical expression from a file. It prints on the stdout the result of the expression (uses a postfix_queue to evaluate expression)
3. assign2: Reads in a data file and a commands file. It then converts the data file or "corpus" into a green tree or red black tree depending on if the -g or -r option is specified. It then reads a list of commands to perform on the tree into a queue and then executes the commands on either the green or red black tree. *Tested with the entire works of William Shakespeare*
4. assign3: Runs kruskal's algorithm on a set. It appears I have lost this file for the time being... will get it back at some point

### Database Management Systems

1. normalization: Determines  if a table is in 1NF, 2NF or 3NF for a given set of functional dependencies FDs
2. mysql: Implements a website where it performs different mysql commands based on the buttons/text input given. Server is not running. And, it is not protected against injection.
3. final-project: Implements the operations required to process and execute NoSQL queries on several files, which correspond to either a column family or a table in a Column Store database.  Parses a query to identify which operations it is requesting, performs the operations on the specified column family and/or table and displays the results. Implements some of the software that would be used by a NoSQL Column Store DBMS.

### Operating Systems

1. * collatz-conjecture: Takes a positive integer and implements the [Collatz Conjecture](https://en.wikipedia.org/wiki/Collatz_conjecture) for that integer N.
   * shell-history: Runs a simplified version of the shell. Able to fork commands and display a history of commands.
2. * 3.20 - implements the API for a PID manager. Uses a bitmap as an underlying data structure
   * multithreaded_statistics - A multithreaded program that calculates various statistical values for a list of numbers. This program        is passed a series of numbers on the command line and then creates three separate worker threads. One thread determines the average      of the numbers, the second determines the maximum value, and the third determines the minimum value.
   * pid_manager - implements 3.20 (the PID manager API) and tests it
   * submission - an amalgamation of the previous 3 projects
3. * pid_manager_with_mutex - implements a PID manager using mutex locks
   * producer_consumer - creates a bunch of consumer and producer threads. The main then sleeps and upon awakening, terminates the            program. A buffer is used for creating and consuming.
   * submission - an amalgamation of the previous 2 projects
4. dispatcher - simulates a process dispatcher
5. lru - simulates the [Least Recently Used](https://en.wikipedia.org/wiki/Page_replacement_algorithm#Least_recently_used) page            replacement algorithm. Uses a backing store, TLB, and page_table.
