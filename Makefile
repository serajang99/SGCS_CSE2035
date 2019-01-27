proj1_20181679 : main.o
	gcc -w -W -o proj1_20181679 main.o

main.o : proj1_20181679.c
	gcc -w -W -c -o main.o proj1_20181679.c

clean : 
	rm -f *.o
	rm -f proj1_20181679
