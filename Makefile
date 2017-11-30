files = *.c
headers = *.h

all: $(files) $(headers)
	gcc $(CFLAGS) -O3 -lrt -o EvalCircuit.exe $(files)  

profile: $(files) $(headers)
	gcc $(CFLAGS) -pg -g -O3 -o EvalCircuit.exe $(files) 
