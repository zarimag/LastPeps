// Wrapper.h
#pragma once
#include "Computations.h"
using namespace System;

namespace Wrapper {

	public ref class WrapperClass
	{
		private:
			double confidenceInterval;
			double prix;
			double PL;
			array<double> ^delta;
			array<double> ^ic;


			array<double> ^curr;
			array<double> ^past;
		public:

			array<double> ^ptfValues;
			array<double> ^cashValues;
			//Temp constructor for a basket option
			WrapperClass() { 
				confidenceInterval = prix = 0; 
				PL = 0;
				
		/*		//Livre
				dividend[option_size] = 0.04;
				//Dollars
				dividend[option_size+1] = 0.035;
				//CHF
				dividend[option_size + 2] = 0.045;
				//Yen
				dividend[option_size + 3] = 0.03;*/

				/*curr = gcnew array<double>(option_size);
				for (int i = 0; i < option_size; i++){
					curr[i] = -1;
				}
				
				curr[2] = 20;
				curr[3] = 20;
				curr[4] = 20;
				curr[5] = 21;
				curr[6] = 21;
				curr[7] = 22;
				curr[9] = 20;
				curr[10] = 20;
				curr[11] = 21;
				curr[12] = 21;
				curr[13] = 21;
				curr[14] = 21;
				curr[15] = 21;
				curr[16] = 23;
				delta = gcnew array<double>(option_size+nb_curr);
				ic = gcnew array<double>(option_size+nb_curr);*/

				//TODO: instantiate delta et ic au premier appel de computePrice
			};

			void computePrice(array<double>^ stockPrices, array<double>^ interestRates, array<double>^ stockToFxIndex, 
				int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int nbDatesStockPrices, double finiteDifferenceStep);
			void computePrice(double t);
			void computeDelta(array<double>^ stockPrices, array<double>^ interestRates, array<double>^ stockToFxIndex,
				int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int nbDatesStockPrices, double finiteDifferenceStep);
			void computeDelta(double t);
			void computeHedge(array<double>^ stockPrices, array<double>^ interestRates, array<double>^ stockToFxIndex,
				int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int dimStockPast, double finiteDifferenceStep, double H);
			void computeVol();
			double getPrice() { return prix; };
			double getIC() { return confidenceInterval; };
			double getPL() { return PL; };
			array<double> ^getDelta(){ return delta; };
			array<double> ^getDeltaIC(){ return ic; };
	};
}
