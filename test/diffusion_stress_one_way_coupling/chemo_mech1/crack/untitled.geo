//+
lc = DefineNumber[ 0.625, Name "Parameters/lc" ];
//+
Point(1) = {0, 0, 0, 1.0};
//+
Point(2) = {0.625, 0, 0, 1.0};
//+
Point(3) = {0.625, 0.625, 0, 1.0};
//+
Point(4) = {-0.625, 0.625, 0, 1.0};
//+
Point(5) = {-0.625, -0.625, 0, 1.0};
//+
Point(6) = {0.625, -0.625, 0, 1.0};
//+
Point(7) = {0.625, 0, 0, 1.0};
//+
Line(1) = {1, 2};
//+
Line(2) = {2, 3};
//+
Line(3) = {3, 4};
//+
Line(4) = {4, 5};
//+
Line(5) = {5, 6};
//+
Line(6) = {6, 7};
//+
Line(7) = {7, 1};
//+
Curve Loop(1) = {1, 2, 3, 4, 5, 6, 7};
//+
Plane Surface(1) = {1};
//+
Physical Curve("top") = {3};
//+
Physical Curve("bot") = {5};
//+
Physical Curve("left") = {4};
//+
Characteristic Length(1) = 0.01;
//+
Physical Surface("all") = {1};
//+
Recombine Surface {1};
