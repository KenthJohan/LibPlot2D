/*===================================================================================
                                    DataPlotter
                          Copyright Kerry R. Loux 2011-2016

                   This code is licensed under the GPLv2 License
                     (http://opensource.org/licenses/GPL-2.0).

===================================================================================*/

// File:  textRendering.h
// Created:  5/2/2011
// Author:  K. Loux
// Description:  Derived from Primitive, this class is used to draw text.

#ifndef TEXT_RENDERING_H_
#define TEXT_RENDERING_H_

// wxWidgets headers
#include <wx/wx.h>

// Local headers
#include "lp2d/renderer/primitives/primitive.h"
#include "lp2d/renderer/text.h"

namespace LibPlot2D
{

class TextRendering : public Primitive
{
public:
	TextRendering(RenderWindow &renderWindow);
	~TextRendering();

	// Set option methods
	void SetAngle(const double& angle) { this->angle = angle; modified = true; }// [rad]
	void InitializeFonts(const std::string& fontFileName, const double& size);
	void SetText(const wxString& text) { this->text = text; modified = true; }
	void SetPosition(const double& x, const double& y) { this->x = x; this->y = y; modified = true; }
	void SetCentered(const bool& centered) { this->centered = centered; modified = true; }

	double GetTextHeight();
	double GetTextWidth();
	wxString GetText() const { return text; }

protected:
	// Mandatory overloads from Primitive - for creating geometry and testing the
	// validity of this object's parameters
	virtual bool HasValidParameters();
	virtual void Update(const unsigned int& i);
	virtual void GenerateGeometry();

private:
	double angle;// 0 is horizontal, angle builds counter-clockwise about an axis out of the screen

	wxString text;
	Text font;

	// Flag indicating whether the text is centered at (X, Y) or if, if false,
	// (0, 0) represents the lower left corner of the text bounding box
	bool centered;
	double x, y;
};

}// namespace LibPlot2D

#endif// TEXT_RENDERING_H_
