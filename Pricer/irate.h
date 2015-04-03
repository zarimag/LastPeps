#ifndef _IRATE_H
#define _IRATE_H

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>
#include <ctime>
#include <cassert>

/// \brief Mod�le du taux d'int�r�t
class IRate
{
public:

	double initPoint_; //must be reset at each time t

	virtual void calibrateParametersLeastSquare(const PnlVect *histShortRate, double historyRateTimeStep) = 0;
	virtual void calibrateParametersMaxLikelihood(const PnlVect *histShortRate, double historyRateTimeStep) = 0;

	/**
	* Calcule l'int�grale du taux d'int�r�t entre t et T
	* @param[in] t date jusqu'� laquelle on connait la trajectoire
	* t n'est pas forc�ment une date de discr�tisation
	* @param[in] N nombre de pas de constatation
	* @param[in] T date jusqu'� laquelle on simule la trajectoire
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
	* donn�e jusqu'� l'instant t par la matrice past
	* d�ja allou�e de taille N+1
	*/
	void simuRatePath(PnlVect *path, double t, int N, double T,
		PnlRng *rng);

};


#endif /* _IRATE_H */