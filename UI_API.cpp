#include "UI_API.h"
#include <queue>

using namespace std;
queue<st_ui_image> fila_imagem_GUI;
queue<st_ui_image> fila_imagem_Controler;

queue<st_ui_image> fila_transf_imagem_GUI;
queue<st_ui_image> fila_transf_imagem_Controler;

queue<st_ui_aut> fila_autent_GUI;
queue<st_ui_aut> fila_autent_Controler;

pthread_mutex_t lock_print;

///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\



st_ui_image* carregar_imagem_GUI(uint8_t cmd_controle, st_ui_image* val)
{
	//Máquina de Estados referente ao comando de Carregar Imagem lado GUI
	// cmd_controle -> Estado MQ_Imagem de solicitação
	// *val			-> Ponteiro da estrutura da imagem
	// Return: Estrutura st_ui_image contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.

	static st_ui_image tmp = { 0 };
	static uint8_t cmd_anterior = 0;
	//tmp.byte_controle_anterior = val->byte_controle_anterior;

	switch (cmd_controle)
	{
	case UI_Null: // Reseta MQ
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		ui_api_debug("+[GUI][MQ] : Comando UI_Null - MQ carregar_imagem_GUI Resetada");
		break;
	case UI_Ok: // CTRL -> GUI
		ui_api_debug("+[GUI][MQ] : Comando UI_Ok");
		tmp.byte_controle = cmd_controle;
		break;
	case UI_Timeout:
		break;
	case UI_Aguardar:
		break;
	case UI_Falha: 
		ui_api_debug("+[GUI][MQ] : Comando UI_Falha");
		tmp.byte_controle = cmd_controle;
		break;
	case UI_Carregar_Imagem:
		ui_api_debug("+[GUI][MQ] : Comando UI_Carregar_Imagem");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		fila_imagem_Controler.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Informacoes_Imagem:
		ui_api_debug("+[GUI][MQ] : Comando UI_Informacoes_Imagem");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		fila_imagem_Controler.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Cancelar:
		ui_api_debug("+[GUI][MQ] : Comando UI_Cancelar");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		fila_imagem_Controler.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
		break;
	default:
		//Caso cmd_controle nao contemplado
		tmp.byte_controle = UI_Null;
		tmp.byte_controle_anterior = UI_Null;
		break;
	}
	return &tmp;
}
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\




st_ui_image* carregar_imagem_Controler(uint8_t cmd_controle, st_ui_image* val)
{
	// Máquina de Estados referente ao comando de Carregar Imagem lado Controler
	// cmd_controle -> Estado MQ_Imagem de solicitação
	// *val			-> Ponteiro da estrutura da imagem
	// Return: Estrutura st_ui_image contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.

	static st_ui_image tmp;
	static uint8_t cmd_anterior = 0;

		switch (cmd_controle)
		{
		case UI_Falha:
			tmp = *val;
			tmp.byte_controle = cmd_controle;
			ui_api_debug("+[CTL][MQ] : Comando UI_Falha");
			fila_imagem_GUI.push(tmp);
			tmp.byte_controle = UI_Aguardar;
			break;
		case UI_Ok:
			tmp = *val;
			tmp.byte_controle = cmd_controle;
			ui_api_debug("+[CTL][MQ] : Comando UI_Ok");
			fila_imagem_GUI.push(tmp);
			tmp.byte_controle = UI_Aguardar;
			break;
		case UI_Timeout:
			break;
		case UI_Aguardar:
			break;
		case UI_Null: // Reseta MQ
			tmp = *val;
			tmp.byte_controle = cmd_controle;
			ui_api_debug("+[CTL][MQ] : Comando UI_Null - MQ carregar_imagem_Controler Resetada");
			break;
		case UI_Carregar_Imagem:
			ui_api_debug("+[CTL][MQ] : Comando UI_Carregar_Imagem");
			tmp = *val;
			tmp.byte_controle = cmd_controle;
			fila_imagem_GUI.push(tmp);
			tmp.byte_controle = UI_Aguardar;
			break;
		case UI_Informacoes_Imagem:
			ui_api_debug("+[CTL][MQ] : Comando UI_Informacoes_Imagem");
			tmp = *val;
			tmp.byte_controle = cmd_controle;
			fila_imagem_GUI.push(tmp);
			tmp.byte_controle = UI_Aguardar;
			break;
		case UI_Cancelar:
			ui_api_debug("+[CTL][MQ] : Comando UI_Cancelar");
			tmp = *val;
			tmp.byte_controle = cmd_controle;
			fila_imagem_GUI.push(tmp);
			tmp.byte_controle = UI_Aguardar;
			break;
		default:
			//Caso cmd_controle nao contemplado
			tmp.byte_controle = UI_Null;
			tmp.byte_controle_anterior = UI_Null;
			break;
		}
	
	return &tmp;
}

