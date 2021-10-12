#include <iostream>
#include <string>
#include <fstream>

#include "../header/Car.hpp"
#include "../header/structs.hpp"
#include "../header/data_math.hpp"

const float speed_tolerance = 0.25;
const int mean_array_size = 10;

Car::Car(std::string input_file_path, std::string output_file_path){
	car_data_file.open(input_file_path, std::ifstream::in);
	car_output_file.open(output_file_path, std::ofstream::out | std::ofstream::trunc);
	car_time_previous = 0;
	car_speed_Y = 0;
	car_speed_Z = 0;
	car_rotation_speed_X = 0;
	eof_flag = 0;
}

struct data Car::tokenize_line(){
	struct data data_return; //data struct to be returned 
	std::string::size_type index; //starting inde of stof
	std::string line;
	if (car_data_file.is_open()){
		if(std::getline(car_data_file, line)){
			data_return.timestamp = std::stof(line, &index); //timestamp
			index++;
			line = line.substr(index);
			std::stof(line,&index); //accel_X
			index++;
			line = line.substr(index);
			data_return.accel_Y = std::stof(line, &index); //accel_Y
			index++;
			line = line.substr(index);
			data_return.accel_Z = std::stof(line, &index); //accel_Z
			index++;
			line = line.substr(index);
			data_return.rot_speed_X = std::stof(line, &index); //rot_spd_X
		}
		else{
			eof_flag = 1; //file is fully read
		}
	}
	else{ 
		eof_flag = 1; //file not open handle as if fully read
	}
	return data_return;
}

void Car::update_car_speed(struct data d){
	//convert m/s^2 into m/time_dif^2
	float accel_scale; //acceleration m/s^2 scaled to 
	float time_dif = d.timestamp - car_time_previous; //imu sends data every 100ms 
	accel_scale = time_dif * time_dif; // accel_scale ^ 2

	//apply
	//also make sure that new data is far enough way from the zero +/- stev margine
	//accel_Y
	int accel_Y_lower = car_mean.accel_Y - car_standard_deviation.accel_Y > d.accel_Y;
	int accel_Y_upper = d.accel_Y > car_mean.accel_Y + car_standard_deviation.accel_Y;
	if (accel_Y_upper || accel_Y_lower){
		car_speed_Y += (d.accel_Y - car_mean.accel_Y) * accel_scale; 
	}

	//accel_Z
	int accel_Z_lower = car_mean.accel_Z - car_standard_deviation.accel_Z > d.accel_Z;
	int accel_Z_upper = d.accel_Z > car_mean.accel_Z + car_standard_deviation.accel_Z;
	if (accel_Z_upper || accel_Z_lower){
		car_speed_Z += (d.accel_Z - car_mean.accel_Z) * accel_scale;  
	}

	//car_rotation_speed_X
	int car_rotation_speed_X_lower = car_mean.rot_speed_X - car_standard_deviation.rot_speed_X > d.rot_speed_X;
	int car_rotation_speed_X_upper = car_mean.rot_speed_X + car_standard_deviation.rot_speed_X < d.rot_speed_X;
if (car_rotation_speed_X_lower || car_rotation_speed_X_upper){
		car_rotation_speed_X = d.rot_speed_X - car_mean.rot_speed_X;
	}
	
	car_time_previous = d.timestamp;

}

void Car::write_to_output(float time, int flag){
	if(car_output_file.is_open()){
		car_output_file << time << "," << flag << std::endl;
	}
	else {
		eof_flag = 1; //if file not open handle as if eof
	}
}

void Car::run_moving(){
	struct data cur; //curent data
	
	struct data mean_array[mean_array_size]; //array be used for mean and stdev
	for (int i = 0; i < mean_array_size; i++){
		mean_array[i] = tokenize_line();
		car_time_previous = mean_array[i].timestamp;
		write_to_output(mean_array[i].timestamp, 1);//vehicle is standing still during this time
	}
	car_mean = mean_data(mean_array, mean_array_size); //find mean
	car_standard_deviation = standard_deviation_data(car_mean, mean_array, mean_array_size); //find stdev
	
	//do the rest
	cur = tokenize_line();
	while (!eof_flag){
		update_car_speed(cur);
		float speed_magnitude = get_speed(car_speed_Y, car_speed_Z);
		
		//is moving?
		int speed_check = speed_magnitude >= speed_tolerance;
		int rotation_check_upper = car_rotation_speed_X >= speed_tolerance;
	       	int rotation_check_lower = car_rotation_speed_X <= -speed_tolerance;
		if (speed_check || (rotation_check_upper && rotation_check_lower)){ //check if moving
			write_to_output(cur.timestamp, 0); // moving
		}
		else{
			write_to_output(cur.timestamp, 1); //not moving			
		}
		cur = tokenize_line();
	}
}

Car::~Car(){
	car_data_file.close();
	car_output_file.close();
}
