mystr="hello world"
#echo $myint

nuue="liuxin"
printf -- '-----------------------------------------------\n';
printf -- "lenth of mystr"
echo ${#mystr}
echo `expr index "$mystr" eo`
int_vector=(
"hello"
"world"
"liu"
"xin"
12
243
)
echo ${int_vector[*]}
echo "first : $0"
echo "second : $1"


