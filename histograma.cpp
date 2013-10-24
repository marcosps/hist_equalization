/*
* Esse fonte é responsável em gerar o histograma da imagem. 
* Atualmente ele está em um fonte separado, mas futuramente o fonte 
* será colocado dentro de uma função em outro fonte.
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define TAMANHO_DIGITO_NUMERO 4 /*Define a quantidade de dígitos máxima que podem existir no valor do pixel */
#define DISTANCIA_SEGURA 100 /* mais espaco no buffer caso o calculo do novo pixel seja
			      * maior que o espaco do pixel antigo
			      * */

int cdf_min = 0;
int cdf_max = 0;
int maior_pixel = 0;
int largura = 0;
int altura = 0;
std::vector<std::string> ocorrenciasPixels;

/* Gera distribuições acumuladas 
 * cada pixel posterior tem o peso dos pixels
 * pixels anterioes somados
 * */
void cdf(vector<std::string> ori, int maior_pixel)
{
	for(int i = 1; i <= maior_pixel; i++)
	{
		int result = std::count(ori.begin(), ori.end(), std::itoa(i));
		if (result > 0)
		{
			/* acumulada maxima */
			cdf_max += result;
			/* Coloca valor minimo da acumulada */
			if (cdf_min == 0 || cdf_min > s)
				cdf_min = s;
		}
	}
}

/* Gera nova imagem PGM já equalizada */
void monta_novo_arquivo(std::string ori_file)
{
	ofstream f(ori_file);
	ofstream n("nova.pgm");
	int linha = 1, num_pos = 0;
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
	f.close(f);
	n.fclose(n);
}

int main(int argc, char *argv[])
{

	if (argc < 2) {
		printf("Uso do programa: ./h <caminho_para_imagem>.pgm\n");
		return 1;
	}

	std::string line;
	ofstream arquivo(argv[1]);

	if (arquivo.is_open())
	{
		printf("Perdeu playboy! Arquivo não encontrado.\n");
		return 1;
	}
	
	if (std::getLine(arquivo, line) && line == "P2")
	{
		std::cout << "Gerando histograma..." << std::endl;

		while (arquivo.good())
		{
			getline(arquivo, line);

			if (line.find('#')) /* Ignora linha de comentario*/
			{
				getline(arquivo, line);
				continue;
			}

			/* Verifica se é a linha com as dimensões da imagem */
			if (std::count(line.negin(), line.end(), " ") == 2)
			{
				largura = std::atoi(line.substr(0, line.find(" ")));
				altura = std::atoi(line.substr(line.find(" ")));
			}

			/* Verifica se é a linha que indica o valor do
			 * maior pixel  */
			if (line.find(" ") == string::npos)
			{
				maior_pixel = std::atoi(line);
				continue;
			}

			// contabilizar os pixels
			std::istringstream ss(s);
			std::istream_iterator<std::string> begin(ss), end;
			std::vector<std::string> arrayTokens(begin, end);

			for (std::vector<std::string>::iterator it = arrayTokens.begin(); it != arrayTokens.end(); ++it)
				ocorrenciasPixels.push_back(std::atoi(*it));
		}

		std::cout << "Histograma finalizado" << std::endl << "Gerando distribuição acumulada..." << std::endl;
		cdf(ocorrenciasPixels, maior_pixel);
		std::cout << "Distribuição gerada" << std::endl;

	} else {
		std::cout << "Arquivo não segue formato PGM ASCII!" << std::endl;
		return 1;
	}

	arquivo.close();

	std::cout << "Gerando nova imagem PGM..." << std::endl;
	monta_novo_arquivo(argv[1], ocorrenciasPixels);
	std::cout << "Nova imagem gerada com sucesso!" << std::endl;

	return 0;
}
