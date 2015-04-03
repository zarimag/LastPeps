#include <iostream>
#include "mc.h"

using namespace std;

MonteCarlo::MonteCarlo(double T_, int TimeSteps_, int size_, double r_, double* curr_, double* dividend_, 
				PnlMat* stocks, int samples_, int sizeAsset, int dimStocks, double finiteDifferenceStep)
{
	PnlVect* spot = pnl_vect_create(size_);
	pnl_mat_get_row(spot, stocks, dimStocks - 1);
	PnlVect* dividend = pnl_vect_create_from_ptr(size_, dividend_);
	PnlVect* curr = pnl_vect_create_from_ptr(sizeAsset, curr_);
	// Construction de BS
	this->mod_ = new BS(size_, r_, dividend, spot);
	this->opt_ = new Moduleis(curr, T_, TimeSteps_, size_, sizeAsset);
	// Initialisation du generateur a MERSENNE : type 7 page 63
	rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	// Sample number et pas de difference finie
	this->samples_ = samples_;
	this->h_ = finiteDifferenceStep;
	pnl_vect_free(&spot);
}

MonteCarlo::~MonteCarlo()
{
#ifdef _DEBUG
	cout << "~MonteCarlo() : Ready to call pnl_rng_free " << endl;
#endif
	pnl_rng_free(&rng);
	delete this->mod_;
	delete this->opt_;
#ifdef _DEBUG
	cout << "~MonteCarlo() : Successfull call of pnl_rng_free" << endl;
#endif
}

void MonteCarlo::estimVolHistMethod(const PnlMat* historicalStockPrices, PnlVect* estimatedVol, double timeStep) {
	// Nb of columns == Nb cof underlyings && nb of rows == nb of historical data
	int nbAssets = historicalStockPrices->n, nbData = historicalStockPrices->m;

	for (int j = 0; j < nbAssets; j++) {
		double logSj = MGET(historicalStockPrices, 1, j) / MGET(historicalStockPrices, 0, j);
		double u = log(logSj), v = 0;
		for (int i = 2; i < nbData; i++) {
			logSj = MGET(historicalStockPrices, i, j) / MGET(historicalStockPrices, i - 1, j);
			v += (i - 1) * (logSj - u) * (logSj - u) / i;
			u += (logSj - u) / i;
		}
		LET(estimatedVol, j) = sqrt(v / (nbData - 1) / timeStep);
	}

}

void MonteCarlo::price(double &prix, double &ic)
{
	PnlMat *generatedPath = pnl_mat_create(this->opt_->TimeSteps_ + 1, this->opt_->size_);
	double payoff = 0;
	double sumPayoff = 0;
	double sumPayoffSquare = 0;
	prix = 0;
	ic = 0;
	for (int i = 0; i < this->samples_; i++)
	{
		this->mod_->asset(generatedPath, this->opt_->T_, this->opt_->TimeSteps_, this->rng);
		payoff = this->opt_->payoff(generatedPath);
		sumPayoff += payoff;
		sumPayoffSquare += payoff*payoff;
	}
	prix = exp(-this->mod_->r_*this->opt_->T_)*(sumPayoff / this->samples_);
	double x = exp(-2 * this->mod_->r_*this->opt_->T_)*((sumPayoffSquare / this->samples_) - (sumPayoff / this->samples_)*(sumPayoff / this->samples_));
	ic = 2 * 1.96*sqrt(x) / sqrt(this->samples_);
	pnl_mat_free(&generatedPath);
}

void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic)
{
	PnlMat *generatedPath = pnl_mat_create(this->opt_->TimeSteps_ + 1, this->opt_->size_);
	double payoff = 0;
	double sumPayoff = 0;
	double sumPayoffSquare = 0;
	double precision = this->samples_*(1.0e-16);
	prix = 0;
	ic = 0;
	for (int i = 0; i < this->samples_; i++)
	{
		this->mod_->asset(generatedPath, t, this->opt_->TimeSteps_, this->opt_->T_, this->rng, past);
		//pnl_mat_print(generatedPath);
		payoff = this->opt_->payoff(generatedPath);
		sumPayoff += payoff;
		sumPayoffSquare += payoff*payoff;
	}
	prix = exp(-this->mod_->r_*(this->opt_->T_ - t))*(sumPayoff / this->samples_);
	double x = exp(-2 * this->mod_->r_*(this->opt_->T_ - t))*(sumPayoffSquare / this->samples_) - prix*prix;
	if (abs(x) < precision)
		x = 0;
	ic = 2 * 1.96*sqrt(x) / sqrt(this->samples_);
	pnl_mat_free(&generatedPath);
}

