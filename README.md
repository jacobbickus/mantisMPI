# mantis with mpi

NRF_Database.tar.gz needs to be unpacked and the user must add the following to their .bashrc:
export G4NRFGAMMADATA=/path/to/Database1.1
To Run with MPI run 
mpiexec -n #cores ./mantis mantis.in