///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\


st_ui_image* transferir_imagem_GUI(uint8_t cmd_controle, st_ui_image* val) {

	//Máquina de Estados referente ao comando de Transferir Imagem
	// cmd_controle -> Estado MQ_Imagem de solicitação
	// *val			-> Ponteiro da estrutura da imagem
	// Return: Estrutura st_ui_image contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.

	static st_ui_image& tmp = *val;
	static uint8_t cmd_anterior = 0;
	switch (cmd_controle)
	{
	case UI_Falha:
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		ui_api_debug("+[GUI][MQ] : Comando UI_Falha");
		fila_transf_imagem_Controler.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Ok:
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		ui_api_debug("+[GUI][MQ] : Comando UI_Ok");
		fila_transf_imagem_Controler.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Timeout:
		break;
	case UI_Aguardar:
		break;
	case UI_Null: // Reseta MQ
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		ui_api_debug("+[GUI][MQ] : Comando UI_Null - MQ transferir_imagem_GUI Resetada");
		break;
	case UI_Cancelar:
		ui_api_debug("+[GUI][MQ] : Comando UI_Cancelar");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		fila_transf_imagem_Controler.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Iniciar_Transferencia:
		ui_api_debug("+[GUI][MQ] : Comando UI_Iniciar_Transferencia");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		fila_transf_imagem_Controler.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Andamento_Transferencia:
		ui_api_debug("+[GUI][MQ] : Comando UI_Andamento_Transferencia");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		fila_transf_imagem_Controler.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Fim_Transferencia:
		ui_api_debug("+[GUI][MQ] : Comando UI_Fim_Transferencia");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		fila_transf_imagem_Controler.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	default:
		//Caso cmd_controle nao contemplado
		tmp.byte_controle = UI_Null;
		tmp.byte_controle_anterior = UI_Null;
		break;
	}
	return &tmp;
}
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

st_ui_image* transferir_imagem_Controler(uint8_t cmd_controle, st_ui_image* val) {

	//Máquina de Estados referente ao comando de Transferir Imagem
	// cmd_controle -> Estado MQ_Imagem de solicitação
	// *val			-> Ponteiro da estrutura da imagem
	// Return: Estrutura st_ui_image contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.

	static st_ui_image tmp;
	static uint8_t cmd_anterior = 0;
	tmp.byte_controle_anterior = val->byte_controle_anterior;
	if (tmp.byte_controle_anterior != UI_Aguardar)
		tmp.byte_controle_anterior = cmd_controle;

	switch (cmd_controle)
	{
	case UI_Falha:
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		ui_api_debug("+[CTL][MQ] : Comando UI_Falha");
		fila_transf_imagem_GUI.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Ok:
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		ui_api_debug("+[CTL][MQ] : Comando UI_Ok");
		fila_transf_imagem_GUI.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Timeout:
		break;
	case UI_Aguardar:
		break;
	case UI_Null: // Reseta MQ
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		ui_api_debug("+[CTL][MQ] : Comando UI_Null - MQ transferir_imagem_Controler Resetada");
		break;
	case UI_Iniciar_Transferencia:
		ui_api_debug("+[CTL][MQ] : Comando UI_Iniciar_Transferencia");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		fila_transf_imagem_GUI.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Andamento_Transferencia:
		ui_api_debug("+[CTL][MQ] : Comando UI_Andamento_Transferencia");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		fila_transf_imagem_GUI.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Fim_Transferencia:
		ui_api_debug("+[CTL][MQ] : Comando UI_Fim_Transferencia");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		fila_transf_imagem_GUI.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Cancelar:
		ui_api_debug("+[CTL][MQ] : Comando UI_Cancelar");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		fila_transf_imagem_GUI.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	default:
		//Caso cmd_controle nao contemplado
		tmp.byte_controle = UI_Null;
		tmp.byte_controle_anterior = UI_Null;
		break;
	}

	return &tmp;
}
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