void MonteCarlo::delta_(const PnlMat *past, double t, PnlVect *delta)
{
	pnl_vect_set_all(delta, 0);
	double T = opt_->T_;
	int size = mod_->size_;
	int N = opt_->TimeSteps_;

	PnlMat *path = pnl_mat_create(N + 1, size);

	for (int i = 0; i < samples_; i++)
	{
		mod_->asset(path, t, N, T, rng, past);
		for (int d = 0; d < size; d++)
		{
			for (int k = int(N*t / T); k < N + 1; k++)
			{
				pnl_mat_set(path, k, d, (1 + h_) * pnl_mat_get(path, k, d));
			}

			pnl_vect_set(delta, d, opt_->payoff(path) + pnl_vect_get(delta, d));

			for (int k = int(N*t / T); k < N + 1; k++)
			{
				pnl_mat_set(path, k, d, (1 - h_) / (1 + h_) * pnl_mat_get(path, k, d));
			}

			pnl_vect_set(delta, d, pnl_vect_get(delta, d) - opt_->payoff(path));

			for (int k = int(N*t / T); k < N + 1; k++)
			{
				pnl_mat_set(path, k, d, pnl_mat_get(path, k, d) / (1 - h_));
			}
		}

	}

	for (int d = 0; d < size; d++)
	{
		pnl_vect_set(delta, d, exp((-mod_->r_)*(opt_->T_ - t)) / (samples_ * 2 * h_ * pnl_mat_get(past, int(N*t / T), d)));

	}

	pnl_mat_free(&path);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta)
{
	pnl_vect_set_all(delta, 0);
	PnlMat *generatedPath = pnl_mat_create(this->opt_->TimeSteps_ + 1, this->opt_->size_);
	PnlMat *shiftPath_Right = pnl_mat_create(this->opt_->TimeSteps_ + 1, this->opt_->size_);
	PnlMat *shiftPath_Left = pnl_mat_create(this->opt_->TimeSteps_ + 1, this->opt_->size_);
	double tmp = 0;
	// The vector St
	PnlVect *St = pnl_vect_create_from_zero(this->opt_->size_);
	int lastIndexOfPast = 0;
	if (t == 0.0)
		pnl_vect_clone(St, this->mod_->spot_);
	else
	{
		double step = this->opt_->T_ / this->opt_->TimeSteps_;
		double dt = t / step;
		double Error = abs(dt - round(dt)) / (dt);
		if (Error <= 0.05)
			lastIndexOfPast = round(dt);
		else
			lastIndexOfPast = floor(dt) + 1;
	}

	for (int j = 0; j<this->samples_; j++)
	{
		//Generation d'une trajectoire
		this->mod_->asset(generatedPath, t, this->opt_->TimeSteps_, this->opt_->T_, this->rng, past);
		for (int d = 0; d<this->mod_->size_; d++)
		{
			// Shift à droite
			this->mod_->shift_asset(shiftPath_Right, generatedPath, d, this->h_, t, this->opt_->T_, this->opt_->TimeSteps_);
			// Shift à droite
			this->mod_->shift_asset(shiftPath_Left, generatedPath, d, -this->h_, t, this->opt_->T_, this->opt_->TimeSteps_);
			// Delta payoff Right Left
			tmp = this->opt_->payoff(shiftPath_Right) - this->opt_->payoff(shiftPath_Left);
			pnl_vect_set(delta, d, GET(delta, d) + tmp);
		}
	}
	for (int d = 0; d<this->opt_->size_; d++)
	{
		pnl_vect_set(St, d, MGET(past, lastIndexOfPast, d));
		pnl_vect_set(delta, d, GET(delta, d)*exp(-this->mod_->r_*(this->opt_->T_ - t)) / (this->samples_*this->h_ * 2 * GET(St, d)));
	}
	pnl_mat_free(&generatedPath);
	pnl_mat_free(&shiftPath_Right);
	pnl_mat_free(&shiftPath_Left);
	pnl_vect_free(&St);
}

