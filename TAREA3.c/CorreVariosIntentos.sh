a=0
maximo=$1
programa=$2

while [ $a -lt $maximo ]
do
   ./$programa >> "Resultados_$programa_$maximo.txt"
   echo "Intento " $a
   a=$((a+1))
done

less "Resultados_$programa_$maximo.txt"
