/* -----------------------------------------------------*/
/* Fichier  : Detection_FeuRouge.cpp                        */
/* Role     : Detection par traitement d'image          */
/* @Auteur  : Nelson Grullon Rosario                    */
/*          : Projet detecttion Panneux Signalisation   */
/* -----------------------------------------------------*/

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

string imgPath = "/home/ness/Downloads/images_test/feu_vert_5.jpg";
Mat imgOri, imgGray, imgCanny, imgTraite, imgGauss, imgDilat, imgEro, imgCoupe, imgReduced, imgLarged, imgFeu;
vector<Point> InitPoints;

vector<vector<Point>> contours;
vector<Vec4i> hierarchy;  
vector<Point> GrandP; 		// Vector de donnees pour le plus grand des points du contour

//variables pour Traitement RVB
Mat imgCree; 

Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));

int main ()
{
    imgOri = imread(imgPath);

    //int iRmin = 200, iRmax = 255, iVmin = 0, iVmax = 80, iBmin = 0, iBmax = 255; // Rouge
    //int iRmin = 85, iRmax = 255, iVmin = 0, iVmax = 30, iBmin = 6, iBmax = 255; // Rouge signaux
    int iRmin = 113, iRmax = 255, iVmin = 0, iVmax = 52, iBmin = 10, iBmax = 47; // Rouge Feu
    
    unsigned char Rmin, Rmax, Vmin, Vmax, Bmin, Bmax;
	int x,y;

	Rmin = (unsigned char)iRmin;
    Rmax = (unsigned char)iRmax;
    Vmin = (unsigned char)iVmin;
    Vmax = (unsigned char)iVmax;
    Bmin = (unsigned char)iBmin;
    Bmax = (unsigned char)iBmax;

	/* --- Creation de l'image Resultat --- */
    imgCree.create(imgOri.rows, imgOri.cols, CV_8U );
    for(y = 0; y < imgOri.rows; y++)
    {
        for(x = 0; x < imgOri.cols; x++)
        {
            if((imgOri.at<cv::Vec3b>(y,x)[0] >= Bmin) // 0)       //Bmin)
            && (imgOri.at<cv::Vec3b>(y,x)[0] <= Bmax) //100)      //Bmax)
            && (imgOri.at<cv::Vec3b>(y,x)[1] >= Vmin) //0)      //Vmin)
            && (imgOri.at<cv::Vec3b>(y,x)[1] <= Vmax) //10)      //Vmax)
            && (imgOri.at<cv::Vec3b>(y,x)[2] >= Rmin) //90)      //Rmin)
            && (imgOri.at<cv::Vec3b>(y,x)[2] <= Rmax)) //255))    //Rmax))
            imgCree.at<uchar>(y,x) = 255;
            else
            imgCree.at<uchar>(y,x) = 0;
        }
    }
	
    dilate(imgCree, imgCree, kernel, Point(),3);
	erode(imgCree, imgCree, kernel, Point(), 2);

    int maxArea = 0;
    findContours(imgCree, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> conPoly(contours.size());  //Vecteur des contours
	vector<Rect> boundRect(contours.size());        // Vecteur pour boites des contours (Rect = Rectangles)

    	for (int i = 0; i< contours.size(); i++)
	    {
            int area = contourArea(contours[i]);
            
            fprintf(stderr, "L'aire du contour est : %d \n",area);
            //drawContours(imgOri,contours,i, Scalar(255,0,255), 4);
            if (area > 900 )  // 900 pour image 932*582
            {
                float peri = arcLength(contours[i], true);
                cout << "Longueur perimetre : " << peri << endl;
                	
                approxPolyDP(contours[i],conPoly[i], 0.02*peri, true);   
                cout << "Points du contour  : " << conPoly[i] << endl;
                //drawContours(imgOri,conPoly,i, Scalar(255,0,255), 4);
                boundRect[i] = boundingRect(conPoly[i]);
                
                if (area > 900 && conPoly[i].size() >= 8 )
                {                       
                    GrandP = {conPoly[i][0],conPoly[i][1],conPoly[i][2],conPoly[i][3]};
                    maxArea = area;

                    rectangle(imgOri,boundRect[i].tl(),boundRect[i].br(), Scalar(255,0,255), 2);
                    putText(imgOri, "Feu Rouge", {boundRect[i].x, boundRect[i].y}, FONT_ITALIC, 1, Scalar(0,255,0), 2);
                }
                    
            }
		 
	    }
    imshow("Detection des Semaphores", imgOri);
    waitKey(0);

    return 0;
}