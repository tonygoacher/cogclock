include <text_on.scad>

r=58;

array = [
"11",
"12",
" 2",
"10",
" 6",
" 5",
" 1",
" 3",
" 8",
" 7",
" 4",
" 9" ];
projection(slice=true)
{
for( angle = [0:len(array)-1])
{
 
    rotate([0,0,angle * (360/len(array))])
    {
        text_on_circle(str(array[angle]),size=8,r=r,font="DejaVuSansCondensed",        spacing=1);
    }
}
}



