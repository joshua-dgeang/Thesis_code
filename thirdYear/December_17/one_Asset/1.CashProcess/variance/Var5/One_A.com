#$ -S /bin/bash
#$ -q serial
#$ -N Variance_5 

source /etc/profile

./a.out
