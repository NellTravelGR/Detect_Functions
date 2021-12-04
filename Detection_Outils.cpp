/* -----------------------------------------------------*/
/* Fichier  : Detection_Outils.cpp                      */
/* Role     : Outils detection des objets               */
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

Mat creaMasqueHSV(Mat src)
{
    /*int hmin = 0, smin = 150, vmin = 163;
    int hmax = 179, smax = 252, vmax = 255;*/
    int hmin = 120, smin = 167, vmin = 63;           // Rouge Panneaux imprimees
    int hmax = 179, smax = 255, vmax = 143;

    Mat imgMasque, imgHSV;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));  //creation d'une matrice 3x3 de type morpholgie 

    //Creation de masque par filtrage HSV
    cvtColor(src, imgHSV, COLOR_BGR2HSV);
    //	huge saturatio value
	Scalar minTh(hmin, smin, vmin);
	Scalar maxTh(hmax, smax, vmax);
	//     limit (threshold), img dst
	inRange (imgHSV, minTh, maxTh, imgMasque); //application d'un masque et la sortie d'une imaga avec le filtre appliqué 
    
    //imshow("masque avant dilate + ero HSV", imgMasque);
    //dilate(imgMasque, imgMasque, kernel, Point(),3);
	//erode(imgMasque, imgMasque, kernel, Point(), 2);
    //imshow("masque apres dilate + ero HSV", imgMasque);
    return imgMasque;
}

Mat creaMasqueRVB(Mat src)
{
    //Creation de masque par filtrage RVB // 85
    //int iRmin = 0, iRmax = 255, iVmin = 0, iVmax = 30, iBmin = 5, iBmax = 30; // Rouge
    int iRmin = 73, iRmax = 191, iVmin = 0, iVmax = 40, iBmin = 0, iBmax = 50; // Rouge
    unsigned char Rmin, Rmax, Vmin, Vmax, Bmin, Bmax;
	int x,y;

    Mat imgMasque;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));  //creation d'une matrice 3x3 de type morpholgie 
	
    Rmin = (unsigned char)iRmin;
    Rmax = (unsigned char)iRmax;
    Vmin = (unsigned char)iVmin;
    Vmax = (unsigned char)iVmax;
    Bmin = (unsigned char)iBmin;
    Bmax = (unsigned char)iBmax;

	/* --- Creation de l'image Resultat 
    par balayage et comparaison des files et colones de l'image --- */
    imgMasque.create(src.rows, src.cols, CV_8U );
    for(y = 0; y < src.rows; y++)
    {
        for(x = 0; x < src.cols; x++)
        {
            if((src.at<cv::Vec3b>(y,x)[0] >= Bmin)
            && (src.at<cv::Vec3b>(y,x)[0] <= Bmax) 
            && (src.at<cv::Vec3b>(y,x)[1] >= Vmin)
            && (src.at<cv::Vec3b>(y,x)[1] <= Vmax)
            && (src.at<cv::Vec3b>(y,x)[2] >= Rmin) 
            && (src.at<cv::Vec3b>(y,x)[2] <= Rmax)) {
                imgMasque.at<uchar>(y,x) = 255;
            }else{
                imgMasque.at<uchar>(y,x) = 0;
            }
        }
    }
    //imshow("masque avant dilate + ero RVB, imgMasque);
    //dilate(imgMasque, imgMasque, kernel, Point(),3);
	//erode(imgMasque, imgMasque, kernel, Point(), 2);
    //imshow("masque apres dilate + ero RVB", imgMasque);
    return imgMasque;
}

