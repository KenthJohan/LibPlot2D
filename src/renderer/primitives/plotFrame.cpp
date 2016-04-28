/*===================================================================================
                                    DataPlotter
                          Copyright Kerry R. Loux 2011-2016

                   This code is licensed under the GPLv2 License
                     (http://opensource.org/licenses/GPL-2.0).

===================================================================================*/

// File:  plotFrame.cpp
// Created:  4/27/2016
// Author:  K. Loux
// Description:  Derived from Primitive for masking outer edges of plot area.
// History:

// Local headers
#include "renderer/primitives/plotFrame.h"
#include "renderer/primitives/axis.h"
#include "renderer/renderWindow.h"

PlotFrame::PlotFrame(RenderWindow &renderWindow, const Axis& top, const Axis& bottom,
	const Axis& left, const Axis& right) : Primitive(renderWindow),
	top(&top), bottom(&bottom), left(&left), right(&right)
{
	// Draw AFTER plot curves but BEFORE axis labels
	SetDrawOrder(2000);
}

PlotFrame::~PlotFrame()
{
}

void PlotFrame::GenerateGeometry()
{
	// TODO:  This doesn't get updated on a re-size
	int width, height;
	renderWindow.GetSize(&width, &height);

	glBegin(GL_QUADS);

	// Left
	glVertex2i(0, 0);
	glVertex2i(left->GetOffsetFromWindowEdge(), 0);
	glVertex2i(left->GetOffsetFromWindowEdge(), height);
	glVertex2i(0, height);

	// Bottom
	glVertex2i(left->GetOffsetFromWindowEdge(), 0);
	glVertex2i(width - right->GetOffsetFromWindowEdge(), 0);
	glVertex2i(width - right->GetOffsetFromWindowEdge(), bottom->GetOffsetFromWindowEdge());
	glVertex2i(left->GetOffsetFromWindowEdge(), bottom->GetOffsetFromWindowEdge());

	// Right
	glVertex2i(width - right->GetOffsetFromWindowEdge(), 0);
	glVertex2i(width, 0);
	glVertex2i(width, height);
	glVertex2i(width - right->GetOffsetFromWindowEdge(), height);

	// Top
	glVertex2i(left->GetOffsetFromWindowEdge(), height - top->GetOffsetFromWindowEdge());
	glVertex2i(width - right->GetOffsetFromWindowEdge(), height - top->GetOffsetFromWindowEdge());
	glVertex2i(width - right->GetOffsetFromWindowEdge(), height);
	glVertex2i(left->GetOffsetFromWindowEdge(), height);

	glEnd();
}

bool PlotFrame::HasValidParameters()
{
	return true;
}