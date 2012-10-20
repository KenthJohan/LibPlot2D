/*===================================================================================
                                    DataPlotter
                          Copyright Kerry R. Loux 2011-2012

     No requirement for distribution of wxWidgets libraries, source, or binaries.
                             (http://www.wxwidgets.org/)

===================================================================================*/

// File:  filter.cpp
// Created:  5/16/2011
// Author:  K. Loux
// Description:  Base class (abstract) for digital filters.
// History:

// Standard C++ headers
#include <cstdlib>
#include <algorithm>

// Local headers
#include "utilities/signals/filter.h"
#include "utilities/math/expressionTree.h"
#include "utilities/math/plotMath.h"

//==========================================================================
// Class:			Filter
// Function:		Filter
//
// Description:		Constructor for the Filter class.
//
// Input Arguments:
//		_sampleRate		= const double& specifying the sampling rate in Hz
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
Filter::Filter(const double &_sampleRate) : sampleRate(_sampleRate)
{
	a = NULL;
	b = NULL;
	u = NULL;
	y = NULL;
}

//==========================================================================
// Class:			Filter
// Function:		Filter
//
// Description:		Constructor for the Filter class for arbitrary filters.
//					Passed arguments are assumed to be for a continuous time
//					filter (s-domain), and will be translated into filter
//					coefficients according to the specified sample rate.
//
// Input Arguments:
//		_sampleRate		= const double& specifying the sampling rate in Hz
//		numerator		= const std::vector<double> containing the numerator
//						  coefficients from highest power to zero power
//		denominaotr		= const std::vector<double> containing the denominator
//						  coefficients from highest power to zero power
//		initialValue	= const double&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
Filter::Filter(const double &_sampleRate, const std::vector<double> &numerator,
	const std::vector<double> &denominator, const double &initialValue) : sampleRate(_sampleRate)
{
	GenerateCoefficients(numerator, denominator);
	Initialize(initialValue);
}

//==========================================================================
// Class:			Filter
// Function:		Filter
//
// Description:		Copy constructor for the Filter class.
//
// Input Arguments:
//		f	= const Filter&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
Filter::Filter(const Filter &f) : sampleRate(f.sampleRate)
{
	*this = f;
}

//==========================================================================
// Class:			Filter
// Function:		~Filter
//
// Description:		Destructor for the Filter class.
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
Filter::~Filter()
{
	DeleteArrays();
}

//==========================================================================
// Class:			Filter
// Function:		GenerateCoefficients
//
// Description:		Generates the discrete-time (z-domain) coefficients for
//					a filter equivalent to the continuous-time (s-domain)
//					arguments.  Uses bilinear transform:
//					s = 2 * (1 - z^-1) / (T * (1 + z^-1)).
//
// Input Arguments:
//		numerator	= const std::vector<double>& continuous time coefficients,
//					  highest power of s to lowest power of s
//		denominator	= const std::vector<double>& continuous time coefficients,
//					  highest power of s to lowest power of s
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void Filter::GenerateCoefficients(const std::vector<double> &numerator,
	const std::vector<double> &denominator)
{
	unsigned int highestPower = std::max(numerator.size(), denominator.size()) - 1;
	std::string numString = AssembleZExpression(numerator, highestPower);
	std::string denString = AssembleZExpression(denominator, highestPower);

	std::vector<double> zNum = CoefficientsFromString(numString);
	std::vector<double> zDen = CoefficientsFromString(denString);
	AllocateArrays(zNum.size(), zDen.size());

	unsigned int i;
	for (i = 0; i < zNum.size(); i++)
		a[i] = zNum[i];
	for (i = 0; i < zDen.size(); i++)
		b[i] = zDen[i];
}

//==========================================================================
// Class:			Filter
// Function:		AssembleZExpression
//
// Description:		Assembles the z-domain expression equivalent to the s-domain
//					coefficients provided.
//
// Input Arguments:
//		coefficients	= const std::vector<double>& continuous time coefficients,
//						  highest power of s to lowest power of s
//		highestPower	= const unsigned int&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::string
//
//==========================================================================
std::string Filter::AssembleZExpression(const std::vector<double>& coefficients,
	const unsigned int &highestPower) const
{
	const unsigned int tempSize(256);
	char temp[tempSize];
#ifdef __WXWIN__
	sprintf_s(temp, tempSize, "(%f*(1+z^-1))", 1.0 / sampleRate);
#else
	sprintf(temp, "(%f*(1+z^-1))", 1.0 / sampleRate);
#endif
	std::string posBilinTerm(temp), negBilinTerm("(2*(1-z^-1))");
	std::string result;

	unsigned int i;
	for (i = 0; i < coefficients.size(); i++)
	{
		if (PlotMath::IsZero(coefficients[i]))
			continue;

#ifdef __WXWIN__
		sprintf_s(temp, tempSize, "%f", coefficients[i]);
#else
		sprintf(temp, "%f", coefficients[i]);
#endif
		if (!result.empty() && coefficients[i] > 0.0)
			result.append("+");
		result.append(temp);

		if (coefficients.size() - 1 > i)
			result.append("*" + negBilinTerm);
		if (coefficients.size() - 1 > 1 + i)
		{
#ifdef __WXWIN__
			sprintf_s(temp, tempSize, "^%lu", coefficients.size() - i - 1);
#else
			sprintf(temp, "^%lu", coefficients.size() - i - 1);
#endif
			result.append(temp);
		}

		if (highestPower + i > coefficients.size() - 1)
			result.append("*" + posBilinTerm);
		if (highestPower + i > coefficients.size())
		{
#ifdef __WXWIN__
				sprintf_s(temp, tempSize, "^%lu", highestPower - coefficients.size() + 1 + i);
#else
				sprintf(temp, "^%lu", highestPower - coefficients.size() + 1 + i);
#endif
			result.append(temp);
		}
	}

	return result;
}

