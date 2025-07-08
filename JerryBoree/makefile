JerryBoree: Jerry.o LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o
	gcc Jerry.o LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o -o JerryBoree

Jerry.o: Jerry.c Jerry.h Defs.h
	gcc -c Jerry.c

LinkedList.o: LinkedList.c LinkedList.h Defs.h
	gcc -c LinkedList.c

KeyValuePair.o: KeyValuePair.c KeyValuePair.h Defs.h
	gcc -c KeyValuePair.c

HashTable.o: HashTable.c LinkedList.h KeyValuePair.h HashTable.h Defs.h
	gcc -c HashTable.c

MultiValueHashTable.o: MultiValueHashTable.c MultiValueHashTable.h HashTable.h LinkedList.h Defs.h
	gcc -c MultiValueHashTable.c

JerryBoreeMain.o: JerryBoreeMain.c Defs.h LinkedList.h HashTable.h MultiValueHashTable.h Jerry.h  
	gcc -c JerryBoreeMain.c

clean:
	rm -f *.o JerryBoree
