/*=============================================================================
                                   LibPlot2D
                       Copyright Kerry R. Loux 2011-2016

                  This code is licensed under the GPLv2 License
                    (http://opensource.org/licenses/GPL-2.0).
=============================================================================*/

// File:  complex.cpp
// Date:  5/2/2011
// Auth:  K. Loux
// Desc:  Contains class functionality for complex number class.

// Standard C++ headers
#include <iostream>

// wxWidgets headers
#include <wx/wx.h>

// Local headers
#include "lp2d/utilities/math/complex.h"

namespace LibPlot2D
{

//=============================================================================
// Class:			Complex
// Function:		Complex
//
// Description:		Constructor for the Complex class.
//
// Input Arguments:
//		real		= const double& real component of the complex number
//		imaginary	= const double& imaginary component of the complex number
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//=============================================================================
Complex::Complex(const double &real, const double &imaginary) : mReal(real),
	mImaginary(imaginary)
{
}

//=============================================================================
// Class:			Complex
// Function:		Constant Definitions
//
// Description:		Defines class level constants for the Complex class.
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
//=============================================================================
const Complex Complex::mI(0.0, 1.0);

//=============================================================================
// Class:			friend of Complex
// Function:		operator <<
//
// Description:		Writes the value of Target to a stream.
//
// Input Arguments:
//		writeOut	= ostream& to write out to
//		complex		= const Complex& value to be written to the stream
//
// Output Arguments:
//		None
//
// Return Value:
//		&ostream containing the formatted value
//
//=============================================================================
std::ostream &operator << (std::ostream &writeOut, const Complex &complex)
{
	writeOut << complex.Print();

	return writeOut;
}

//=============================================================================
// Class:			Complex
// Function:		Print
//
// Description:		Prints this object to a string.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		wxString containing the formatted value of this object
//
//=============================================================================
wxString Complex::Print() const
{
	wxString temp;

	if (mImaginary >= 0)
		temp.Printf("%0.3f + %0.3f i", mReal, mImaginary);
	else
		temp.Printf("%0.3f - %0.3f i", mReal, -mImaginary);

	return temp;
}

//=============================================================================
// Class:			Complex
// Function:		operator+
//
// Description:		Addition operator for the Complex class.
//
// Input Arguments:
//		complex	= const Complex& to add to this
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex result of the addition
//
//=============================================================================
const Complex Complex::operator+(const Complex &complex) const
{
	// Make a copy of this object
	Complex temp = *this;

	// Do the addition
	temp += complex;

	return temp;
}

//=============================================================================
// Class:			Complex
// Function:		operator-
//
// Description:		Subraction operator for the Complex class.
//
// Input Arguments:
//		complex	= const Complex& to subtract from this
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex result of the subtraction
//
//=============================================================================
const Complex Complex::operator-(const Complex &complex) const
{
	// Make a copy of this object
	Complex temp = *this;

	// Do the subtraction
	temp -= complex;

	return temp;
}

//=============================================================================
// Class:			Complex
// Function:		operator*
//
// Description:		Multiplication operator for the Complex class.
//
// Input Arguments:
//		complex	= const Complex& to multiply with this
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex result of the multiplication
//
//=============================================================================
const Complex Complex::operator*(const Complex &complex) const
{
	// Make a copy of this object
	Complex temp = *this;

	// Do the multiplication
	temp *= complex;

	return temp;
}

//=============================================================================
// Class:			Complex
// Function:		operator/
//
// Description:		Division operator for the Complex class.
//
// Input Arguments:
//		complex	= const Complex& to divide by this
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex result of the division
//
//=============================================================================
const Complex Complex::operator/(const Complex &complex) const
{
	// Make a copy of this object
	Complex temp = *this;

	// Do the division
	temp /= complex;

	return temp;
}

//=============================================================================
// Class:			Complex
// Function:		ToPower
//
// Description:		Raises this object to the specified power.
//
// Input Arguments:
//		Power	= const double& specifiying the power to which this will be raised
//
// Output Arguments:
//		None
//
// Return Value:
//		Complex& result of the power
//
//=============================================================================
Complex& Complex::ToPower(const double &power)
{
	// Convert this from Cartesian to polar form
	double r = sqrt(mReal * mReal + mImaginary * mImaginary);
	double theta = atan2(mImaginary, mReal);

	// Use De Moivre's theorem to raise this to a power
	r = pow(r, power);
	theta *= power;

	// Convert back to Cartesian form
	mReal = r * cos(theta);
	mImaginary = r * sin(theta);

	return *this;
}

//=============================================================================
// Class:			Complex
// Function:		ToPower
//
// Description:		Raises this object to the specified power (const version).
//
// Input Arguments:
//		power	= const double& specifiying the power to which this will be raised
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex result of the power
//
//=============================================================================
const Complex Complex::ToPower(const double &power) const
{
	Complex result(*this);
	result.ToPower(power);

	return result;
}

//=============================================================================
// Class:			Complex
// Function:		ToPower
//
// Description:		Raises this object to the specified complex power.
//
// Input Arguments:
//		power	= const Complex& specifiying the power to which this will be raised
//
// Output Arguments:
//		None
//
// Return Value:
//		Complex& result of the power
//
//=============================================================================
Complex& Complex::ToPower(const Complex &power)
{
	Complex original(*this);

	double r = GetPolarLength();
	double theta = GetPolarAngle();

	double factor = pow(r, power.mReal) * exp(-power.mImaginary * theta);
	double angle = power.mImaginary * log(r) + power.mReal * theta;

	mReal = factor * cos(angle);
	mImaginary = factor * sin(angle);

	return *this;
}

//=============================================================================
// Class:			Complex
// Function:		ToPower
//
// Description:		Raises this object to the specified complex power (const version).
//
// Input Arguments:
//		power	= const Complex& specifiying the power to which this will be raised
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex result of the power
//
//=============================================================================
const Complex Complex::ToPower(const Complex &power) const
{
	Complex result(*this);

	return result.ToPower(power);
}

//=============================================================================
// Class:			Complex
// Function:		operator+=
//
// Description:		Addition assignment operator for the Complex class.
//
// Input Arguments:
//		complex	= const Complex& to add to this
//
// Output Arguments:
//		None
//
// Return Value:
//		Complex& result of the addition
//
//=============================================================================
Complex& Complex::operator+=(const Complex &complex)
{
	mReal += complex.mReal;
	mImaginary += complex.mImaginary;

	return *this;
}

//=============================================================================
// Class:			Complex
// Function:		operator-=
//
// Description:		Subraction assignment operator for the Complex class.
//
// Input Arguments:
//		complex	= const Complex& to subtract from this
//
// Output Arguments:
//		None
//
// Return Value:
//		Complex& result of the subtraction
//
//=============================================================================
Complex& Complex::operator-=(const Complex &complex)
{
	mReal -= complex.mReal;
	mImaginary -= complex.mImaginary;

	return *this;
}

//=============================================================================
// Class:			Complex
// Function:		operator *=
//
// Description:		Multiplication assignment operator for the Complex class.
//
// Input Arguments:
//		complex	= const Complex& to multiply by this
//
// Output Arguments:
//		None
//
// Return Value:
//		Complex& result of the multiplication
//
//=============================================================================
Complex& Complex::operator*=(const Complex &complex)
{
	double temp(mReal);

	// Similar to a dot product, the real component of the result
	// is the sum of the products of the like components
	mReal = mReal * complex.mReal - mImaginary * complex.mImaginary;

	// Similar to a cross product, the imaginary component of the
	// result is the sum of the products of the opposite components
	mImaginary = temp * complex.mImaginary + mImaginary * complex.mReal;

	return *this;
}

//=============================================================================
// Class:			Complex
// Function:		operator/=
//
// Description:		Division assignment operator for the Complex class.
//
// Input Arguments:
//		complex	= const Complex& to divide by this
//
// Output Arguments:
//		None
//
// Return Value:
//		Complex& result of the division
//
//=============================================================================
Complex& Complex::operator/=(const Complex &complex)
{
	double temp(mReal);

	// Compute the real portion of the result
	mReal = (mReal * complex.mReal + mImaginary * complex.mImaginary) /
		(complex.mReal * complex.mReal + complex.mImaginary * complex.mImaginary);

	// Compute the imaginary portion of the result
	mImaginary = (mImaginary * complex.mReal - temp * complex.mImaginary) /
		(complex.mReal * complex.mReal + complex.mImaginary * complex.mImaginary);

	return *this;
}

//=============================================================================
// Class:			Complex
// Function:		operator==
//
// Description:		Equal to comparison operator for the Complex class.
//
// Input Arguments:
//		complex	= const Complex& to compare to this
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, true for equal to, false for not equal to
//
//=============================================================================
bool Complex::operator==(const Complex &complex) const
{
	// Check to see if both the real and imaginary components are equal
	if (mReal == complex.mReal && mImaginary == complex.mImaginary)
		return true;
	else
		return false;
}

//=============================================================================
// Class:			Complex
// Function:		operator !=
//
// Description:		Not equal to comparison operator for the Complex class.
//
// Input Arguments:
//		complex	= const Complex& to compare to this
//
// Output Arguments:
//		None
//
// Return Value:
//		bool, false for equal to, true for not equal to
//
//=============================================================================
bool Complex::operator!=(const Complex &complex) const
{
	return !(*this == complex);
}

//=============================================================================
// Class:			Complex
// Function:		GetConjugate
//
// Description:		Returns the complex conjugate of this object.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex with the same real part as this and an imaginary component
//		with opposite magnitude as this
//
//=============================================================================
const Complex Complex::GetConjugate() const
{
	Complex temp;

	// Direct assignment of the real component
	temp.mReal = mReal;

	// The imaginary part is the opposite of this
	temp.mImaginary = -mImaginary;

	return temp;
}

//=============================================================================
// Class:			Complex
// Function:		GetPolarLength
//
// Description:		Converts to polar coordinates, returns the length (r).
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		double
//
//=============================================================================
double Complex::GetPolarLength() const
{
	return sqrt(mReal * mReal + mImaginary * mImaginary);
}

//=============================================================================
// Class:			Complex
// Function:		GetPolarAngle
//
// Description:		Converts to polar coordinates, returns the angle (theta).
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		double
//
//=============================================================================
double Complex::GetPolarAngle() const
{
	return atan2(mImaginary, mReal);
}

//=============================================================================
// Class:			Complex
// Function:		operator+
//
// Description:		Addition operator for the Complex class.
//
// Input Arguments:
//		value	= const double& to add to this
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex result of the addition
//
//=============================================================================
const Complex Complex::operator+(const double &value) const
{
	Complex temp;

	// Add the real component
	temp.mReal = mReal + value;

	// Direct assignment of the imaginary componet
	temp.mImaginary = mImaginary;

	return temp;
}

//=============================================================================
// Class:			Complex
// Function:		operator-
//
// Description:		Subtraction operator for the Complex class.
//
// Input Arguments:
//		value	= const double& to subtract from this
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex result of the subtraction
//
//=============================================================================
const Complex Complex::operator-(const double &value) const
{
	Complex temp;

	// Subtract the real component
	temp.mReal = mReal - value;

	// Direct assignment of the imaginary component
	temp.mImaginary = mImaginary;

	return temp;
}

//=============================================================================
// Class:			Complex
// Function:		operator*
//
// Description:		Multiplication operator for the Complex class.
//
// Input Arguments:
//		value	= const double& to multiply by this
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex result of the multiplication
//
//=============================================================================
const Complex Complex::operator*(const double &value) const
{
	Complex temp;

	// Perform the multiplication on both components
	temp.mReal = mReal * value;
	temp.mImaginary = mImaginary * value;

	return temp;
}

//=============================================================================
// Class:			Complex
// Function:		operator/
//
// Description:		Division operator for the Complex class.
//
// Input Arguments:
//		value	= const double& to divide by this
//
// Output Arguments:
//		None
//
// Return Value:
//		const Complex result of the division
//
//=============================================================================
const Complex Complex::operator/(const double &value) const
{
	Complex temp;

	// Perform the division on both components
	temp.mReal = mReal / value;
	temp.mImaginary = mImaginary / value;

	return temp;
}

}// namespace LibPlot2D
