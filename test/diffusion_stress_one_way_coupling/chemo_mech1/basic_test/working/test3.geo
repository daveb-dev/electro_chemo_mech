// Gmsh project created on Thu Sep 13 14:03:29 2018
SetFactory("OpenCASCADE");
//+
inner_sq = DefineNumber[ 0.5, Name "Parameters/inner_sq" ];
//+
rad = DefineNumber[ 2.0, Name "Parameters/rad" ];
//+
outer_sq = DefineNumber[ 3.8, Name "Parameters/outer_sq" ];
//+
Point(1) = {0, 0, 0, 1.0};
//+
Point(2) = {inner_sq, 0, 0, 1.0};
//+
Point(3) = {inner_sq, inner_sq, 0, 1.0};
//+
Point(4) = {0, inner_sq, 0, 1.0};
//+
Point(5) = {-inner_sq, 0, 0, 1.0};
//+
Point(6) = {-inner_sq, -inner_sq, 0, 1.0};
//+
Point(7) = {0, -inner_sq, 0, 1.0};
//+
Point(8) = {inner_sq, -inner_sq, 0, 1.0};
//+
Point(9) = {rad, 0, 0, 1.0};
//+
Point(10) = {0, rad, 0, 1.0};
//+
Point(11) = {-rad, 0, 0, 1.0};
//+
Point(12) = {0, -rad, 0, 1.0};
//+
Point(13) = {outer_sq/2, 0, 0, 1.0};
//+
Recursive Delete {
  Point{13}; 
}
//+
Point(13) = {outer_sq, 0, 0, 1.0};
//+
Point(14) = {outer_sq, outer_sq, 0, 1.0};
//+
Point(15) = {0, outer_sq, 0, 1.0};
//+
Point(16) = {-outer_sq, outer_sq, 0, 1.0};
//+
Point(17) = {-outer_sq, 0, 0, 1.0};
//+
Point(18) = {-outer_sq, -outer_sq, 0, 1.0};
//+
Point(19) = {0, -outer_sq, 0, 1.0};
//+
Point(20) = {outer_sq, -outer_sq, 0, 1.0};
//+
Point(21) = {-inner_sq, inner_sq, 0, 1.0};
//+
Line(1) = {2, 3};
//+
Line(2) = {3, 4};
//+
Line(3) = {4, 21};
//+
Line(4) = {21, 5};
//+
Line(5) = {5, 6};
//+
Line(6) = {6, 7};
//+
Line(7) = {7, 8};
//+
Line(8) = {8, 2};
//+
Line(9) = {2, 9};
//+
Line(10) = {5, 11};
//+
Line(11) = {7, 12};
//+
Line(12) = {4, 10};
//+
Line(13) = {10, 15};
//+
Line(14) = {19, 12};
//+
Line(15) = {9, 13};
//+
Line(16) = {11, 17};
//+
Line(17) = {13, 14};
//+
Line(18) = {14, 15};
//+
Line(19) = {15, 16};
//+
Line(20) = {16, 17};
//+
Line(21) = {17, 18};
//+
Line(22) = {18, 19};
//+
Line(23) = {19, 20};
//+
Line(24) = {20, 13};
//+
Circle(25) = {9, 1, 10};
//+
Circle(26) = {10, 1, 11};
//+
Circle(27) = {11, 1, 12};
//+
Circle(28) = {12, 1, 9};
//+
Curve Loop(1) = {9, 25, -12, -2, -1};
//+
Plane Surface(1) = {1};
//+
Curve Loop(2) = {12, 26, -10, -4, -3};
//+
Plane Surface(2) = {2};
//+
Curve Loop(3) = {27, -11, -6, -5, 10};
//+
Plane Surface(3) = {3};
//+
Curve Loop(4) = {11, 28, -9, -8, -7};
//+
Plane Surface(4) = {4};
//+
Curve Loop(5) = {3, 4, 5, 6, 7, 8, 1, 2};
//+
Plane Surface(5) = {5};
//+
Curve Loop(6) = {15, 17, 18, -13, -25};
//+
Plane Surface(6) = {6};
//+
Curve Loop(7) = {13, 19, 20, -16, -26};
//+
Plane Surface(7) = {7};
//+
Curve Loop(8) = {16, 21, 22, 14, -27};
//+
Plane Surface(8) = {8};
//+
Curve Loop(9) = {14, 28, 15, -24, -23};
//+
Plane Surface(9) = {9};
//+
Physical Curve("inner") = {25, 26, 27, 28};
//+
Physical Curve("outer") = {19, 20, 21, 22, 23, 24, 17, 18};
//+
Physical Surface("outer") += {7, 8, 9, 6};
//+
Physical Curve("top") = {19, 18};
//+
Physical Curve("bot") = {22, 23};
//+
Physical Curve("left") = {20, 21};
//+
Physical Curve("right") = {17, 24};
//+
Physical Surface("inner") += {3, 4, 1, 2, 5};
//+
Transfinite Curve {9, 25, 12, 26, 10, 27, 11, 28} = 10 Using Progression 1;
//+
Transfinite Curve {5, 6, 7, 8, 1, 2, 3, 4} = 4 Using Progression 1;
//+
Transfinite Curve {21, 22, 23, 14, 16, 20, 19, 13, 18, 17, 15, 24} = 10 Using Progression 1;
//+
Transfinite Surface {5} = {6, 8, 3, 21} Alternated;
//+
Transfinite Surface {1} = {4, 3, 2, 10} Alternated;
