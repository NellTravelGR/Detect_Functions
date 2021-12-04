vector<Point> getContours(Mat imgDilat) 
{
	// Liste des points  avec le type de donnee vector<vector<Points
	/*{{Point(20,30), Point(50,60)}, 
	{}, 
	{}}*/
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy; // vector de 4 integ ( (4*valeurs)
	
	//											methode      type de aproximation
	findContours(imgDilat, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	// 			src , contours, tous = -1 , color, thicknes
	//drawContours(img,contours,-1, Scalar(0,0,0), 2); 
    vector<vector<Point>> conPoly(contours.size());  //Vecteur des contours
	vector<Rect> boundRect(contours.size());        // Vecteur pour boites des contours (Rect = Rectangles)
    // Vector de donnees pour le plus grand des points du contour
    vector<Point> GrandP;

    int maxArea = 0;
	// Filtre des contours par aire
	for (int i = 0; i< contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		//cout << area << endl;
			
					
		if (area > 1000)
		{
			// arcLength nos da la longueur de los contornos
			//true= para indicar que los contornos son cerrados
			float peri = arcLength(contours[i], true);
			// pour trouver le nombre des cotes (coins =< 4=carre)
			// conpoly = vector pour la comparaison
			//    //true= para indicar cerrado////possible d'utiliser avec entier   	
			approxPolyDP(contours[i],conPoly[i], 0.02*peri, true);   
			
            // Analyse des points pour verification (carre = 4 || octogone = 8)
            // Loop pour enregistrer les points de l'image
			boundRect[i] = boundingRect(conPoly[i]);
            if (area > maxArea && conPoly[i].size() == 8)
            {   
				//drawContours(imgOri,conPoly,i, Scalar(255,0,255), 4);
                GrandP = {conPoly[i][0],conPoly[i][1],conPoly[i][2],conPoly[i][3],conPoly[i][4],conPoly[i][5], conPoly[i][6], conPoly[i][7]};
                maxArea = area;
                for (int j = 0; j < 8 ; j++ )
                {
                    cout << "L'element '[" << j << "] contient les points" << conPoly[i][j] << endl;
                }
				//Countour du element trouve
				//drawContours(imgOri,conPoly,i, Scalar(255,0,255), 4);
				rectangle(imgOri,boundRect[i].tl(),boundRect[i].br(), Scalar(255,0,255), 2);
				
				Rect mesROI(boundRect[i].x,boundRect[i].y,boundRect[i].width,boundRect[i].height);
                imgCoupe = imgOri(mesROI);
				imshow("Visualisation Image Coupe avant traitement", imgCoupe);
				// identification avec texte
				//putText(imgTraite, "Octogone", {boundRect[i].x, boundRect[i].y}, FONT_ITALIC, 0.5, Scalar(0,255,0), 1);
            }
				
	   	}
	}
	return GrandP;
} 