st_ui_image* informacoes_imagem(uint8_t cmd_controle, st_ui_image* val)
{

	//Máquina de Estados referente ao comando de solicitar informações da Imagem
	// cmd_controle -> Estado MQ_Imagem de solicitação
	// *val			-> Ponteiro da estrutura da imagem
	// Return: Estrutura st_ui_image contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.

	static st_ui_image& tmp = *val;
	static uint8_t cmd_anterior = 0;
	switch (cmd_controle)
	{
	default:
		break;
	}
	return &tmp;
}
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

int iniciar_UI_interface(st_ui_image* val, st_ui_aut* val2)
{
	//Função de inicialização da UI_Interface
	// stats		-> Se inicialização ocorreu com sucesso, retorna 1
	int stats = 0;
	memset(val, 0x0, sizeof(st_ui_image));

	ui_api_debug("+[GUI] Inicializando UI API... ");
	carregar_imagem_GUI(UI_Null, val);
	transferir_imagem_GUI(UI_Null, val);
	carregar_imagem_Controler(UI_Null, val);
	transferir_imagem_Controler(UI_Null, val);
	verificar_autenticacao_GUI(UI_Null, val2);
	verificar_autenticacao_Controler(UI_Null, val2);


	stats = 1;

	return stats;
}

///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

int kill_UI_interface(st_ui_image* val)
{
	//Função de finalização da UI_Interface
	// stats		-> Se inicialização ocorreu com sucesso, retorna 1
	int stats = 0;

	st_ui_image& tmp = *val;

	stats = 1;

	return stats;
}

///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

st_info_conexao* listar_conexoes(uint8_t cmd_controle, st_info_conexao* val) {
	// Função que deve requisitar a lista de conexões.
	// cmd_controle -> Estado MQ de solicitação
	// *val			-> Ponteiro da estrutura da conexao
	// Return: Estrutura st_info_conexao contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.
	st_info_conexao& tmp = *val;
	static uint8_t cmd_anterior = 0;
	switch (cmd_controle)
	{
	default:
		break;
	}
	return &tmp;
}
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

st_info_conexao* estabelecer_conexao(uint8_t cmd_controle, st_info_conexao* val) {

	// Função que deve requisitar o estabelecimento da conexão.
	// cmd_controle -> Estado MQ de solicitação
	// *val			-> Ponteiro da estrutura da conexao
	// Return: Estrutura st_info_conexao contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.

	st_info_conexao& tmp = *val;
	static uint8_t cmd_anterior = 0;
	switch (cmd_controle)
	{
	default:
		break;
	}
	return &tmp;
}
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

st_info_conexao* encerrar_conexao(uint8_t cmd_controle, st_info_conexao* val) {

	// Função que deve requisitar o encerramento da conexão.
	// cmd_controle -> Estado MQ de solicitação
	// *val			-> Ponteiro da estrutura da conexao
	// Return: Estrutura st_info_conexao contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.

	st_info_conexao& tmp = *val;
	static uint8_t cmd_anterior = 0;
	switch (cmd_controle)
	{
	default:
		break;
	}
	return &tmp;
}
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\

st_info_conexao* verificar_conexao(uint8_t cmd_controle, st_info_conexao* val)
{

	// Função que deve verificar se existe conexão.
	// cmd_controle -> Estado MQ de solicitação
	// *val			-> Ponteiro da estrutura da conexao
	// Return: Estrutura st_info_conexao contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.

	st_info_conexao& tmp = *val;
	static uint8_t cmd_anterior = 0;
	switch (cmd_controle)
	{
	default:
		break;
	}
	return &tmp;
}
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\


