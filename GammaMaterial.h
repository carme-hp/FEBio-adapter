#include <FEBioMech/FEElasticMaterialPoint.h>
#include <FEBioMech/FETransIsoMooneyRivlin.h>
#include <FEBioMech/FEActiveContractionMaterial.h>

/*
 * Opengamma Material Point
 * Adds field m_gamma to FEElasticMaterialPoint
 * in order to couple with Opengammas FastMonodomainSolver
 */ 
class GammaMaterialPoint 
	: public FEElasticMaterialPoint
{
public:
    	GammaMaterialPoint(FEMaterialPointData *mp = nullptr)
    	        : FEElasticMaterialPoint(mp), m_gamma(0) {}

    	double m_gamma; // coupling vatiable
};

/*
 * Opengamma Material
 * Behaves the same as FETransIsoMooneyRivlin Material
 * but uses GammaMaterialPoint instead of FEElasticMaterialPoint
 */
class GammaMaterial 
	: public FETransIsoMooneyRivlin
{
public:

    	GammaMaterial(FEModel *pfem) 
    	        : FETransIsoMooneyRivlin(pfem) {}

    	virtual FEMaterialPointData *CreateMaterialPointData() override;

    	DECLARE_FECORE_CLASS();
};

/*
 * Opengamma Active Contraction
 * Implements Opengammas active contraction model,
 * see https://opendihu.readthedocs.io/en/latest/settings/muscle_contraction_solver.html
 * Use in combination with GammaMaterial
 */
class gammaContraction 
	: public FEActiveContractionMaterial
{
public:
    	gammaContraction(FEModel *pfem)
    	        : FEActiveContractionMaterial(pfem) {}

    	virtual mat3ds ActiveStress(FEMaterialPoint &mp, const vec3d &a0) override;
    	virtual tens4ds ActiveStiffness(FEMaterialPoint &mp, const vec3d &a0) override;

    	double m_pmax; 				// maximum PK2 active stress
	double m_lamOpt; 			// 1.2 constant in Opengamma
	bool m_enableForceLengthRelation; 	// whether f(lam/lam_opt) should be multiplied
 
    	DECLARE_FECORE_CLASS();
};
