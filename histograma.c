/*
* Esse fonte é responsável em gerar o histograma da imagem. 
* Atualmente ele está em um fonte separado, mas futuramente o fonte 
* será colocado dentro de uma função em outro fonte.
* TODO - Definir a quantidade de dígitos do valor do pixel dinamicamente
*/

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

#define TAMANHO_DIGITO_NUMERO 3 /*Define a quantidade de dígitos máxima que podem existir no valor do pixel */

int main(void)
{
	/*É somando 1 para que não seja necessário utilizar o posição 0.
	 *Dessa forma o número de ocorrências de pixels com valor 1 fica
         *na posição 1 e assim em diante */
	int *ocorrenciasPixels, i, maior_pixel = 0;
	char numero[TAMANHO_DIGITO_NUMERO];
	FILE *arquivo = fopen("Unequalized_Hawkes_Bay.pgm","r");

	if(!arquivo){
		printf("Perdeu playboy! Arquivo não encontradoo.\n");
		return 1;
	}
	
	/*Zerando as posições de memória*/
	memset(numero, 0, sizeof(numero));
	
	if((fgetc(arquivo) == 'P' && fgetc(arquivo) == '2')){
		int linhas = 1, pos_numero = 0;
		char charAtual;

		/*Lê o arquivo até o talo! */
		while (!feof(arquivo)) {
			charAtual = fgetc(arquivo);

			if(charAtual == '#') { /* Ignora linha de comentario*/
                        	while(fgetc(arquivo) != '\n');
				continue;
			} else if (charAtual == '\n') {
				linhas++;
			}

			/* Verifica se é a linha que indica o valor do
			 * maior pixel  */
			if (linhas == 3) {
				charAtual = fgetc(arquivo);
				while (charAtual != '\n') {
					numero[pos_numero++] = charAtual;
					charAtual = fgetc(arquivo);
				}
				maior_pixel = atoi(numero);
				printf("Maior pixel == %d\n", atoi(numero));
				ocorrenciasPixels = (int *)malloc(sizeof(int) * (maior_pixel + 1));
				memset(ocorrenciasPixels, 0, maior_pixel + 1);
				memset(numero, 0, sizeof(numero));
				pos_numero = 0;
				linhas++;
				continue;
			}

			/* Pula cabecalho */
			if (linhas <= 3)
				continue;

			if (isdigit(charAtual)) {
				/*Insere digito no array para formar o valor do pixel */
				numero[pos_numero++] = charAtual;

			} else if (isblank(charAtual) || charAtual == '\n') {
				/*Contabiliza o pixel*/
				ocorrenciasPixels[atoi(numero)]++;
				/*Limpa array do valor do pixel*/
				memset(numero, 0, sizeof(numero));
				pos_numero = 0;

			} else if(charAtual != EOF) {
				/*Caracter estranho*/
				printf("Caracter não esperado! Encontrado: %d :(\n", charAtual);
				return 1;
			}
		}

		for(i = 1; i <= maior_pixel; i++)
			if (ocorrenciasPixels[i] > 0)
				printf("O valor %d possui %d ocorrências\n", i, ocorrenciasPixels[i]);

	} else {
		printf("Arquivo não segue formato PGM ASCII!\n");
	}

	fclose(arquivo);
	printf("Acabou!\n");

	return 0;
}
