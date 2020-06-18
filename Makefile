all:
	gcc -Wall -Wextra -Wwrite-strings -fPIC -c -o camlink.o camlink.c
	gcc -Wall -Wextra -Wwrite-strings -shared -o camlink.so camlink.o -ldl

clean:
	rm -f *.so *.o
