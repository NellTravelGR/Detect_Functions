/* -----------------------------------------------------*/
/* Fichier  : Detection_Outils.cpp                      */
/* Role     : Outils detection des objets               */
/* @Auteur  : Nelson Grullon Rosario                    */
/*          : Projet detecttion Panneux Signalisation   */
/* -----------------------------------------------------*/

using namespace cv;
using namespace std;

Mat creaMasqueHSV(Mat src);
Mat creaMasqueRVB(Mat src);
int compareImage (Mat src, Mat imgComp);
Rect detectContours (Mat imgMasque, int aireMin, Rect *dStop, Rect *dPass);