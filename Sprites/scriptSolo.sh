#!/bin/bash

echo "Start script"
for NUMTYPE in {0..9}
do
NUMCOLOR=0

convert $NUMTYPE.png -normalize -resize '64x64' -background transparent -gravity center -extent 64x64 tmp.png

convert tmp.png -colorspace Gray gray.png
#convert -background transparent -black-threshold 50% gray.png black.png

#convert $NUMTYPE.png -colorspace Gray gray.png
#convert 2.png -resize 64 -background fuchsia -compose Copy -gravity center -extent 64x64 -quality 92 3.png
for color in lime red blue yellow cyan purple orange white green DarkBlue 
do
   echo $color
   convert -background transparent -fill $color -tint 100% gray.png $color.png
   cp $color.png ./Solo/plane-$NUMCOLOR-$NUMTYPE.png
   #cp $color.png save.png
   #for i in {1..64}
   #do
   #      j=$(echo "scale=2;360/64*$i" | bc)
   #      convert $color.png -distort SRT $j tmp.png 
   #      montage -background none -geometry +0+0 tot.png tmp.png tot.png
   #done
   #mv tot.png plane-$NUMCOLOR-$NUMTYPE.png
   #cp  black.png plane-$NUMCOLOR-$NUMTYPE-wreck.png
   rm $color.png
   NUMCOLOR=`expr $NUMCOLOR + 1` 
done
done
rm tmp.png
#rm black.png
  

