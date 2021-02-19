#All Targets
all : splflix

splflix : bin/User.o bin/Action.o bin/Session.o bin/Watchable.o bin/main.o 
	g++ -o bin/splflix bin/User.o bin/Action.o bin/Session.o bin/Watchable.o bin/main.o 

bin/User.o : src/User.cpp include/User.h
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp
	
bin/Action.o : src/Action.cpp include/Action.h
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
	
bin/Session.o : include/Session.h src/Session.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp
	
	
bin/Watchable.o : src/Watchable.cpp include/Watchable.h
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp
	
bin/main.o : src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

clean: 
	rm -f bin/*
