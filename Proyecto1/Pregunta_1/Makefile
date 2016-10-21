CC = gcc
CXX = g++
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT = --state_map --no_abstraction --history_len=0

psvn2c_core.c:
	cp ../src/psvn2c_core.c ./psvn2c_core.c

psvn2c_state_map.c:
	cp ../src/psvn2c_state_map.c ./psvn2c_state_map.c

psvn2c_abstraction.c:
	cp ../src/psvn2c_abstraction.c ./psvn2c_abstraction.c

%.c: %.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	../bin/psvn2c $(PSVNOPT) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

.PRECIOUS: %.c

%.calculoProfundidad: %.c ./calculoProfundidad.cpp
	$(CXX) $(OPT) ./calculoProfundidad.cpp -include $< -o $@
	rm -f $*.c

%.busqueda_iter_dfs_nodup: %.c ./busqueda_iter_dfs_nodup.cpp
	$(CXX) $(OPT) ./busqueda_iter_dfs_nodup.cpp -include $< -o $@
	rm -f $*.c

%.busqueda_iter_dfs: %.c ./busqueda_iter_dfs.cpp
	$(CXX) $(OPT)  ./busqueda_iter_dfs.cpp -include $< -o $@
	rm -f $*.c

.PHONY: clean
clean:
	rm -fr *.succ *.dist *.distSummary psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c *.dSYM *.o *~
