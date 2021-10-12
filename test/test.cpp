#include <iostream>
#include <string>

#include "../header/Car.hpp"
#include "../header/structs.hpp"


bool operator==(struct data a, struct data b){
	return ((a.timestamp == b.timestamp) && (a.turk_value == b.turk_value));
}

int main(int argc, char** argv){
	if (argc != 3){
		std::cout << "incorrect number of inputs" << std::endl;
	}
	Car c(argv[1], argv[2]);
	
	//test tokenize_line	
	struct data expected{12, -12};
	struct data line_to_test;
	const int line_test = 12;
	for (int i = 0; i < line_test; i++){
		c.tokenize_line();
	}
	line_to_test = c.tokenize_line();
	if (expected == line_to_test){
		std::cout << "line read test: \033[1;32mSuccess!\033[0m" << std::endl; 
	}
	else{
		std::cout << "line read test: \033[1;31mFailure\033[0m" << std::endl;
		std::cout << "got time: " << line_to_test.timestamp << " tick: " << line_to_test.turk_value << std::endl;
		std::cout << "expected time: " << expected.timestamp << " tick: " << expected.turk_value << std::endl;
	
	}

	c.~Car(); //so I don't have to worry about open files

	//test calc_ticks_moved
	Car d(argv[1], argv[2]);

	const int seg_size = 10;
	float expected_f = 9;
	struct data segment[seg_size];
	for (int i = 0; i < seg_size;i++){
		segment[i] = d.tokenize_line();
	}
	
	float got_f = d.calc_ticks_moved(segment, seg_size);
	if (expected_f == got_f){
		std::cout << "calc ticks moved test: \033[1;32mSuccess!\033[0m" << std::endl; 
	}
	else{
		std::cout << "callc ticks moved test: \033[1;31mFailure\033[0m" << std::endl;
		std::cout << "got: " << got_f << std::endl;
		std::cout << "expected: " << expected_f << std::endl;
	
	}

	//is moving test
	int expected_moving = 0; //moving flag
	int expected_n_moving = 1; //not moving flag
	float moving = 12000000000; //outragiously large number but guarentees moving
	float n_moving = 0;

	int got_m = d.is_moving(moving);
	int got_n_m = d.is_moving(n_moving);
	if (expected_moving == got_m){
		std::cout << "is moving test: \033[1;32mSuccess!\033[0m" << std::endl; 
	}
	else{
		std::cout << "is moving test: \033[1;31mFailure\033[0m" << std::endl;
		std::cout << "got: " << got_m << std::endl;
		std::cout << "expected: " << expected_moving << std::endl;
	
	}

	if (expected_n_moving == got_n_m){
		std::cout << "not moving test: \033[1;32mSuccess!\033[0m" << std::endl; 
	}
	else{
		std::cout << "not moving test: \033[1;31mFailure\033[0m" << std::endl;
		std::cout << "got: " << got_n_m << std::endl;
		std::cout << "expected: " << expected_n_moving << std::endl;
	
	}

}
