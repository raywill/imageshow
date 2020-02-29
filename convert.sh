find $1 -name "*jpg" -print | parallel -j 50 --bar -I% --max-args 1 ./enc %
