/*
* Esse fonte é responsável em gerar o histograma da imagem. Atualmente ele está em um fonte separado, mas futuramente o fonte será colocado 
* dentro de uma função em outro fonte.
* É necessário também que o código seja otmizado. Porque o código está horrivel! D:
* TODO - Otimização
* TODO - Definir o maior valor possível de um pixel dinamicamente
* TODO - Definir a quantidade de dígitos do valor do pixel dinamicamente
*/

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

#define NOVA_LINHA 10  /*Define do \n ( nova linha ) */
#define ESPACO 32 /*Define do  ' ' ( espaço em branco) */
#define TAMANHO_DIGITO_NUMERO 3 /*Define a quantidade de dígitos máxima que podem existir no valor do pixel */
#define MAIOR_VALOR_PIXEL 255 /*Define o maior valor possível de um pixel*/

int main(void)
{
	/*É somando 1 para que não seja necessário utilizar o posição 0.
	 *Dessa forma o número de ocorrências de pixels com valor 1 fica
         *na posição 1 e assim em diante */
	int ocorrenciasPixels[MAIOR_VALOR_PIXEL+1], i;
	char numero[TAMANHO_DIGITO_NUMERO];
	FILE *arquivo = fopen("Unequalized_Hawkes_Bay.pgm","r");

	if(!arquivo){
		printf("Perdeu playboy! Arquivo não encontradoo.\n");
		return 1;
	}

	printf("Eh nois que voa bruxão!\n");
	
	/*Zerando as posições de memória*/
	memset(ocorrenciasPixels, 0, sizeof(ocorrenciasPixels));

	for(i = 0; i < TAMANHO_DIGITO_NUMERO;i++)
		numero[i] = -1;
	
	if((fgetc(arquivo) == 'P' && fgetc(arquivo) == '2')){
		int linhasPuladas = 0;
		char charAtual;

		/*Lê o arquivo até o talo! */
		while (!feof(arquivo)) {
			charAtual = fgetc(arquivo);

			if(charAtual == '#') /* Ignora linha de comentario*/
                        	while(fgetc(arquivo) != NOVA_LINHA);

                        else if (charAtual == NOVA_LINHA)
				linhasPuladas++;

			/* Pula cabecalho */
			if (linhasPuladas < 3)
				continue;

			if (isdigit(charAtual)){
				/*Insere digito no array para formar o valor do pixel */
				for(i = 0; i < TAMANHO_DIGITO_NUMERO; i++)
					if(numero[i] == -1){
						numero[i] = charAtual;
						break;
					}

			} else if (isblank(charAtual) || charAtual == NOVA_LINHA){
				/*Contabiliza o pixel*/
				int valorPixel = atoi(numero);
				ocorrenciasPixels[valorPixel]++;
				/*Limpa array do valor do pixel*/
				for(i = 0; i <= TAMANHO_DIGITO_NUMERO;i++)
					numero[i] = -1;

			} else if(charAtual != EOF){
				/*Caracter estranho*/
				printf("Caracter não esperado! Encontrado: %d :(\n", charAtual);
				return 1;
			}
		}

		for(i = 1; i <= MAIOR_VALOR_PIXEL; i++)
			if (ocorrenciasPixels[i] > 0)
				printf("O valor %d possui %d ocorrências\n", i,ocorrenciasPixels[i]);

	} else {
		printf("Arquivo não segue formato PGM ASCII!\n");
	}

	fclose(arquivo);
	printf("Acabou!\n");

	return 0;
}
