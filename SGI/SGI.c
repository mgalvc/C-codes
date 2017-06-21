/*
Autor: Matheus Galvão Correia
Componente Curricular: MI - Algoritmos
Concluído em: 09/05/2016
Declaro que este código foi elaborado por mim de forma individual e não contém nenhum trecho de código de outro colega
ou de outro autor, tais como provindos de livros e apostilas, e páginas ou documentos eletrônicos da Internet. Qualquer
trecho de código de outra autoria que não a minha está destacado com uma citação para o autor e a fonte do código, e
estou ciente que estes trechos não serão considerados para fins de avaliação.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//estrutura da célula que vai guardar os dados do paciente vacinado
struct celula {
	int id;
	char nome_posto[250];
	char data[12];
	char nome_paciente[250];
	int rg;
	int idade;
	char endereco[250];

	//ponteiro para o próximo endereço da lista
	struct celula *proximo;
};

typedef struct celula Pessoa;

//cadastra um novo paciente
int novo_cadastro(Pessoa **, int);
//exibe o relatório em ordem alfabética
void relatorio(Pessoa *);
//limpa a tela da janela de comando
void limpa_tela();
//limpa o buffer do teclado
void limpa_buffer();
//realiza consulta pelo número de cadastro do paciente
void consulta_id(Pessoa *, int);
//realiza consulta pelo nome do paciente
void consulta_nome_paciente(Pessoa *, char *);
//realiza consulta pelo RG do paciente
void consulta_rg(Pessoa *, int);
//atualiza os dados de um paciente
void atualiza_dados(Pessoa *);
//exibe todos os dados de um paciente
void exibe_dados(Pessoa *);
//exibe as opções do menu de consulta
void exibe_menu_consulta(Pessoa *);
//remove um paciente da lista
void remove_dados(Pessoa **);
//helper para criar a lista ordenada
Pessoa *add_relatorio(Pessoa *, Pessoa *);
//verifica se um RG já existe no momento do novo cadastro
int rg_existe(Pessoa *, int);
//verifica se a lista está vazia (a cabeça aponta para NULL)
int nenhum_paciente(Pessoa *);

int main() {
	//"cabeça" da lista
	Pessoa *p = NULL;

	//menu - guardar a escolha no menu principal; consulta - guardar a escolha no menu de consulta
	//vacinados - guardar o total de vacinados; chave - guardar o RG ou número de cadastro para consulta
	int menu, consulta, vacinados = 1, chave;

	//nome do paciente para consulta
	char nome_paciente_consulta[250];

	limpa_tela();

	do {
		printf("----------Menu--------\n");
		printf("1 - Novo Cadastro\n");
		printf("2 - Consulta\n");
		printf("3 - Gerar Relatorio\n");
		printf("4 - Remover cadastro\n");
		printf("\n5 - Sair\n");

		scanf("%d", &menu);
		limpa_buffer();

		switch(menu) {
			//adiciona um novo paciente à lista
			case 1:
				limpa_tela();
				if(novo_cadastro(&p, vacinados))
					vacinados++;
				break;
			//realiza consulta
			case 2:
				limpa_tela();

				//verifica se existe algum paciente cadastrado
				if(nenhum_paciente(p)) {
					break;
				}
	
				printf("--------Consulta--------\n");
				printf("1 - Por numero de cadastro\n");
				printf("2 - Por nome de paciente\n");
				printf("3 - Por RG\n");

				scanf("%d", &consulta);
				limpa_buffer();

				switch(consulta) {
					//realiza consulta pelo numero de cadastro do paciente
					case 1:
						limpa_tela();
						printf("Numero de cadastro do paciente: ");
						scanf("%d", &chave);
						limpa_buffer();

						limpa_tela();
						consulta_id(p, chave);
						break;
					//realiza consulta pelo nome do paciente
					case 2:
						limpa_tela();
						printf("Nome do paciente: ");
						fgets(nome_paciente_consulta, 250, stdin);
						limpa_buffer();

						limpa_tela();
						consulta_nome_paciente(p, nome_paciente_consulta);
						break;
					//realiza consulta pelo RG do paciente
					case 3:
						limpa_tela();
						printf("RG do paciente: ");
						scanf("%d", &chave);
						limpa_buffer();

						limpa_tela();
						consulta_rg(p, chave);
						break;
					//para nenhum dos casos acima...
					default:
						limpa_tela();
						printf("Opcao invalida\n\n");
						break;
				}
			break;
			//exibe o relatório
			case 3:
				limpa_tela();

				//verifica se existe algum paciente cadastrado
				if (nenhum_paciente(p)) {
					break;
				}

				limpa_tela();
				relatorio(p);
				break;
			//remove um cadastro
			case 4:
				limpa_tela();

				//verifica se existe algum paciente cadastrado
				if (nenhum_paciente(p)) {
					break;
				}

				remove_dados(&p);
				break;
			case 5:
				printf("Programa Encerrado\n");
				exit(0);
			//para nenhum dos casos acima...
			default:
				limpa_tela();
				printf("Opcao invalida\n\n");
				break;
		}
	} while(menu != 5);

	return 0;
}

int nenhum_paciente(Pessoa *p) {
	//se a cabeça aponta para NULL...
	if(p == NULL) {
		printf("Nenhum paciente cadastrado\n\n");
		return 1;
	}

	return 0;
}

void remove_dados(Pessoa **p) {
	//guardar o numero do cadastro do paciente a ser removido
	int chave;
	//"aux" para percorrer a lista, "anterior" para guardar a célula a ser ligada com a próxima da que será removida
	Pessoa *aux = *p, *anterior;
	
	printf("Numero do cadastro: ");
	scanf("%d", &chave);
	limpa_buffer();

	//percorre a lista buscando o cadastro a ser eliminado
	while (aux != NULL && aux->id != chave) {
		anterior = aux;
		aux = aux->proximo;
	}

	//caso o paciente não exista...
	if (aux == NULL) {
		limpa_tela();
		printf("Paciente nao encontrado\n\n");
		return;
	}

	//caso de remoção da primeira célula da lista
	if (aux == *p) {
		aux = aux->proximo;
		free(*p);
		*p = aux;
	//demais casos: células do meio e fim
	} else {
		anterior->proximo = aux->proximo;
		free(aux);
	}

	limpa_tela();

	printf("Paciente de codigo %d removido\n\n", chave);
	 
}

int rg_existe(Pessoa *p, int rg) {
	Pessoa *aux = p;

	//percorre a lista procurando o RG
	while(aux != NULL) {
		if(aux->rg == rg) {
			return 1;
		}
		aux = aux->proximo;
	}

	return 0;
}

int novo_cadastro(Pessoa **p, int vacinados) {
	Pessoa *novo;

	//cria a nova célula
	novo = (Pessoa *)malloc(sizeof(Pessoa));
	if (novo == NULL) {
		printf("Sem espaço para novo cadastro\n\n");
		return;
	}

	//liga a nova célula à cabeça da lista
	novo->proximo = *p;

	//atribui o número de cadastro
	novo->id = vacinados;

	//preenche a nova célula
	printf("-----Novo Cadastro-----\n");

	printf("RG: ");
	scanf("%d", &novo->rg);
	limpa_buffer();

	//verifica se o RG já foi cadastrado
	if(rg_existe(*p, novo->rg)) {
		limpa_tela();
		printf("Paciente ja cadastrado\n\n");
		return 0;
	}

	//recebe os dados e preenche a célula
	printf("Nome: ");
	fgets(novo->nome_paciente, 250, stdin);
	limpa_buffer();

	printf("Idade: ");
	scanf("%d", &novo->idade);
	limpa_buffer();

	printf("Endereco: ");
	fgets(novo->endereco, 250, stdin);
	limpa_buffer();
		
	printf("Posto de Saude: ");
	fgets(novo->nome_posto, 250, stdin);
	limpa_buffer();

	printf("Data de Vacinacao: ");
	fgets(novo->data, 11 , stdin);
	limpa_buffer();

	system("pause");

	limpa_tela();
	printf("Cadastro realizado com sucesso\n\n");
	
	//a nova célula agora é a cabeça da lista
	*p = novo;

	return 1;
}

void relatorio(Pessoa *p) {
	//"aux" para percorrer a lista, "aux2" para auxiliar a ordenação
	Pessoa *aux = p, *aux2;
	//"chave" para guardar a idade a ser pesquisada, "tamanho" para guardar o tamanho da lista a ser ordenada, "i" 
	int chave, tamanho = 0;

	//cabeça da lista com os pacientes a serem listados
	Pessoa *relatorio = NULL;

	printf("Idade: ");
	scanf("%d", &chave);
	limpa_buffer();

	//procura as células com a idade informada e cria uma nova lista a partir delas
	while (aux != NULL) {
		if(aux->idade == chave) {
			relatorio = add_relatorio(relatorio, aux);
			tamanho++;
		}
		aux = aux->proximo;
	}

	//caso não existam pacientes com a idade informada...
	if (relatorio == NULL) {
		limpa_tela();
		printf("Nenhum paciente com %d anos\n\n", chave);
		return;

	}

	aux = relatorio;
	
	//cria uma célula para auxiliar a ordenação
	Pessoa *aux3 = (Pessoa *)malloc(sizeof(Pessoa));

	//ordena a lista do relatório
	while(aux != NULL) {
		aux2 = aux->proximo;		
		while(aux2 != NULL) {
			if(strcmp(aux->nome_paciente, aux2->nome_paciente) > 0) {
				//troca as células de posição
				aux3->id = aux2->id;
				strcpy(aux3->nome_posto, aux2->nome_posto);
				strcpy(aux3->data, aux2->data);
				strcpy(aux3->nome_paciente, aux2->nome_paciente);
				aux3->rg = aux2->rg;
				aux3->idade = aux2->idade;
				strcpy(aux3->endereco, aux2->endereco);

				aux2->id = aux->id;
				strcpy(aux2->nome_posto, aux->nome_posto);
				strcpy(aux2->data, aux->data);
				strcpy(aux2->nome_paciente, aux->nome_paciente);
				aux2->rg = aux->rg;
				aux2->idade = aux->idade;
				strcpy(aux2->endereco, aux->endereco);

				aux->id = aux3->id;
				strcpy(aux->nome_posto, aux3->nome_posto);
				strcpy(aux->data, aux3->data);
				strcpy(aux->nome_paciente, aux3->nome_paciente);
				aux->rg = aux3->rg;
				aux->idade = aux3->idade;
				strcpy(aux->endereco, aux3->endereco);
			}
			aux2 = aux2->proximo;
		}
		aux = aux->proximo;
	}

	limpa_tela();

	printf("-------Relatorio-------\n\n");

	//exibe o relatório para o usuário
	while (relatorio != NULL) {
		exibe_dados(relatorio);
		relatorio = relatorio->proximo;
	}

	system("pause");

	limpa_tela();

}

Pessoa *add_relatorio(Pessoa *relatorio, Pessoa *aux) {
	//nova célula
	Pessoa *novo;

	//cria a nova célula
	novo = (Pessoa *)malloc(sizeof(Pessoa));

	//aponta a nova célula para a "cabeça" da lista
	novo->proximo = relatorio;

	//passa a "cabeça" para a nova célula
	relatorio = novo;

	//preenche a nova célula
	relatorio->id = aux->id;
	strcpy(relatorio->nome_posto, aux->nome_posto);
	strcpy(relatorio->data, aux->data);
	strcpy(relatorio->nome_paciente, aux->nome_paciente);
	relatorio->rg = aux->rg;
	relatorio->idade = aux->idade;
	strcpy(relatorio->endereco, aux->endereco);

	//retorna o endereço da nova lista
	return relatorio;
}

void consulta_id(Pessoa *p, int chave) {
	//"aux" para percorrer a lista
	Pessoa *aux = p;
	//guardar a opção escolhida no menu 
	int menu;

	//busca o paciente com o número de cadastro informado
	while (aux != NULL) {
		if (aux->id == chave) {
			exibe_dados(aux);

			exibe_menu_consulta(aux);

			return;
		}
		//passa para a próxima célula
		aux = aux->proximo;
	}

	limpa_tela();
	printf("Paciente nao encontrado\n\n");
	return;
}

void exibe_menu_consulta(Pessoa *aux) {
	int menu;

	printf("-------Opcoes-------\n");
	printf("1 - Atualizar Dados\n");
	printf("2 - Voltar\n");
	scanf("%d", &menu);
	limpa_buffer();

	limpa_tela();

	switch(menu) {
		//atualizar os dados do paciente
		case 1:
			atualiza_dados(aux);
			break;
		default:
			break;
	}
}

void consulta_nome_paciente(Pessoa *p, char *nome) {
	Pessoa *aux = p;

	//busca o paciente com o nome informado
	while(aux != NULL) {
		if (strcmp(aux->nome_paciente, nome) == 0) {
			exibe_dados(aux);

			exibe_menu_consulta(aux);

			return;
		}
		aux = aux->proximo;
	}
	
	limpa_tela();
	printf("Paciente nao encontrado\n\n");
	return;
}

void consulta_rg(Pessoa *p, int chave) {
	Pessoa *aux = p;

	//busca o paciente com o RG informado
	while(aux != NULL) {
		if(aux->rg == chave) {
			exibe_dados(aux);

			exibe_menu_consulta(aux);

			return;
		}

		aux = aux->proximo;
	}

	limpa_tela();
	printf("Paciente nao encontrado\n\n");
	return;	
}

void atualiza_dados(Pessoa *aux) {
	int menu, rg;

	exibe_dados(aux);

	printf("--------Atualizar--------\n");
	printf("1 - Posto de Saude\n");
	printf("2 - Data de Vacinacao\n");
	printf("3 - Nome\n");
	printf("4 - RG\n");
	printf("5 - Idade\n");
	printf("6 - Endereco\n");
	scanf("%d", &menu);
	limpa_buffer();

	limpa_tela();

	exibe_dados(aux);

	//atualiza o dado de acordo com a seleção do menu
	switch(menu) {
		case 1:
			printf("Posto de Saude: ");
			fgets(aux->nome_posto, 250, stdin);
			limpa_buffer();
			break;
		case 2:
			printf("Data de Vacinacao: ");
			fgets(aux->data, 11, stdin);
			limpa_buffer();
			break;
		case 3:
			printf("Nome: ");
			fgets(aux->nome_paciente, 250, stdin);
			limpa_buffer();
			break;
		case 4:
			printf("RG: ");
			scanf("%d", &aux->rg);
			limpa_buffer();
			break;
		case 5:
			printf("Idade: ");
			scanf("%d", &aux->idade);
			limpa_buffer();
			break;
		case 6:
			printf("Endereco: ");
			fgets(aux->endereco, 250, stdin);
			limpa_buffer();
			break;

	}

	limpa_tela();
}

void exibe_dados(Pessoa *aux) {
	printf("Numero de cadastro: %d\n", aux->id);	
	printf("Posto de Saude: %s", aux->nome_posto);
	printf("Data da vacinacao: %s", aux->data);
	printf("Nome: %s", aux->nome_paciente);
	printf("RG: %d\n", aux->rg);
	printf("Idade: %d\n", aux->idade);
	printf("Endereco: %s\n", aux->endereco);
}

void limpa_tela() {
	system("cls");
}

void limpa_buffer() {
	fflush(stdin);
}