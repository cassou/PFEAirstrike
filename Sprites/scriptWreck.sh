#!/bin/bash
for NUMTYPE in {0..9}
do
echo plane $NUMTYPE
convert $NUMTYPE.png -normalize -resize '56x56' -background transparent -gravity center -extent 56x56 tmp.png
convert tmp.png -normalize -resize '64x64>' -background transparent -gravity center -extent 64x64 tmp.png
convert tmp.png -colorspace Gray gray.png 
convert -background transparent -black-threshold 50% gray.png black.png
   for i in {0..64}
   do
         j=$(echo "scale=2;180+360/64*$i" | bc)
         convert black.png -distort SRT $j tmp.png 
	 if [ $i -eq 0 ]
	 then
	 	cp tmp.png tot.png
	 else
         	montage -background none -geometry +0+0 tot.png tmp.png tot.png
	 fi	
   done
mv tot.png ./Planes/plane-$NUMTYPE-wreck.png
rm black.png 
rm tmp.png 
done
