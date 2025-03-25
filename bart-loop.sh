#!/bin/tcsh
set id = $1
#__________________________________________ ATOMS _____________________________________________ #
setenv gnrm				1.0E-05
setenv Inflection			4 
setenv NATOMS				39800			# Number of atoms in the problem
setenv type1				Cu			# Element type for the xyz output. Add type2, etc. if needed
#setenv type2				Cu			# Element type for the xyz output. Add type2, etc. if needed

#___________________________________________ ART _____________________________________________ #
setenv EVENT_TYPE			NEW
setenv Temperature			-0.5			# Temperature in eV, if negative always reject the event
setenv Max_Number_Events		10			# Maximum number of events
setenv Type_of_Events			local			# Initial move for events - global or local
setenv Radius_Initial_Deformation	3.0 #3.692
setenv Central_Atom			$id
#setenv sym_break_dist			0.001			# Breaks the symmetry of the crystal by randomly displacing
								# all atoms by this distance
setenv ENERGY_CALC			LAM			# Choice of forcefield SWP, SWA or LAM (LAMMPS) 

#setenv UNITS_CONVERSION		'real'			# Converts the energy and distance units provided by the force code
								# into a desired value for ART - affects only the parameters in ART and kART
								# units available are real , electron and si
								# be very careful when choosing other units, you must change parameters of energy 
								# and distance (if it's not in Angstrom) in input files for LAMMPS 

setenv Activation_MaxIter		750			# Maximum number of iteraction for reaching the saddle point
setenv Increment_Size			0.06			# Overall scale for the increment moves
setenv Force_Threshold_Perp_Rel		0.1 #0.005			# Threshold for perpendicular relaxation
#----------------------------- LOCAL FORCE
setenv LOCAL_FORCE 			.true.  # Use local forces (def: .false.)  #.true.
setenv INNER_REGION_RADIUS 		15.0 	# Radius of inner region
setenv OUTER_REGION_WIDTH 		6.0 	# Thickness of outer region shell
#___________________________________________ HARMONIC WELL_____________________________________________ #
setenv Initial_Step_Size		0.5 #1.2 #2			# Size of initial displacement, in A
setenv Basin_Factor			2.4 #2.4			# Factor multiplying Increment_Size for leaving the basin
setenv Max_Perp_Moves_Basin		5			# Maximum number of perpendicular steps leaving basin
setenv Min_Number_KSteps		0			# Min. number of ksteps before calling lanczos 
setenv Eigenvalue_Threshold		-1 #-1 #-0.01			# Eigenvalue threshold for leaving basin
setenv Max_Iter_Basin			20 #20			# Maximum number of iteraction for leaving the basin (kter)

#_____________________________ LANCZOS
setenv Lanczos_of_minimum		.False.			# Calculation of the Hessian for each minimum
setenv Number_Lanczos_Vectors		15			# Number of vectors included in lanczos procedure
setenv delta_disp_Lanczos		0.005			# Step of the numerical derivative of forces in lanczos (Ang)
#_____________________________ CONVERGENCE
setenv Exit_Force_Threshold		0.05 #0.05			# Threshold for convergence at saddle point
setenv Prefactor_Push_Over_Saddle	0.5			# Fraction of displacement over the saddle
setenv Save_Conf_Int			.False.			# Save the configuration at every step?
#_____________________________ DIIS
setenv Iterative			.False.			# Iterative use of Lanczos & DIIS
setenv Use_DIIS				.False.			# Use DIIS for the final convergence to saddle
setenv DIIS_Force_Threshold		1.0 #1.5			 # Force threshold for call DIIS
setenv DIIS_Memory			5			# Number of vectors kepts in memory for algorithm
setenv DIIS_Check_Eigenvector		.True.			# Check that the final state is indeed a saddle
setenv DIIS_Step_siz			0.03			# prefactor multiplying forces
setenv FACTOR_DIIS			5.0			# times Increment_Size, max allowed diis step size
setenv MAX_DIIS				150			# max diis iterations per call
#_____________________________ INPUT 
setenv FILECOUNTER			filecounter		# File tracking  the file (event) number - facultative
setenv REFCONFIG			refconfig		# Reference configuration (actual local minimum)
setenv INPUT_LAMMPS_FILE		'in.lammps'		# LAMMPS input file when using ENERGY_TYPE = LAM
#_____________________________ OUTPUT 
setenv LOGFILE				log.file		# General output for message
setenv EVENTSLIST			events.list		# list of events with success or failure
setenv Write_restart_file		.False.			# It is useful only for ab-initio
setenv RESTART				restart.dat		# current data for restarting event
setenv Write_JMOL			.False.			# Writes the configuration in jmol format.
###### RUN THE SIMULATION #######################################################################
/home/anqiq/artn-master/source/ARTn_exec
