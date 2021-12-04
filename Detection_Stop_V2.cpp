/* -----------------------------------------------------*/
/* Fichier  : Detection_Stop_V2.cpp                        */
/* Role     : Detection par traitement d'image          */
/* @Auteur  : Nelson Grullon Rosario                    */
/*          : Projet detecttion Panneux Signalisation   */
/* -----------------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <string>
#include "opencv2/core/core.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include "opencv2/xfeatures2d.hpp"


using namespace cv;
using namespace std;
#include "Detection_Outils.h" 


#define LARGEUR 640
#define HAUTEUR 480
#define aireMin 3000

string pathComp = "/home/ness/Downloads/images_test/Stop_signal_base.jpg";
Mat imgOri, maskHSV, maskRVB, imgObjet, imgComp;

VideoCapture camera(2);

int main () 
{
    camera.set(CAP_PROP_FRAME_WIDTH, LARGEUR);
    camera.set(CAP_PROP_FRAME_HEIGHT, HAUTEUR);
	//camera.read (imgOri);

    imgComp = imread(pathComp);
    //Reduction de la taille d'image original
    //resize(imgOri,imgOri, Size(), 0.5, 0.5);

    while(camera.isOpened())
    {
        camera.read(imgOri);	

        // Pre-Traitement de l'image
        maskHSV = creaMasqueHSV(imgOri);
        maskRVB = creaMasqueRVB(imgOri);
        // Creation d'une masque a partir des masques RVB et HSV
        Mat or_result;
        bitwise_or(maskRVB, maskHSV, or_result);
        imshow("masque avant dilate + ero", or_result); //affichage de l'image avec un filtrage RVB  et HSV
        
        Rect boitEnglobante;
        if (!or_result.empty())
        {
            //boitEnglobante = detectContours(or_result, aireMin);
            //cout << "points de boite englobante : " << boitEnglobante << endl;
        }

        if ((boitEnglobante.x != 0) && (boitEnglobante.y != 0))
        {
            //Rect mesROI(boitEnglobante.x, boitEnglobante.y, boitEnglobante.width, boitEnglobante.height); //creation d'un rectangle pour le stockage des regions d'interets
            //imgObjet = imgOri(mesROI); 
            //rectangle(imgOri,boitEnglobante.tl(),boitEnglobante.br(), Scalar(255,0,255), 2);                                                                   //découpe des regions d'interets
            //imshow("decoupe ", imgObjet);
            //int detection = 0;
            //detection = compareImage(imgObjet, imgComp); //affichage des regions d'interets (debug)*/
            
            /*if (detection == 1)
            {
                rectangle(imgOri,boitEnglobante.tl(),boitEnglobante.br(), Scalar(255,0,255), 2);                                                                   //découpe des regions d'interets
                putText(imgOri, "Panneau Stop", {boitEnglobante.x, boitEnglobante.y}, FONT_ITALIC, 1, Scalar(0,255,0), 2);

            } */
        }

        namedWindow( "Visualisation Image Originale", WINDOW_AUTOSIZE );
        imshow("Visualisation Image Originale", imgOri);

	    if(waitKey(1) == 'q')
        {
            break;
        }
        
    }

	return 0;

}