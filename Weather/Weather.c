#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//protótipos das funções
void converte_data_para_nome_arquivos(char *, char *, char *, char *);
void le_data(char *, char *, char *);
void le_estacao(char *);
void le_hora(int *);
void consulta(char *, char *, int);
void le_arquivos(char *, int);
void converte_indice(char *, int);
void quick_sort_crescente(float *, int *, int, int);
void quick_sort_decrescente(float *, int *, int, int);
void converte_indice2(int, char *);
void gera_saida(FILE *, float *, int *, char *);
void recria_ordem_cidades(int *ordem_cidades);
int valida_estacao(char *);

//struct que recebe os dados lidos dos arquivos
struct Dados {
	int instante;
	float temperatura;
	float pressao_atmosferica;
	float umidade;
	int precipitacao;
	float velocidade_vento;
	char direcao_vento;
};

int main() {
	int menu, hora, validacao;
	char ano[5], mes[3], dia[3], data_formatada[10], estacao[4], dias = 0;

	do {
		printf("\n\n1 - Consulta\n");
		printf("2 - Processamento de dados\n");
		printf("3 - Sair\n");
		printf("\nOpção: ");
		scanf("%d", &menu);
		//limpa a "sujeira" do buffer
		getchar();

		switch (menu) {
			case 1:
				//realiza a consulta
				le_estacao(estacao);
				
				//valida a estação escolhida
				if (valida_estacao(estacao) == 0) {
					printf("\nEstação Inválida\n");
					break;
				}

				le_data(dia, mes, ano);
				le_hora(&hora);
				converte_data_para_nome_arquivos(dia, mes, ano, data_formatada);
				consulta(estacao, data_formatada, hora);
				break;
			case 2:
				//processa os dados do dia
				le_data(dia, mes, ano);
				converte_data_para_nome_arquivos(dia, mes, ano, data_formatada);
				le_arquivos(data_formatada, dias);
				dias++;
				break;
			case 3:
				printf("Encerrado\n");
				break;
			default:
				printf("\nOpção Inválida");
				break;		
		}
	} while (menu != 3);

	return 0;
}

//lê a hora desejada para consulta
void le_hora(int *hora) {
	printf("Hora: ");
	scanf("%d", hora);
	getchar();
}

//lê a estação desejada para consulta
void le_estacao(char *estacao) {
	printf("\nEstação: ");
	fgets(estacao, 4, stdin);
	getchar();
}

//verifica se a estação escolhida para consulta realmente existe
int valida_estacao(char *estacao) {
	char sigla[4];
	int i;

	for (i = 0; i < 10; i++) {
		converte_indice2(i, sigla);
		if (strcmp(estacao, sigla) == 0) {
			return 1;
		}
	}

	return 0;
}

//lê a data desejada para consulta
void le_data(char *dia, char *mes, char *ano) {
	printf("Dia: ");
	fgets(dia, 3, stdin);
	getchar();
	printf("Mes: ");
	fgets(mes, 3, stdin);
	getchar();
	printf("Ano: ");
	fgets(ano, 5, stdin);
	getchar();	
}

//converte a data para o formato "anomesdia" usado no nome dos arquivos
void converte_data_para_nome_arquivos(char *dia, char *mes, char *ano, char *data_formatada) {
	strcpy(data_formatada, ano);
	strcat(data_formatada, mes);
	strcat(data_formatada, dia);
}

//abre o arquivo desejado e imprime os dados da hora escolhida
void consulta(char *estacao, char *data_formatada, int hora) {
	FILE *arquivo;
	char nome[26], direcao_vento[5];
	int i, instante, precipitacao;
	float temp, pressao, umidade, velocidade_vento; 

	//forma o nome do arquivo a ser lido
	strcpy(nome, "entradas/");
	strcat(nome, estacao);
	strcat(nome, "_");
	strcat(nome, data_formatada);
	strcat(nome, ".txt");

	//abre o arquivo somente para leitura
	arquivo = fopen(nome, "r");

	//lê o arquivo até encontrar a linha com a hora desejada
	for (i = 1; i <= hora; i++) {
		fscanf(arquivo, "%d %f %f %f %d %f %s", &instante, &temp, &pressao, &umidade, &precipitacao, &velocidade_vento, direcao_vento);
	}

	printf("\nTemperatura: %.1f\n", temp);
	printf("Pressão Atmosférica: %.1f\n", pressao);
	printf("Umidade: %.1f\n", umidade);
	printf("Precipitação: %d\n", precipitacao);
	printf("Velocidade do Vento: %.1f\n", velocidade_vento);
	printf("Direção do Vento: %s\n", direcao_vento);
}

