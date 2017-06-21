/*
Autor: Matheus Galvão Correia
Componente Curricular: MI - Algoritmos
Concluído em: 04/03/2016
Declaro que este código foi elaborado por mim de forma individual e não contém nenhum trecho de código de outro colega
ou de outro autor, tais como provindos de livros e apostilas, e páginas ou documentos eletrônicos da Internet. Qualquer
trecho de código de outra autoria que não a minha está destacado com uma citação para o autor e a fonte do código, e
estou ciente que estes trechos não serão considerados para fins de avaliação.
*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
	//guardar os RGs dos passageiros
	long int passageiros[6][24];
	
	//poltronas disponíveis e ocupadas  	
	int poltronas[6][24];     	
	
	//i para percorrer linha, j para percorrer coluna
	int i, j;

	//auxiliar para validar a entrada do RG
	long int rg;

	int linha, coluna, disponiveis = 144, menu, rgInvalido;

	//linhaEmChar para guardar a poltrona digitada pelo usuário, 'confirmacao' para guardar a confirmação da compra
	char linhaEmChar, confirmacao;

	//zerar as matrizes
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 24; j++) {
			poltronas[i][j] = 0;
			passageiros[i][j] = 0;
		}
	}

	//só vende enquanto há poltronas disponíveis
	while (disponiveis != 0) {
		
		do {
			printf("\n------------Menu--------------\n");
			printf("1 - Nova venda\n");
			printf("2 - Visualizar esquema de voo\n");
			printf("3 - Desocupar poltrona\n");
			printf("4 - Cancelar voo\n");
			printf("\n5 - Encerrar vendas\n");

			printf("\nDigite sua escolha: ");

			scanf("%d", &menu);

			//validação da entrada do menu
			if (menu < 1 || menu > 5) {
				printf("\nOpcao Invalida\n");
			}
		} while (menu < 1 || menu > 5);		//garante que o menu será reexibido enquanto uma opção válida não for digitada

		switch (menu) {

			//iniciar uma nova venda
			case 1:
				printf("\n-----------------------");
				printf("\nNova venda iniciada\n");
				printf("Fileiras de A a F, corredores de 1 a 24\n");

				printf("\nDisponiveis: %d\n", disponiveis);
				printf("Ocupadas: %d\n", 144-disponiveis);

				do {
					printf("\nPara cancelar esta venda, digite -2. Para encerrar o programa, digite -1.");
					printf("\nRG do passageiro: ");
					scanf("%ld", &rg);

					//controle da validade do RG
					rgInvalido = 0;

					if (rg < -2 || rg == 0) {
						rgInvalido = 1;
						printf("\nRG invalido\n");
					} else {
						//percorre a matriz de passageiros para garantir que um RG só compre uma única poltrona
						for (i = 0; i < 6; i++) {
							for (j = 0; j < 24; j++) {
								if (rg == passageiros[i][j]) {
									rgInvalido = 1;

									printf("\nVoce ja comprou uma poltrona\n");
								}
							}
						}
					}

				} while(rgInvalido == 1);		//pede o RG do passageiro enquanto este não entrar com um RG válido

				if (rg == -2) {
					printf("\nVenda Cancelada\n");
					break;	
				} else
					if (rg == -1) {
						printf("Deseja realmente encerrar o programa? [s/n]");
						scanf(" %c", &confirmacao);

						if (confirmacao == 's') {
							printf("Programa Encerrado\n");
							exit(0);
						}	
					} else {
						
						do {
							printf("\nPoltrona desejada: ");
							scanf(" %c%d", &linhaEmChar, &coluna);

							//transforma uma possivel digitação minúscula em maiúscula para evitar erros na execução
							if (islower(linhaEmChar)) {
								linhaEmChar = toupper(linhaEmChar);
							}

							//converte o caractere em inteiro para manipular a matriz
							switch(linhaEmChar) {
								case 'A':
									linha = 5;
									break;
			
								case 'B':
									linha = 4;
									break;
			
								case 'C':
									linha = 3;
									break;
			
								case 'D':
									linha = 2;
									break;
			
								case 'E':
									linha = 1;
									break;
			
								case 'F':
									linha = 0;
									break;

								default:
									linha = -1;
									break;
							}	

							//mensagens para poltronas inválidas
							if (linha > 5 || linha < 0) {
								printf("Poltrona nao existe, selecione uma fileira entre A e F\n");
							} else {			
								if (coluna > 24 || coluna < 1) {
									printf("Poltrona nao existe, selecione um corredor entre 1 e 24\n");
								}

							}

							//mensagem para poltronas ocupadas
							if (poltronas[linha][coluna-1] == 1) {
								printf("\nPoltrona ocupada\n");
							}		
						} while (poltronas[linha][coluna-1] == 1 || coluna > 24 || coluna < 1 || linha > 5 || linha < 0);

						//confirma a compra
						printf("Poltrona %c%d disponivel. Confirmar compra? [s/n]", linhaEmChar, coluna);
						scanf(" %c", &confirmacao);

						//transforma uma possivel digitação maiúscula em minúscula para evitar erros na execução
						if (isupper(confirmacao)) {
							confirmacao = tolower(confirmacao);
						}

						if(confirmacao == 's') {
							poltronas[linha][coluna-1] = 1;
							passageiros[linha][coluna-1] = rg;

							disponiveis--;

							printf("Poltrona reservada\n");
						}
					}	
				break;
			
			//visualizar o esquema de vôo
			case 2: 
				printf("\nEsquema do voo [1 - ocupada, 0 - desocupada]\n\n");

				//imprime a matriz como esquema de vôo
				for (i = 0; i < 6; i++) {
					for (j = 0; j < 24; j++) {
						printf("%d ", poltronas[i][j]);
					}
					printf("\n");
				}

				printf("\n");

				//imprime a relação passagerio(RG) - poltrona
				for (i = 0; i < 6; i++) {
					for (j = 0; j < 24; j++) {
						//garante que só serão exibidas as poltronas ocupadas 
						if (passageiros[i][j] != 0) {
							//troca os índices inteiros pelos caracteres que representam as fileiras
							switch (i) {
								case 0:
									linhaEmChar = 'F';
									break;
								case 1:
									linhaEmChar = 'E';
									break;
								case 2:
									linhaEmChar = 'D';
									break;
								case 3:
									linhaEmChar = 'C';
									break;
								case 4:
									linhaEmChar = 'B';
									break;
								case 5:
									linhaEmChar = 'A';
									break;
							}

							printf("RG %ld: poltrona %c%d \n", passageiros[i][j], linhaEmChar, j+1);
						}
					}
				}
				break;
			
			//desocupar uma poltrona
			case 3:
				printf("RG do passageiro: ");
				scanf("%ld", &rg);

				rgInvalido = 1;

				//procura a poltrona correspondente ao RG
				for (i = 0; i < 6; i++) {
					for (j = 0; j < 24; j++) {
						if (passageiros[i][j] == rg) {
							//transforma o índice em letra para exibição
							switch (i) {
								case 0:
									linhaEmChar = 'F';
									break;
								case 1:
									linhaEmChar = 'E';
									break;
								case 2:
									linhaEmChar = 'D';
									break;
								case 3:
									linhaEmChar = 'C';
									break;
								case 4:
									linhaEmChar = 'B';
									break;
								case 5:
									linhaEmChar = 'A';
									break;
							}

							//se achou o RG na matriz, então ele não é inválido
							rgInvalido = 0;

							printf("Deseja desocupar a poltrona %c%d? [s/n]", linhaEmChar, j+1);
							scanf(" %c", &confirmacao);

							//transforma uma possivel digitação maiúscula em minúscula para evitar erros na execução
							if (isupper(confirmacao)) {
								confirmacao = tolower(confirmacao);
							}

							if (confirmacao == 's') {
								passageiros[i][j] = 0;
								poltronas[i][j] = 0;

								printf("\nPoltrona %c%d liberada\n", linhaEmChar, j+1);

							}
							
						}
					}
				}

				if (rgInvalido == 1) {
					printf("\nNao existe passageiro com este RG\n");
				}
				break;
			
			//cancelar o vôo, zerando as duas matrizes
			case 4:
				printf("Deseja realmente cancelar este voo? [s/n]");
				scanf(" %c", &confirmacao);

				//transforma uma possivel digitação maiúscula em minúscula para evitar erros na execução
				if (isupper(confirmacao)) {
					confirmacao = tolower(confirmacao);
				}

				if (confirmacao == 's') {
					printf("\nVoo Cancelado\n"); 
					main();
				}

				break;
			
			//encerrar completamente o programa
			case 5:
				printf("Deseja realmente encerrar as vendas? [s/n]");
				scanf(" %c", &confirmacao);

				//transforma uma possivel digitação maiúscula em minúscula para evitar erros na execução
				if (isupper(confirmacao)) {
					confirmacao = tolower(confirmacao);
				}

				if (confirmacao == 's') {
					printf("Vendas Encerradas\n");
					exit(0);
				}

				break;
		}
	}

	//quando as poltronas estiverem todas ocupadas...

	printf("\nVoo lotado\n");

	//imprime a relação passagerio(RG) - poltrona
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 24; j++) {
			//garante que só serão exibidas as poltronas ocupadas 
			if (passageiros[i][j] != 0) {
				//troca os índices inteiros pelos caracteres que representam as fileiras
				switch (i) {
					case 0:
						linhaEmChar = 'F';
						break;
					case 1:
						linhaEmChar = 'E';
						break;
					case 2:
						linhaEmChar = 'D';
						break;
					case 3:
						linhaEmChar = 'C';
						break;
					case 4:
						linhaEmChar = 'B';
						break;
					case 5:
						linhaEmChar = 'A';
						break;
				}

				printf("RG %ld: poltrona %c%d \n", passageiros[i][j], linhaEmChar, j+1);
			}
		}
	}

	return 0;

}