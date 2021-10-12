compile:
	@echo "Compiling..."
	@g++ -o detector.out src/detector.cpp src/Car.cpp src/data_math.cpp
	@echo "finished!"

run:
	@echo "Compiling..."
	@g++ -o detector.out src/detector.cpp src/Car.cpp src/data_math.cpp
	@echo "finished!"
	./detector.out imu.csv output.csv


clean:
	rm *.out
