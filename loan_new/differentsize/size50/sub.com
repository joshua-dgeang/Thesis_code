 #$ -S /bin/bash

#$ -q serial
#$ -N size50


source /etc/profile

./a.out
