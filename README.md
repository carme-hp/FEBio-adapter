# The FEBio Adapter

Welcome to the FEBio adapter for [preCICE](https://precice.org/)!

This adapter allows you to use preCICE v3 to run coupled electromechanics simulations for skeletal muscles, thereby enabling coupling between the FEBio's mechanics solver to the OpenDiHu electrophysiology solver. 


## Related work

The code of this adapter was originally written by computer science students of the University of Stuttgart, and was later ported to preCICE v3. If you are using preCICE v2, we refer them to their [code](https://github.com/silasnatterer/bfp/tree/main).

In a separated work effort, another FEBio adapter was developed for simulations of the liver. This [experimental FEBio adapter](https://github.com/precice/febio-adapter) may be the way to go depending on your application. 
 

## Prerequisites

- [preCICE](https://precice.org/) version => 3.0.0
- [OpenDiHu](https://github.com/opendihu/opendihu) version = 1.5
- [FEBioStudio](https://github.com/febiosoftware/FEBioStudio) version <= 2.10

Downloading the correct FEBioStudio version might be tricky. I recommend downloading the installer for version 2.10 which can be found under [previous versions](https://febio.org/downloads/#/). You will need to extract the installer and make it executable before running it. During the installation, choose the option to include SDK. 

## Building

To build the adapter, clone this repository and then do

```
mkdir build
cd build
cmake .. -DFEBio_SDK="path/to/FEBioStudio/sdk"
make
```

Alternatively, you can simply run the script `build.sh`.

## Load the Plug-In

This step is needed so that FEBio finds the adapter. Basically, the adapter can be loaded as a plug-in in FEBioStudio. 

- Option 1: Edit the `FEBioStudio/bin/febio.xml` file.

```xml
<import>pathToAdapter/build/lib/libpreCICEAdapter.so</import>
```

- Option 2: Use the FEBioStudio's GUI. Go to *FEBio->Manage FEBio Plugins* and select `libpreCICEAdapter.so`.

## Usage

For the PreCICE coupling to work you have to add the following lines to your *model.feb* file 
```xml
<Code>
	<callback name="precice_callback"\>
</Code>
```
In addition you have to change the material to use the custom *DiHuMaterial* and *DiHuContraction* classes
```xml
<Material>
	<material id="material_id" name="material_name" type="DiHuMaterial">
		<density>OpenDiHu density</density>
		<k>1000</k>
		<pressure_model>default</pressure_model>
		<c1>OpenDiHu c1</c1>
		<c2>OpenDiHu c2</c2>
		<c3>0</c3>
		<c4>1</c4>
		<c5>OpenDiHu b</c5>
		<lam_max>1</lam_max>
		<fiber type="vector">
			<vector>Direction of fibers</vector>
		</fiber>
		<active_contraction type="DiHuContraction">
			<pmax>OpenDiHu Pmax</pmax>
			<lam_opt>1.2</lam_opt>
			<enable_force_length_relation>1</enable_force_length_relation>
		</active_contraction>
	</material>
</Material>
```
Note that the following names are hardcoded in the plugin:
- *Muscle*: name of the preCICE participant 
- *MuscleMesh*: name of the preCICE mesh
- *Gamma*, *Geometry*: name of the preCICE fields to couple
- *MusclePart*: name of the FEBio part to couple 

By default the adapter will use *./precice-config.xml* for the preCICE configuration path.
To change this, set the *BFP_CONFIG* environment variable.
You can then run the case with
```bash
BFP_CONFIG="config" mpirun -n 1 FEBioStudio/bin/febio4 model.feb
```

