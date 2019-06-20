 
##echo "$1"


##echo"$0"

read -p "input num :" num

echo "---------------------------------"

##for while until 
for((i=0;i<=${num};i++))
do 
  sum_for=$(($sum_for +$i))
done
echo "for_result``: $sum_for"
echo "---------------------------------"

unset i
i=0
while(($i<=num))
do 
  sum_while=$(($sum_while+$i))
  ((i++))
done 
echo "while_result :$sum_while"
echo "---------------------------------"

unset i
i=1
until(($i>$num))
do 
  sum_until=$(($sum_until+$i))
  let "i++"
done 
echo "until_result :$sum_until"

