#include <iostream>
#include <string>
#include <fstream>

#include "../header/Car.hpp"
#include "../header/structs.hpp"

//constants
const float distance_tolerance = 0.5; //minimum distance in one second to be considered moving
const float tire_diameter = 0.635; //the radius (meters) of the tires on a 2001 Honda accord (my car). 
const float turk_tick_full_rotation = 10000; //ticks to full rotation
const int ticks_per_second = 100; //time turk takes to update in seconds

const int car_is_moving = 0;
const int car_not_moving = 1;

//data validation constants
const int csv_columns = 3;
const float distance_tolerance_too_fast = 67; //maximum distance covered in one second to be accepted (150 mph)

Car::Car(std::string input_file_path, std::string output_file_path){
	car_data_file.open(input_file_path, std::ifstream::in);
	car_output_file.open(output_file_path, std::ofstream::out | std::ofstream::trunc);
	eof_flag = 0;
	read_line_num = 0;

	//calculate tick threshold
	float distance_per_tick = 3.14159 * tire_diameter; // circumfrence of tire pi * diameter
	distance_per_tick /= turk_tick_full_rotation; // find distance per tick 
	tick_min_threshold = distance_tolerance / distance_per_tick; //distance covered ot be considered moving in ticks	
	tick_max_threshold = distance_tolerance_too_fast / distance_per_tick;
}

struct data Car::tokenize_line(){
	struct data data_return; //data struct to be returned 
	std::string::size_type index; //starting inde of stof
	std::string line;
	if (car_data_file.is_open()){
		if(std::getline(car_data_file, line)){
			//validate line from csv
			int number_count = 0;
			int comma_count = 0;
			int special_count = 0; //'.' '-' and 'e' 
			read_line_num++;

			for (int i = 0; i < line.length(); i++){
				char cur = line[i];
				if (cur == ','){
					comma_count++;
				}
				else if (cur >= (int)'0' && line[i] <= (int)'9'){
					number_count++;
				}
				else if(cur == '.' || cur == '-' || cur == 'e'){
					special_count++;
				}
			}

			bool valid_data_count = (number_count + comma_count + special_count == line.length());
			bool valid_comma_count = (comma_count == csv_columns - 1);
			if(valid_data_count && valid_comma_count){
				data_return.timestamp = std::stof(line, &index); //timestamp
				index++;
				line = line.substr(index);
				std::stof(line,&index); //2nd col
				index++;
				line = line.substr(index);
				data_return.turk_value = std::stof(line, &index); //turk_tick
			}
			else{
				eof_flag = 2; //bad data
				std::cout << "data on line: " << read_line_num << " is not of a valid format" << std::endl;
			}
		}
		else{
			eof_flag = 1; //file is fully read
		}
	}
	else{ 
		eof_flag = 2; //file not open handle as if fully read
		std::cout<< "input file not opened succesfully" << std::endl;
	}
	return data_return;
}

int Car::is_moving(float ticks_moved){
	

	bool moving_flag = (ticks_moved >= tick_min_threshold); 
	if(moving_flag){
		return car_is_moving;
	}
	else{
		return car_not_moving;
	}
}

void Car::write_to_output(float time, int flag){
	if(car_output_file.is_open()){
		car_output_file << time << "," << flag << std::endl;
	}
	else {
		eof_flag = 1; //if file not open handle as if eof
		std::cout << "output file is not open" << std::endl;
	}
}

float Car::calc_ticks_moved(struct data* data_segment, const int size){
	float moved = 0;
	for (int i = 1; i < size ; i++){
		moved += data_segment[i].turk_value - data_segment[i - 1].turk_value; 
	}
	
	//error checking in data
	bool too_fast = (moved >= tick_max_threshold);
	if (too_fast){
		eof_flag = 2;
		std::cout << "car is moving too fast on lines: " << read_line_num - size << " - " << read_line_num << std::endl;
	}
	return -moved; //data is negative
}

void Car::run_moving(){
	struct data data_segment[ticks_per_second]; //holds data for a segment
	float ticks_moved;
	int moving_flag;
	while (eof_flag == 0){
		//fill segment
		for (int i = 0; i < ticks_per_second;i++){
			data_segment[i] = tokenize_line();
			if (eof_flag == 2){ //check data not in csv format
				break;
			}
		}

		ticks_moved = calc_ticks_moved(data_segment, ticks_per_second);

		//write segment to file
		moving_flag = is_moving(ticks_moved);
		
		if(eof_flag == 2){ //check data does not increase too fast
			break;
		}

		for (int j = 0; j < ticks_per_second; j++){
			write_to_output(data_segment[j].timestamp, moving_flag);
		}
	}
	if(eof_flag == 2){
		std::cout << "file read exited prematurely" << std::endl;
	}
}

Car::~Car(){
	car_data_file.close();
	car_output_file.close();
}
