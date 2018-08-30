//+
SetFactory("OpenCASCADE");
Rectangle(1) = {0, 0, 0, 1, 1, 0};
//+
Curve Loop(2) = {3, 4, 1, 2};
//+
Plane Surface(2) = {2};
Extrude Surface {2, {0,0,1}}