void MonteCarlo::hedge(PnlVect *V, PnlVect *ptfValues, double &PL, int H, const PnlMat *marketPath)
{
	PnlVect *delta_cour = pnl_vect_create_from_zero(this->opt_->size_);
	PnlVect *delta_prec = pnl_vect_create_from_zero(this->opt_->size_);
	PnlVect *divi = pnl_vect_create_from_zero(this->opt_->size_);
	PnlVect *St = pnl_vect_copy(this->mod_->spot_);
	PnlMat *past = pnl_mat_create(this->opt_->TimeSteps_ + 1, this->opt_->size_);

	pnl_mat_set_row(past, St, 0);

	int M = (H / this->opt_->TimeSteps_);
	int ti = 1;
	double p = 0;
	double ic = 0;
	double value = 0;

	//void pnl_vect_fprint (FILE ∗fic, const PnlVect ∗V)

	//void pnl_mat_fprint (FILE ∗fic, const PnlMat ∗M)

	PnlVect *Prix = pnl_vect_create(H);
	PnlMat *Deltas = pnl_mat_create(H, this->opt_->size_);

	// Affichage des valeurs du portefeuille	
	cout << " ---- \t Date n° \t PortFolio Value " << endl;
	// Initialisation du portefeuille
	this->price(p, ic);
	this->delta(past, 0, delta_cour);
	LET(Prix, 0) = p;
	pnl_mat_set_row(Deltas, delta_cour, 0);


	for (int d = 0; d < opt_->size_; d++)
	{
		LET(divi, d) = exp(GET(this->mod_->dividend_, d)*this->opt_->T_ / H);
	}

	value = p - pnl_vect_scalar_prod(delta_cour, St);
	pnl_vect_set(V, 0, value);
	cout << " ---- \t " << 0 << " \t \t" << GET(V, 0) << endl;
	// Calcul des Vi
	for (int i = 1; i<H; i++)
	{
		pnl_mat_get_row(St, marketPath, i);
		pnl_mat_set_row(past, St, ti);
		if ((i % M) == 0)
			ti++;
		pnl_vect_clone(delta_prec, delta_cour);
		this->price(past, i*this->opt_->T_ / H, p, ic);
		this->delta(past, i*this->opt_->T_ / H, delta_cour);
		LET(Prix, i) = p;
		pnl_mat_set_row(Deltas, delta_cour, i);
		pnl_vect_minus_vect(delta_prec, delta_cour);

		value = GET(V, i - 1)*exp(this->mod_->r_*this->opt_->T_ / H) + pnl_vect_scalar_prod(delta_prec, St);
		pnl_vect_mult_vect_term(St, delta_cour);
		pnl_vect_mult_scalar(St, opt_->T_ / H);
		value += pnl_vect_sum(St);
		pnl_vect_set(V, i, value);
		cout << " ---- \t " << i << " \t \t" << GET(V, i) << endl;
	}
	// Calcul de VH
	pnl_mat_get_row(St, marketPath, H);
	value = GET(V, H - 1)*exp(this->mod_->r_*this->opt_->T_ / H);
	pnl_vect_set(V, H, value);
	cout << " ---- \t " << H << " \t \t" << GET(V, H) << "\t Valeur avant livraison" << endl;
	// calcul P&L
	double payoff = this->opt_->payoff(past);
	PL = GET(V, H) + pnl_vect_scalar_prod(delta_cour, St) - payoff;
	cout << " " << endl;
	cout << "  ---- Prix de l'option en 0 = " << GET(Prix, 0) << endl;
	cout << "  ---- Erreur de couverture relative en % = " << (PL / p) * 100 << endl;

	FILE * pFile, *dFile, *mFile;
	pFile = fopen("../past/Prix.txt", "w");
	dFile = fopen("../past/Deltas.txt", "w");
	mFile = fopen("../past/Market.txt", "w");
	pnl_vect_fprint(pFile, Prix);
	pnl_mat_fprint(dFile, Deltas);
	pnl_mat_fprint(mFile, marketPath);
	fclose(pFile);
	fclose(dFile);
	fclose(mFile);
	// memory free
	pnl_vect_free(&Prix);
	pnl_vect_free(&St);
	pnl_vect_free(&delta_cour);
	pnl_vect_free(&delta_prec);
	pnl_mat_free(&Deltas);
	pnl_mat_free(&past);
}

void MonteCarlo::hedge(PnlVect *V, double &PL, int H)
{
	PnlMat *marketPath = pnl_mat_create_from_zero(H + 1, this->opt_->size_);
	this->mod_->asset(marketPath, this->opt_->T_, H, this->rng);
	//this->hedge(V, PL, H, marketPath);
	pnl_mat_free(&marketPath);
}
