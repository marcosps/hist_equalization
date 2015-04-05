/*
* Esse fonte é responsável em gerar o histograma da imagem. 
* Atualmente ele está em um fonte separado, mas futuramente o fonte 
* será colocado dentro de uma função em outro fonte.
*
* TODO - Definir a quantidade de dígitos do valor do pixel dinamicamente
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define TAMANHO_DIGITO_NUMERO 4 /*Define a quantidade de dígitos máxima que podem existir no valor do pixel */
#define DISTANCIA_SEGURA 100 /* mais espaco no buffer caso o calculo do novo pixel seja
			      * maior que o espaco do pixel antigo
			      * */

int cdf_min = 0;
int cdf_max = 0;
int maior_pixel = 0;
int largura = 0;
int altura = 0;

/* Gera distribuições acumuladas 
 * cada pixel posterior tem o peso dos pixels
 * pixels anterioes somados
 * */
void cdf(int *ori, int *nova)
{
	int i, s = 0;

	printf("Gerando distribuição acumulada...\n");

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
	printf("Distribuição gerada\n");
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
			char tmp[read + DISTANCIA_SEGURA], 
			     numero[TAMANHO_DIGITO_NUMERO];
			memset(numero, 0, sizeof(numero));
			memset(tmp, 0, sizeof(tmp));

			for(i = 0; i < read - 1; i++) {
				if (isdigit(line[i])) {
					numero[num_pos++] = line[i];
				} else {
					if (atoi(numero) > 0) {
						double tmp_var = cdf[atoi(numero)] - cdf_min;
						tmp_var /= (largura * altura - cdf_min);
						tmp_var *= maior_pixel;
						he = round(tmp_var);

						if (he < 0)
							printf("linha == %d e numero %s\n", linha, numero);

						/* se o novo valor equalizado tiver menos de 3 digitos */
						if (he < 100)
							strcat(tmp, " ");

					} else {
						he = 0;
					}
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
			linha++;
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

	FILE *arquivo = fopen(argv[1],"r");
	if(!arquivo) {
		printf("Perdeu playboy! Arquivo não encontrado.\n");
		return 1;
	}
	
	char* line = NULL;
	size_t len = 0;
	ssize_t read;

	// first line shows the version of PMG file
	read = getline(&line, &len, arquivo);
	if (read == -1) {
		perror("format read");
		return 1;
	}

	if (strncmp(line, "P2", 2)) {
		printf("File needs to be a P2 file format: Format found: %s\n", line);
		return 1;
	}

	read = getline(&line, &len, arquivo);
	if (read == -1) {
		perror("dimensions read");
		return -1;
	}

	// get width and height from line
	if (sscanf(line, "%d %d", &largura, &altura) == EOF) {
		perror("sscanf");
		return -1;
	}

	// store all pixels in this vector
	int pixels[largura * altura];
	bzero(pixels, largura * altura);

	// read the max pixel value
	read = getline(&line, &len, arquivo);
	if (read == -1) {
		perror("max pixel read");
		return -1;
	}

	// get max pixel value
	if (sscanf(line, "%d", &maior_pixel) == EOF) {
		perror("sscanf");
		return -1;
	}

	int ocorrenciasPixels[maior_pixel + 1];
	int acumulada[maior_pixel + 1];
	bzero(ocorrenciasPixels, sizeof(ocorrenciasPixels));
	bzero(acumulada, sizeof(acumulada));

	printf("Gerando histograma...\n");

	// index of current position inside pixels array
	int i = 0;
	while (getline(&line, &len, arquivo) != -1) {
		// skip commented line
		if (!strncmp("#", line, 1))
			continue;

		char *saveptr, *num;
		for (num = strtok_r(line, " ", &saveptr); num && num > 0; num = strtok_r(NULL, " ", &saveptr)) {
			int number = atoi(num);
			// store number in pixels array
			//*(pixels + i) = number;
			//printf("Pixels pos: %d value: %d array size: %d sizeof: %ld\n", i, number, altura * largura, sizeof(pixels));
			ocorrenciasPixels[number]++;
			i++;
		}
	}

	printf("Histograma finalizado\n");

	fclose(arquivo);
	cdf(ocorrenciasPixels, acumulada);
	monta_novo_arquivo(argv[1], acumulada);

	return 0;
}
