

#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-1.1;
#declare YMAX=1.1;

#declare PREVIEW = 1;

#include "scene.inc"

#include "colors.inc"
#include "textures.inc"
#include "fun.inc"

#declare num = 800;

#declare I = seed(2455);

union{
#while (num > 0)
sphere{<fun6(rand(I)) - 0.5, fun6(rand(I))-0.5, fun6(rand(I))-0.5>*fun5(clock + 0.1)*1.6
	, pow(rand(I),2)*0.17*fun5(clock + 0.7) }
#declare num = num - 1;
#end
texture{
pigment{Gray50}
normal{wrinkles 0.3 scale 0.1}
finish{ ambient 0.5 }
}
}