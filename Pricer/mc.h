#ifndef _MC_H
#define _MC_H

#include "option.h"
#include "moduleis.h"
#include "bs.h"
#include <cmath>
#include <cassert>
#include <ctime>
#include <iomanip> 
#include "pnl/pnl_random.h"
#include <cassert>

class MonteCarlo
{
public:
	BS *mod_; /*! pointeur vers le modèle */
	Option *opt_; /*! pointeur sur l'option */
	PnlRng *rng; /*! pointeur sur le générateur */
	double h_; /*! pas de différence finie */
	int samples_; /*! nombre de tirages Monte Carlo */

	MonteCarlo(double T_, int TimeSteps_, int size_, double r_, double* curr_, double* dividend_, 
		PnlMat* stocks, int samples_, int sizeAsset, int dimStocks, double finiteDifferenceStep);


	~MonteCarlo();

	/**
	* Calcule le prix de l'option à la date 0
	*
	* @param[out] prix valeur de l'estimateur Monte Carlo
	* @param[out] ic largeur de l'intervalle de confiance
	*/
	void price(double &prix, double &ic);

	void price_master(double &prix, double &ic);

	void price_slave(double &prix, double &ic);

	/**
	* Calcule le prix de l'option à la date t
	*
	* @param[in]  past contient la trajectoire du sous-jacent
	* jusqu'à l'instant t
	* @param[in] t date à laquelle le calcul est fait
	* @param[out] prix contient le prix
	* @param[out] ic contient la largeur de l'intervalle
	* de confiance sur le calcul du prix
	*/
	void price(const PnlMat *past, double t, double &prix, double &ic);


	/**
	* Calcule le delta de l'option à la date t
	*
	* @param[in] past contient la trajectoire du sous-jacent
	* jusqu'à l'instant t
	* @param[in] t date à laquelle le calcul est fait
	* @param[out] delta contient le vecteur de delta
	*/
	void delta_(const PnlMat *past, double t, PnlVect *delta);

	/**
	* Calcule le delta de l'option à la date t
	*
	* @param[in] past contient la trajectoire du sous-jacent
	* jusqu'à l'instant t
	* @param[in] t date à laquelle le calcul est fait
	* @param[out] delta contient le vecteur de delta
	*/
	void delta(const PnlMat *past, double t, PnlVect *delta);

	/**
	* Construit le portefeuille de couverture et calcule le P&L
	*
	* @param[in]  H nombre de date de constatation
	* @param[in]  marketPath matrice de taille d x (H+1) qui contient
	* une simulation du marché
	* @param[out] V vecteur des valeurs de portefeuille de couverture
	* de dimension H+1
	* @param[out] PL Profit and Loss
	*/
	void hedge(PnlVect *V, PnlVect *ptfValues, double &PL, int H, const PnlMat *marketPath);

	/**
	* Construit le portefeuille de couverture et calcule le P&L
	*
	* @param[in]  H nombre de date de constatation
	* @param[out] V vecteur des valeurs de portefeuille de couverture
	* de dimension H+1
	* @param[out] PL Profit and Loss
	*/
	void hedge(PnlVect *V, double &PL, int H);

	//suppose estimatedVol is already allocated with the right nb of elts
	void estimVolHistMethod(const PnlMat* historicalStockPrices, PnlVect* estimatedVol, double timeStep);
};

#endif /* _MC_H */

