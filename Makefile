compile:
	@echo "Compiling..."
	@g++ -o detector.out src/detector.cpp src/Car.cpp
	@echo "finished!"

run:
	@echo "Compiling..."
	@g++ -o detector.out src/detector.cpp src/Car.cpp
	@echo "finished!"
	./detector.out enc.csv output.csv
tests:
	@echo "Compiling..."
	@g++ -o test.out test/test.cpp src/Car.cpp
	@echo "finished!"
	./test.out test/enc.csv test/output.csv
	@rm test/output.csv test.out



clean:
	rm *.out
