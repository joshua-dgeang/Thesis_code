#$ -S /bin/bash
#$ -q serial
#$ -N one_A_29_11

source /etc/profile

./a.out
