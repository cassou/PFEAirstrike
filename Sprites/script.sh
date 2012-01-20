#!/bin/bash

NUMTYPE=$1
NUMCOLOR=0

convert $NUMTYPE.png -normalize -resize '56x56' -background transparent -gravity center -extent 56x56 tmp.png
convert tmp.png -normalize -resize '64x64>' -background transparent -gravity center -extent 64x64 tmp.png

convert tmp.png -colorspace Gray gray.png
#convert -background transparent -black-threshold 50% gray.png ./Planes/plane-$NUMTYPE-wreck.png

#convert $NUMTYPE.png -colorspace Gray gray.png
#convert 2.png -resize 64 -background fuchsia -compose Copy -gravity center -extent 64x64 -quality 92 3.png
for color in lime red blue yellow cyan purple orange white green DarkBlue 
do
   echo $color
   convert -background transparent -fill $color -tint 100% gray.png $color.png
   #cp $color.png tot.png
   #rm tot.png
   #cp $color.png save.png
   for i in {0..64}
   do
         j=$(echo "scale=2;180+360/64*$i" | bc)
         convert $color.png -distort SRT $j tmp.png 
	 if [ $i -eq 0 ]
	 then
	 	cp tmp.png tot.png
	 else
         	montage -background none -geometry +0+0 tot.png tmp.png tot.png
	 fi	
   done
   mv tot.png ./Planes/plane-$NUMCOLOR-$NUMTYPE.png
   rm $color.png
   NUMCOLOR=`expr $NUMCOLOR + 1` 
done
rm tmp.png

  

