#ifndef __CAR__
#define __CAR__

#include <iostream>
#include <string>
#include <fstream>

#include "structs.hpp"


class Car{
public:
	Car(std::string input_file_path, std::string output_file_path); //constructor input filepath name
	void run_moving(); //reads the file and decides whether moving
	~Car(); //closes the files

private:
	//private functions
	struct data tokenize_line(); //read next line from file and separate data into data struct
	int is_moving(float ticks_moved); //updates speed of car, 0 if moving, 1 if not
	void write_to_output(float time, int flag); //writes data to new csv
	float calc_ticks_moved(struct data* data_segment, const int size);
	
	//private data
	std::ifstream car_data_file; //stores opened file
	std::ofstream car_output_file; //stores new csv file
	int read_line_num; //line that is currently being read
	int eof_flag; //end of file flag, 1 if exit normally, 2 if early exit
	float tick_min_threshold; //minimum ticks to be considered moving
	float tick_max_threshold; //moving to fast to be real data 
};

#endif
