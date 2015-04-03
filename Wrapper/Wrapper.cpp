// This is the main DLL file.
#include "stdafx.h"

#include "Wrapper.h"
using namespace Computations;
	namespace Wrapper {
		void WrapperClass::computePrice(array<double>^ stockPrices, array<double>^ interestRates, array<double>^ stockToFxIndex,
			int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int nbDatesStockPrices, double finiteDifferenceStep){
			
			double ic, px;

			pin_ptr<double> stocktoCurrIndex = &stockToFxIndex[0];
			pin_ptr<double> interestRate = &interestRates[0];
			pin_ptr<double> stocks = &stockPrices[0];

			compute_price(ic, px, stocks, interestRate, stocktoCurrIndex, assetNb, fxNb, maturity, mcSamples, timeSteps, nbDatesStockPrices, finiteDifferenceStep);
			this->confidenceInterval = ic;
			this->prix = px;
			
		}

		void WrapperClass::computePrice(double t){
			/*
			double ic, px;
			pin_ptr<double> pSpot = &spot[0];
			pin_ptr<double> pSigma = &sigma[0];
			pin_ptr<double> pTrend = &trend[0];
			pin_ptr<double> pCoeff = &coeff[0];
			pin_ptr<double> pDividend = &dividend[0];
			pin_ptr<double> pCurr = &curr[0];
			pin_ptr<double> pPast = &past[0];
			compute_price(ic, px,t, pPast, option_size + nb_curr, pDividend, pCurr, pSpot, pSigma, pTrend, r, rho, h, H, maturity, timeSteps, pCoeff, samples, option_size);

			this->confidenceInterval = ic;
			this->prix = px;*/


		}
		void WrapperClass::computeDelta(array<double>^ stockPrices, array<double>^ interestRates, array<double>^ stockToFxIndex,
			int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int nbDatesStockPrices, double finiteDifferenceStep){

			delta = gcnew array<double>(assetNb + fxNb);
			pin_ptr<double> pDelta = &delta[0];
			pin_ptr<double> stocktoCurrIndex = &stockToFxIndex[0];
			pin_ptr<double> interestRate = &interestRates[0];
			pin_ptr<double> stocks = &stockPrices[0];

			compute_delta(pDelta, stocks, interestRate, stocktoCurrIndex, assetNb, fxNb, 
				maturity, mcSamples, timeSteps, nbDatesStockPrices, finiteDifferenceStep);
			
		}

		void WrapperClass::computeDelta(double t){
			//pin_ptr<double> pDeltaIc = &ic[0];
			//
			//pin_ptr<double> pSpot = &spot[0];
			//pin_ptr<double> pSigma = &sigma[0];
			//pin_ptr<double> pTrend = &trend[0];
			//pin_ptr<double> pCoeff = &coeff[0];
			//pin_ptr<double> pDividend = &dividend[0];
			//pin_ptr<double> pCurr = &curr[0];
			//pin_ptr<double> pPast = &past[0];
			//compute_delta(pDelta, pDeltaIc, t, pPast, option_size + nb_curr, pDividend, pCurr, pSpot, pSigma, pTrend, r, rho, h, H, maturity, timeSteps, pCoeff, samples, option_size);
		
		}

		void WrapperClass::computeVol(){
			/**/
			/*pin_ptr<double> pSpot = &spot[0];
			pin_ptr<double> pSigma = &sigma[0];
			pin_ptr<double> pTrend = &trend[0];
			pin_ptr<double> pCoeff = &coeff[0];
			pin_ptr<double> pDividend = &dividend[0];
			pin_ptr<double> pCurr = &curr[0];
			pin_ptr<double> pHist = &historicalStockPrices[0];
			compute_vol(pHist, option_size + nb_curr, pDividend, pCurr, pSpot, pSigma, pTrend, r, rho, h, H, maturity, timeSteps, pCoeff, samples, option_size);
*/
		}


		void WrapperClass::computeHedge(array<double>^ stockPrices, array<double>^ interestRates, array<double>^ stockToFxIndex,
			int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int dimStockPast, double finiteDifferenceStep, double H){
			
			double pnl;
			ptfValues = gcnew array<double>(H + 1);
			cashValues = gcnew array<double>(H + 1);
			pin_ptr<double> V = &cashValues[0];
			pin_ptr<double> ptfV = &ptfValues[0];
			pin_ptr<double> stocktoCurrIndex = &stockToFxIndex[0];
			pin_ptr<double> stocks = &stockPrices[0];
			pin_ptr<double> interestRate = &interestRates[0];
			
			compute_hedge(V, ptfV, stocks, interestRate, stocktoCurrIndex, assetNb, fxNb, maturity,
				mcSamples, timeSteps, dimStockPast, finiteDifferenceStep, H);
		

			double Pl;/*
			pin_ptr<double> pSpot = &spot[0];
			pin_ptr<double> pSigma = &sigma[0];
			pin_ptr<double> pTrend = &trend[0];
			pin_ptr<double> pCoeff = &coeff[0];
			pin_ptr<double> pDividend = &dividend[0];
			pin_ptr<double> pCurr = &curr[0];
			compute_hedge(Pl, option_size + nb_curr, pDividend, pCurr, pSpot, pSigma, pTrend, r, rho, h, H, maturity, timeSteps, pCoeff, samples, option_size);
			this->PL = Pl;*/
		}
}