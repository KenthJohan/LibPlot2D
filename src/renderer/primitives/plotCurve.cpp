/*===================================================================================
                                    DataPlotter
                          Copyright Kerry R. Loux 2011-2013

                   This code is licensed under the GPLv2 License
                     (http://opensource.org/licenses/GPL-2.0).

===================================================================================*/

// File:  plotCurve.cpp
// Created:  5/2/2011
// Author:  K. Loux
// Description:  Derived from Primitive for creating data curves on a plot.
// History:
//	11/9/2010	- Modified to accomodate 3D plots, K. Loux.

// Local headers
#include "renderer/primitives/plotCurve.h"
#include "renderer/renderWindow.h"
#include "renderer/primitives/axis.h"
#include "utilities/dataset2D.h"
#include "utilities/math/plotMath.h"

//==========================================================================
// Class:			PlotCurve
// Function:		PlotCurve
//
// Description:		Constructor for the PlotCurve class.
//
// Input Arguments:
//		renderWindow	= RenderWindow* pointing to the object that owns this
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
PlotCurve::PlotCurve(RenderWindow &renderWindow) : Primitive(renderWindow)
{
	xAxis = NULL;
	yAxis = NULL;

	lineSize = 1;
	markerSize = -1;
}

//==========================================================================
// Class:			PlotCurve
// Function:		PlotCurve
//
// Description:		Copy constructor for the PlotCurve class.
//
// Input Arguments:
//		plotCurve	= const PlotCurve& to copy to this object
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
PlotCurve::PlotCurve(const PlotCurve &plotCurve) : Primitive(plotCurve)
{
	*this = plotCurve;
}

//==========================================================================
// Class:			PlotCurve
// Function:		~PlotCurve
//
// Description:		Destructor for the PlotCurve class.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
PlotCurve::~PlotCurve()
{
}

