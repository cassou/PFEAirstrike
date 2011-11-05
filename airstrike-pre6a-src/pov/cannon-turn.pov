
#declare XMIN=-9;
#declare XMAX=9;

#declare YMIN=-3.1;
#declare YMAX=9;

//#declare PREVIEW = 1;
#include "scene.inc"


#declare start_angle = 50;
#declare end_angle = 130;

// set animation clock / cannon angle
#declare clk = 0;
#declare heading = start_angle + (end_angle - start_angle) * clock;

#include "cannon.inc"
