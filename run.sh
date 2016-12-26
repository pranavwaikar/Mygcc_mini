#Author:PKW
#This is a test script...it is useful to verify various conditions on any file containing c code & named hello.c.
# A test file hello.c is given in package


echo "\nWelcome to mygcc code all testings:--\n"
echo "\nbuilding the mygcc:\n1)32 BIT  2)64 BIT\nEnter your choice:";
read choice;

case "$choice" in
	1) 
		gcc -o mygcc mygcc.c
	;;
	2)
		gcc -o mygcc mygcc_64.c
	;;
	*)
		echo "Invalid choice";
esac
clear

echo "\nTest 1:mygcc hello.c\n"; 
	./mygcc hello.c
	cat build.log
	echo "\nPress any key to continue\n";
	read next;
	rm a.out
	clear
	

echo "\nTest2:mygcc -S hello.c\n";
	./mygcc -S hello.c
	cat build.log
	echo "\nPress any key to continue\n";
	read next;
	rm a.out.s
	clear

echo "\nTest3:mygcc -c hello.c\n";
	./mygcc -c hello.c
	cat build.log
	echo "\nPress any key to continue\n";
	read next;
	rm a.out.o
	clear

echo "\nTest4:mygcc -o hello hello.c\n";
	./mygcc -o hello hello.c
	cat build.log
	echo "\nPress any key to continue\n";
	read next;
	rm hello
	clear
	
echo "\nTest4:mygcc -S -o hello hello.c\n";
	./mygcc -S -o hello hello.c
	cat build.log
	echo "\nPress any key to continue\n";
	read next;
	rm hello.s
	clear

echo "\nTest5:mygcc -c -o hello hello.c\n";
	./mygcc -c -o hello hello.c
	cat build.log
	echo "\nPress any key to continue\n";
	read next;
	rm hello.o
	clear

echo "\nTest6:Do not build if source is not changed\n";
	echo "\nFirst creating a custom build\n";
	./mygcc -o hello hello.c
	echo "\nFirst build created now trying again..observe the log..any key to continue:\n";
	read next;
	./mygcc -o hello hello.c
	cat build.log
	echo "\nPress any key to continue\n";
	read next;
	rm hello
	clear

echo "\nTets7:Build when source is changed\n";
	echo "\nFirst creating a custom build\n";
	./mygcc -o hello hello.c
	echo "\nFirst build created now modifying source code..any key to continue:\n";
	read next;
	touch -m hello.c
	echo "\nobserve the log now:--\n";
	read next
	./mygcc -o hello hello.c
	cat build.log
	echo "\nPress any key to EXIT\n";
	read next;
	rm hello
	rm build.log
	rm mygcc
	clear
	
