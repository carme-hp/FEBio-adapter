// Defines the exported functions for the DLL application.

#include <FECore/sdk.h>
#include "PreciceCallback.h"
#include "GammaMaterial.h"

FECORE_PLUGIN int GetSDKVersion()
{
	return FE_SDK_VERSION;
}

FECORE_PLUGIN void GetPlugInVersion(int &major, int &minor, int &patch)
{
	major = 1;	
	minor = 0;
	patch = 0;
}

FECORE_PLUGIN void PlugInInitialize(FECoreKernel &fecore)
{
	FECoreKernel::SetInstance(&fecore);
	// register adapter callback
   	REGISTER_FECORE_CLASS(PreciceCallback, "preCICEAdapter");
	// register custom material
    	REGISTER_FECORE_CLASS(GammaMaterial, "GammaMaterial");
	// register custom contraction
	REGISTER_FECORE_CLASS(gammaContraction, "gammaContraction");
}
