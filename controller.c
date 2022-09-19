#include <stdio.h>
#include <stdlib.h>

/* Module interfaces */
#include "iauthentication.h"

void tests(void);

int
main(int argc, char **argv)
{
	tests();
	return EXIT_SUCCESS;
}

/**********/
/* TESTES */
/**********/

void
tests(void)
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

	printf("%d\n", signIn(user, password));
}
