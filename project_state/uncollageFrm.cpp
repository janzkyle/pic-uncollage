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
#include "Images/uncollageFrm_rightBtn_XPM.xpm"
#include "Images/uncollageFrm_leftBtn_XPM.xpm"
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
	EVT_ACTIVATE(uncollageFrm::uncollageFrmActivate)
	EVT_BUTTON(ID_RIGHTBTN,uncollageFrm::rightBtnClick)
	EVT_BUTTON(ID_LEFTBTN,uncollageFrm::leftBtnClick)
	EVT_BUTTON(ID_SAVEBTN,uncollageFrm::saveBtnClick)
	EVT_BUTTON(ID_CLEARBTN,uncollageFrm::clearBtnClick)
	EVT_BUTTON(ID_CROPBTN,uncollageFrm::cropBtnClick)
	EVT_BUTTON(ID_LOADBTN,uncollageFrm::loadBtnClick)
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

	SaveFileDialog =  new wxFileDialog(this, _("Choose a file"), _(""), _(""), _("*.*"), wxFD_SAVE);

	OpenFileDialog =  new wxFileDialog(this, _("Choose a file"), _(""), _(""), _("*.bmp;*gif;*.png;*.jpeg;*.xpm"), wxFD_OPEN);

	wxBitmap rightBtn_BITMAP (uncollageFrm_rightBtn_XPM);
	rightBtn = new wxBitmapButton(this, ID_RIGHTBTN, rightBtn_BITMAP, wxPoint(618, 321), wxSize(150, 50), wxBU_AUTODRAW, wxDefaultValidator, _("rightBtn"));
	rightBtn->Enable(false);
	rightBtn->SetBackgroundColour(wxColour(_("BLACK")));

	wxBitmap leftBtn_BITMAP (uncollageFrm_leftBtn_XPM);
	leftBtn = new wxBitmapButton(this, ID_LEFTBTN, leftBtn_BITMAP, wxPoint(465, 321), wxSize(150, 50), wxBU_AUTODRAW, wxDefaultValidator, _("leftBtn"));
	leftBtn->Enable(false);
	leftBtn->SetForegroundColour(wxColour(_("BLACK")));
	leftBtn->SetBackgroundColour(wxColour(_("BLACK")));

	uploadImgLabel = new wxStaticText(this, ID_UPLOADIMGLABEL, _(" Image Preview"), wxPoint(118, 1), wxDefaultSize, 0, _("uploadImgLabel"));

	saveBtn = new wxButton(this, ID_SAVEBTN, _("Save"), wxPoint(543, 375), wxSize(150, 50), 0, wxDefaultValidator, _("saveBtn"));
	saveBtn->Enable(false);

	croppedImgLabel = new wxStaticText(this, ID_CROPPEDIMGLABEL, _("Cropped Image Preview"), wxPoint(559, 0), wxDefaultSize, 0, _("croppedImgLabel"));

	bitmapView = new wxStaticBitmap(this, ID_BITMAPVIEW, wxNullBitmap, wxPoint(468, 19), wxSize(300, 300) );
	bitmapView->SetBackgroundColour(wxColour(_("WHITE")));

	clearBtn = new wxButton(this, ID_CLEARBTN, _("Clear"), wxPoint(97, 379), wxSize(150, 50), 0, wxDefaultValidator, _("clearBtn"));

	cropBtn = new wxButton(this, ID_CROPBTN, _("Crop"), wxPoint(177, 327), wxSize(150, 50), 0, wxDefaultValidator, _("cropBtn"));

	loadBtn = new wxButton(this, ID_LOADBTN, _("Load Image"), wxPoint(19, 327), wxSize(150, 50), 0, wxDefaultValidator, _("loadBtn"));

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

bool rgbWithinRange(
    unsigned char rBasis, 
    unsigned char gBasis, 
    unsigned char bBasis, 
    unsigned char rCurrent, 
    unsigned char gCurrent, 
    unsigned char bCurrent,
    int threshold) {
    if(rBasis - threshold <= rCurrent && rBasis + threshold >= rCurrent) {
        if(gBasis - threshold <= gCurrent && gBasis + threshold >= gCurrent) {
            if(bBasis - threshold <= bCurrent && bBasis + threshold >= bCurrent) {
                return true;
            }
        }
    }
    
    return false;
}

