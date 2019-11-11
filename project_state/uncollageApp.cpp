//---------------------------------------------------------------------------
//
// Name:        uncollageApp.cpp
// Author:      raymart
// Created:     9/29/2019 1:38:13 AM
// Description: 
//
//---------------------------------------------------------------------------

#include "uncollageApp.h"
#include "uncollageFrm.h"

IMPLEMENT_APP(uncollageFrmApp)

bool uncollageFrmApp::OnInit()
{
    uncollageFrm* frame = new uncollageFrm(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int uncollageFrmApp::OnExit()
{
	return 0;
}
