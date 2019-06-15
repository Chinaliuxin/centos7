mystr="hello world"
#echo $myint

name="liuxin"
printf -- '-----------------------------------------------\n';
printf -- "lenth of mystr"
echo ${#mystr}
echo `expr index "$mystr" eo`
echo "first : $0"
echo "second : $1"
for i in "$*";
do
  echo   $i
done
ret=`expr 2 + 2`
echo "ret is ${ret}"
printf "%-10s  %d" name 2

a=11
b=1
if [ $a==$b ]; then
mkdir liuxin
fi 

