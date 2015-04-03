#ifndef _IRATE_H
#define _IRATE_H

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>
#include <ctime>
#include <cassert>

/// \brief Modèle du taux d'intérêt
class IRate
{
public:

	double initPoint_; //must be reset at each time t

	virtual void calibrateParametersLeastSquare(const PnlVect *histShortRate, double historyRateTimeStep) = 0;
	virtual void calibrateParametersMaxLikelihood(const PnlVect *histShortRate, double historyRateTimeStep) = 0;

	/**
	* Calcule l'intégrale du taux d'intérêt entre t et T
	* @param[in] t date jusqu'à laquelle on connait la trajectoire
	* t n'est pas forcément une date de discrétisation
	* @param[in] N nombre de pas de constatation
	* @param[in] T date jusqu'à laquelle on simule la trajectoire
	*/
	virtual void getDiscountFactor(double &result, double t, int N, double T,
		PnlRng *rng) = 0;

	virtual ~IRate()
	{
#ifdef _DEBUG
		std::cout << "~IRate()" << std::endl;
#endif
	}
private:
	/**
	* @param[out] path  contient une trajectoire du sous-jacent
	* donnée jusqu'à l'instant t par la matrice past
	* déja allouée de taille N+1
	*/
	void simuRatePath(PnlVect *path, double t, int N, double T,
		PnlRng *rng);

};


#endif /* _IRATE_H */