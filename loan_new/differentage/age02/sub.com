 #$ -S /bin/bash

#$ -q serial
#$ -N age02 


source /etc/profile

./a.out
