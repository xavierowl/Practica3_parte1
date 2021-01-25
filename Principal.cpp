
// Librerías que contienen funciones estándar de C++
#include <iostream>
#include <cstdlib>


// Librería que contiene funciones matemáticas
#include <cmath>

// Librerías de OpenCV

// Se pueden cargar todas las Librerías incluyendo
//#include <opencv2/opencv.hpp>

// Contiene las definiciones fundamentales de las matrices e imágenes 
#include <opencv2/core/core.hpp> 
// Procesamiento de imágenes
#include <opencv2/imgproc/imgproc.hpp>
// Códecs de imágenes
#include <opencv2/imgcodecs/imgcodecs.hpp>
// Manejo de ventanas y eventos de ratón, teclado, etc.
#include <opencv2/highgui/highgui.hpp>
// Lectura de video
#include <opencv2/video/video.hpp>
// Escritura de video
#include <opencv2/videoio/videoio.hpp>

using namespace std;
using namespace cv; // Espacio de nombres de OpenCV (Contiene funciones y definiciones de varios elementos de OpenCV)

Mat imagen;

Mat teemo;
Mat ruido_sal;
Mat ruido_pimienta;
Mat filtro_mediana;
Mat filtro_gaussiano;
Mat filtro_blur;

int ruido_sal_k = 0;
int ruido_pimienta_k = 0;
int kernel_mediana = 0;
int kernel_gaussiano = 0;
int kernel_blur = 0;

//Función para agregar ruidos
void agregar_ruidos(){
    ruido_sal = imagen.clone();
    
    int pixeles_ruido = (int)((imagen.rows * imagen.cols * ruido_sal_k) / 100);
    if(ruido_sal_k == 100){
        for(int i = 0; i<imagen.rows; i++){
            for(int j = 0; j<imagen.cols; j++){
                ruido_sal.at<uchar>(i,j)=255;
            }
        }
    }
    else{
        int fila = 0;
        int col = 0;
        srand(time(0));
    
        for(int i = 0; i<pixeles_ruido; i++){
            fila = rand() % imagen.rows;
            col = rand() % imagen.cols;
            ruido_sal.at<uchar>(fila,col)=255;
        }
    
    }
    
    imshow("Ruido de sal", ruido_sal);
    
    ruido_pimienta = imagen.clone();
    
    pixeles_ruido = (int)((imagen.rows * imagen.cols * ruido_pimienta_k) / 100);
    if(ruido_pimienta_k == 100){
        for(int i = 0; i<imagen.rows; i++){
            for(int j = 0; j<imagen.cols; j++){
                ruido_pimienta.at<uchar>(i,j)=0;
            }
        }
    }
    else{
        int fila = 0;
        int col = 0;
        srand(time(0));
    
        for(int i = 0; i<pixeles_ruido; i++){
            fila = rand() % imagen.rows;
            col = rand() % imagen.cols;
            ruido_pimienta.at<uchar>(fila,col)=0;
        }
    
    }
    
    imshow("Ruido de pimienta", ruido_pimienta);
    
    if(kernel_mediana%2 != 0){
        medianBlur(imagen, filtro_mediana, kernel_mediana);
    }
    
    imshow("Filtro mediana", filtro_mediana);
    
    if(kernel_blur%2 != 0){
        blur(imagen, filtro_blur, Size(kernel_blur, kernel_blur));
    }
    
    imshow("Filtro Blur", filtro_blur);
    
    if(kernel_gaussiano%2 != 0){
        GaussianBlur(imagen, filtro_gaussiano, Size(kernel_gaussiano, kernel_gaussiano), 2, 2);
    }
    
    imshow("Filtro Gaussiano", filtro_gaussiano);
}

//Función para el trackbar del ruido de sal
void functionTrackBarSal(int v, void *p){
    agregar_ruidos();
}

//Función para el trackbar del ruido de pimienta
void functionTrackBarPimienta(int v, void *p){
    agregar_ruidos();
}

//Función para el trackbar del ruido de pimienta
void functionTrackBarMediana(int v, void *p){
    agregar_ruidos();
}

void functionTrackBarBlur(int v, void *p){
    agregar_ruidos();
}

void functionTrackBarGaussiano(int v, void *p){
    agregar_ruidos();
}

