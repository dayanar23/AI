CC = gcc
CXX = g++
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT = --state_map --abstraction --backwards_moves --history_len=1

psvn2c_core.c:
	cp ../../src/psvn2c_core.c ./psvn2c_core.c

psvn2c_state_map.c:
	cp ../../src/psvn2c_state_map.c ./psvn2c_state_map.c

psvn2c_abstraction.c:
	cp ../../src/psvn2c_abstraction.c ./psvn2c_abstraction.c

%.c: %.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	../../bin/psvn2c $(PSVNOPT) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

.PRECIOUS: %.c

%.succ: %.c ../../global/succ.c
	$(CC) $(OPT) ../../global/succ.c -include $< -o $@
	rm -f $*.c

%.a_star: %.c ../../global/a_star.cpp
	$(CXX) $(OPT) ../../global/a_star.cpp -include $< -o $@
	rm -f $*.c

%.dist: %.c ../../global/dist.cpp
	$(CXX) $(OPT) ../../global/dist.cpp -include $< -o $@
	rm -f $*.c

%.distSummary: %.c ../../global/distSummary.cpp
	$(CXX) $(OPT) ../../global/distSummary.cpp -include $< -o $@
	rm -f $*.c

%.ida_star_gap: %.c ../../global/ida_star_gap.cpp
	$(CXX) $(OPT) ../../global/ida_star_gap.cpp -include $< -o $@
	rm -f $*.c

%.ida_star_pdb: %.c ../../global/ida_star_pdb.cpp
	$(CXX) $(OPT) ../../global/ida_star_pdb.cpp  -include $< -o $@
	rm -f $*.c

%.ida_star_m: %.c ../../global/ida_star_m.cpp
	$(CXX) $(OPT) ../../global/ida_star_m.cpp  -include $< -o $@
	rm -f $*.c

.PHONY: clean
clean:
	rm -fr *.succ *.dist *.distSummary psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c *.dSYM *.o *~
