# Motion_Detector_Turk
## Contents  
- [How to Use](#How-to-Use)
- [Task Description](#Task-Description)
- [Though Process](#Though-Process)
- [Description of Files](#Description-of-Files)

## How to Use
clone this project with: `git clone https://github.com/dit555/Motion_Detector.git`  
run `make` to compile project  
to run the project: `./detector.out <input.csv name> <output.csv name>`

## Task Description
Attached enc.csv file is the information from the wheel encoder.  Time is the first column, measurements is third column. This is a Turk absolute encoder, attached to the rear vehicle wheel, which outputs 10000 counts per wheel rotation. (in other words, when wheel rotates on 360 degrees, encoder output will change on 10000).  At least first 20 seconds of the data set is a static period, and at least last 10 seconds is also static period. There are some static periods in the middle of the trajectory. Can you catch them with your software? 

## Thought Process 
  
Since 10,000 ticks is one full rotation, we can calculate how much distance one tick moves the car is (pi * tire_diameter) / 10,000. If we consider the car is moving when it is going 0.5 m/s then in the time span of one second there must be at least 2506 ticks. Using this we can separate the data into 1 second segments and find out how many ticks have been added. if more than 2506 ticks have been added, then we know that in 1 second the car has moved 0.5 meters. Increasing the speed threshold and size of segments will decrease the effect of noise, as it becomes a less significant part of the data. 

### Results:
We define flips as the moving flag shifting to 1 or 0 from its previous value in the output file. When changing the minimum speed tolerance in src/Car.cpp I noticed that there were less flips in the data when the minimum speed tolerance is increased. For example, these are the graphs of the output file flags vs their row in the csv:  
<img src="https://user-images.githubusercontent.com/56750709/137031522-11b4190f-59be-44f2-ab4b-41b886afe307.png" width=300>
<img src="https://user-images.githubusercontent.com/56750709/137031535-0922a920-9397-49c0-b9cc-365378935036.png" width=300>  

As we can see, the graph with tolerance of 1 m/s (right) has less flips than the graph with 0.5 m/s tolerance (left).    
  

<img src="https://user-images.githubusercontent.com/56750709/137033862-a82688eb-08bd-44f0-a21f-e30e475a8e97.png" width=300>  

Above we see a graph of ticks/second vs segment (1s). *Note: increasing segment size to 2s or 3s did not make a visible difference to the appearance of the graph. From this we can conclude that increasing or decreasing the size of segments does not create a significant change in the way the data appears without going to extremes.*    

In the above ticks/s vs segment graph we notice that every value that is above 2506 ticks/s is considered a 0 on the output file graph with 0.5 m tolerance. In addition, every value that is above 5012 ticks/s is considered a 0 on the output file graph with 1 m tolerance.

### Error Checking
To make sure that the input csv has valid data the program checks:
1. Each row of input csv contains 3 columns separated by 2 commas.
2. rach row only contains numbers and the characters `,` `.` `e` `-`
  
To make sure that the data is valid:
1. Each segment is check to see if its total change in ticks is less than the maximum speed tolerance. Currently the tolerance is set to 150 mph or 67 m/s  

If any of these errors are detected, the program will stop running immediatly and will print out the appropriate error message and line number(s).
  
### Testing:
When running `make tests` test/test.cpp is compiled as the main and it tests `tokenize line()`, `calc_ticks_moved(...)`, and `is_moving(...)`  
after running the tests it prints a bright green success or a bright red failure with the expected value and recieved value. Sample output:  
<img src="https://user-images.githubusercontent.com/56750709/137043231-3675fe88-5e4f-4075-ad47-8a4b6e118c6b.jpg" width=500>



### Other considerations:  
Multithreading: Since we are reading the document in segments, using multithreading is an option to process the data much faster, at the cost of more memory. I chose not to explore this option as I can not test it on my linux virtual machine, as it only has one thread. As a result, I would not be able to adequately test the speed improvments.


## Description of files  
### emc.csv:   
* contains the data from the imu

### output.csv
* csv with columns: timestamp, motion flag
* timestamp is the timestamp form emc.csv
* motion flag is 0 when car is moving, 1 when it is not moving

### Makefile
* `make` or `make compile`: compiles the project as normal
* `make run`: compiles the project and runs with imu.csv as input and output.csv as output 
* `make tests`: compiles and runs using test/test.cpp
*  `make clean`: removes \*.out files

### header/structs.hpp:
* contains a row of data read from the csv file

### header/Car.hpp and src/Car.cpp:
* `std::string input_file_path, std::string output_file_path)`: constructor for car class, takes the file path for the input csv, and open the input and output files
* `struct data tokenize_line()`: reads a line from the csv and sparates the values into a data struct
* `int is_moving(float ticks_moved)`: returns the moving code if moving or not.
* `void write_to_output(float time, int flag)`: writes to the output csv
* `void run_moving()`: s
  * reads file in segments of 10 nd devides if the car is moving or not
* `~Car`: the destructor for car class, closes both files.

### src/detector.cpp:
* contains the main that runs the program

### test/test.cpp:
* runs the main for tests

### test/enc_gen.py:
* really quick way to generate an enc file for testing

### test/enc.csv
* a csv file for testing puroses
