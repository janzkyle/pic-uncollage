///-----------------------------------------------------------------
///
/// @file      uncollageFrm.h
/// @author    raymart
/// Created:   9/29/2019 1:38:13 AM
/// @section   DESCRIPTION
///            uncollageFrm class declaration
///
///------------------------------------------------------------------

#ifndef __UNCOLLAGEFRM_H__
#define __UNCOLLAGEFRM_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/filedlg.h>
#include <wx/button.h>
#include <wx/statbmp.h>
////Header Include End
#include <vector>

////Dialog Style Start
#undef uncollageFrm_STYLE
#define uncollageFrm_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class uncollageFrm : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		uncollageFrm(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("uncollage"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = uncollageFrm_STYLE);
		virtual ~uncollageFrm();
		void uploadBtnClick(wxCommandEvent& event);
		void clearBtnClick(wxCommandEvent& event);
		void cropBtnClick(wxCommandEvent& event);
		
	private:
		//Do not add custom control declarations between
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxFileDialog *OpenFileDialog;
		wxFileDialog *SaveFileDialog;
		wxStaticBitmap *WxStaticBitmap1;
		wxButton *clearBtn;
		wxButton *cropBtn;
		wxButton *uploadBtn;
		wxStaticBitmap *bitmapDisplay;
		////GUI Control Declaration End
		wxImage Upload_Pic;
		
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXSTATICBITMAP1 = 1008,
			ID_CLEARBTN = 1005,
			ID_CROPBTN = 1004,
			ID_UPLOADBTN = 1003,
			ID_BITMAPDISPLAY = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
		
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
