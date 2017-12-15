#$ -S /bin/bash
#$ -q serial
#$ -N mu0_and_rr0

source /etc/profile

./a.out
