

#declare XMIN=-1.3333;
#declare XMAX=1.3333;

#declare YMIN=-1;
#declare YMAX=1;

#declare PREVIEW = 1;

camera {
    orthographic
    location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-5>
    look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
    up y*(YMAX - YMIN)
    right x*(XMAX - XMIN)
  }

background { color rgb<0.8,0.75,1.0> }

#ifdef (PREVIEW)
light_source { <-1, 5, 3>*4 color rgb<1,0.9,0.8>}
//light_source { <0, 0, -5> color rgb <1,1,1>*0.5 }
#else
light_source { <-1, 5, -5>*4 color rgb<1,0.9,0.8>
	area_light
	10*x,10*y,5,5
	adaptive 4
	circular orient}

light_source { <0, 0, -5> color rgb <1,1,1>*0.5 
	area_light
	10*x,10*y,5,5
	adaptive 4
	circular orient}
#end


#include "colors.inc"
#include "textures.inc"
#include "fun.inc"

#declare tree =
union{
	sphere{ <0,0,0>,0.1 
		texture{pigment{ color rgb <0,0.5,0> }
			normal{ bumps 0.5 scale 0.009 }
			finish{ambient 0.3 }
		}
	}
	cylinder{ <0,0,0>,<0,-0.13,0> 0.01 pigment{Brown}}
	scale <1,1.41,1> 
}
#include "skies.inc"
//object{O_Cloud1 scale 0.001}

union{
sphere{ <-1,-1.8,0>,1.3}
sphere{ <-0.1,-2.3,-1>,1.7}
sphere{ <1,-1.8,0>,1.3}
texture{ pigment{ bozo
	color_map{
		[0 Green]
		[1 Yellow]
	}
	scale 0.7
	translate z*34
	}
	normal{ bumps 0.8 scale 0.008 }
}
	translate y*-0.2
}

//object{ tree translate <0.6,-0.6,-0.5>  }