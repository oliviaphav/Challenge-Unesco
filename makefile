CC = gcc
CCFLAGS = -Wall -g 

main : main.o haversine.o lectureFichiers.o site.o liste.o
	$(CC) $(CCFLAGS) main.o haversine.o lectureFichiers.o site.o liste.o  -o main -lm

main.o : main.c 
	$(CC) $(CCFLAGS) -c main.c

haversine.o : haversine.c haversine.h
	$(CC) $(CCFLAGS) -c haversine.c

lectureFichiers.o : lectureFichiers.c lectureFichiers.h
	$(CC) $(CCFLAGS) -c lectureFichiers.c

site.o : site.c site.h
	$(CC) $(CCFLAGS) -c site.c

liste.o : liste.c liste.h
	$(CC) $(CCFLAGS) -c liste.c

clean :
	rm -f *.o main
