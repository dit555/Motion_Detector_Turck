#include <iostream>
#include <string>

#include "../header/Car.hpp"


int main(int argv, char** argc){
	if(argv != 3) {
		std::cout << "invalid arguments" << std::endl;
		return 0;
	}
	std::string read_file_path = argc[1];
	std::string write_file_path = argc[2];
	std::cout << "reading file: " << read_file_path << std::endl;
	std::cout << "output file: " << write_file_path << std::endl;
	Car c(read_file_path, write_file_path);
	c.run_moving();
	std::cout << "Done!" << std::endl;

	return 0;
}

