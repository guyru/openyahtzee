#pragma once

#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bitmap.h>

#include "openyahtzee_png.h"

inline wxIcon GetAppIcon(int size = 32)
{
	wxMemoryInputStream stream(openyahtzee_png, openyahtzee_png_len);
	wxImage img(stream, wxBITMAP_TYPE_PNG);
	img.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
	wxIcon icon;
	icon.CopyFromBitmap(wxBitmap(img));
	return icon;
}

inline wxBitmap GetAppBitmap(int size)
{
	wxMemoryInputStream stream(openyahtzee_png, openyahtzee_png_len);
	wxImage img(stream, wxBITMAP_TYPE_PNG);
	img.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
	return wxBitmap(img);
}
