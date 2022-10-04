#include <stdio.h>
#include <stdlib.h>

/* Module interfaces */
#include "iauthentication.h"
#include "UI.h"

void tests(int);

int
main(int argc, char **argv)
{
	enum UI_INTERFACE state;
	/* authentication system */
	while (1) {
		switch (state) {
			case UI_Falha:
				/* TODO - logar mensagem de erro */
				break;
			case UI_Ok:
				break;
			case UI_Timeout:
				break;
			case UI_Aguardar:
				break;
			case UI_Null:
				break;
			case UI_Iniciar_Transferencia:
				break;
			case UI_Andamento_Transferencia:
				break;
			case UI_Cancelar:
				break;
			case UI_Fim_Transferencia:
				break;
			case UI_Carregar_Imagem:
				break;
			case UI_Informacoes_Imagem:
				break;
			case UI_EnviarLogin:
				break;
			default:
				break;
		}
		break;
	}
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
