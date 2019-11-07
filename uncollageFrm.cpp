///-----------------------------------------------------------------
///
/// @file      uncollageFrm.cpp
/// @author    raymart
/// Created:   9/29/2019 1:38:13 AM
/// @section   DESCRIPTION
///            uncollageFrm class implementation
///
///------------------------------------------------------------------

#include "uncollageFrm.h"
#include <bits/stdc++.h> 
#include <cmath>
#include <vector>

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// uncollageFrm
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(uncollageFrm,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(uncollageFrm::OnClose)
	EVT_BUTTON(ID_CLEARBTN,uncollageFrm::clearBtnClick)
	EVT_BUTTON(ID_UPLOADBTN,uncollageFrm::uploadBtnClick)
END_EVENT_TABLE()
////Event Table End

uncollageFrm::uncollageFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

uncollageFrm::~uncollageFrm()
{
}

void uncollageFrm::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	wxInitAllImageHandlers();   //Initialize graphic format handlers

	OpenFileDialog =  new wxFileDialog(this, _("Choose a file"), _(""), _(""), _("*.bmp;*gif;*.png;*.jpeg;*.xpm"), wxFD_OPEN);

	saveBtn = new wxButton(this, ID_SAVEBTN, _("Save As ..."), wxPoint(177, 379), wxSize(150, 50), 0, wxDefaultValidator, _("saveBtn"));

	clearBtn = new wxButton(this, ID_CLEARBTN, _("Clear"), wxPoint(25, 379), wxSize(150, 50), 0, wxDefaultValidator, _("clearBtn"));

	cropBtn = new wxButton(this, ID_CROPBTN, _("Crop"), wxPoint(177, 327), wxSize(150, 50), 0, wxDefaultValidator, _("cropBtn"));

	uploadBtn = new wxButton(this, ID_UPLOADBTN, _("Upload"), wxPoint(25, 327), wxSize(150, 50), 0, wxDefaultValidator, _("uploadBtn"));

	bitmapDisplay = new wxStaticBitmap(this, ID_BITMAPDISPLAY, wxNullBitmap, wxPoint(25, 20), wxSize(300, 300) );
	bitmapDisplay->SetForegroundColour(wxColour(_("WHITE")));
	bitmapDisplay->SetBackgroundColour(wxColour(_("WHITE")));

	SetTitle(_("Uncollage"));
	SetIcon(wxNullIcon);
	SetSize(8,8,800,470);
	Center();
	
	////GUI Items Creation End
}

void uncollageFrm::OnClose(wxCloseEvent& event)
{
	Destroy();
}

/*
 * uploadBtnClick
 */
