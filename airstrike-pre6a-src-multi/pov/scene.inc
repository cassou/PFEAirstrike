
camera {
    orthographic
    location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-5>
    look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
    up y*(YMAX - YMIN)
    right x*(XMAX - XMIN)
  }

background { color rgb<0.8,0.75,1.0> }

#ifdef (PREVIEW)
light_source { <-1, 5, -5>*4 color rgb<1,0.9,0.8>}
light_source { <0, 0, -5> color rgb <1,1,1>*0.5 }
#else
light_source { <-1, 5, -3>*4 color rgb<1,0.9,0.8>
	area_light
	20*x,20*y,5,5
	adaptive 4
	circular orient}

light_source { <0, 0, -5> color rgb <1,1,1>*0.5 
	area_light
	10*x,10*y,5,5
	adaptive 4
	circular orient}
#end

#include "textures.inc"

plane{<0,-1,0>,-100 texture{ Shadow_Clouds scale 20} } // sky plane
plane{<0,1,0>,-100 texture{ Jade scale 20} } // ground plane

#macro dirty_color(col,clean)
	pigment{ bozo color_map 
		{ [clean col]
  		  [1 col*<clean,clean,0.8*clean>] 
		}
	scale 0.5
	}	
#end
		
