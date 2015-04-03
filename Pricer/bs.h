#ifndef _BS_H
#define _BS_H

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>
#include <ctime>
#include <cassert>

/// \brief Mod�le de Black Scholes
class BS
{
public:
	int size_; /// nombre d'actifs du mod�le
	double r_; /// taux d'int�r�t
	double rho_; /// param�tre de corr�lation
	PnlVect *sigma_; /// vecteur de volatilit�s
	PnlVect *spot_; /// valeurs initiales du sous-jacent
	PnlVect *dividend_; /// valeurs des dividendes vers�es
	PnlVect *trend_; /// vecteur des tendances des sous-jacent
	PnlMat *L; /// Cholesky factorization of the correlation matrix

	BS(int size_, double r_, PnlVect* dividend_, PnlVect* spot_);
	~BS();

	/**
	* G�n�re une trajectoire du mod�le et la stocke dans path
	*
	* @param[out] path contient une trajectoire du mod�le.
	* C'est une matrice de taille d x (N+1)
	* @param[in] T  maturit�
	* @param[in] N nombre de dates de constatation
	*/
	void asset(PnlMat *path, double T, int N, PnlRng *rng);

	/**
	* Calcule une trajectoire du sous-jacent connaissant le
	* pass� jusqu' � la date t
	*
	* @param[out] path  contient une trajectoire du sous-jacent
	* donn�e jusqu'� l'instant t par la matrice past
	* @param[in] t date jusqu'� laquelle on connait la trajectoire
	* t n'est pas forc�ment une date de discr�tisation
	* @param[in] N nombre de pas de constatation
	* @param[in] T date jusqu'� laquelle on simule la trajectoire
	* @param[in] past trajectoire r�alis�e jusqu'a la date t
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
	* dont la composante d a �t� shift�e par (1+h)
	* � partir de la date t.
	* @param[in] t date � partir de laquelle on shift
	* @param[in] h pas de diff�rences finies
	* @param[in] d indice du sous-jacent � shifter
	* @param[in] timestep pas de constatation du sous-jacent
	*/
	void shift_asset(PnlMat *shift_path, const PnlMat *path,
		int d, double h, double t, double T, double timestep);

	/**
	* G�n�re une simulation du march� et la stocke dans path
	*
	* @param[out] path contient une simulation du march�.
	* C'est une matrice de taille (H+1) x d
	* @param[in] T  maturit�
	* @param[in] H nombre de dates de constatation
	*/
	void simul_market(PnlMat *path, double T, int H, PnlRng *rng);

	void calibrate(const PnlMat* past, double dt);

};


#endif /* _BS_H */

