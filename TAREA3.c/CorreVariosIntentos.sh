a=0
maximo=$1
programa=$2
archivo="Resultados_"$2"_"$1".csv"

echo "Local Best, Global Best" > $archivo

while [ $a -lt $maximo ]
do
   ./$programa >> $archivo
   echo "Intento " $a
   a=$((a+1))
done

printf "Abriendo archivo"

a=0
while [ $a -lt 3 ]; do
  sleep 1s
  printf "."
  a=$((a+1))
done
echo "."

less $archivo
