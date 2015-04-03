#include "Computations.h"
#include "mc.h"
#include "iostream"
#include "ctime"
#include "pnl/pnl_random.h"
using namespace std;


void Computations::testmethod() {

}

void Computations::compute_price(double &ic, double &prix, double *stockPrices, double *interestRates, 
			double *stockToFxIndex, int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int dimStockPast, double finiteDifferenceStep)
{
	PnlMat* stocksPx = pnl_mat_create_from_ptr(dimStockPast, assetNb+fxNb, stockPrices);

	MonteCarlo* mc = new MonteCarlo(maturity, timeSteps, assetNb+fxNb, interestRates[0], stockToFxIndex, 
		interestRates + 1, stocksPx, mcSamples, assetNb, dimStockPast, finiteDifferenceStep);
	PnlMat* marketPath = pnl_mat_create_from_ptr(dimStockPast, assetNb+fxNb, stockPrices);
	mc->mod_->calibrate(marketPath, 1/252.);
	mc->price(prix,ic);
	pnl_mat_free(&marketPath);
	pnl_mat_free(&stocksPx);
	delete mc;
}

void Computations::compute_price(double &ic, double &prix,double t,double *past, int option_size, double* dividend, double* curr, double *spot, double *sigma, double* trend, double r, double rho, double h, int H, double maturity, int timeSteps, double* payoffCoeff, int samples, int sizeAsset)
{

	//int optionType_;
	//MonteCarlo* mc = new MonteCarlo(maturity, timeSteps, option_size, optionType_, r, rho, curr, dividend, sigma, spot, trend, samples, sizeAsset);
	////m rows and n columns
	//double step = maturity / timeSteps;
	////double dt = t / step;
	//int m;
	////valeur exacte
	//if (floor(t)==t){
	//	m = floor(t);
	//}
	//else{
	//	m = floor(t)+1;
	//}
	////We have to include the spot price
	//m++;
	////PnlMat* pastMat=pnl_mat_create_from_zero(timeSteps+1, option_size);
	//PnlMat* pastMat = pnl_mat_create_from_ptr(m,option_size,past);
	//mc->price(pastMat,t,prix, ic);
	//delete mc;
}


void Computations::compute_delta(double *delta, double *stockPrices, double *interestRates,
	double *stockToFxIndex, int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int dimStockPast, double finiteDifferenceStep)
{
	PnlMat* stocksPx = pnl_mat_create_from_ptr(dimStockPast, assetNb + fxNb, stockPrices);

	MonteCarlo* mc = new MonteCarlo(maturity, timeSteps, assetNb + fxNb, interestRates[0], stockToFxIndex,
		interestRates + 1, stocksPx, mcSamples, assetNb, dimStockPast, finiteDifferenceStep);
	PnlMat* marketPath = pnl_mat_create_from_ptr(dimStockPast, assetNb + fxNb, stockPrices);
	mc->mod_->calibrate(marketPath, 1 / 252.);

	PnlVect* deltaVect = pnl_vect_create(assetNb + fxNb);
	PnlMat* past = pnl_mat_create(1, assetNb + fxNb);
	PnlVect* spot_ = pnl_vect_create(assetNb + fxNb);

	pnl_mat_get_row(spot_, stocksPx, dimStockPast - 1);
	pnl_mat_set_row(past, spot_, 0);
	mc->delta(past, 0.0, deltaVect);
	for (int i = 0; i < assetNb + fxNb; i++){
		delta[i]= GET(deltaVect,i);
		//ic[i]= GET(icVect,i);
	}
	delete mc;
	pnl_mat_free(&marketPath);
	pnl_vect_free(&deltaVect);
	pnl_mat_free(&past);
	pnl_vect_free(&spot_);
}

