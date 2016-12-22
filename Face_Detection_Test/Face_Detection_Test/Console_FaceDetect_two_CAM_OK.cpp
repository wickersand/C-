/* Projeto M2 - Face detecting com WebCam
Prof. Kesede Julio - CGPI
Membros:
Everton Luiz Peres Nascimento
Thiago Americo Moreton
William Ferreira

*/


#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <windows.system.threading.h>


using namespace std;
using namespace cv;

/*Declaracao das Funcoes - Header*/
void detectAndDisplayCAM0(Mat frame);
void detectAndDisplayCAM1(Mat frame2);

/*Variaveis Globais*/
string RootCascade = "F:\\Projects\\Haardcascades_Files\\";
string Extension = ".xml";
String face_cascade_name = RootCascade + "haarcascade_frontalface_alt2" + Extension;
String eyes_cascade_name = RootCascade + "haarcascade_eye_tree_eyeglasses" + Extension;
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_00 = "CAM0 'Notebook' - Face Detection by Wcks";
string window_01 = "CAM1 'USB' - Face Detection by Wcks";



RNG rng(12345); //???

/*Inicio da Main*/

int main(int argc, const char** argv) {

	CvCapture *capture = 0;
	CvCapture *capture2 = 0;
	
	Mat frame;
	Mat frame2;
	
	/*--1.Carregando os cascades*/
	if (!face_cascade.load(face_cascade_name)) {
		printf("--(!) Erro ao detectar a face...\n");
		return -1;
	}

	if (!eyes_cascade.load(eyes_cascade_name)) {
		printf("--(!) Erro ao detectar os olhos...\n");
		return -1;
	}

	/*--2. Lendo video Stream*/
	
	capture = cvCaptureFromCAM(0);
	capture2 = cvCaptureFromCAM(1);

	if (capture && capture2) {
	
			while (true) {

			/*Setando tamanho da tela de captura WebCAM - Notebook*/
			capture = cvCreateCameraCapture(0);
			cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 320); /*Define altura do window frame - Default 320*/
			cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 240); /*Define largura do window frame - Default 240*/

			/*Setando tamanho da tela de captura WebCAM - USB */
			capture2 = cvCreateCameraCapture(1);
			cvSetCaptureProperty(capture2, CV_CAP_PROP_FRAME_WIDTH, 320); /*Define altura do window frame - Default 320*/
			cvSetCaptureProperty(capture2, CV_CAP_PROP_FRAME_HEIGHT, 240); /*Define largura do window frame - Default 240*/

			frame = cvQueryFrame(capture);
			frame2 = cvQueryFrame(capture2);

			if (!frame.empty() && !frame2.empty()) {
				
				detectAndDisplayCAM0(frame); // Executa funcao na CAM0 (Notebook)
				detectAndDisplayCAM1(frame2); // Execute funcao na CAM1 (USB) 
			}
			
			else {
			
				printf("**(!) Não foi possível capturar o frame ** Travou!");
				break;
			}

			int c = waitKey(10);
			if ((char)c == 'q' || (char)c == 'Q') {
				printf("Pressione q ou Q para fechar o programa...");
				cvReleaseCapture(&capture);
				cvReleaseCapture(&capture2);
				break;
			}
		}
	}
	
	return 0;
}


/*Funcao detectAndDisplayCAM0*/
void detectAndDisplayCAM0(Mat frame) {

	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	/*Detectando a Face*/
	/*Para melhorar a precisao adicionar os valores: 1.15, 5, 0, CV_Size(30,30)*/
	face_cascade.detectMultiScale(frame_gray, faces, 1.15, 5, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++) {
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(0, 255, 0), 4, 8, 0);
		
		
		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect>eyes;

		/*Detecta olhos em cada face*/
		/*Para melhorar a precisao adicionar os valores: 1.15, 5, 0, CV_Size(30,30)*/
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.15, 5, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++) {
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
			//putText(frame, "Face Detecting - Wcks", cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(250, 200, 250, 1), CV_AA);
		}
	}

	/*Mostra Imagem capturada CAM0*/
	
	imshow(window_00, frame);
}

/*Funcao detectAndDisplayCAM1*/
void detectAndDisplayCAM1(Mat frame2) {

	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame2, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	/*Detectando a Face*/
	/*Para melhorar a precisao adicionar os valores: 1.15, 5, 0, CV_Size(30,30)*/
	face_cascade.detectMultiScale(frame_gray, faces, 1.15, 5, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++) {
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame2, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(0, 255, 0), 4, 8, 0);


		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect>eyes;

		/*Detecta olhos em cada face*/
		/*Para melhorar a precisao adicionar os valores: 1.15, 5, 0, CV_Size(30,30)*/
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.15, 5, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++) {
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame2, center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
	}

	/*Mostra Imagem capturada CAM1*/
	imshow(window_01, frame2);
}