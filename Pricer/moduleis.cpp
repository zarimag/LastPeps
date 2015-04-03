#include <iostream>
#include "moduleis.h"

using namespace std;
Moduleis::Moduleis(PnlVect* curr_, double T_, int TimeSteps_, int size_, int sizeAsset_)
{
	this->T_ = T_;
	this->TimeSteps_ = TimeSteps_;
	this->size_ = size_;
	this->Levels_ = 6;
	this->LevelStep_ = 25;
	this->nbRecDates = 9;
	this->borneInf = -15;
	this->sizeAsset_ = sizeAsset_;
	this->curr_= curr_;

}

Moduleis::~Moduleis() 
{
#ifdef _DEBUG
	cout << "~Moduleis()" << endl;
#endif
}

double Moduleis::computeYield(const PnlMat *path, int date, int numAsset){
	int curr = int(GET(curr_, numAsset));
	double S0 = MGET(path, 0, numAsset);
	if (curr<0) return (MGET(path, date, numAsset) - S0) / S0;
	S0 = S0 / MGET(path, 0, curr);
	return (MGET(path, date, numAsset) / MGET(path, date, GET(curr_, numAsset)) - S0) / S0;
}

double Moduleis::computeLevel(double yield){
	if (yield<0) return (yield * 100);
	double perf = int(yield * 100 / LevelStep_)*LevelStep_;
	//TO DO: link MIN from pnl
	if (perf< LevelStep_*Levels_){
		return perf;
	}
	else{
		return LevelStep_*Levels_;
	}
}

// a la date 0 (ne gere pas encore la date t)
void Moduleis::computePerformance(PnlVect *Performance, const PnlMat *path, PnlMat *performances){
	for (int i = 0; i < path->m; ++i)
	{
		for (int j = 0; j < path->n; ++j)
		{
			// pour le deuxieme MGET on pourrait passer la matrice path en parametre 
			// a partir de 1 et en choisissant bien les date de constatation
			MLET(performances, i, j) = computeLevel(MGET(performances, i, j) / MGET(path, 0, j) - 1);
		}
	}
	pnl_mat_max(Performance, path, 'r');
}

	
double Moduleis::payoff(const PnlMat *path)
{
	PnlVect *Performances = pnl_vect_create(this->sizeAsset_);
	int indexPath;

	int ti = 0;
	indexPath = (ti + 1)*TimeSteps_ / nbRecDates;
	for (int d = 0; d< this->sizeAsset_; d++)
	{
		LET(Performances, d) = computeLevel(computeYield(path, indexPath, d));
	}

	for (int ti = 1; ti < nbRecDates - 1; ti++)
	{
		indexPath = (ti + 1)*TimeSteps_ / nbRecDates;
		for (int d = 0; d< this->sizeAsset_; d++)
		{
			//TODO replace with max
			if (GET(Performances, d)>computeLevel(computeYield(path, indexPath, d))){
				LET(Performances, d) = GET(Performances, d);
			}
			else{
				LET(Performances, d) = computeLevel(computeYield(path, indexPath, d));
			}
		}
	}

	//pnl_mat_get_row(ST,path,ti);
	indexPath = TimeSteps_;
	for (int d = 0; d< this->sizeAsset_; d++)
	{
		

		double resultMax;
		//TODO replace with max
		if (GET(Performances, d)>100 * computeYield(path, indexPath, d)){
			resultMax = GET(Performances, d);
		}
		else{
			resultMax = 100 * computeYield(path, indexPath, d);
		}
		//TODO replace with min
		if (Levels_*LevelStep_<resultMax){
			LET(Performances, d) = Levels_*LevelStep_;
		}
		else{
			LET(Performances, d) = resultMax;
		}

	
	}
	// quisort Performances
	pnl_vect_qsort(Performances, 'i');
	// multiplier la plus petites moitie 
	double payoff = 0.;
	for (int d = 0; d < this->sizeAsset_ / 2; d++)
	{
		payoff += GET(Performances, d);
	}
	
	if ((payoff*2.0 / (double)this->sizeAsset_) > borneInf){
		payoff = payoff*2.0 / (double)this->sizeAsset_;
	}
	else{
		payoff = borneInf;
	}

	pnl_vect_free(&Performances);
	return payoff+100 ;
}

void Moduleis::print()
{
	cout << "option type" << this->optionType_ << endl;
	cout << "maturity " << this->T_ << endl;
	cout << "timestep number " << this->TimeSteps_ << endl;
	cout << "option size " << this->size_ << endl;
	cout << "nombre de paliers " << this->Levels_ << endl;
	cout << "la difference de rendement entre deux paliers " << this->LevelStep_ << endl;
	cout << "nombre de dates de constatation " << this->nbRecDates << endl;
}
