#$ -S /bin/bash

#$ -q serial
#$ -N loans_optimal

source /etc/profile

./a.out
