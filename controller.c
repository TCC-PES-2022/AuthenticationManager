#include <stdio.h>
#include <stdlib.h>

/* Module interfaces */
#include "iauthentication.h"

typedef enum controller_states {
	CO_ERROR = 9
} Controller_states;

void tests(int);

int
main(int argc, char **argv)
{
	/* authentication system */
	//while (1) {
	//	switch (state) {
	//		case CO_ERROR:
	//			/* TODO - logar mensagem de erro */
	//			exit(EXIT_FAILURE);
	//			break;
	//		default:
	//			break;
	//	}
	//}
	tests(argc);
	return EXIT_SUCCESS;
}

/**********/
/* TESTES */
/**********/

void
tests(int argc)
{
	char user[MAX_LEN_USER];
	char password[MAX_LEN_PW];

	/* ---------------- */
	/* just for testing */
	printf("user: ");
	scanf("%s", user);
	printf("password: ");
	scanf("%s", password);
	/* just for testing */
	/* ---------------- */

	if (argc == 2)
		printf("%d\n", signUp(user, password));
	else
		printf("%d\n", login(user, password));
}