int compareImage (Mat src, Mat imgComp) 
{
    Ptr<Feature2D> detector = xfeatures2d::SURF::create(); //detecteur des points equivalents 
    vector<KeyPoint> keypoints_objet, keypoints_comp; //stockage des points d'interets 
    Mat descripteur_Comp, descripteur_Objet;  //stockage des points d'interets 
    FlannBasedMatcher matcher; //detection des points d'interets 
    vector<DMatch> matches; //detection des points d'interets 
    vector<DMatch> good_matches; //detection des points d'interets qui ont le plus d'importance 

    detector->detect (imgComp, keypoints_comp); //premiere phase de detection des points d'interets de l'image de comparaison 
    detector->detect (src, keypoints_objet); //premiere phase de detection des region d'interets de l'image de comparaison 
    detector->compute(imgComp, keypoints_comp, descripteur_Comp); //deuxieme phase creation d'un descripteur des points d'interets 
    detector->compute(src, keypoints_objet, descripteur_Objet);//deuxieme phase creation d'un descripteur des regions d'interets

    matcher.match(descripteur_Comp, descripteur_Objet, matches); //comparaison des points d'interets avec les regions d'interets
    double max_dist = 0, min_dist = 100;                         // ordre de gradeur de distance en pixels
    //-- Calcul de distance  min et max entre les keypoints
    for (int i = 0; i < descripteur_Comp.rows; i++) // balyage des points d'interets trouvés par le match
    {
        //fprintf(stderr, "--matches : %f \n", matches[i]);

        double dist = matches[i].distance; // recuperation des distances entre chaques points par rapport à un point
        //fprintf(stderr, "--matches dist : %f \n", dist);
        if (dist < min_dist) //comparaison de distance en pixel entre deux points
        {
            min_dist = dist; // mise à jour de distance la plus courte
        }
        if (dist > max_dist)
        {
            max_dist = dist; //mise à jour de la distance la longue
        }
    }
    for (int i = 0; i < descripteur_Objet.rows; i++) //balaygae de l'image pour déterminer les élements imporants
    {
        if (matches[i].distance < (2 * max_dist)) //un facteur 2 permet un gain de perfomance sur la qualité de la capture des points
        {
            good_matches.push_back(matches[i]); //recuperation des bons points trvoués
            //fprintf(stderr, "--Element Ok : %f \n", good_matches[i].distance);
        }
    }
    fprintf(stderr, "bon points : %ld \n", good_matches.size()); //affichage des points (debug)
    int count_matches = 0;
    //int indexElemenTrouve = 0;
    int detect_etat = 0;
    // Verification du nombre des points trouvés
    for (int i = 0; i < good_matches.size(); i++)
    {
        count_matches++;
        if (count_matches >= 30) //object trouvé
        {
            //indexElemenTrouve = i;
            detect_etat = 1;
            //fprintf(stderr, "--Signal Trouvé\n");
        }
        else
        {
            detect_etat = 0;
        }
    }
    /*Mat img_matches;
    drawMatches(imgComp, keypoints_comp, src, keypoints_objet,
    good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS); //debug
    imshow( "Detection des Objets par comparaison ", img_matches );*/
    
    return detect_etat;
}

Rect detectContours (Mat imgMasque, int aireMin, Rect *dStop, Rect *dPass)
{
    int maxAire = 0;
    int indexContourStop = 0, indexContourCPassage = 0;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;  

    findContours(imgMasque, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // recherche de contours avec la methode  approximation et de chainage simple 
    
    vector<vector<Point>> conPoly(contours.size());  //Vecteur des contours
	vector<Rect> boundRect(contours.size());        // Vecteur pour boites des contours (Rect = Rectangles)

    	for (int i = 0; i< contours.size(); i++) //balayage des coutours 
	    {
            int aire = contourArea(contours[i]); // recupairation de l'air du contour 
            float peri = arcLength(contours[i], true); //recuperation du perimetre 
            //fprintf(stderr, "L'aire du contour est : %d \n", aire);
           
            if (aire > aireMin) 
            {
                //cout << "Longueur perimetre : " << peri << endl;
                	
                approxPolyDP(contours[i],conPoly[i], 0.0255 * peri, true);   //appro de poly à partir d'un coeff
              
                boundRect[i] = boundingRect(conPoly[i]); //declaration des boite englobande pour les contours trouvés 
                
                if (aire > maxAire && conPoly[i].size() == 8 ) // de test de recherche de 8 points 
                {                       
                    cout << "Longueur perimetre : " << peri << endl;
                    fprintf(stderr, "L'aire du contour est : %d \n", aire);
                    //pointsContour = {conPoly[i][0],conPoly[i][1],conPoly[i][2],conPoly[i][3],conPoly[i][4],conPoly[i][5], conPoly[i][6], conPoly[i][7]};
                    maxAire = aire; // mise à jour des contours,
                    indexContourStop = i;
                    /*rectangle(imgOri,boundRect[i].tl(),boundRect[i].br(), Scalar(255,0,255), 2);
                    putText(imgOri, "Panneau Stop", {boundRect[i].x, boundRect[i].y}, FONT_ITALIC, 1, Scalar(0,255,0), 2);
                   */
                }
            }
	    }
    return boundRect[indexContourStop];
}