void agregar_ruido_y_filtros(Mat imagen){
    namedWindow("Video", WINDOW_AUTOSIZE);
    imshow("Video", imagen);
    
    ruido_sal = imagen.clone();
    ruido_pimienta = imagen.clone();
    filtro_mediana = imagen.clone();
    filtro_gaussiano = imagen.clone();
    filtro_blur = imagen.clone();
    
    namedWindow("Original", WINDOW_AUTOSIZE);
    moveWindow("Original", 695, 85);
    namedWindow("Ruido de sal", WINDOW_AUTOSIZE);
    moveWindow("Ruido de sal", 10, 30);
    namedWindow("Ruido de pimienta", WINDOW_AUTOSIZE);
    moveWindow("Ruido de pimienta",1320, 30);
    namedWindow("Filtro mediana", WINDOW_AUTOSIZE);
    moveWindow("Filtro mediana",10, 500);
    namedWindow("Filtro Gaussiano");
    moveWindow("Filtro Gaussiano", 695, 500);
    namedWindow("Filtro Blur");
    moveWindow("Filtro Blur", 1320, 500);

    createTrackbar("Ruido","Ruido de sal", &ruido_sal_k, 100, functionTrackBarSal);
    createTrackbar("Ruido","Ruido de pimienta", &ruido_pimienta_k, 100, functionTrackBarPimienta);
    createTrackbar("Kernel","Filtro mediana", &kernel_mediana, 100, functionTrackBarMediana);
    createTrackbar("Kernel","Filtro Gaussiano", &kernel_gaussiano, 100, functionTrackBarGaussiano);
    createTrackbar("Kernel","Filtro Blur", &kernel_blur, 100, functionTrackBarBlur);
    
    imshow("Original", imagen);
    imshow("Ruido de sal", ruido_sal);
    imshow("Ruido de pimienta", ruido_pimienta);
    imshow("Filtro mediana", filtro_mediana);
    imshow("Filtro Gaussiano", filtro_gaussiano);
    imshow("Filtro Blur", filtro_blur);
}

int main(int argc, char *argv[]){
    VideoCapture video("video");
    
    if(video.isOpened()){
        while(3==3){
            video >> imagen;
            resize(imagen, imagen, Size(), 0.95,0.95);            
                        
            cvtColor(imagen, imagen, COLOR_BGR2GRAY);
            
            agregar_ruido_y_filtros(imagen);
            
            agregar_ruidos();
                    
            if(waitKey(23) > 0)
                break;
            
        }
        destroyAllWindows();
    }
    else{
        
        cout<<"No se ha podido capturar el video";
    }
    
    //Detección de bordes
    Mat sobel;
    Mat sobel_filtro_mediana;
    Mat gX;
    Mat gY;
    
    Sobel(imagen, gX, CV_16S, 1, 0, 3);
    convertScaleAbs(gX, gX);
    Sobel(imagen, gY, CV_16S, 0, 1, 3);
    convertScaleAbs(gY, gY);
    addWeighted(gX, 0.5, gY, 0.5, 0, sobel);
    
    //namedWindow("Sobel gX", WINDOW_AUTOSIZE);
    //moveWindow("Sobel gX",10, 150);
    //namedWindow("Sobel gY", WINDOW_AUTOSIZE);
    //moveWindow("Sobel gY", 695, 150);
    //imshow("Sobel gX", gX);
    //imshow("Sobel gY", gY);
    
    namedWindow("Sobel Bordes", WINDOW_AUTOSIZE);
    moveWindow("Sobel Bordes", 350, 100);
    imshow("Sobel Bordes", sobel);
    
    Sobel(filtro_mediana, gX, CV_16S, 1, 0, 3);
    convertScaleAbs(gX, gX);
    Sobel(filtro_mediana, gY, CV_16S, 0, 1, 3);
    convertScaleAbs(gY, gY);
    addWeighted(gX, 0.5, gY, 0.5, 0, sobel_filtro_mediana);
    
    //namedWindow("Sobel gX (Filtro mediana)", WINDOW_AUTOSIZE);
    //moveWindow("Sobel gX (Filtro mediana)",10, 550);
    //namedWindow("Sobel gY (Filtro mediana)", WINDOW_AUTOSIZE);
    //moveWindow("Sobel gY (Filtro mediana)", 695, 550);
    //imshow("Sobel gX (Filtro mediana)", gX);
    //imshow("Sobel gY (Filtro mediana)", gY);
    namedWindow("Sobel Bordes (Filtro mediana)", WINDOW_AUTOSIZE);
    moveWindow("Sobel Bordes (Filtro mediana)", 1000, 100);
    imshow("Sobel Bordes (Filtro mediana)", sobel_filtro_mediana);
    
    Mat laplace;
    Mat laplace_filtro_gaussiano;
    
    Laplacian(filtro_gaussiano, laplace_filtro_gaussiano, CV_16S, 3);
    convertScaleAbs(laplace_filtro_gaussiano, laplace_filtro_gaussiano);
    
    Laplacian(imagen, laplace, CV_16S, 3);
    convertScaleAbs(laplace, laplace);
    
    namedWindow("Laplace", WINDOW_AUTOSIZE);
    moveWindow("Laplace",350, 550);
    imshow("Laplace", laplace);
    
    namedWindow("Laplace (Filtro Gaussiano)", WINDOW_AUTOSIZE);
    moveWindow("Laplace (Filtro Gaussiano)",1000, 550);
    imshow("Laplace (Filtro Gaussiano)", laplace_filtro_gaussiano);
    
    waitKey(0);
    destroyAllWindows();
    
    return 0;
}