//==========================================================================
// Class:			Filter
// Function:		operator=
//
// Description:		Assignment operator.
//
// Input Arguments:
//		f	=	const Filter&
//
// Output Arguments:
//		None
//
// Return Value:
//		Filter&, reference to this
//
//==========================================================================
Filter& Filter::operator=(const Filter &f)
{
	if (this == &f)
		return *this;

	DeleteArrays();

	a = new double[f.inSize];
	b = new double[f.outSize];
	u = new double[f.inSize];
	y = new double[f.outSize];

	unsigned int i;
	for (i = 0; i < inSize; i++)
	{
		a[i] = f.a[i];
		u[i] = f.u[i];
	}

	for (i = 0; i < outSize; i++)
	{
		b[i] = f.b[i];
		y[i] = f.y[i];
	}

	return *this;
}

//==========================================================================
// Class:			Filter
// Function:		DeleteArrays
//
// Description:		Deletes dynamically allocated memory for this object.
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
void Filter::DeleteArrays(void)
{
	delete [] a;
	delete [] b;
	delete [] y;
	delete [] u;
}

//==========================================================================
// Class:			Filter
// Function:		Initialize
//
// Description:		Initializes the filter to the specified value.
//
// Input Arguments:
//		initialValue	= const double&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void Filter::Initialize(const double &initialValue)
{
	unsigned int i;
	for (i = 0; i < inSize; i++)
		u[i] = initialValue;

	for (i = 0; i < outSize; i++)
		y[i] = initialValue;
}

//==========================================================================
// Class:			Filter
// Function:		Apply
//
// Description:		Applies the filter to the new input value.
//
// Input Arguments:
//		_u	= const double&
//
// Output Arguments:
//		None
//
// Return Value:
//		double containing the filtered value
//
//==========================================================================
double Filter::Apply(const double &_u)
{
	ShiftArray(u, inSize);
	u[0] = _u;

	ShiftArray(y, outSize);

	y[0] = 0.0;
	unsigned int i;
	for (i = 0; i < inSize; i++)
		y[0] += a[i] * u[i];
	for (i = 1; i < outSize; i++)
		y[0] -= b[i] * y[i];
	y[0] /= b[0];

	return y[0];
}

//==========================================================================
// Class:			Filter
// Function:		ShiftArray
//
// Description:		Shifts the array values by one index (value with highest
//					index is lost).
//
// Input Arguments:
//		s		= double*
//		size	= const unsigned int& indicating the size of the array
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void Filter::ShiftArray(double *s, const unsigned int &size)
{
	unsigned int i;
	for (i = size - 1; i > 0; i--)
		s[i] = s[i - 1];
}

//==========================================================================
// Class:			Filter
// Function:		AllocateArrays
//
// Description:		Allocates the coefficient and input/output storage arrays.
//
// Input Arguments:
//		_inSize		= const unsigned int&
//		_outSize	= const unsigned int&
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
void Filter::AllocateArrays(const unsigned int &_inSize, const unsigned int &_outSize)
{
	inSize = _inSize;
	outSize = _outSize;

	a = new double[inSize];
	b = new double[outSize];
	u = new double[inSize];
	y = new double[outSize];
}

//==========================================================================
// Class:			Filter
// Function:		CoefficientsFromString
//
// Description:		Creates a vector of coefficients from highes power to lowest
//					power, based on a string representing the expression.
//
// Input Arguments:
//		s	= const std::string&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::vector<double>
//
//==========================================================================
std::vector<double> Filter::CoefficientsFromString(const std::string &s)
{
	ExpressionTree e;
	std::string expression;
	std::string errorString = e.Solve(s, expression);
	if (!errorString.empty())
	{
		// FIXME:  Warning here?
	}
	// FIXME:  Can we cound on it always going Coefficient * s ^ power?  Or could it be s ^ power * Coefficient?

	std::vector<std::pair<int, double> > terms =
		ExpressionTree::FindPowersAndCoefficients(ExpressionTree::BreakApartTerms(expression));

	terms = CollectLikeTerms(terms);
	// FIXME:  What if a power is missing:  need to insert a zero coefficient element for that power
	std::sort(terms.begin(), terms.end());

	std::vector<double> coefficients;
	unsigned int i;
	for (i = 0; i < terms.size(); i++)
		coefficients.push_back(terms[terms.size() - 1 - i].second);

	return coefficients;
}

//==========================================================================
// Class:			Filter
// Function:		CollectLikeTerms
//
// Description:		Collects all terms with the same exponent and adds the
//					coefficients together.
//
// Input Arguments:
//		terms	= std::vector<std::pair<int, double> >
//
// Output Arguments:
//		None
//
// Return Value:
//		std::vector<std::pair<int, double> >
//
//==========================================================================
std::vector<std::pair<int, double> > Filter::CollectLikeTerms(std::vector<std::pair<int, double> > terms)
{
	unsigned int i, j;
	for (i = 0; i < terms.size(); i++)
	{
		for (j = i + 1; j < terms.size(); j++)
		{
			if (terms[i].first == terms[j].first)
			{
				terms[i].second += terms[j].second;
				terms.erase(terms.begin() + j);
				j--;
			}
		}
	}

	return terms;
}