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
	int i;

	/* -------------------- Inputs -------------------------- */

	const char *user_valid[] = {"UmNovoUsuario", "UsuarioNovo",
	"AnotherUser", "UserAnother", "OtherUser"};

	const char *user_invalid[] = {"umnovousuario", "", "user", "OA", "alkjdfls"};

	const char *password_valid[] = {"SenhaMarot@123", "SenhaMarota@123",
	"NovaSenha@123", "Uma@Senha1Qualquer", "Another@Senha1"};

	const char *password_invalid[] = {"", "SenhaMarot@", "senhaqualquer",
	"NovaSenha", "alkjdf123123"};

	/* -------------------- sugnUP user Fail -------------------- */

	for (i = 0; i < 5; i++)
		assert(3 == signUp(user_invalid[i], password_valid[i]));

	/* -------------------- sugnUP password Fail ---------------- */

	for (i = 0; i < 5; i++)
		assert(3 == signUp(user_valid[i], password_invalid[i]));

	/* -------------------- sugnUP user Success ----------------- */

	for (i = 0; i < 5; i++)
		assert(2 == signUp(user_valid[i], password_valid[i]));

	/* -------------------- Max users --------------------------- */

	assert(7 == signUp("Usuariozinho", "Senhaz1nh@"));

	/* -------------------- Success login ----------------------- */

	for (i = 0; i < 5; i++)
		assert(0 == login(user_valid[i], password_valid[i]));

	/* -------------------- Fail login -------------------------- */

	for (i = 0; i < 5; i++)
		assert(1 == login(user_valid[i], password_invalid[i]));

	/* -------------------- Remove users fail ------------------- */

	for (i = 0; i < 5; i++)
		assert(5 == removeUser(user_invalid[i]));

	/* -------------------- Remove users ------------------------ */

	for (i = 0; i < 5; i++)
		assert(4 == removeUser(user_valid[i]));

	/* -------------------- signUP same user -------------------- */

	assert(2 == signUp(user_valid[0], password_valid[0]));
	assert(6 == signUp(user_valid[0], password_valid[1]));
	assert(4 == removeUser(user_valid[0]));

	puts("All tests passed!");

	return;
}
