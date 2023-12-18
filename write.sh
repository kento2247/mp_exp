touch ./mips/soft/test.c
cat ./main.c > ./mips/soft/test.c
cd ./mips/soft
make clean
make
cd ../hard
make clean
make
make program