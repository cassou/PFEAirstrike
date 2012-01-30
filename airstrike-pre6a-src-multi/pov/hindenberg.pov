/*
 * A Hindenberg zeppelin model for the Airstrike game
 * 
 * Notes:
 * - Hindenburg zeppelin had a rigid body made of aluminium,
 *   later versions used copper because it's less inflammable
 * - uses unnaturally bright colors
 *
 * (w) 2002 by Eero Tamminen
 */

#include "colors.inc"
#include "finish.inc"
#include "metals.inc"


#declare green_sec =
torus {
	1, 0.3
	pigment { Green }
	scale <3.2, 1.7, 1.7>
	translate 0.3 * z
}

#declare white_sec =
torus {
	1, 0.3
	pigment { White }
	scale <3.2, 1.7, 1.7>
	//scale <2.7, 1, 1.4>
	translate 0.3 * z
}

#declare body =
difference {
	sphere {
		0, 1
		scale <2.5, 1, 1>
		pigment { Red }
	}
	
	#declare turn = 0;
	#while (turn <= 360)
	object {
		green_sec
		rotate x * turn
	}
	#declare turn = turn + 30;
	#end
	
	#declare turn = 15;
	#while (turn <= 360)
	object {
		white_sec
		rotate x * turn
	}
	#declare turn = turn + 30;
	#end
}


#declare fin =
difference {
	sphere {
		0, 1
		scale <1.1, 0.7, 0.12>
		pigment { White }
	}
	sphere {
		0, 1
		scale <1, 2, 1>
		translate -x
	}
	sphere {
		0, 1
		scale <0.7, 1.2, 0.3>
		translate <-0.1, 0, 0.26>
	}
	sphere {
		0, 1
		scale <0.7, 1.2, 0.3>
		translate <-0.1, 0, -0.26>
	}
	pigment { Green }
	translate -2.3 * x
}


// gondola window y position and radius
#declare winy = -0.13;
#declare winr = 0.09;

#declare gondola =
difference {
	superellipsoid {
		<1, 0.25>
		rotate x * 90
		scale <0.9, 0.24, 0.4>
		translate -0.1 * y
	}
	cylinder {
		<-0.4, winy, 0.5>, <-0.4, winy, -0.5>, winr
	}
	cylinder {
		<0, winy, 0.5>, <0, winy, -0.5>, winr
	}
	cylinder {
		<0.4, winy, 0.5>, <0.4, winy, -0.5>, winr
	}
	texture { T_Brass_5C }
}

// hindenberg
union {
	object { body }
	object {
		fin
	}
	object {
		fin
		rotate 90 * x
	}
	object {
		gondola
		translate -0.8 * y
	}
	// zeppelin turning
	rotate 180 * clock * y
	finish {
	       Glossy
	       phong 0.5
	}
}


// general lighting
light_source {
	<-2, -2, -10>
	color White
}
light_source {
	<2, 3, -8>
	color White
}

camera {
	location <0, -1, -4>
	look_at  <0, 0, 0>
}
