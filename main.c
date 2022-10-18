#include <stdlib.h>
#include <getopt.h>
#include <assert.h>

/* Module interfaces */
#include "iauthentication.h"

static void test(void);

int
main(int argc, char **argv)
{
	int opt;
	char user[32];
	char password[32];

	while ((opt = getopt(argc, argv, "l:s:r:f:tawd")) != -1) {
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
			case 'd':
				test();
				break;
			case '?':
				printf("unknown option: %c\n", optopt);	
				break;
		}
	}
	return EXIT_SUCCESS;
}

static void
test(void)
{
	/* -------------------- Inputs -------------------------- */

	/* Capital letter */
	char user1[32]     = "UmNovoUsuario";
	/* without capital letter */
	char user2[32]     = "umnovousuario";
	/* empty user */
	char user3[32]     = "";
	/* short user */
	char user4[32]     = "user";

	/* Capital letter, numbers and special characters */
	char password1[32] = "SenhaMarot@123";
	/* Without Capital letter */
	char password2[32] = "senhasarot@123";
	/* Without special letter */
	char password3[32] = "SenhaMarota123";
	/* Without numbers */
	char password4[32] = "SenhaMarot@";
	/* empty password */
	char password5[32] = "";

	/* -------------------- sugnUP user  -------------------- */

	assert(2 == signUp(user1,password1));
	assert(6 == signUp(user1,password1));
	assert(3 == signUp(user1,password2));
	assert(3 == signUp(user1,password3));
	assert(3 == signUp(user1,password4));
	assert(3 == signUp(user1,password5));

	assert(3 == signUp(user2,password1));
	assert(3 == signUp(user2,password2));
	assert(3 == signUp(user2,password3));
	assert(3 == signUp(user2,password4));
	assert(3 == signUp(user2,password5));

	assert(3 == signUp(user3,password1));
	assert(3 == signUp(user3,password2));
	assert(3 == signUp(user3,password3));
	assert(3 == signUp(user3,password4));
	assert(3 == signUp(user3,password5));

	assert(3 == signUp(user4,password1));
	assert(3 == signUp(user4,password2));
	assert(3 == signUp(user4,password3));
	assert(3 == signUp(user4,password4));
	assert(3 == signUp(user4,password5));

	/* -------------------- authenticate -------------------- */

	assert(0 == login(user1,password1));
	assert(1 == login(user1,password2));
	assert(1 == login(user1,password3));
	assert(1 == login(user1,password4));
	assert(1 == login(user1,password5));

	assert(1 == login(user2,password1));
	assert(1 == login(user2,password2));
	assert(1 == login(user2,password3));
	assert(1 == login(user2,password4));
	assert(1 == login(user2,password5));

	assert(1 == login(user3,password1));
	assert(1 == login(user3,password2));
	assert(1 == login(user3,password3));
	assert(1 == login(user3,password4));
	assert(1 == login(user3,password5));

	assert(1 == login(user4,password1));
	assert(1 == login(user4,password2));
	assert(1 == login(user4,password3));
	assert(1 == login(user4,password4));
	assert(1 == login(user4,password5));

	/* -------------------- Remove user --------------------- */

	assert(4 == removeUser(user1));
	assert(5 == removeUser(user2));
	assert(5 == removeUser(user3));
	assert(5 == removeUser(user4));

	puts("All tests passed!");

	return;
}
