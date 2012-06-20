/*
* Esse fonte é responsável em gerar o histograma da imagem. 
* Atualmente ele está em um fonte separado, mas futuramente o fonte 
* será colocado dentro de uma função em outro fonte.
*
* TODO - Definir a quantidade de dígitos do valor do pixel dinamicamente
*/

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define TAMANHO_DIGITO_NUMERO 4 /*Define a quantidade de dígitos máxima que podem existir no valor do pixel */

int cdf_min = 0;
int cdf_max = 0;
int maior_pixel = 0;
int largura = 0;
int altura = 0;

/* Gera distribuições acumuladas 
 * cada pixel posterior tem o peso dos pixels
 * pixels anterioes somados
 * */
void cdf(int *ori, int *nova, int maior_pixel)
{
	int i, s = 0;

	for(i = 1; i <= maior_pixel; i++) {
		if (ori[i] > 0) {
			s += ori[i];
			/* Coloca valor minimo da acumulada */
			if (cdf_min == 0)
				cdf_min = s;

			nova[i] = s;
		}
	}

	/* acumulada maxima */
	cdf_max = s;
}

/* Gera nova imagem PGM já equalizada */
void monta_novo_arquivo(char *ori_file, int *cdf)
{
	FILE *f = fopen(ori_file,"r");
	FILE *n = fopen("nova.pgm", "w");
	int linha = 1, num_pos = 0;
	char *line = NULL;
	ssize_t read;
	size_t len = 0;
	
	while((read = getline(&line, &len, f)) != -1) {
		if (line[0] == '#') {
			fprintf(n, "%s", line);
		} else if (linha <= 3) {
			fprintf(n, "%s", line);
			linha++;
		} else {
			int i, he;
			char tmp[read], numero[TAMANHO_DIGITO_NUMERO];
			memset(numero, 0, sizeof(numero));
			memset(tmp, 0, sizeof(tmp));

			for(i = 0; i < read - 1; i++) {
				if (isdigit(line[i])) {
					numero[num_pos++] = line[i];
				} else {					
					double tmp_var = cdf[atoi(numero)] - cdf_min;
					tmp_var /= (largura * altura - cdf_min);
					tmp_var *= maior_pixel;
					he = round(tmp_var);
					/* se o novo valor equalizado tiver menos de 3 digitos */
					if (he < 100)
						strcat(tmp, " ");

					sprintf(numero, "%d", he);
					strcat(tmp, numero);

					memset(numero, 0, sizeof(numero));
					num_pos = 0;
					/* separar pixels */
					strcat(tmp, " ");
				}
			}
			strcat(tmp, "\n");
			fprintf(n, "%s", tmp);
		}
	}
	fclose(f);
	fclose(n);
	free(line);
}

int main(int argc, char *argv[])
{

	if (argc < 2) {
		printf("Uso do programa: ./h <caminho_para_imagem>.pgm\n");
		return 1;
	}

	/*É somando 1 para que não seja necessário utilizar o posição 0.
	 *Dessa forma o número de ocorrências de pixels com valor 1 fica
         *na posição 1 e assim em diante */
	int *ocorrenciasPixels, *acumulada;
	char numero[TAMANHO_DIGITO_NUMERO];
	FILE *arquivo = fopen(argv[1],"r");

	if(!arquivo){
		printf("Perdeu playboy! Arquivo não encontrado.\n");
		return 1;
	}
	
	/*Zerando as posições de memória*/
	memset(numero, 0, sizeof(numero));
	
	if((fgetc(arquivo) == 'P' && fgetc(arquivo) == '2')){
		int linhas = 1, pos_numero = 0;
		char charAtual;

		printf("Gerando histograma...\n");

		/*Lê o arquivo até o talo! */
		while (!feof(arquivo)) {
			charAtual = fgetc(arquivo);

			if(charAtual == '#') { /* Ignora linha de comentario*/
                        	while(fgetc(arquivo) != '\n');
				continue;
			} else if (charAtual == '\n') {
				linhas++;
				continue;
			}

			/* Verifica se é a linha com as dimensões da imagem */
			if (linhas == 2) {
				while(charAtual != ' ') {
					numero[pos_numero++] = charAtual;
					charAtual = fgetc(arquivo);
				}
				largura = atoi(numero);
				memset(numero, 0, sizeof(numero));
				pos_numero = 0;

				charAtual = fgetc(arquivo);
				while(charAtual != '\n') {
					numero[pos_numero++] = charAtual;
					charAtual = fgetc(arquivo);
				}
				altura = atoi(numero);
				
				memset(numero, 0, sizeof(numero));
				pos_numero = 0;
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
				ocorrenciasPixels = (int *)malloc(sizeof(int) * (maior_pixel + 1));
				acumulada = (int *)malloc(sizeof(int) * (maior_pixel + 1));
				memset(ocorrenciasPixels, 0, maior_pixel + 1);
				memset(acumulada, 0, maior_pixel + 1);
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

		printf("Histograma finalizado\n");
		printf("Gerando distribuição acumulada...\n");

		cdf(ocorrenciasPixels, acumulada, maior_pixel);

		printf("Distribuição gerada\n");

	} else {
		printf("Arquivo não segue formato PGM ASCII!\n");
	}

	fclose(arquivo);
	
	printf("Gerando nova imagem PGM...\n");
	monta_novo_arquivo(argv[1], acumulada);
	printf("Nova imagem gerada com sucesso!\n");

	free(ocorrenciasPixels);
	free(acumulada);

	return 0;
}
