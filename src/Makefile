FLAGS = -pedantic-errors -std=c++11
main: main.o Menu_Generator.o Console_Operation.o RankList.o
	g++ $(FLAGS) $^ -o $@
	
main.o: main.cpp Menu_Generator.h Console_Operation.h RankList.h
	g++ $(FLAGS) -lpthread -c $<
	
Menu_Generator.o: Menu_Generator.cpp Menu_Generator.h Console_Operation.h RankList.h
	g++ $(FLAGS) -c $<
	
Console_Operation.o: Console_Operation.cpp
	g++ $(FLAGS) -c $<

RankList.o: RankList.cpp RankList.h Console_Operation.h
	g++ $(FLAGS) -c $<
	
clean:
	rm -f *.o main

.PHONY: clean
