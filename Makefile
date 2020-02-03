make:
	gcc -c system_call.c 
	gcc -c context_switch.c 

runsc: 
	gcc -o runsc system_call.o
	./runsc

runcs:
	gcc -o runcs context_switch.o
	./runcs

clean:
	rm -f runsc runcs context_switch.o system_call.o