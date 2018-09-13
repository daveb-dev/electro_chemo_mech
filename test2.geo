//+
Point(1) = {0, 0, 0, 1.0};
//+
Point(2) = {2, 0, 0, 1.0};
//+
Point(3) = {0, 2, 0, 1.0};
//+
Point(4) = {-2, 0, 0, 1.0};
//+
Point(5) = {0, -2, 0, 1.0};
//+
Line(1) = {1, 2};
//+
Line(2) = {1, 3};
//+
Line(3) = {1, 4};
//+
Line(4) = {1, 5};
//+
Circle(5) = {2, 1, 3};
//+
Circle(6) = {3, 1, 4};
//+
Circle(7) = {4, 1, 5};
//+
Circle(8) = {5, 1, 2};
//+
Curve Loop(1) = {5, -2, 1};
//+
Plane Surface(1) = {1};
//+
Curve Loop(2) = {6, -3, 2};
//+
Plane Surface(2) = {2};
//+
Curve Loop(3) = {7, -4, 3};
//+
Plane Surface(3) = {3};
//+
Curve Loop(4) = {8, -1, 4};
//+
Plane Surface(4) = {4};
//+
Physical Curve("outer") = {5, 6, 7, 8};
//+
//+
Transfinite Curve {1} = 10 Using Progression 1;
//+
Transfinite Curve {5} = 10 Using Progression 1;
//+
Transfinite Curve {2} = 10 Using Progression 1;
//+
Transfinite Curve {6} = 10 Using Progression 1;
//+
Transfinite Curve {3} = 10 Using Progression 1;
//+
Transfinite Curve {7} = 10 Using Progression 1;
//+
Transfinite Curve {4} = 10 Using Progression 1;
//+
Transfinite Curve {8} = 10 Using Progression 1;
//+
Recombine Surface {1, 2, 3, 4};
//+
Physical Surface("all") = {1, 2, 3, 4};
//+
Transfinite Surface {1} = {1, 2, 3} Alternated;
//+
Transfinite Surface {2} = {1, 3, 4} Alternated;
//+
Transfinite Surface {3} = {1, 4, 5} Alternated;
//+
Transfinite Surface {4} = {1, 5, 2} Alternated;
//+
