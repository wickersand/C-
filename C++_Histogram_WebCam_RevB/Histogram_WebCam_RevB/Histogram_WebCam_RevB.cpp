/* Projeto M2 - Calculo de Histograma utilizando WebCam
Prof. Kesede Julio - CGPI
Membros:
Everton Luiz Peres Nascimento
Thiago Americo Moreton
William Ferreira

*/

/* O Histograma são desenhados em 256 BINS, para facilitar a visualização
Definição de 'BIN'
Cada BIN corresponde a um nível de intensidade, onde 0 é totalmente preto e 255, totalmente branco.
Divide toda a gama de valores para uma série de intervalos e depois conta o número de valores que diminui em 
cada intervalo. Os 'bins' são normalmente especificadas como intervalos consecutivos, sem sobreposição de uma variável.
*/

/*Adicionando os includes*/
#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2\opencv.hpp>
#include <opencv\cv.h>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv\highgui.h>
#include <opencv2\imgproc\imgproc.hpp>
#include "compat.hpp"
#include <windows.system.threading.h>


int main(int argc, char *argv[])
{
	/*Váriavel para alocar a entrada do teclado*/
	char d;

	/*Inicializa os flags como falso. Todos os flags serão utilizamos pelo teclado*/
	bool Q = false;
	bool R = false;
	bool G = false;
	bool B = false;

	/*Setando a WebCam. O valor 0 (zero) é o valor padrão para WebCam Interna (notevbook)*/
	CvCapture* capture = cvCaptureFromCAM(0);

	/*Alocando memoria para as imagens geradas*/
	IplImage *src_img;
	IplImage *histogram;
	IplImage *disp_img;
	IplImage *gray_img;
	IplImage *red_img, *red_histogram;
	IplImage *green_img, *green_histogram;
	IplImage *blue_img, *blue_histogram;

	/*Inicializando o Histograma*/
	int hist_size = 256;
	float range[] = { 0, 256 };
	float *ranges[] = { range };

	
	/*A função cvCreateHist é utilizada para criar o Histograma, no qual serão passados cinco argumentos:
	1) Define quantas dimensões deverá ter o Histograma;
	
	2) Define o tamanho das dimensões. Ao utilizar um Histograma com um dimensão muito grande, deverá passar um 
	vetor de tamanho para cada dimensão. Neste exemplo, iremos utilizar um vetor e passar somente um único valor. Usando o tamanho da dimensão de 256, 
	devido a profundidade da imagem de 8 bits que irá ser utilizada.
	
	3) Informa ao OpenCV como guardar os dados do histograma. Utilizamos o parametro CV_HIST_ARRAY, utilizado 
	para efetuar o calculo em gráfico 1D.

	4) Especifica o range dos valores de cada dimensão. Cada range é alocado dentro de um vetor de dois valores, minimo
	e máximo.

	5) Define se os intervalos estão divididos de forma uniforme no grafico. Esse argumento irá organizar o grafico 
	para que cada coluna seja igual a anterior.
	*/

	CvHistogram* hist_red = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	CvHistogram* hist_blue = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	CvHistogram* hist_green = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	double max_value = 0;
	double max_value_red = 0;
	double max_value_green = 0;
	double max_value_blue = 0;
	double find_max = 0;

	/*Criando as janelas do GUI*/
	/*WebCam - Janela de visualização da WebCam*/
	cvNamedWindow("WebCam", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("WebCam", 5, 5);

	/*Histograma_Grafico - Janela de visualização do grafico do Histograma*/
	cvNamedWindow("Histograma_Grafico", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Histograma_Grafico", 435, 5);

	// Print instructions for keyboard input
	/*Legenda dos comandos para visualização do grafico*/
	printf("Utilize a legenda abaixo para habilitar/desabilitar os graficos do Histograma\n\n");
	printf("Pressione R para Ligar/Desligar o canal RED (Vermelho): 'r'\n");
	printf("Pressione G para Ligar/Desligar o canal GREEN (Verde): 'g'\n");
	printf("Pressione B para Ligar/Desligar o canal BLUE (Azul): 'b'\n");
	printf("Pressione Q para fechar o grafico.'\n");

	/*Executa os paramentros infinitamente, mantendo a WebCam capturando online as imagens*/
	while (1)
	{
		/*Seta todos os valores MAX para Zero (0)*/
		max_value = 0; max_value_red = 0; max_value_green = 0; max_value_blue = 0;

		/*Carrega o template para mostrar os spikes do grafico.*/
		histogram = cvLoadImage("F:\\Projects\\Histogram_WebCam_RevB\\Histogram_WebCam_RevB\\histogram_scale.png");

		/*Inicializa as três imagens que irá mostrar cada histograma (RGB)*/
		red_histogram = cvCreateImage(cvGetSize(histogram), IPL_DEPTH_8U, 3);
		green_histogram = cvCreateImage(cvGetSize(histogram), IPL_DEPTH_8U, 3);
		blue_histogram = cvCreateImage(cvGetSize(histogram), IPL_DEPTH_8U, 3);

		/*Seta o frame principal, utilizando fila e captura e ajusta o LCD.*/
		src_img = cvQueryFrame(capture);
		disp_img = cvCreateImage(cvSize((src_img->width) / 1.6, (src_img->height) / 1.6), IPL_DEPTH_8U, 3);
		cvResize(src_img, disp_img, CV_INTER_LINEAR);

		/*Cria três canais do tipo imagem para guardar os dados recebidos e divide na forma dos canais RGB
		  Cada canal envia uma imagem do mesmo tamanho (altura e largura). A profundidade da imagem é de 8 bits
		*/
		blue_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
		green_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
		red_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);

		/* Essa função separa a fonte de dados em três canais diferentes e aloca os dados na imagem criada*/
		cvCvtPixToPlane(src_img, blue_img, green_img, red_img, 0);

		/*Calcula o histograma para cada canal*/
		cvCalcHist(&red_img, hist_red, 0, NULL);
		cvCalcHist(&blue_img, hist_blue, 0, NULL);
		cvCalcHist(&green_img, hist_green, 0, NULL);

		/*Efetua uma busca dos valores maximos do histograma e salva na memoria*/
		for (int i = 0; i < hist_size; i++)
		{
			find_max = cvQueryHistValue_1D(hist_red, i);
			if (find_max > max_value_red)
			{
				max_value_red = find_max;
			}
		}
		for (int i = 0; i < hist_size; i++)
		{
			find_max = cvQueryHistValue_1D(hist_green, i);
			if (find_max > max_value_green)
			{
				max_value_green = find_max;
			}
		}
		for (int i = 0; i < hist_size; i++)
		{
			find_max = cvQueryHistValue_1D(hist_blue, i);
			if (find_max > max_value_blue)
			{
		}
		
		/*Procura o maior valor do histograma*/
		max_value = max(max(max_value_red, max_value_green), max_value_blue);

				max_value_blue = find_max;
			}
		
		/* Desenha o histograma de cada canal (RGB), setado o valor maximo de pixel em 438 (melhor visualizacao)*/
		if (R)
		{
			cvScale(hist_red->bins, hist_red->bins, 438 / max_value);
			for (int i = 0; i < hist_size; i++)
			{
				cvRectangle(red_histogram, cvPoint(i * 3 + 15, 448), cvPoint(i * 3 + 16, 448 - cvRound(cvQueryHistValue_1D(hist_red, i))), cvScalar(0x00, 0x00, 0xff, 0), -1);
			}
			cvAdd(histogram, red_histogram, histogram, 0);
		}
		if (G)
		{
			cvScale(hist_green->bins, hist_green->bins, 438 / max_value);
			for (int i = 0; i < hist_size; i++)
			{
				cvRectangle(green_histogram, cvPoint(i * 3 + 15, 448), cvPoint(i * 3 + 16, 448 - cvRound(cvQueryHistValue_1D(hist_green, i))), cvScalar(0x00, 0xff, 0x00, 0), -1);
			}
			cvAdd(histogram, green_histogram, histogram, 0);
		}
		if (B)
		{
			cvScale(hist_blue->bins, hist_blue->bins, 438 / max_value);
			for (int i = 0; i < hist_size; i++)
			{
				cvRectangle(blue_histogram, cvPoint(i * 3 + 15, 448), cvPoint(i * 3 + 16, 448 - cvRound(cvQueryHistValue_1D(hist_blue, i))), cvScalar(0xff, 0x00, 0x00, 0), -1);
			}
			cvAdd(histogram, blue_histogram, histogram, 0);
		}

		/* Mostra na tela as janelas*/
		cvShowImage("WebCam", disp_img);
		cvShowImage("Histograma_Grafico", histogram);

		/*Seta os flags*/
		d = cvWaitKey(15);
		
		/*Tratamento do input do teclado*/
		switch (d)
		{
		case 'r':	R = R ^ 1;	break;
		case 'g':	G = G ^ 1;	break;
		case 'b':	B = B ^ 1;	break;
		case 'q':	Q = true;	break;
		default:	break;
		}
		if (Q)break;	/*Fecha a janela*/

		/* Libera todas as imagens da memoria*/
		cvReleaseImage(&disp_img);
		cvReleaseImage(&red_img);
		cvReleaseImage(&green_img);
		cvReleaseImage(&blue_img);
		cvReleaseImage(&red_histogram);
		cvReleaseImage(&green_histogram);
		cvReleaseImage(&blue_histogram);
		cvReleaseImage(&histogram);

	}
	return 0;
}

