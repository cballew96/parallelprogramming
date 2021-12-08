CC = mpicc
EXE0 = gausscollect_mpi.exe
EXE1 = gausspoint_mpi.exe

%.exe : %.o
	$(CC) -o $@ $^

%.o : %.c
	$(CC) -c -o $@ $^

all: $(EXE0) $(EXE1)

clean:
	rm -f *.exe *.o
