#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <windows.system.threading.h>

using namespace std;
using namespace cv;

/*Declaracao das Funcoes - Header*/
void detectAndDisplay(Mat frame);

/*Variaveis Globais*/
string RootCascade = "F:\\Projects\\Haardcascades_Files\\";
string Extension = ".xml";
String face_cascade_name = RootCascade + "haarcascade_frontalface_alt2" + Extension;
String eyes_cascade_name = RootCascade + "haarcascade_eye_tree_eyeglasses" + Extension;
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string windows_name = "Capture - Face Detection by Wcks";
RNG rng(12345);

/*Inicio da Main*/

int main(int argc, const char** argv) {

	CvCapture* capture;
	Mat frame;

	/*--1.Carregando os cascades*/
	if (!face_cascade.load(face_cascade_name)) {
		printf("--(!) Face Error Loading...\n");
		return -1;
	}

	if (!eyes_cascade.load(eyes_cascade_name)) {
		printf("--(!) Eyes Error Loading...\n");
		return -1;
	}

	/*--2. Lendo video Stream*/
	capture = cvCaptureFromCAM(0);
	if (capture) {

		while (true) {
			frame = cvQueryFrame(capture);

			if (!frame.empty()) {
				detectAndDisplay(frame);
			} 
			else {
				printf("--(!) No captured frame -- Break!");
				break;
			}

			int c = waitKey(10);
			if ((char)c == 'q' || (char)c == 'Q') {
				printf("Pressione q ou Q para fechar o programa...");
				break;
			}
		}
	}
	
	return 0;
}


/*Funcao detectAndDisplay*/
void detectAndDisplay(Mat frame) {

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
		}
	}

	/*Mostra Imagem capturada*/
	imshow(windows_name, frame);
		
}