st_ui_aut* verificar_autenticacao_GUI(uint8_t cmd_controle, st_ui_aut* val)
{
	// Função que deve verificar a autenticação fornecida pelo usuário.
	// cmd_controle -> Estado MQ de solicitação
	// *val			-> Ponteiro da estrutura da autenticação
	// Return: Estrutura st_ui_aut contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.

	static st_ui_aut& tmp = *val;
	static uint8_t cmd_anterior;
	switch (cmd_controle)
	{
	case UI_EnviarLogin:
		ui_api_debug("+[GUI][MQ] : Comando UI_EnviarLogin");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		fila_autent_Controler.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Falha:
		ui_api_debug("+[GUI][MQ] : Comando UI_Falha");
		tmp.byte_controle = cmd_controle;
		break;
	case UI_Ok:
		ui_api_debug("+[GUI][MQ] : Comando UI_Ok");
		tmp.byte_controle = cmd_controle;
		break;
	case UI_Null:
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		ui_api_debug("+[GUI][MQ] : Comando UI_Null - MQ verificar_autenticacao_GUI Resetada");
		break;
	default:
		break;
	}
	return &tmp;
}
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\


st_ui_aut* verificar_autenticacao_Controler(uint8_t cmd_controle, st_ui_aut* val)
{
	// Função que deve verificar a autenticação fornecida pelo usuário.
	// cmd_controle -> Estado MQ de solicitação
	// *val			-> Ponteiro da estrutura da autenticação
	// Return: Estrutura st_ui_aut contendo todas as informações referentes a imagem e o estado da ultima alteração feita pela MQ.

	static st_ui_aut& tmp = *val;
	static uint8_t cmd_anterior;
	switch (cmd_controle)
	{
	case UI_EnviarLogin:
		ui_api_debug("+[CTL][MQ] : Comando UI_EnviarLogin");
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		fila_autent_GUI.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Falha:
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		ui_api_debug("+[CTL][MQ] : Comando UI_Falha");
		fila_autent_GUI.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Ok:
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		ui_api_debug("+[CTL][MQ] : Comando UI_Ok");
		fila_autent_GUI.push(tmp);
		tmp.byte_controle = UI_Aguardar;
		break;
	case UI_Null:
		tmp = *val;
		tmp.byte_controle = cmd_controle;
		tmp.byte_controle_anterior = cmd_controle;
		ui_api_debug("+[CTL][MQ] : Comando UI_Null - MQ verificar_autenticacao_Controler Resetada");
		break;
	default:
		break;
	}
	return &tmp;
}
///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\


uint8_t verificarFilas(st_ui_image* val, st_ui_aut *val2,  uint8_t thread)
{
	st_ui_image tmp;
	st_ui_aut tmp2;
	if (thread == UI__GUI) {
		if (!fila_imagem_Controler.empty())
		{
			tmp = fila_imagem_Controler.front();
			memcpy(val, &tmp, sizeof(st_ui_image));
			fila_imagem_Controler.pop();
			return RP_fila_imagem;
		}
		if (!fila_transf_imagem_Controler.empty())
		{
			tmp = fila_transf_imagem_Controler.front();
			memcpy(val, &tmp, sizeof(st_ui_image));
			fila_transf_imagem_Controler.pop();
			return RP_fila_transf_imagem;
		}
		if (!fila_autent_Controler.empty())
		{
			tmp2 = fila_autent_Controler.front();
			memcpy(val2, &tmp2, sizeof(st_ui_aut));
			fila_autent_Controler.pop();
			return RP_fila_autenticacao;
		}
	}
	else
	{
		if (!fila_imagem_GUI.empty())
		{
			tmp = fila_imagem_GUI.front();
			memcpy(val, &tmp, sizeof(st_ui_image));
			fila_imagem_GUI.pop();
			return RP_fila_imagem;
		}
		if (!fila_transf_imagem_GUI.empty())
		{
			tmp = fila_transf_imagem_GUI.front();
			memcpy(val, &tmp, sizeof(st_ui_image));
			fila_transf_imagem_GUI.pop();
			return RP_fila_transf_imagem;
		}
		if (!fila_autent_GUI.empty())
		{
			tmp2 = fila_autent_GUI.front();
			memcpy(val2, &tmp2, sizeof(st_ui_aut));
			fila_autent_GUI.pop();
			return RP_fila_autenticacao;
		}
	}

return 0;
}

///\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\  //\\ 
///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\///\\\


int ui_api_debug(string txt)
{
#ifdef DEBUG_UI_API
	pthread_mutex_lock(&lock_print);
	std::cout  << txt << endl;
	pthread_mutex_unlock(&lock_print);
#else

#endif
	return 1;

}

int quadrado(int a)

{

	return a * a;
}