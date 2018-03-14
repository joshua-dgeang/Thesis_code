 #$ -S /bin/bash

#$ -q serial
#$ -N interest004


source /etc/profile

./a.out
