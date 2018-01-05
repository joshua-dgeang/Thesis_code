#$ -S /bin/bash

#$ -q serial
#$ -N ZeroKN   

source /etc/profile

./a.out
