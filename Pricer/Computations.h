#pragma once
#define DLLEXP   __declspec( dllexport )
namespace Computations{

	DLLEXP void testmethod();

	DLLEXP void compute_price(double &ic, double &prix, double *stockPrices, double *interestRates,
		double *stockToFxIndex, int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int dimStockPast, double finiteDifferenceStep);

	DLLEXP void compute_price(double &ic, double &prix,double t,double* past, int option_size, double* dividend, 
		double* curr, double *spot, double *sigma, double* trend, double r, double rho, double h, int H, double maturity, int timeSteps, double* payoffCoeff, int samples, int sizeAsset);

	DLLEXP void compute_vol(double*historicalPrice, int option_size, double* dividend, double* curr, double *spot, double *sigma, double* trend, double r, double rho, double h, int H, double maturity, int timeSteps, double* payoffCoeff, int samples, int sizeAsset);

	DLLEXP void compute_delta(double *delta, double *stockPrices, double *interestRates,
		double *stockToFxIndex, int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int dimStockPast, double finiteDifferenceStep);
	
	DLLEXP void compute_delta(double *deltaVect, double *icVect, double t, double* past, int option_size, double *spot, double* dividend, double* curr, double *sigma, double* trend, double r, double rho, double h, int H, double maturity, int timeSteps, double* payoffCoeff, int samples, int sizeAsset);

	DLLEXP void compute_hedge(double *V, double *ptfV, double *stockPrices, double *interestRates, double * stockToFxIndex,
		int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int dimStockPast,
		double finiteDifferenceStep, double H);
}