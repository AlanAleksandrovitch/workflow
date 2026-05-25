.PHONY: all, clean, test, test-san

all: rb_unit rb_stress rb_adv
rb_unit: main.o RBBST.o
	g++ -std=c++17 main.o RBBST.o -o rb_unit
rb_stress: stress.o RBBST.o
	g++ -std=c++17 stress.o RBBST.o -o rb_stress
rb_adv: adversarial.o RBBST.o
	g++ -std=c++17 adversarial.o RBBST.o -o rb_adv
main.o: main.cpp RBBST.h
	g++ -std=c++17 -c main.cpp
stress.o: stress.cpp RBBST.h
	g++ -std=c++17 -c stress.cpp
adversarial.o: adversarial.cpp RBBST.h
	g++ -std=c++17 -c adversarial.cpp
RBBST.o: RBBST.cpp RBBST.h
	g++ -std=c++17 -c RBBST.cpp
test: all
	./rb_unit && ./rb_stress && ./rb_adv
test-san:
	g++ -std=c++17 -g -fsanitize=address,undefined main.cpp RBBST.cpp -o rb_unit_san
	g++ -std=c++17 -g -fsanitize=address,undefined stress.cpp RBBST.cpp -o rb_stress_san
	g++ -std=c++17 -g -fsanitize=address,undefined adversarial.cpp RBBST.cpp -o rb_adv_san
	./rb_unit_san && ./rb_stress_san && ./rb_adv_san
clean:
	rm -f *.o rb_unit rb_stress rb_adv rb_unit_san rb_stress_san rb_adv_san
	rm -rf *.dSYM
