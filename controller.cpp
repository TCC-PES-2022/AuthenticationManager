#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>

/* Module interfaces */
#include "iauthentication.h"
#include "UI_API.h"

/* TESTE GUI */
void* GUI(void* arg)
{
    char login[32] = "NewUserFLS";
    char senha[32] = "NEWPassword@";
    uint8_t val_teste = 0;
    static st_ui_image img_gui;
    static st_ui_aut aut_gui;
    if (iniciar_UI_interface(&img_gui,&aut_gui)) {
        //img_gui.byte_controle = UI_Carregar_Imagem; // APENAS PARA TESTES
        aut_gui.byte_controle = UI_EnviarLogin; // APENAS PARA TESTES
	memcpy(aut_gui.autenticao.login, login, sizeof(login));
	memcpy(aut_gui.autenticao.senha, senha, sizeof(senha));
        val_teste = UI_EnviarLogin;
    }
    pthread_detach(pthread_self());
    sleep(1);
    while (1)
    {
        if ((img_gui.byte_controle != UI_Aguardar) && (img_gui.byte_controle != UI_Null) || (aut_gui.byte_controle != UI_Aguardar) && (aut_gui.byte_controle != UI_Null)) {
            std::cout << "+[GUI] Cmd Enviado " << unsigned(val_teste) << endl;
           //img_gui = *carregar_imagem_GUI(val_teste, &img_gui);
           //img_gui = *transferir_imagem_GUI(val_teste, &img_gui);
           aut_gui = *verificar_autenticacao_GUI(val_teste, &aut_gui);
        }
        uint8_t val = verificarFilas(&img_gui,&aut_gui, UI__CTL);
        if (val == RP_fila_imagem) {
            std::cout << "+[GUI] Cmd Recebido " << unsigned(img_gui.byte_controle) << endl;
            std::cout << "+[GUI] Cmd Anterior " << unsigned(img_gui.byte_controle_anterior) << endl;
            usleep(2000);
            //reseta MQ
            std::cout << "+[GUI] Resetando MQ... " << endl;
            img_gui = *carregar_imagem_GUI(UI_Null, &img_gui);
        }
        else if (val == 2) {
            std::cout << "+[GUI] Cmd Recebido " << unsigned(img_gui.byte_controle) << endl;
            std::cout << "+[GUI] Cmd Anterior " << unsigned(img_gui.byte_controle_anterior) << endl;
            usleep(2000);
            //reseta MQ
            std::cout << "+[GUI] Resetando MQ... " << endl;
            img_gui = *carregar_imagem_GUI(UI_Null, &img_gui);
        }
        else if (val == 3)
        {
            std::cout << "+[GUI] Cmd Recebido " << unsigned(aut_gui.byte_controle) << endl;
            std::cout << "+[GUI] Cmd Anterior " << unsigned(aut_gui.byte_controle_anterior) << endl;
            usleep(2000);
            //reseta MQ
            std::cout << "+[GUI] Resetando MQ... " << endl;
            aut_gui = *verificar_autenticacao_GUI(UI_Null, &aut_gui);

        }
        //else
            //std::cout << "+[CTL] Cmd Nao Recebido \r\n";
        sleep(1);
    }

    // exit the current thread
    pthread_exit(NULL);
}
/* FIM TESTE GUI */

int
main(int argc, char **argv)
{
	int opt;

	pthread_t t;
	pthread_create(&t, NULL, GUI, NULL);

	unsigned char val;
	static st_ui_image img_controler = {0};
	static st_ui_aut aut_controler = {0};
	
	while (1) {
		val = verificarFilas(&img_controler, &aut_controler, UI__GUI);
		if (val == RP_fila_autenticacao) {
			aut_controler.byte_controle = UI_Aguardar;
			printf("%s\n", aut_controler.autenticao.login);
			printf("%s\n", aut_controler.autenticao.senha);

			printf("TEST: %d\n",
			login((char*)aut_controler.autenticao.login, (char*)
			aut_controler.autenticao.senha));

			aut_controler =
			*verificar_autenticacao_Controler(UI_Ok,
			&aut_controler);
		}
		sleep(1);
	}

	return EXIT_SUCCESS;
}
