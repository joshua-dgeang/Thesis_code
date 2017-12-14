#$ -S /bin/bash
#$ -q serial
#$ -N Variance_10

source /etc/profile

./a.out
