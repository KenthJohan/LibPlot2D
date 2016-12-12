/*===================================================================================
                                    DataPlotter
                          Copyright Kerry R. Loux 2011-2016

                   This code is licensed under the GPLv2 License
                     (http://opensource.org/licenses/GPL-2.0).

===================================================================================*/

// File:  fftDialog.h
// Created:  8/15/2012
// Author:  K. Loux
// Description:  Dialog for specification of FFT options.
// History:

#ifndef FFT_DIALOG_H_
#define FFT_DIALOG_H_

// wxWidgets headers
#include <wx/wx.h>

// Local headers
#include "lp2d/utilities/signals/fft.h"

namespace LibPlot2D
{

class FFTDialog : public wxDialog
{
public:
	// Constructor
	FFTDialog(wxWindow *parent, const unsigned int &dataPoints,
		const unsigned int &zoomDataPoints, const double &sampleTime);

	FastFourierTransform::FFTWindow GetFFTWindow() const;
	unsigned int GetWindowSize() const;
	double GetOverlap() const;
	bool GetUseZoomedData() const;
	bool GetSubtractMean() const;

private:
	void CreateControls();
	wxSizer* CreateInputControls();
	wxSizer* CreateOutputControls();
	void ConfigureControls();
	void SetCheckBoxDefaults();

	unsigned int dataPoints;
	unsigned int zoomDataPoints;
	double sampleTime;

	unsigned int GetPointCount() const;

	wxComboBox *windowSizeCombo;
	wxComboBox *windowCombo;
	wxTextCtrl *overlapTextBox;
	wxCheckBox *useZoomCheckBox;
	wxCheckBox *subtractMeanCheckBox;

	wxStaticText *frequencyRange;
	wxStaticText *frequencyResolution;
	wxStaticText *numberOfAverages;

	virtual bool TransferDataFromWindow();

	void OnCheckBoxEvent(wxCommandEvent &event);
	void OnComboBoxEvent(wxCommandEvent &event);
	void OnTextBoxEvent(wxCommandEvent &event);

	wxArrayString GetWindowList() const;
	void UpdateOutputControls();

	DECLARE_EVENT_TABLE()
};

}// namespace LibPlot2D

#endif// FFT_DIALOG_H_