//lê os arquivos diários de entrada para gerar os arquivos de saída
void le_arquivos(char *data_formatada, int dias) {
	int cidade, hora, i, j, k, ordem_cidades[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	char nome[26], nome_medias[31], nome_minimas[28], nome_maximas[28], nome_medias2[27], sigla[5];
	struct Dados dados[30][10][24];
	float temperatura_minima[10], temperatura_media[10], temperatura_maxima[10];

	FILE *arquivo_entrada;
	FILE *arquivo_medias;
	FILE *arquivo_minimas;
	FILE *arquivo_medias2;
	FILE *arquivo_maximas;

	//forma os nomes dos arquivos
	strcpy(nome_minimas, "saidas/minimas_");
	strcat(nome_minimas, data_formatada);
	strcat(nome_minimas, ".txt");

	strcpy(nome_medias2, "saidas/medias_");
	strcat(nome_medias2, data_formatada);
	strcat(nome_medias2, ".txt");

	strcpy(nome_maximas, "saidas/maximas_");
	strcat(nome_maximas, data_formatada);
	strcat(nome_maximas, ".txt");

	//abre os arquivos somente para escrita
	arquivo_minimas = fopen(nome_minimas, "w");
	arquivo_medias2 = fopen(nome_medias2, "w");
	arquivo_maximas = fopen(nome_maximas, "w");

	//garante uma estrutura com os últimos 30 dias de medição
	if (dias > 29) {
		for (i = 1; i < 30; i++) {
			for (j = 0; j < 10; j++) {
				for (k = 0; k < 24; k++) {
					dados[i-1][j][k] = dados[i][j][k];
				}
			}
		} 

		dias = 29;
	}


	for (cidade = 0; cidade < 10; cidade++) {
		float media_temperatura = 0;
		float media_pressao = 0;
		float media_umidade = 0;
		float media_precipitacao = 0;
		
		//forma o nome dos arquivos a serem lidos
		strcpy(nome, "entradas/");		
		converte_indice(nome, cidade);		
		strcat(nome, data_formatada);		
		strcat(nome, ".txt");
		
		//forma o nome dos arquivos de saída
		strcpy(nome_medias, "saidas/");
		strcat(nome_medias, "medias_");
		converte_indice(nome_medias, cidade);
		strcat(nome_medias, data_formatada);
		strcat(nome_medias, ".txt");

		//abre o arquivo somente para leitura
		arquivo_entrada = fopen(nome, "r");
		//abre o arquivo somente para escrita
		arquivo_medias = fopen(nome_medias, "w");

		//lê todas as linhas do arquivo e salva os dados na matriz de structs
		for (hora = 0; hora < 24; hora++) {
			fscanf(arquivo_entrada, "%d %f %f %f %d %f %s", &dados[dias][cidade][hora].instante, &dados[dias][cidade][hora].temperatura, &dados[dias][cidade][hora].pressao_atmosferica, &dados[dias][cidade][hora].umidade, &dados[dias][cidade][hora].precipitacao, &dados[dias][cidade][hora].velocidade_vento, &dados[dias][cidade][hora].direcao_vento);
			
			//calcula as médias
			media_temperatura += dados[dias][cidade][hora].temperatura/24.0;
			media_pressao += dados[dias][cidade][hora].pressao_atmosferica/24;
			media_umidade += dados[dias][cidade][hora].umidade/24;
			media_precipitacao += dados[dias][cidade][hora].precipitacao/24.0;
		}

		//preenche o arquivo com as médias da cidade
		fprintf(arquivo_medias, "Temperatura: %.2f\n", media_temperatura);
		fprintf(arquivo_medias, "Pressão Atmosférca: %.2f\n", media_pressao);
		fprintf(arquivo_medias, "Umidade: %.2f\n", media_umidade);
		fprintf(arquivo_medias, "Precipitação: %.2f\n", media_precipitacao);

		temperatura_minima[cidade] = dados[dias][cidade][0].temperatura;
		temperatura_maxima[cidade] = dados[dias][cidade][0].temperatura;
		temperatura_media[cidade] = media_temperatura; 

		//procura a maior e menor temperatura da cidade
		for (hora = 1; hora < 24; hora++) {
			if (dados[dias][cidade][hora].temperatura < temperatura_minima[cidade])
				temperatura_minima[cidade] =  dados[dias][cidade][hora].temperatura;

			if (dados[dias][cidade][hora].temperatura > temperatura_maxima[cidade])
				temperatura_maxima[cidade] =  dados[dias][cidade][hora].temperatura;
		}

		//fecha os arquivos
		fclose(arquivo_entrada);
		fclose(arquivo_medias);
	}

	quick_sort_crescente(temperatura_minima, ordem_cidades, 0, 10);

	gera_saida(arquivo_minimas, temperatura_minima, ordem_cidades, sigla);

	recria_ordem_cidades(ordem_cidades);

	quick_sort_crescente(temperatura_media, ordem_cidades, 0, 10);

	gera_saida(arquivo_medias2, temperatura_media, ordem_cidades, sigla);

	recria_ordem_cidades(ordem_cidades);

	quick_sort_decrescente(temperatura_maxima, ordem_cidades, 0, 10);

	gera_saida(arquivo_maximas, temperatura_maxima, ordem_cidades, sigla);
}

//gera o arquivo de saída
void gera_saida(FILE *arquivo, float *vetor, int *ordem_cidades, char *sigla) {
	int i;

	//preenche os arquivos com as temperaturas de cada cidade
	for (i = 0; i < 10; i++) {
		converte_indice2(ordem_cidades[i], sigla);
		fprintf(arquivo, "%s: %.2f\n", sigla, vetor[i]);
	}

	fclose(arquivo);
}

//auxilia na formação no nome do arquivo para leitura
void converte_indice(char *nome, int cidade) {
	switch(cidade) {
		case 0: 
			strcat(nome, "FSA_");
			break;
		case 1: 
			strcat(nome, "SSA_");
			break;
		case 2: 
			strcat(nome, "VDC_");
			break;
		case 3: 
			strcat(nome, "CAM_");
			break;
		case 4: 
			strcat(nome, "ITA_");
			break;
		case 5: 
			strcat(nome, "JUA_");
			break;
		case 6: 
			strcat(nome, "ILH_");
			break;
		case 7: 
			strcat(nome, "LDF_");
			break;
		case 8: 
			strcat(nome, "JEQ_");
			break;
		case 9: 
			strcat(nome, "TDF_");
			break;
	}
}

//auxilia na formação do nome do arquivo de saída
void converte_indice2(int i, char *sigla) {
	switch(i) {
		case 0: 
			strcpy(sigla, "FSA");
			break;
		case 1: 
			strcpy(sigla, "SSA");
			break;
		case 2: 
			strcpy(sigla, "VDC");
			break;
		case 3: 
			strcpy(sigla, "CAM");
			break;
		case 4: 
			strcpy(sigla, "ITA");
			break;
		case 5: 
			strcpy(sigla, "JUA");
			break;
		case 6: 
			strcpy(sigla, "ILH");
			break;
		case 7: 
			strcpy(sigla, "LDF");
			break;
		case 8: 
			strcpy(sigla, "JEQ");
			break;
		case 9: 
			strcpy(sigla, "TDF");
			break;
	}
}

//ordena o vetor em ordem crescente
void quick_sort_crescente(float *vetor, int *cidades, int inicio, int fim) {
   int i, j, aux_cidades;
   float meio, aux;
 
   i = inicio;
   j = fim;
   meio = vetor[(inicio + fim) / 2];
 
   do
   {
      while(vetor[i] < meio)
         i++;
      while(vetor[j] > meio)
         j--;
      if(i <= j)
      {
         aux = vetor[i];
         aux_cidades = cidades[i];
         vetor[i] = vetor[j];
         cidades[i] = cidades[j];
         vetor[j] = aux;
         cidades[j] = aux_cidades;
         i++;
         j--;
      }
   }while(i <= j);
 
   if(inicio < j)
      quick_sort_crescente(vetor, cidades, inicio, j);
   if(i < fim)
      quick_sort_crescente(vetor, cidades, i, fim);
}

//ordena o vetor em ordem decrescente
void quick_sort_decrescente(float *vetor, int *cidades, int inicio, int fim) {
   int i, j, aux_cidades;
   float meio, aux;
 
   i = inicio;
   j = fim;
   meio = vetor[(inicio + fim) / 2];
 
   do
   {
      while(vetor[i] > meio)
         i++;
      while(vetor[j] < meio)
         j--;
      if(i <= j)
      {
         aux = vetor[i];
         aux_cidades = cidades[i];
         vetor[i] = vetor[j];
         cidades[i] = cidades[j];
         vetor[j] = aux;
         cidades[j] = aux_cidades;
         i++;
         j--;
      }
   }while(i <= j);
 
   if(inicio < j)
      quick_sort_decrescente(vetor, cidades, inicio, j);
   if(i < fim)
      quick_sort_decrescente(vetor, cidades, i, fim);
}

//reordena o vetor para usá-lo novamente
void recria_ordem_cidades(int *ordem_cidades) {
	int i;

	for (i = 0; i < 10; i++) {
		ordem_cidades[i] = i;
	}
}
