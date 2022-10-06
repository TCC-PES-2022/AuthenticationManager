#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	system("./vsfls -f filename");
	system("$?");
	return 0;
}
