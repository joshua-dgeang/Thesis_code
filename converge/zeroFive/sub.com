 #$ -S /bin/bash

#$ -q serial
#$ -N zero_five

source /etc/profile

./a.out
