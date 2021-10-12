compile:
	@echo "Compiling..."
	@g++ -o detector.out src/detector.cpp src/Car.cpp
	@echo "finished!"

run:
	@echo "Compiling..."
	@g++ -o detector.out src/detector.cpp src/Car.cpp
	@echo "finished!"
	./detector.out enc.csv output.csv


clean:
	rm *.out
