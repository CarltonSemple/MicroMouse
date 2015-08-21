
EXE_NAME = uMouse


CXX = clang++

DIAGNOSTIC = -fcaret-diagnostics -fdiagnostics-fixit-info -v

CXXFLAGS := -std=c++11  -stdlib=libc++ $(DIAGNOSTIC)

all : gpio.o main.o 
	$(CXX) $(CXXFLAGS) main.o -o $(EXE_NAME)

main.o : main.cpp 
	$(CXX) $(CXXFLAGS) $(inputs) 
	
	
# Navigation, movement, and sensors
nav.o : nav.cpp nav.h motor.h lidar.h packet.h
	$(CXX) $(CXXFLAGS) $(input) -c

lidar.o : lidar.cpp lidar.h tty.h tty.tcc packet.h
	$(CXX) $(CXXFLAGS) $(input) -c
	
packet.o : packet.cpp packet.h
	$(CXX) $(CXXFLAGS) $(input) -c
	
motor.o : motor.cpp motor.h gpio.h device.h device.tcc
	$(CXX) $(CXXFLAGS) $(input) -c
	
gpio.o : gpio.cpp gpio.h device.h device.tcc
	$(CXX) $(CXXFLAGS) $(input) -c
	
device.o : device.cpp device.h device.tcc 
	$(CXX) $(CXXFLAGS) $(input) -c




# Mapping and algorithms
star.o : star.cpp star.h grid.h cell.h wall.h lidar.h packet.h
	$(CXX) $(CXXFLAGS) -c $(inputs) -o $(output)

grid.o : grid.cpp grid.h cell.h wall.h
	$(CXX) $(CXXFLAGS) -c $(inputs) -o $(output)

cell.o : cell.cpp cell.h
	$(CXX) $(CXXFLAGS) -c $(inputs) -o $(output)
	
wall.o : wall.cpp cell.h
	$(CXX) $(CXXFLAGS) -c $(inputs) -o $(output)

