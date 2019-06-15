myint=12
myfloat=1.1
mystr="hello world"

:<<! 
echo "sdfsdf"
ecj 
!


echo ${mystr}
echo "  "
#echo $myint

name="liuxin"
echo "  author ${name}"
printf -- 'this is my first shell  script ${name}\n';
printf -- '-----------------------------------------------\n';


echo `expr index "$mystr" eo:`
int_vector=(
"hello"
"world"
"liu"
"xin"
12
243
)
echo ${int_vector[@]}

