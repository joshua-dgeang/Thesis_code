#$ -S /bin/bash

#$ -q serial
#$ -N ZeroKPandKN   

source /etc/profile

./a.out
