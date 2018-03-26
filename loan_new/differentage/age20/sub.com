 #$ -S /bin/bash

#$ -q serial
#$ -N age20 


source /etc/profile

./a.out
