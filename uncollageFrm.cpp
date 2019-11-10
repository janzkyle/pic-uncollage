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
	EVT_BUTTON(ID_CROPBTN,uncollageFrm::cropBtnClick)
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

	SaveFileDialog =  new wxFileDialog(this, _("Choose a file"), _(""), _(""), _("*.*"), wxFD_SAVE);

	WxStaticBitmap1 = new wxStaticBitmap(this, ID_WXSTATICBITMAP1, wxNullBitmap, wxPoint(467, 19), wxSize(300, 300) );
	WxStaticBitmap1->SetForegroundColour(wxColour(_("WHITE")));
	WxStaticBitmap1->SetBackgroundColour(wxColour(_("WHITE")));

	clearBtn = new wxButton(this, ID_CLEARBTN, _("Clear"), wxPoint(97, 379), wxSize(150, 50), 0, wxDefaultValidator, _("clearBtn"));

	cropBtn = new wxButton(this, ID_CROPBTN, _("Crop and Save"), wxPoint(177, 327), wxSize(150, 50), 0, wxDefaultValidator, _("cropBtn"));

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

void cropHorizontalRecursion(wxImage& image, std::vector<wxImage>& images, bool cropAgain=false);

void cropVerticalRecursion(wxImage& image, std::vector<wxImage>& images, bool cropAgain=false) {
    int h = image.GetHeight();
    int w = image.GetWidth();
    
    bool onImage = false;
    int leftBorder = -1;
    int rightBorder = -1;
    
    for(int col = 0; col < w; col++) {
        unsigned char rPixel = image.GetRed(col, 0);
        unsigned char gPixel = image.GetGreen(col, 0);
        unsigned char bPixel = image.GetBlue(col, 0);
        for(int row = 1; row < h; row++) {
            if(image.GetRed(col, row) != rPixel || image.GetGreen(col, row) != gPixel || image.GetBlue(col, row) != bPixel) {
                onImage = true;
                break;
            }
            if(row == h-1) {
                onImage = false;
            }
        }
        
        if(leftBorder == -1 && onImage) {
            leftBorder = col;
        }
        else if(!onImage && leftBorder != -1) {
            rightBorder = col;
            break;
        }
    }
    
    if(rightBorder != -1) {
        int cropLeftWidth = rightBorder - leftBorder;
        wxRect leftRect = wxRect(leftBorder, 0, cropLeftWidth, h);
        wxImage croppedLeftImage = image.GetSubImage(leftRect);
        
        int cropRightWidth = w - rightBorder;
        wxRect rightRect = wxRect(rightBorder, 0, cropRightWidth, h);
        wxImage croppedRightImage = image.GetSubImage(rightRect);

        cropHorizontalRecursion(croppedLeftImage, images);
        cropHorizontalRecursion(croppedRightImage, images, true);
    }
    else if(leftBorder != -1) {
        if(cropAgain) {
            cropHorizontalRecursion(image, images);
        }
        else {
            images.push_back(image);
        }
    }
}

void cropHorizontalRecursion(wxImage& image, std::vector<wxImage>& images, bool cropAgain) {
    int h = image.GetHeight();
    int w = image.GetWidth();
    
    bool onImage = false;
    int topBorder = -1;
    int bottomBorder = -1;
    
    for(int row = 0; row < h; row++) {
        unsigned char rPixel = image.GetRed(0, row);
        unsigned char gPixel = image.GetGreen(0, row);
        unsigned char bPixel = image.GetBlue(0, row);
        for(int col = 1; col < w; col++) {
            if(image.GetRed(col, row) != rPixel || image.GetGreen(col, row) != gPixel || image.GetBlue(col, row) != bPixel) {
                onImage = true;
                break;
            }
            if(col == w-1) {
                onImage = false;
            }
        }
        
        if(topBorder == -1 && onImage) {
            topBorder = row;
        }
        else if(!onImage && topBorder != -1) {
            bottomBorder = row;
            break;
        }
    }
    
    if(bottomBorder != -1) {
        int cropTopHeight = bottomBorder - topBorder;
        wxRect topRect = wxRect(0, topBorder, w, cropTopHeight);
        wxImage croppedTopImage = image.GetSubImage(topRect);
        
        int cropBottomHeight = h - bottomBorder;
        wxRect bottomRect = wxRect(0, bottomBorder, w, cropBottomHeight);
        wxImage croppedBottomImage = image.GetSubImage(bottomRect);

        cropVerticalRecursion(croppedTopImage, images);
        cropVerticalRecursion(croppedBottomImage, images, true);
    }
    else if(topBorder != -1) {
        if(cropAgain) {
            cropVerticalRecursion(image, images);
        }
        else {
            images.push_back(image);
        }
    }
}
void saveImage(std::vector<wxImage>& images) {
  

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
    //int h = img.GetHeight();
    //int w = img.GetWidth();
        
 
    
    bitmapDisplay -> SetBitmap(img);
    

}

/*
 * clearBtnClick
 */
void uncollageFrm::clearBtnClick(wxCommandEvent& event)
{
    
    bitmapDisplay -> SetBitmap(wxNullBitmap);
}

/*
 * cropBtnClick
 */              
void uncollageFrm:: cropBtnClick(wxCommandEvent& event)
{
	std::vector<wxImage> croppedImgs;
    cropVerticalRecursion(Upload_Pic,croppedImgs);
    	wxFileDialog *SaveFileDialog = new wxFileDialog(this, _("Save Image"), _(""), _(""), _("BMP file (*.bmp)|*.bmp|GIF file (*.gif)|*.gif|JPEG file (*.jpg)|*.jpg|PNG file (*.png)|*.png|TIFF file (*.tif)|*.tif"), wxFD_SAVE);
    for (int i = 0; i<croppedImgs.size(); i++)
        {                 
	       if (SaveFileDialog->ShowModal() == wxID_OK)
        	{  
                wxString savePath = SaveFileDialog->GetPath();
		        wxImage currentImage = croppedImgs[i]; 
		        if (currentImage.IsOk())
		        {
    		      currentImage.SaveFile(savePath); // File type depends on extension
                }
            }
            else 
            {
                SaveFileDialog->Close();
            }
        }
    
    SaveFileDialog->Destroy();
}
