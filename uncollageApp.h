//---------------------------------------------------------------------------
//
// Name:        uncollageApp.h
// Author:      raymart
// Created:     9/29/2019 1:38:13 AM
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __UNCOLLAGEFRMApp_h__
#define __UNCOLLAGEFRMApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class uncollageFrmApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
