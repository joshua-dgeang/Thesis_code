 #$ -S /bin/bash

#$ -q serial
#$ -N zeroTwoFive


source /etc/profile

./a.out
