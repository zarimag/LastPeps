#ifndef _OPTION_H
#define _OPTION_H

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

// #define _DEBUG

/// \brief Classe Option abstraite
class Option
{
public:
	double T_; /// maturité
	int TimeSteps_; /// nombre de pas de temps de discrétisation
	int size_; /// dimension du modèle, redondant avec BS::size_
	int sizeAsset_; /// nombre d'actifs risqués
	PnlVect *curr_; /// indice de la devise etrangere 
	char *optionType_; /// Type de l'option

	virtual ~Option()
	{

	}
	/**
	* Calcule la valeur du payoff sur la trajectoire
	*
	* @param[in] path est une matrice de taille d x (N+1)
	* contenant une trajectoire du modèle telle que créée
	* par la fonction asset.
	* @return phi(trajectoire)
	*/
	virtual double payoff(const PnlMat *path) = 0;

	/**
	*
	*/
	virtual void print() = 0;
};


#endif /* _OPTION_H */

