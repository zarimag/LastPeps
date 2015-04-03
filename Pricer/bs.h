#ifndef _BS_H
#define _BS_H

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>
#include <ctime>
#include <cassert>

/// \brief Modèle de Black Scholes
class BS
{
public:
	int size_; /// nombre d'actifs du modèle
	double r_; /// taux d'intérêt
	double rho_; /// paramètre de corrélation
	PnlVect *sigma_; /// vecteur de volatilités
	PnlVect *spot_; /// valeurs initiales du sous-jacent
	PnlVect *dividend_; /// valeurs des dividendes versées
	PnlVect *trend_; /// vecteur des tendances des sous-jacent
	PnlMat *L; /// Cholesky factorization of the correlation matrix

	BS(int size_, double r_, PnlVect* dividend_, PnlVect* spot_);
	~BS();

	/**
	* Génère une trajectoire du modèle et la stocke dans path
	*
	* @param[out] path contient une trajectoire du modèle.
	* C'est une matrice de taille d x (N+1)
	* @param[in] T  maturité
	* @param[in] N nombre de dates de constatation
	*/
	void asset(PnlMat *path, double T, int N, PnlRng *rng);

	/**
	* Calcule une trajectoire du sous-jacent connaissant le
	* passé jusqu' à la date t
	*
	* @param[out] path  contient une trajectoire du sous-jacent
	* donnée jusqu'à l'instant t par la matrice past
	* @param[in] t date jusqu'à laquelle on connait la trajectoire
	* t n'est pas forcément une date de discrétisation
	* @param[in] N nombre de pas de constatation
	* @param[in] T date jusqu'à laquelle on simule la trajectoire
	* @param[in] past trajectoire réalisée jusqu'a la date t
	*/
	void asset(PnlMat *path, double t, int N, double T,
		PnlRng *rng, const PnlMat *past);

	void asset_(PnlMat *path, double t, int N, double T,
		PnlRng *rng, const PnlMat *past);

	/**
	* Shift d'une trajectoire du sous-jacent
	*
	* @param[in]  path contient en input la trajectoire
	* du sous-jacent
	* @param[out] shift_path contient la trajectoire path
	* dont la composante d a été shiftée par (1+h)
	* à partir de la date t.
	* @param[in] t date à partir de laquelle on shift
	* @param[in] h pas de différences finies
	* @param[in] d indice du sous-jacent à shifter
	* @param[in] timestep pas de constatation du sous-jacent
	*/
	void shift_asset(PnlMat *shift_path, const PnlMat *path,
		int d, double h, double t, double T, double timestep);

	/**
	* Génère une simulation du marché et la stocke dans path
	*
	* @param[out] path contient une simulation du marché.
	* C'est une matrice de taille (H+1) x d
	* @param[in] T  maturité
	* @param[in] H nombre de dates de constatation
	*/
	void simul_market(PnlMat *path, double T, int H, PnlRng *rng);

	void calibrate(const PnlMat* past, double dt);

};


#endif /* _BS_H */

