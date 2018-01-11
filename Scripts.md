# Lab1/Moore/Room307/makefile
```bash
project: hello.o hello1.o hello2.o hello3.o
	gcc hello.o hello1.o hello2.o hello3.o -o project

hello.o: hello.c hello.h
	gcc -c hello.c

hello1.o: hello1.c hello.h
	gcc -c hello1.c

hello2.o: hello2.c hello.h
	gcc -c hello2.c

hello3.o: hello3.c hello.h
	gcc -c hello3.c
```

# Lab1-2012/scriptlab1
```bash
#!/bin/sh

set Data/*   
> ab1b-results
for a in $*; do
  ./sum $a >> lab1b-results
done
```