void Computations::compute_delta(double *delta, double *ic, double t, double *past, int option_size, double* dividend, double* curr, double *spot, double *sigma, double* trend, double r, double rho, double h, int H, double maturity, int timeSteps, double* payoffCoeff, int samples, int sizeAsset)
{
	//int optionType_;
	//MonteCarlo* mc = new MonteCarlo(maturity, timeSteps, option_size, optionType_, r, rho, curr, dividend, sigma, spot, trend, samples, sizeAsset);
	//PnlVect* deltaVect = pnl_vect_new();
	//deltaVect = pnl_vect_create(option_size);
	////PnlVect* icVect = pnl_vect_new();
	////icVect = pnl_vect_create(option_size);
	//int m;
	////valeur exacte
	//if (floor(t) == t){
	//	m = floor(t);
	//}
	//else{
	//	m = floor(t) + 1;
	//}
	////We have to include the spot price
	//m++;
	////PnlMat* pastMat=pnl_mat_create_from_zero(timeSteps+1, option_size);
	//PnlMat* pastMat = pnl_mat_create_from_ptr(m, option_size, past);

	//PnlVect* spot_ = pnl_vect_create_from_ptr(option_size, spot);
	//
	//mc->delta(pastMat, t, deltaVect);
	//for (int i = 0; i < option_size; i++){
	//	delta[i] = GET(deltaVect, i);
	//	//ic[i]= GET(icVect,i);
	//}
	//delete mc;
	//pnl_vect_free(&deltaVect);
	//pnl_mat_free(&pastMat);
	//pnl_vect_free(&spot_);
}

void Computations::compute_vol(double*historicalPrice, int option_size, double* dividend, double* curr, double *spot, double *sigma, double* trend, double r, double rho, double h, int H, double maturity, int timeSteps, double* payoffCoeff, int samples, int sizeAsset){
	int optionType_;

	//MonteCarlo* mc = new MonteCarlo(maturity, timeSteps, option_size, optionType_, r, rho, curr, dividend, sigma, spot, trend, samples, sizeAsset);
	//PnlVect* volVect = pnl_vect_new();
	//volVect = pnl_vect_create(option_size);
	//PnlMat* histMat = pnl_mat_create_from_ptr(63, option_size, historicalPrice);
	//mc->estimVolHistMethod(histMat, volVect, 1 );
	//for (int i = 0; i < option_size; i++){
	//	sigma[i] = GET(volVect, i);
	//	//ic[i]= GET(icVect,i);
	//}
	//delete mc;
	//pnl_vect_free(&volVect);
}



void Computations::compute_hedge(double *V, double *ptfV, double *stockPrices, double *interestRates, double * stockToFxIndex, 
	int assetNb, int fxNb, double maturity, int mcSamples, int timeSteps, int dimStockPast,
				 double finiteDifferenceStep, double H){
	int calibrationValuesNb = dimStockPast / 10;
	PnlMat *calibratePath = pnl_mat_create(calibrationValuesNb, assetNb + fxNb);
	PnlMat* marketPath = pnl_mat_create(dimStockPast - calibrationValuesNb, assetNb + fxNb);
	double PL = 0;
	int k = 0;
	for (int i = 0; i < calibrationValuesNb; i++) {
		for (int j = 0; j < assetNb + fxNb; j++) {
			MLET(calibratePath, i, j) = stockPrices[k++];
		}
	}
	k = 0;
	for (int i = calibrationValuesNb; i < dimStockPast; i++) {
		for (int j = 0; j < assetNb + fxNb; j++) {
			MLET(marketPath, i, j) = stockPrices[k++];
		}
	}

	MonteCarlo* mc = new MonteCarlo(maturity, timeSteps, assetNb + fxNb, interestRates[0], stockToFxIndex,
		interestRates + 1, calibratePath, mcSamples, assetNb, calibrationValuesNb, finiteDifferenceStep);

	mc->mod_->calibrate(calibratePath, 1 / 252.);

	PnlVect *cashValues = pnl_vect_create(H + 1);
	PnlVect *ptfValues = pnl_vect_create(H + 1);
	//mc->hedge(cashValues, ptfValues, PL, H, marketPath);
	//PL = 0;
	////temps = MPI_Wtime();
	//mc->hedge(V, PL, H);
	pnl_vect_free(&ptfValues);
	pnl_vect_free(&cashValues);
	pnl_mat_free(&marketPath);
	pnl_mat_free(&calibratePath);
}
