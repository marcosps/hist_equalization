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

#define NOVA_LINHA 10  /*Define do \n ( nova linha ) */
#define ESPACO 32 /*Define do  ' ' ( espaço em branco) */
#define TAMANHO_DIGITO_NUMERO 3 /*Define a quantidade de dígitos máxima que podem existir no valor do pixel */
#define MAIOR_VALOR_PIXEL 255 /*Define o maior valor possível de um pixel*/

int main(void)
{
	FILE *arquivo = fopen("Unequalized_Hawkes_Bay.pgm","r");
	char numero[TAMANHO_DIGITO_NUMERO];
	/*É somando 1 para que não seja necessário utilizar o posição 0.
	 *Dessa forma o número de ocorrências de pixels com valor 1 fica
         *na posição 1 e assim em diante */
	int ocorrenciasPixels[MAIOR_VALOR_PIXEL+1];
	
	/*Zerando as posições de memória*/
	for(int index = 1; index <= MAIOR_VALOR_PIXEL;index++){
		ocorrenciasPixels[index] = 0;
	}	
	for(int index = 0; index < TAMANHO_DIGITO_NUMERO;index++){
		numero[index] = -1;
	}
	
	if(!arquivo){
		printf("Perdeu playboy! Arquivo não encontradoo.\n");
		return 1;
	}
	printf("Eh nois que voa bruxão!\n");
		if(fgetc(arquivo) == 'P' & fgetc(arquivo) == '2'){
		/*Arquivo do formato esperado*/
		/*Pula o cabeçalho do arquivo*/
		int linhasPuladas = 0;
		while(!feof(arquivo) & linhasPuladas < 3){
			char proximoChar = fgetc(arquivo);
			if(proximoChar == '#'){ /* Ignora linha de comentario*/
                        	while(fgetc(arquivo) != NOVA_LINHA){}
                        }else if(proximoChar == NOVA_LINHA){
				linhasPuladas++;
			}
		}
		/*Lê o arquivo até o talo! */
			while(!feof(arquivo)){
				char charAtual = fgetc(arquivo);
				if(isdigit(charAtual)){
					/*Insere digito no array para formar o valor do pixel */
					for(int index = 0; index < TAMANHO_DIGITO_NUMERO; index++){
						if(numero[index] == -1){
							numero[index] = charAtual;
							break;
						}
					}
				}else if(isblank(charAtual) || charAtual == NOVA_LINHA){
					/*Contabiliza o pixel*/
					int valorPixel = atoi(numero);
					ocorrenciasPixels[valorPixel]++;
					/*Limpa array do valor do pixel*/
					for(int index = 0; index <= TAMANHO_DIGITO_NUMERO;index++){
						numero[index] = -1;
					
					}					
				}else if(charAtual != EOF){
					/*Caracter estranho*/
					printf("Caracter não esperado! Encontrado: %d :(\n", charAtual);
					return 1;
				}
			}		
		for(int index = 1; index <=MAIOR_VALOR_PIXEL;index++){
			if(ocorrenciasPixels[index] > 0){
				printf("O valor %d possui %d ocorrências\n", index,ocorrenciasPixels[index]);
			}
		}		
	}
	fclose(arquivo);
	printf("Acabou!\n");
}