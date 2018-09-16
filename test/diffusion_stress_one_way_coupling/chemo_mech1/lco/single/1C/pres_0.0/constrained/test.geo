// Gmsh project created on Fri Sep 14 10:26:38 2018
SetFactory("OpenCASCADE");
//+
r = DefineNumber[ 2.0, Name "Parameters/r" ];
//+
l = DefineNumber[ 0.5, Name "Parameters/l" ];
//+
Point(1) = {0, 0, 0, 1.0};
//+
Point(2) = {l, 0, 0, 1.0};
//+
Point(3) = {l, l, 0, 1.0};
//+
Point(4) = {0, l, 0, 1.0};
//+
Point(5) = {r, 0, 0, 1.0};
//+
Point(7) = {r/1.41421356237, r/1.41421356237, 0, 1.0};
//+
Point(8) = {0, r, 0, 1.0};
//+
Line(1) = {1, 2};
//+
Line(2) = {2, 3};
//+
Line(3) = {3, 4};
//+
Line(4) = {4, 1};
//+
Line(5) = {2, 5};
//+
Line(6) = {4, 8};
//+
Circle(7) = {8, 1, 7};
//+
Circle(8) = {7, 1, 5};
//+
Line(9) = {3, 7};
//+
Curve Loop(1) = {1, 2, 3, 4};
//+
Plane Surface(1) = {1};
//+
Curve Loop(2) = {5, -8, -9, -2};
//+
Plane Surface(2) = {2};
//+
Curve Loop(3) = {9, -7, -6, -3};
//+
Plane Surface(3) = {3};
//+
Transfinite Curve {1, 2, 3, 4} = 10 Using Progression 1;
//+
Transfinite Curve {5, 8, 9} = 20 Using Progression 1;
//+
Transfinite Curve {7, 6} = 20 Using Progression 1;
//+
Transfinite Surface {1} = {1, 2, 3, 4} Alternated;
//+
Transfinite Surface {2} = {5, 7, 3, 2} Alternated;
//+
Transfinite Surface {3} = {7, 8, 4, 3} Alternated;
//+
l2 = DefineNumber[ 3.5, Name "Parameters/l2" ];
//+
Point(9) = {l2, 0, 0, 1.0};
//+
Point(10) = {l2, l2, 0, 1.0};
//+
Point(11) = {0, l2, 0, 1.0};
//+
Line(10) = {5, 9};
//+
Line(11) = {9, 10};
//+
Line(12) = {10, 11};
//+
Line(13) = {11, 8};
//+
Line(14) = {7, 10};
//+
Transfinite Curve {14, 11, 10, 12, 13} = 20 Using Progression 1;
//+
Curve Loop(4) = {14, -11, -10, -8};
//+
Plane Surface(4) = {4};
//+
Curve Loop(5) = {14, 12, 13, 7};
//+
Plane Surface(5) = {5};
//+
Physical Surface("electrolyte") = {4, 5};
//+
Physical Surface("electrode") = {1, 2, 3};
//+
Physical Curve("left_symm") = {13, 6, 4};
//+
Physical Curve("bottom_symm") = {1, 5, 10};
//+
Physical Curve("inner") = {7, 8};
//+
Physical Curve("top") = {12};
//+
Physical Curve("right") = {11};