void cropVerticalRecursion(
    wxImage& image, 
    std::vector<wxImage>& images, 
    unsigned char rBorder, 
    unsigned char gBorder, 
    unsigned char bBorder, 
    bool cropAgain) {
    int h = image.GetHeight();
    int w = image.GetWidth();
    
    bool onImage = false;
    int leftBorder = -1;
    int rightBorder = -1;
    
    for(int col = 0; col < w; col++) {
        for(int row = 1; row < h; row++) {
            unsigned char rPixel = image.GetRed(col, row);
            unsigned char gPixel = image.GetGreen(col, row);
            unsigned char bPixel = image.GetBlue(col, row);
            if(!rgbWithinRange(rBorder, gBorder, bBorder, rPixel, gPixel, bPixel)) {
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

        cropHorizontalRecursion(croppedLeftImage, images, rBorder, gBorder, bBorder);
        cropHorizontalRecursion(croppedRightImage, images, rBorder, gBorder, bBorder, true);
    }
    else if(leftBorder != -1) {
        if(cropAgain) {
            cropHorizontalRecursion(image, images, rBorder, gBorder, bBorder);
        }
        else {
            images.push_back(image);
        }
    }
}

void cropHorizontalRecursion(
    wxImage& image, 
    std::vector<wxImage>& images, 
    unsigned char rBorder, 
    unsigned char gBorder, 
    unsigned char bBorder, 
    bool cropAgain) {
    int h = image.GetHeight();
    int w = image.GetWidth();
    
    bool onImage = false;
    int topBorder = -1;
    int bottomBorder = -1;
    
    for(int row = 0; row < h; row++) {
        for(int col = 1; col < w; col++) {
            unsigned char rPixel = image.GetRed(col, row);
            unsigned char gPixel = image.GetGreen(col, row);
            unsigned char bPixel = image.GetBlue(col, row);
            if(!rgbWithinRange(rBorder, gBorder, bBorder, rPixel, gPixel, bPixel)) {
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

        cropVerticalRecursion(croppedTopImage, images, rBorder, gBorder, bBorder);
        cropVerticalRecursion(croppedBottomImage, images, rBorder, gBorder, bBorder, true);
    }
    else if(topBorder != -1) {
        if(cropAgain) {
            cropVerticalRecursion(image, images, rBorder, gBorder, bBorder);
        }
        else {
            images.push_back(image);
        }
    }
}


void scaleImage(std::vector<wxImage>& images, wxImage& image, int i) {
     

    double fWScale = 1.0;   // horizontal scaling factor
    double fHScale = 1.0;   // vertical scaling factor
    int iImageH = -1;       // the bitmap's height
    int iImageW = -1;       // the bitmap's width
    int iThisH = 300;        // the panel's height
    int iThisW = 300;        // the panel's width
    int iNewH = 0;          // the bitmap's new height
    int iNewW = 0;          // the bitmap's new width

    iImageH = images[i].GetHeight();   
    iImageW = images[i].GetWidth();
    if( ( iThisH > 0 ) && ( iThisW > 0 ) )
    {
        fHScale = (double) iImageH / (double) iThisH;
        fWScale = (double) iImageW / (double) iThisW;
  
        if(fHScale > fWScale)
        {
            fWScale = fHScale;
        }
        else
        {
            fHScale = fWScale;
        }
  
        iNewH = (int) (((double) iImageH) / fHScale);
        iNewW = (int) (((double) iImageW) / fWScale);
    }
        
    image = images[i].Scale(iNewW, iNewH);
     

}





/*
 * clearBtnClick
 */
void uncollageFrm::clearBtnClick(wxCommandEvent& event)
{
    croppedImgs.clear(); 
    bitmapDisplay -> SetBitmap(wxNullBitmap);
    bitmapView -> SetBitmap(wxNullBitmap);
    saveBtn -> Enable(false); 
    leftBtn->Enable(false);
    rightBtn->Enable(false);
}

/*
 * cropBtnClick
 */              
void uncollageFrm:: cropBtnClick(wxCommandEvent& event)
{
    
    croppedImgs.clear(); 
	unsigned char rBorder = Upload_Pic.GetRed(0,0);
	unsigned char gBorder = Upload_Pic.GetGreen(0,0);
	unsigned char bBorder = Upload_Pic.GetBlue(0,0);
	saveBtn -> Enable(true); 
	rightBtn -> Enable(true); 
	leftBtn -> Enable(true); 
    cropVerticalRecursion(Upload_Pic,croppedImgs, rBorder, gBorder, bBorder, true);
    index=0;
    scaleImage(croppedImgs,scaledImg,index);
    bitmapView -> SetBitmap(wxNullBitmap);
    bitmapView -> SetBitmap(scaledImg);

    
}

/*
 * uncollageFrmActivate
 */
void uncollageFrm::uncollageFrmActivate(wxActivateEvent& event)
{
	// insert your code here
}

/*
 * saveBtnClick
 */
void uncollageFrm::saveBtnClick(wxCommandEvent& event)
{
    wxFileDialog *SaveFileDialog = new wxFileDialog(this, _("Save Image")
    , _(""), _("")
    , _("BMP file (*.bmp)|*.bmp|GIF file (*.gif)|*.gif|JPEG file (*.jpg)|*.jpg|PNG file (*.png)|*.png|TIFF file (*.tif)|*.tif")
    , wxFD_SAVE);
    if (SaveFileDialog->ShowModal() == wxID_OK)
    {
        wxString savePath = SaveFileDialog->GetPath();
		wxImage currentImage = croppedImgs[index]; 
		if (currentImage.IsOk())
		{
    		currentImage.SaveFile(savePath);
        }
    }
    else 
        {
            SaveFileDialog->Close();
        }
    
    SaveFileDialog->Destroy();

 
}

/*
 * loadBtnClick
 */
void uncollageFrm::loadBtnClick(wxCommandEvent& event)
{
    OpenFileDialog -> ShowModal();
    if (OpenFileDialog -> GetPath().IsEmpty())
    {
        return;
    }
    
    Upload_Pic.LoadFile(OpenFileDialog -> GetPath(), wxBITMAP_TYPE_ANY);
    
    wxImage img = Upload_Pic.Scale(300,300);
    bitmapDisplay -> SetBitmap(img);
}


/*
 * leftBtnClick
 */
void uncollageFrm::leftBtnClick(wxCommandEvent& event)
{   

    index--;
    index = (index+croppedImgs.size()) % croppedImgs.size();
    scaleImage(croppedImgs,scaledImg,index);
    bitmapView -> SetBitmap(wxNullBitmap);
    bitmapView -> SetBitmap(scaledImg);

}

/*
 * rightBtnClick
 */
void uncollageFrm::rightBtnClick(wxCommandEvent& event)
{   
    
    index++;
    index %= croppedImgs.size(); 
    scaleImage(croppedImgs,scaledImg,index); 
    bitmapView -> SetBitmap(wxNullBitmap);
    bitmapView -> SetBitmap(im);


}