//==========================================================================
// Class:			PlotCurve
// Function:		GenerateGeometry
//
// Description:		Creates the OpenGL instructions to create this object in
//					the scene.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void PlotCurve::GenerateGeometry()
{
	if (lineSize > 0)
	{
		const double lineSizeScale(1.2);

		line.SetLineColor(color);
		line.SetBackgroundColorForAlphaFade();
		line.SetWidth(lineSize * lineSizeScale);
		points.clear();

		unsigned int i;
		for (i = 0; i < data->GetNumberOfPoints(); i++)
				PlotPoint(i);

		line.Draw(points);
	}

	if (markerSize > 0 || (markerSize < 0 && SmallRange()))
	{
		glColor4d(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		glBegin(GL_QUADS);
		PlotMarkers();
		glEnd();
	}
}

//==========================================================================
// Class:			PlotCurve
// Function:		PlotPoint
//
// Description:		Plots the coordinate with the specified data index.
//
// Input Arguments:
//		i	= const unsigned int&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void PlotCurve::PlotPoint(const unsigned int &i)
{
	PlotPoint(data->GetXData(i), data->GetYData(i));
}

//==========================================================================
// Class:			PlotCurve
// Function:		PlotPoint
//
// Description:		Plots the coordinate with the specified coordinates.
//
// Input Arguments:
//		x	= const double&
//		y	= const double&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void PlotCurve::PlotPoint(const double &x, const double &y)
{
	double doublePoint[2] = {x, y};
	double point[2];

	RescalePoint(doublePoint, point);
	points.push_back(std::make_pair(point[0], point[1]));
}

//==========================================================================
// Class:			PlotCurve
// Function:		PointIsValid
//
// Description:		Checks to see if the specified point is a real, finite number.
//
// Input Arguments:
//		i	= const unsigned int&
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, true for valid, false otherwise
//
//==========================================================================
bool PlotCurve::PointIsValid(const unsigned int &i) const
{
	assert(i < data->GetNumberOfPoints());

	return PlotMath::IsValid<double>(data->GetXData(i)) &&
		PlotMath::IsValid<double>(data->GetYData(i));
}

//==========================================================================
// Class:			PlotCurve
// Function:		HasValidParameters
//
// Description:		Checks to see if the information about this object is
//					valid and complete (gives permission to create the object).
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, true for OK to draw, false otherwise
//
//==========================================================================
bool PlotCurve::HasValidParameters()
{
	if (xAxis != NULL && yAxis != NULL && data->GetNumberOfPoints() > 1)
	{
		if (xAxis->IsHorizontal() && !yAxis->IsHorizontal())
			return true;
	}

	return false;
}

//==========================================================================
// Class:			PlotCurve
// Function:		operator=
//
// Description:		Assignment operator for PlotCurve class.
//
// Input Arguments:
//		plotCurve	= const PlotCurve& to assign to this object
//
// Output Arguments:
//		None
//
// Return Value:
//		PlotCurve&, reference to this object
//
//==========================================================================
PlotCurve& PlotCurve::operator=(const PlotCurve &plotCurve)
{
	if (this == &plotCurve)
		return *this;

	this->Primitive::operator=(plotCurve);

	return *this;
}

//==========================================================================
// Class:			PlotCurve
// Function:		SetData
//
// Description:		Assigns data to the curve.
//
// Input Arguments:
//		data	= const Dataset2D* to plot
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void PlotCurve::SetData(const Dataset2D *data)
{
	this->data = data;
	modified = true;
}

//==========================================================================
// Class:			PlotCurve
// Function:		RescalePoint
//
// Description:		Rescales the onscreen position of the point according to
//					the size of the axis with which this object is associated.
//
// Input Arguments:
//		value	= const double* containing the location of the point in plot
//				  coordinates
//
// Output Arguments:
//		coordinate	= double* specifying the location of the object in screen coordinates
//
// Return Value:
//		None
//
//==========================================================================
void PlotCurve::RescalePoint(const double *value, double *coordinate) const
{
	if (!value || !coordinate)
		return;

	coordinate[0] = xAxis->ValueToPixel(value[0]);
	coordinate[1] = yAxis->ValueToPixel(value[1]);
}

//==========================================================================
// Class:			PlotCurve
// Function:		PlotMarkers
//
// Description:		Plots markers at all un-interpolated points.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void PlotCurve::PlotMarkers() const
{
	unsigned int i;
	for (i = 0; i < data->GetNumberOfPoints(); i++)
			DrawMarker(data->GetXData(i), data->GetYData(i));
}

//==========================================================================
// Class:			PlotCurve
// Function:		DrawMarker
//
// Description:		Draws a marker at the specified location.
//
// Input Arguments:
//		x	= const double&
//		y	= const double&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void PlotCurve::DrawMarker(const double &x, const double &y) const
{
	double doublePoint[2] = {x, y};
	double point[2];
	RescalePoint(doublePoint, point);

	int halfMarkerSize = 2 * markerSize;

	glVertex2i(point[0] + halfMarkerSize, point[1] + halfMarkerSize);
	glVertex2i(point[0] + halfMarkerSize, point[1] - halfMarkerSize);
	glVertex2i(point[0] - halfMarkerSize, point[1] - halfMarkerSize);
	glVertex2i(point[0] - halfMarkerSize, point[1] + halfMarkerSize);
}

//==========================================================================
// Class:			PlotCurve
// Function:		SmallRange
//
// Description:		Determines if the range is small enough to warrant
//					drawing the point markers.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		bool
//
//==========================================================================
bool PlotCurve::SmallRange() const
{
	if (data->GetNumberOfPoints() < 2)
		return false;

	switch (SmallXRange())
	{
	case RangeSizeSmall:
		return true;

	case RangeSizeLarge:
		return false;

	default:
	case RangeSizeUndetermined:
		break;
	}

	return SmallYRange() == RangeSizeSmall;
}

//==========================================================================
// Class:			PlotCurve
// Function:		SmallXRange
//
// Description:		Determines if the x-range is small enough to warrant
//					drawing the point markers.  A "small enough range" is
//					one where there are less than some number of pixels in the
//					x-direction between points (on average).
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		PlotCurve::RangeSize
//
//==========================================================================
PlotCurve::RangeSize PlotCurve::SmallXRange() const
{
	double period = data->GetXData(1) - data->GetXData(0);
	if (period == 0.0)
		return RangeSizeUndetermined;

	unsigned int points = (unsigned int)floor((xAxis->GetMaximum() - xAxis->GetMinimum()) / period);
	if (points == 0)
		return RangeSizeSmall;

	unsigned int spacing = (renderWindow.GetSize().GetWidth()
		- xAxis->GetAxisAtMaxEnd()->GetOffsetFromWindowEdge()
		- xAxis->GetAxisAtMinEnd()->GetOffsetFromWindowEdge()) / points;

	if (spacing > 7)
		return RangeSizeSmall;

	return RangeSizeLarge;
}

//==========================================================================
// Class:			PlotCurve
// Function:		SmallYRange
//
// Description:		Determines if the y-range is small enough to warrant
//					drawing the point markers.  A "small enough range" is
//					one where there are less than some number of pixels in the
//					y-direction between points (on average).
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		PlotCurve::RangeSize
//
//==========================================================================
PlotCurve::RangeSize PlotCurve::SmallYRange() const
{
	double period = data->GetYData(1) - data->GetYData(0);
	if (period == 0.0)
		return RangeSizeUndetermined;

	unsigned int points = (unsigned int)floor((yAxis->GetMaximum() - yAxis->GetMinimum()) / period);
	if (points == 0)
		return RangeSizeSmall;

	unsigned int spacing = (renderWindow.GetSize().GetHeight()
		- yAxis->GetAxisAtMaxEnd()->GetOffsetFromWindowEdge()
		- yAxis->GetAxisAtMinEnd()->GetOffsetFromWindowEdge()) / points;

	if (spacing > 7)
		return RangeSizeSmall;

	return RangeSizeLarge;
}