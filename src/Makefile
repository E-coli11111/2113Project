main.o: main.cpp, Menu_Generator.cpp, Menu_Generator.h, Console_Operation.cpp, Console_Operation.h
  g++ -lpthread -c $<
Menu_Generator.o: Menu_Generator.cpp, Menu_Generator.h, Console_Operation.cpp, Console_Operation.h
  g++ -c $<
Console_Operation.o: Console_Operation.cpp, Console_Operation.h
  g++ -c $<
main: main.o, Menu_Generator.o, Console_Operation.o
  g++ $^ -lpthread -o $@
clean:
  rm -rf *.o
