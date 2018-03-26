#$ -S /bin/bash

#$ -q serial
#$ -N interest0015 


source /etc/profile

./a.out
