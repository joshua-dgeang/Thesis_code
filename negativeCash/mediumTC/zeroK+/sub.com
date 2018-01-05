#$ -S /bin/bash

#$ -q serial
#$ -N ZeroKP   

source /etc/profile

./a.out
