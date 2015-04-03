#include <iostream>
#include "vasicek.h"

using namespace std;

Vasicek::Vasicek() {

}

Vasicek::~Vasicek() {

}

void buildSumsUtils(const PnlVect *histShortRate, double &Rx, double &Ry, double &Rxx, double &Ryy, double &Rxy) {
	for (int i = 1; i < histShortRate->size - 1; i++) {
		double tmp = GET(histShortRate, i);
		Rx += tmp;
		Rxx += tmp * tmp;
		Rxy += tmp * GET(histShortRate, i - 1);
	}
	double tmp = GET(histShortRate, 0), tmp2 = GET(histShortRate, histShortRate->size - 1);
	Ry = Rx;
	Ryy = Rxx;
	Rx += tmp;
	Ry += tmp2;
	Rxx += tmp * tmp;
	Ryy += tmp2 * tmp2;
	Rxy += tmp2 * GET(histShortRate, histShortRate->size - 2);
}

void Vasicek::calibrateParametersLeastSquare(const PnlVect *histShortRate, double historyRateTimeStep) {
	//double sum = pnl_vect_sum(histShortRate);
	//double Sx = sum - GET(histShortRate, histShortRate->size - 1), Sy = sum - GET(histShortRate, 0);
	double Sx = 0., Sy = 0., Sxx = 0., Syy = 0., Sxy = 0.;
	buildSumsUtils(histShortRate, Sx, Sy, Sxx, Syy, Sxy);
	int n = histShortRate->size - 1;
	double a = (n*Sxy - Sx*Sy) / (n*Sxx - Sx*Sx);
	double b = (Sy - a*Sx) / n;
	double sd = sqrt((n*Syy - Sy*Sy - a*(n*Sxy - Sx*Sy)) / n / (n - 2));

	reversionSpeed_  =	-log(a) / historyRateTimeStep;
	longTermMean_ = b / (1 - a);
	vol_ = sd * sqrt(-2 * log(a) / historyRateTimeStep / (1 - a*a));
}


void Vasicek::calibrateParametersMaxLikelihood(const PnlVect *histShortRate, double historyRateTimeStep) {
	double Sx = 0., Sy = 0., Sxx = 0., Syy = 0., Sxy = 0.;
	buildSumsUtils(histShortRate, Sx, Sy, Sxx, Syy, Sxy);
	int n = histShortRate->size - 1;
	longTermMean_ = (Sy*Sxx - Sx*Sxy) / (n*(Sxx - Sxy) - (Sx * Sx - Sx*Sy));
	reversionSpeed_ = -log((Sxy - longTermMean_*Sx - longTermMean_*Sy + n*longTermMean_ *longTermMean_) / (Sxx - 2 * longTermMean_*Sx + n*longTermMean_ *longTermMean_)) / historyRateTimeStep;
	double a = exp(-reversionSpeed_ * historyRateTimeStep);
	double sigmah2 = (Syy - 2 * a*Sxy + a * a * Sxx - 2 * longTermMean_*(1 - a)*(Sy - a*Sx) + n*longTermMean_*longTermMean_ * (1 - a) * (1 - a)) / n;
	vol_ = sqrt(sigmah2 * 2 * reversionSpeed_ / (1 - a * a));
}

//On nous retourne ici l'intégrale de r
//Meth 2 plus simple: (Paramètres différents!! le t n'est pas le mm que pr l'autre version)
void Vasicek::getDiscountFactor(double &result, double t, int N, double T,
	PnlRng *rng) {
	if (reversionSpeed_ == 0 || longTermMean_ == 0 || vol_ == 0)
		throw std::logic_error("La calibration du modèle n'a pas été effectuée");
	double m = longTermMean_ * (T - t) + (initPoint_ - longTermMean_) * (1 - exp(-reversionSpeed_ * (T - t)))/reversionSpeed_;
	double sigma = sqrt(-vol_ * vol_  * (1 - exp(-reversionSpeed_ * (T - t))) * (1 - exp(-reversionSpeed_ * (T - t))) / (2 * reversionSpeed_ * reversionSpeed_ * reversionSpeed_)
		+ vol_ * vol_ * (T - t - ((1 - exp(-reversionSpeed_ * (T - t))) / reversionSpeed_))/(reversionSpeed_ * reversionSpeed_));
	result = m + sigma * pnl_rng_normal(rng);
}

/*
void Vasicek::getDiscountFactor(double &result, double lastPastIndex, int N, double T,
	PnlRng *rng) {

	PnlVect *path = pnl_vect_create(N + 1 - lastPastIndex);
	LET(path, 0) = this->initPoint_;
	this->simuRatePath(path, lastPastIndex, N, T, rng);
	//Approximation of the integral by trapezoidal method
	double sum = GET(path, 0) / 2.;
	for (int i = 0; i < N - lastPastIndex; i++) {
		sum += GET(path, i);
	}
	sum += GET(path, N - lastPastIndex) / 2.;
	result = sum / (N + 1 - lastPastIndex);
}*/

//On discrétise ici une trajectoire de r
void Vasicek::simuRatePath(PnlVect *path, double lastPastIndex, int N, double T,
	PnlRng *rng) {
	double step = T / ((double)N);
	double firstTerm = exp(-reversionSpeed_ * step);
	double secondTerm = longTermMean_*(1 - firstTerm);
	double thirdTerm = vol_ * sqrt((1 - firstTerm*firstTerm) / (2 * reversionSpeed_));
	for (int i = 1; i < N + 1 - lastPastIndex; i++) {
		LET(path, i) = GET(path, i - 1)*firstTerm
			+ secondTerm + thirdTerm * pnl_rng_normal(rng);
	}
}