void uncollageFrm::uploadBtnClick(wxCommandEvent& event)
{
    OpenFileDialog -> ShowModal();
    if (OpenFileDialog -> GetPath().IsEmpty())
    {
        return;
    }
    
    Upload_Pic.LoadFile(OpenFileDialog -> GetPath(), wxBITMAP_TYPE_ANY);
    
    wxImage img = Upload_Pic.Scale(300,300);
    int h = img.GetHeight();
    int w = img.GetWidth();
        
    //grayscaling
    wxImage grayscaledImg = wxImage(300,300,true);
    for(int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            unsigned char r = img.GetRed(i,j);
            unsigned char g = img.GetGreen(i,j);
            unsigned char b = img.GetBlue(i,j);
            unsigned char gray = (r*299 + g*587 + b*114)/1000;
            grayscaledImg.SetRGB(i,j,gray,gray,gray);
        }
    }
    
    img = grayscaledImg;
    //end grayscaling
    
    //gaussian blur
    wxImage gaussImg = wxImage(300,300,true);
    
    /*int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
    
    int sum;
    for(int row = 1; row < h - 1; row++) {
        for(int col = 1; col < w - 1; col++) {
            sum = 0;
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    sum += img.GetRed(row-1+i,col-1+j) * kernel[i][j];
                }
            }
            sum /= 16;
            gaussImg.SetRGB(row,col,sum,sum,sum);
        }
    }*/
    
    int kernel[5][5] = {
        {1, 4, 7, 4, 1},
        {4, 16, 26, 16, 4},
        {7, 26, 41, 26, 7},
        {4, 16, 26, 16, 4},
        {1, 4, 7, 4, 1}
    };
    
    int sum;
    for(int row = 3; row < h - 3; row++) {
        for(int col = 3; col < w - 3; col++) {
            sum = 0;
            for(int i = 0; i < 5; i++) {
                for(int j = 0; j < 5; j++) {
                    sum += img.GetRed(row-1+i,col-1+j) * kernel[i][j];
                }
            }
            sum /= 273;
            gaussImg.SetRGB(row,col,sum,sum,sum);
        }
    }
    
    //img = gaussImg;
    //end gaussian blur
    
    
    //median filter
    wxImage medianImg = wxImage(300,300,true);
    int ksize = 7;
    
    std::vector<int> window;
    int windowRow = 0;
    int windowCol = 0;
    int median = 0;
    
    for(int row = 0; row < h; row++) {
        for(int col = 0; col < w; col++) {
            window.clear();
            windowRow = 0;
            windowCol = 0;
            
            for(int i = 0; i < ksize; i++) {
                for(int j = 0; j < ksize; j++) {
                    if(row+i-(ksize/2) < 0) {
                        windowRow = 0;
                    }
                    else if(row+i-(ksize/2) >= h) {
                        windowRow = h-1;
                    }
                    else {
                        windowRow = row+i-(ksize/2);
                    }
                    
                    if(col+j-(ksize/2) < 0) {
                        windowCol = 0;
                    }
                    else if(col+j-(ksize/2) >= w) {
                        windowCol = w-1;
                    }
                    else {
                        windowCol = col+j-(ksize/2);
                    }
                    window.push_back(img.GetRed(windowRow,windowCol));
                }
            }
            
            std::sort(window.begin(), window.end());
            if(ksize % 2 == 0) {
                median = (window[(ksize/2)-1] + window[(ksize/2)]) / 2;
            }
            else {
                median = window[(ksize/2) - 1];
            }
            medianImg.SetRGB(row,col,median,median,median);
        }
    }
    
    img = medianImg;
    //end median filter
    
    //resizing
    wxImage resizedImg = wxImage(300,300,true);
    resizedImg = img.Scale(h/4,w/4);
    //img = resizedImg.Scale(h,w);
    //end resizing
    
    //sobel filter
    wxImage sobelImg = wxImage(300,300,true);
    
    int sobelX[3][3] = {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };
    
    int sobelY[3][3] = {
        {-1,-2,1},
        {0,0,0},
        {-1,2,1}
    };
    
    int mag, magX, magY;
    for(int row = 1; row < h - 1; row++) {
        for(int col = 1; col < w - 1; col++) {
            mag = 0;
            magX = 0;
            magY = 0;
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    magX += img.GetRed(row-1+i,col-1+j) * sobelX[i][j];
                    magY += img.GetRed(row-1+i,col-1+j) * sobelY[i][j];
                }
            }
            mag = ceil(sqrt(magX*magX + magY*magY));
            sobelImg.SetRGB(row,col,mag,mag,mag);
        }
    }
    
    img = sobelImg;
    //end sobel filter
    
    //dilation
    wxImage dilatedImg = wxImage(300,300,true);
    int k = 3;
    
    for(int row = 0; row < h; row++) {
        for(int col = 0; col < w; col++) {
            if(img.GetRed(row,col) > 10) {
                dilatedImg.SetRGB(row,col,255,255,255);
            }
            else {
                dilatedImg.SetRGB(row,col,0,0,0);
            }
        }
    }
    
    //img = dilatedImg;
    //end dilation
    
    bitmapDisplay -> SetBitmap(img);
}

/*
 * clearBtnClick
 */
void uncollageFrm::clearBtnClick(wxCommandEvent& event)
{
    
    bitmapDisplay -> SetBitmap(wxNullBitmap);
}
