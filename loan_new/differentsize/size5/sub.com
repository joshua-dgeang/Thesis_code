 #$ -S /bin/bash

#$ -q serial
#$ -N size5 


source /etc/profile

./a.out
