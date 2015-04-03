#ifndef _MODULEIS_H
#define _MODULEIS_H

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>
#include "option.h"
#include <cassert>

/// \brief Classe Option Moduleis
class Moduleis : public Option
{
public:
	int Levels_; /// nombre de paliers
	double LevelStep_; /// la difference de rendement entre deux paliers
	int nbRecDates; /// nombre de dates de constatation
	double borneInf; /// seuil garantie

	Moduleis(PnlVect* curr_, double T_, int TimeSteps_, int size_, int sizeAsset_);
	virtual ~Moduleis();

	double computeYield(const PnlMat *path, int numAsset, int date);
	double computeLevel(double yield);

	void computePerformance(PnlVect *Performance, const PnlMat *path, PnlMat *performances);

	/**
	* Calcule la valeur du payoff sur la trajectoire
	*
	* @param[in] path est une matrice de taille d x (N+1)
	* contenant une trajectoire du modèle telle que créée
	* par la fonction asset.
	* @return phi(trajectoire)
	*/
	double payoff(const PnlMat *path);

	void print();
};


#endif /* _MODULEIS_H */
