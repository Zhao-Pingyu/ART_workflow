# ART_workflow

This is a workflow for using ART nouveau (http://normandmousseau.com/ART-nouveau-81.html) to calculate the activation energy spectrum of grain boundaries, including ground state and metastable grain boundaries. In this example, the activation energies of $Sigma5$(310) metastable grain boundary with grain boundary energy of 1048 $J/m^2$ are calculated. To obtain the full specturm, we need to iterate through all the atoms in the grain boundary region (the atom ids are listed in gb_id.txt). In this example, we only calculate the activation energies for 10 atoms. This workflow requires the installation of ART nouveau, LAMMPS (https://www.lammps.org/), and OVITO (https://www.ovito.org/).

**Files:**

*9775-1048.data*: input file for ART nouveau, initial data file

*bart.sh*: input file for ART nouveau - sets the simulation

*in.lammps*: input file for LAMMPS

*Cu01.eam.alloy*: interatomic potential file for LAMMPS

*refconfig*: Atomic position file

*art-loop.submit*: submission script for ART nouveau jobs to supercomputing clusters

*select_gb.py*: selects the ids of all the atoms in the grain boundary, using the Python interface of OVITO, it generates the file *gb_id.txt*

*gb_id.txt*: contains the ids of all the atoms in the grain boundary

*gb_id_1.txt*: contains the first 10 ids in *gb_id.txt*, to use in this example

*loop_ids_part.sh*: loops over the ids of all the atoms in the grain boundary and submits ART nouveau jobs to supercomputing clusters

*screen_all.cpp*: after all ART nouveau jobs are completed, iterates over output files to extract the activation energies in the simulations, it generates *events_all.list* and *events_clean.list*

*events_all.list*: output file of *screen_all.cpp*, contains the activation energies before removing all redundant searches

*events_clean.list*: output file of *screen_all.cpp*, contains the activation energies after removing all redundant searches
