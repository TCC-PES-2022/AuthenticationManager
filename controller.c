#include <stdlib.h>
#include <getopt.h>

/* Module interfaces */
#include "iauthentication.h"

int
main(int argc, char **argv)
{
	int opt;
	char user[20];
	char password[20];

	while ((opt = getopt(argc, argv, "l:s:r:f:taw")) != -1) {
		/* authentication */
		switch (opt) {
			/* load file to program */
			case 'l':
				strcpy(user, optarg);
				break;
			case 's':
				strcpy(password, optarg);
				break;
			case 'r':
				printf("%d\n", removeUser(optarg));
				break;
			case 'a':
				printf("%d\n", login(user, password));
				break;
			case 'w':
				printf("%d\n", signUp(user, password));
				break;
			case 'f':
				printf("filename: %s\n", optarg);
				exit(200);
				break;
			case '?':
				printf("unknown option: %c\n", optopt);	
				break;
		}
	}
	return EXIT_SUCCESS;
}
