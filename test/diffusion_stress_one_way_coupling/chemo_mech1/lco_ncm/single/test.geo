// Gmsh project created on Mon Aug 27 13:05:13 2018
SetFactory("OpenCASCADE");
//+
Rectangle(1) = {-1.918, -1.918, 0, 3.836, 3.836, 0};
//+
Circle(5) = {0, 0, 0, 1.5, 0, 2*Pi};
//+
Physical Curve("top") = {3};
//+
Physical Curve("left") = {4};
//+
Physical Curve("bot") = {1};
//+
Physical Curve("right") = {2};
//+
Physical Curve("inner") = {5};
//+
Curve Loop(2) = {3, 4, 1, 2};
//+
Curve Loop(3) = {5};
//+
Plane Surface(2) = {2, 3};
//+
Curve Loop(4) = {5};
//+
Plane Surface(3) = {4};
//+
Physical Surface("outer") = {2};
//+
Physical Surface("inner") = {3};

Characteristic Length {1, 2, 3, 4} = 0.1;
Characteristic Length {5} = 0.05;

//+
Recombine Surface {1};
//+
Recombine Surface {3};
//+
Circle(6) = {0, 0, 0, 0.5, 0, 2*Pi};
