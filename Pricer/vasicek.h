#ifndef _VASICEK_H
#define _VASICEK_H

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>
#include "irate.h"
#include <cassert>
#include <stdexcept> 

/// \brief Classe Option PERFORMANCE
class Vasicek : public IRate
{
public:

	double longTermMean_ = 0;
	double reversionSpeed_ = 0;
	double vol_ = 0;

	Vasicek();
	virtual ~Vasicek();

	void calibrateParametersLeastSquare(const PnlVect *histShortRate, double historyRateTimeStep);
	void calibrateParametersMaxLikelihood(const PnlVect *histShortRate, double historyRateTimeStep);

	void getDiscountFactor(double &result, double t, int N, double T,
		PnlRng *rng);

private:
	void simuRatePath(PnlVect *path, double t, int N, double T,
		PnlRng *rng);
};


#endif /* _VASICEK_H */

