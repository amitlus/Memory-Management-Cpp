CXX = g++
CXXFLAGS = -g -Wall -std=c++11
CPPFLAGS = -I ./include -MMD -MP

sourceS = $(wildcard ./source/*.cpp)
OBJS = $(patsubst ./source/%.cpp,./bin/%.o, $(sourceS))
DEPS := $(patsubst %.o,%.d, $(OBJS))

# default target
all: ./bin/studio

# build studio
./bin/studio: $(OBJS)
	@echo "Building ..."
	$(CXX) $(OBJS) -o $@
	@echo "Finished building"

# build cpp files
./bin/%.o: ./source/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# clean build files
clean:
	@rm -f ./bin/*

-include $(DEPS)