#!/bin/bash

echo "Start script"
NUMTYPE=$1
NUMCOLOR=0

convert $NUMTYPE.png -normalize -resize '70x70>' -background transparent -gravity center -extent 70x70 tmp.png

#convert tmp.png -colorspace Gray gray.png

#convert $NUMTYPE.png -colorspace Gray gray.png
#convert 2.png -resize 64 -background fuchsia -compose Copy -gravity center -extent 64x64 -quality 92 3.png
#for color in green red blue yellow cyan purple white silver orange lime
#do
   #echo $color
   #convert -background transparent -fill $color -tint 100% gray.png $color.png
   #cp $color.png tot.png
   #cp $color.png save.png
   for i in {1..64}
   do
         j=$(echo "scale=2;360+360/64*($i+32)" | bc)
         convert nyan-cat.png -distort SRT $j tmp.png 
         montage -background none -geometry +0+0 nyan-cat2.png tmp.png nyan-cat2.png
   done
   #mv tot.png plane-nyan-cat.png
  
#done
rm tmp.png
  

