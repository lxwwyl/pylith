// -*- C++ -*-
//
// ======================================================================
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ======================================================================
//


/* Original mesh
 *
 * Cells are 0-1 and vertices are 2-13.
 *
 *       2,3,4,5 -------- 6,7,8,9 -------- 10,11,12,13
 *
 *                        ^^^^^^^ Vertices forming fault
 *
 * After adding cohesive elements
 *
 * Cells are 0-1,16 and vertices are 4-15.
 *
 *       2,3,4,5 -------- 6,7,8,9 -- 14,15,16,17 -------- 10,11,12,13
 *                                    18,19,20,21
 *                        ^^^^^^^^^^^^^^^^^^^^^^ Cohesive element
 *
 */

#include "CohesiveDynDataHex8.hh"

const char* pylith::faults::CohesiveDynDataHex8::_meshFilename =
  "data/hex8.mesh";

const int pylith::faults::CohesiveDynDataHex8::_spaceDim = 3;

const int pylith::faults::CohesiveDynDataHex8::_cellDim = 2;

const int pylith::faults::CohesiveDynDataHex8::_numBasis = 4;

const int pylith::faults::CohesiveDynDataHex8::_numQuadPts = 4;

const double pylith::faults::CohesiveDynDataHex8::_quadPts[] = {
  -0.57735027, -0.57735027,
  +0.57735027, -0.57735027,
  +0.57735027, +0.57735027,
  -0.57735027, +0.57735027,
};

const double pylith::faults::CohesiveDynDataHex8::_quadWts[] = {
  1.0, 1.0, 1.0, 1.0
};

const double pylith::faults::CohesiveDynDataHex8::_basis[] = {
  0.62200847,  0.16666667,  0.0446582,   0.16666667,
  0.16666667,  0.62200847,  0.16666667,   0.0446582,
  0.0446582,   0.16666667,  0.62200847,  0.16666667,
  0.16666667,   0.0446582,  0.16666667,  0.62200847,
};

const double pylith::faults::CohesiveDynDataHex8::_basisDeriv[] = {
  -0.39433757, -0.39433757,
  +0.39433757, -0.10566243,
  +0.10566243, +0.10566243,
  -0.10566243, +0.39433757,

  -0.39433757, -0.10566243,
  +0.39433757, -0.39433757,
  +0.10566243, +0.39433757,
  -0.10566243, +0.10566243,

  -0.10566243, -0.10566243,
  +0.10566243, -0.39433757,
  +0.39433757, +0.39433757,
  -0.39433757, +0.10566243,

  -0.10566243, -0.39433757,
  +0.10566243, -0.10566243,
  +0.39433757, +0.10566243,
  -0.39433757, +0.39433757,
};

const double pylith::faults::CohesiveDynDataHex8::_verticesRef[] = {
  -1.0, -1.0,
  +1.0, -1.0,
  +1.0, +1.0,
  -1.0, +1.0
};

const int pylith::faults::CohesiveDynDataHex8::_id = 10;

const char* pylith::faults::CohesiveDynDataHex8::_label = "fault";

const char* pylith::faults::CohesiveDynDataHex8::_initialTractFilename = 
  "data/hex8_initialtract.spatialdb";

const double pylith::faults::CohesiveDynDataHex8::_fieldT[] = {
  4.1, 6.1, 8.1,
  4.2, 6.2, 8.2,
  4.3, 6.3, 8.3,
  4.4, 6.4, 8.4,
  4.5, 6.5, 8.5, // 6
  4.6, 6.6, 8.6, // 7
  4.7, 6.7, 8.7, // 8
  4.8, 6.8, 8.8, // 9
  4.9, 6.9, 8.9,
  4.0, 6.0, 8.0,
  5.1, 7.1, 9.1,
  5.2, 7.2, 9.2,
  5.3, 7.3, 9.3, // 14
  5.5, 7.5, 9.5, // 15
  5.7, 7.7, 9.7, // 16
  5.9, 7.9, 9.9, // 17
  5.4, 7.4, 9.4, // 18
  5.6, 7.6, 9.6, // 19
  5.8, 7.8, 9.8, // 20
  5.0, 7.0, 9.0, // 21
};

// :TODO: Make sensible values for Jacobian for DOF on positive and
// negative sides of the fault. Add semi-random values for other DOF.
const double pylith::faults::CohesiveDynDataHex8::_jacobian[] = {
    1,  0.1,  0.2,
  0.3,  0.4,  0.5,
  0.6,  0.7,  0.8,
  0.9,    1,  1.1,
  1.2,  1.3,  1.4,
  1.5,  1.6,  1.7,
  1.8,  1.9,    2,
  2.1,  2.2,  2.3,
  2.4,  2.5,  2.6,
  2.7,  2.8,  2.9,
    3,  3.1,  3.2,
  3.3,  3.4,  3.5,
  3.6,  3.7,  3.8,
  3.9,    4,  4.1,
  4.2,  4.3,  4.4,
  4.5,  4.6,  4.7,
  4.8,  4.9,    5,
  5.1,  5.2,  5.3,
  5.4,  5.5,  5.6,
  5.7,  5.8,  5.9,
    6,    1,  6.1,
  6.2,  6.3,  6.4,
  6.5,  6.6,  6.7,
  6.8,  6.9,    7,
  7.1,  7.2,  7.3,
  7.4,  7.5,  7.6,
  7.7,  7.8,  7.9,
    8,  8.1,  8.2,
  8.3,  8.4,  8.5,
  8.6,  8.7,  8.8,
  8.9,    9,  9.1,
  9.2,  9.3,  9.4,
  9.5,  9.6,  9.7,
  9.8,  9.9,   10,
 10.1, 10.2, 10.3,
 10.4, 10.5, 10.6,
 10.7, 10.8, 10.9,
   11, 11.1, 11.2,
 11.3, 11.4, 11.5,
 11.6, 11.7, 11.8,
 11.9,   12,    1,
 12.1, 12.2, 12.3,
 12.4, 12.5, 12.6,
 12.7, 12.8, 12.9,
   13, 13.1, 13.2,
 13.3, 13.4, 13.5,
 13.6, 13.7, 13.8,
 13.9,   14, 14.1,
 14.2, 14.3, 14.4,
 14.5, 14.6, 14.7,
 14.8, 14.9,   15,
 15.1, 15.2, 15.3,
 15.4, 15.5, 15.6,
 15.7, 15.8, 15.9,
   16, 16.1, 16.2,
 16.3, 16.4, 16.5,
 16.6, 16.7, 16.8,
 16.9,   17, 17.1,
 17.2, 17.3, 17.4,
 17.5, 17.6, 17.7,
 17.8, 17.9,   18,
    1, 18.1, 18.2,
 18.3, 18.4, 18.5,
 18.6, 18.7, 18.8,
 18.9,   19, 19.1,
 19.2, 19.3, 19.4,
 19.5, 19.6, 19.7,
 19.8, 19.9,   20,
 20.1, 20.2, 20.3,
 20.4, 20.5, 20.6,
 20.7, 20.8, 20.9,
   21, 21.1, 21.2,
 21.3, 21.4, 21.5,
 21.6, 21.7, 21.8,
 21.9,   22, 22.1,
 22.2, 22.3, 22.4,
 22.5, 22.6, 22.7,
 22.8, 22.9,   23,
 23.1, 23.2, 23.3,
 23.4, 23.5, 23.6,
 23.7, 23.8, 23.9,
   24,    1, 24.1,
 24.2, 24.3, 24.4,
 24.5, 24.6, 24.7,
 24.8, 24.9,   25,
 25.1, 25.2, 25.3,
 25.4, 25.5, 25.6,
 25.7, 25.8, 25.9,
   26, 26.1, 26.2,
 26.3, 26.4, 26.5,
 26.6, 26.7, 26.8,
 26.9,   27, 27.1,
 27.2, 27.3, 27.4,
 27.5, 27.6, 27.7,
 27.8, 27.9,   28,
 28.1, 28.2, 28.3,
 28.4, 28.5, 28.6,
 28.7, 28.8, 28.9,
   29, 29.1, 29.2,
 29.3, 29.4, 29.5,
 29.6, 29.7, 29.8,
 29.9,   30,    1,
 30.1, 30.2, 30.3,
 30.4, 30.5, 30.6,
 30.7, 30.8, 30.9,
   31, 31.1, 31.2,
 31.3, 31.4, 31.5,
 31.6, 31.7, 31.8,
 31.9,   32, 32.1,
 32.2, 32.3, 32.4,
 32.5, 32.6, 32.7,
 32.8, 32.9,   33,
 33.1, 33.2, 33.3,
 33.4, 33.5, 33.6,
 33.7, 33.8, 33.9,
   34, 34.1, 34.2,
 34.3, 34.4, 34.5,
 34.6, 34.7, 34.8,
 34.9,   35, 35.1,
 35.2, 35.3, 35.4,
 35.5, 35.6, 35.7,
 35.8, 35.9,   36,
    1, 36.1, 36.2,
 36.3, 36.4, 36.5,
 36.6, 36.7, 36.8,
 36.9,   37, 37.1,
 37.2, 37.3, 37.4,
 37.5, 37.6, 37.7,
 37.8, 37.9,   38,
 38.1, 38.2, 38.3,
 38.4, 38.5, 38.6,
 38.7, 38.8, 38.9,
   39, 39.1, 39.2,
 39.3, 39.4, 39.5,
 39.6, 39.7, 39.8,
 39.9,   40, 40.1,
 40.2, 40.3, 40.4,
 40.5, 40.6, 40.7,
 40.8, 40.9,   41,
 41.1, 41.2, 41.3,
 41.4, 41.5, 41.6,
 41.7, 41.8, 41.9,
   42,    1, 42.1,
 42.2, 42.3, 42.4,
 42.5, 42.6, 42.7,
 42.8, 42.9,   43,
 43.1, 43.2, 43.3,
 43.4, 43.5, 43.6,
 43.7, 43.8, 43.9,
   44, 44.1, 44.2,
 44.3, 44.4, 44.5,
 44.6, 44.7, 44.8,
 44.9,   45, 45.1,
 45.2, 45.3, 45.4,
 45.5, 45.6, 45.7,
 45.8, 45.9,   46,
 46.1, 46.2, 46.3,
 46.4, 46.5, 46.6,
 46.7, 46.8, 46.9,
   47, 47.1, 47.2,
 47.3, 47.4, 47.5,
 47.6, 47.7, 47.8,
 47.9,   48,    1,
 48.1, 48.2, 48.3,
 48.4, 48.5, 48.6,
 48.7, 48.8, 48.9,
   49, 49.1, 49.2,
 49.3, 49.4, 49.5,
 49.6, 49.7, 49.8,
 49.9,   50, 50.1,
 50.2, 50.3, 50.4,
 50.5, 50.6, 50.7,
 50.8, 50.9,   51,
 51.1, 51.2, 51.3,
 51.4, 51.5, 51.6,
 51.7, 51.8, 51.9,
   52, 52.1, 52.2,
 52.3, 52.4, 52.5,
 52.6, 52.7, 52.8,
 52.9,   53, 53.1,
 53.2, 53.3, 53.4,
 53.5, 53.6, 53.7,
 53.8, 53.9,   54,
    1, 54.1, 54.2,
 54.3, 54.4, 54.5,
 54.6, 54.7, 54.8,
 54.9,   55, 55.1,
 55.2, 55.3, 55.4,
 55.5, 55.6, 55.7,
 55.8, 55.9,   56,
 56.1, 56.2, 56.3,
 56.4, 56.5, 56.6,
 56.7, 56.8, 56.9,
   57, 57.1, 57.2,
 57.3, 57.4, 57.5,
 57.6, 57.7, 57.8,
 57.9,   58, 58.1,
 58.2, 58.3, 58.4,
 58.5, 58.6, 58.7,
 58.8, 58.9,   59,
 59.1, 59.2, 59.3,
 59.4, 59.5, 59.6,
 59.7, 59.8, 59.9,
   60,    1, 60.1,
 60.2, 60.3, 60.4,
 60.5, 60.6, 60.7,
 60.8, 60.9,   61,
 61.1, 61.2, 61.3,
 61.4, 61.5, 61.6,
 61.7, 61.8, 61.9,
   62, 62.1, 62.2,
 62.3, 62.4, 62.5,
 62.6, 62.7, 62.8,
 62.9,   63, 63.1,
 63.2, 63.3, 63.4,
 63.5, 63.6, 63.7,
 63.8, 63.9,   64,
 64.1, 64.2, 64.3,
 64.4, 64.5, 64.6,
 64.7, 64.8, 64.9,
   65, 65.1, 65.2,
 65.3, 65.4, 65.5,
 65.6, 65.7, 65.8,
 65.9,   66,    1,
 66.1, 66.2, 66.3,
 66.4, 66.5, 66.6,
 66.7, 66.8, 66.9,
   67, 67.1, 67.2,
 67.3, 67.4, 67.5,
 67.6, 67.7, 67.8,
 67.9,   68, 68.1,
 68.2, 68.3, 68.4,
 68.5, 68.6, 68.7,
 68.8, 68.9,   69,
 69.1, 69.2, 69.3,
 69.4, 69.5, 69.6,
 69.7, 69.8, 69.9,
   70, 70.1, 70.2,
 70.3, 70.4, 70.5,
 70.6, 70.7, 70.8,
 70.9,   71, 71.1,
 71.2, 71.3, 71.4,
 71.5, 71.6, 71.7,
 71.8, 71.9,   72,
    1, 72.1, 72.2,
 72.3, 72.4, 72.5,
 72.6, 72.7, 72.8,
 72.9,   73, 73.1,
 73.2, 73.3, 73.4,
 73.5, 73.6, 73.7,
 73.8, 73.9,   74,
 74.1, 74.2, 74.3,
 74.4, 74.5, 74.6,
 74.7, 74.8, 74.9,
   75, 75.1, 75.2,
 75.3, 75.4, 75.5,
 75.6, 75.7,    1,
 75.8, 75.9,   76,
 76.1, 76.2, 76.3,
 76.4, 76.5, 76.6,
 76.7, 76.8, 76.9,
   77, 77.1, 77.2,
 77.3, 77.4, 77.5,
 77.6, 77.7, 77.8,
 77.9,    1,   78,
 78.1, 78.2, 78.3,
 78.4, 78.5, 78.6,
 78.7, 78.8, 78.9,
   79, 79.1, 79.2,
 79.3, 79.4, 79.5,
 79.6, 79.7, 79.8,
 79.9,   80, 80.1,
 80.2, 80.3, 80.4,
 80.5, 80.6, 80.7,
 80.8, 80.9,   81,
 81.1, 81.2, 81.3,
    1, 81.4, 81.5,
 81.6, 81.7, 81.8,
 81.9,   82, 82.1,
 82.2, 82.3, 82.4,
 82.5, 82.6, 82.7,
 82.8, 82.9,   83,
 83.1, 83.2, 83.3,
 83.4, 83.5, 83.6,
 83.7, 83.8,    1,
 83.9,   84, 84.1,
 84.2, 84.3, 84.4,
 84.5, 84.6, 84.7,
 84.8, 84.9,   85,
 85.1, 85.2, 85.3,
 85.4, 85.5, 85.6,
 85.7, 85.8, 85.9,
   86, 86.1, 86.2,
 86.3, 86.4, 86.5,
 86.6, 86.7, 86.8,
 86.9,   87, 87.1,
 87.2,    1, 87.3,
 87.4, 87.5, 87.6,
 87.7, 87.8, 87.9,
   88, 88.1, 88.2,
 88.3, 88.4, 88.5,
 88.6, 88.7, 88.8,
 88.9,   89, 89.1,
 89.2, 89.3, 89.4,
 89.5, 89.6, 89.7,
    1, 89.8, 89.9,
   90, 90.1, 90.2,
 90.3, 90.4, 90.5,
 90.6, 90.7, 90.8,
 90.9,   91, 91.1,
 91.2, 91.3, 91.4,
 91.5, 91.6, 91.7,
 91.8, 91.9,   92,
 92.1, 92.2, 92.3,
 92.4, 92.5, 92.6,
 92.7, 92.8, 92.9,
   93, 93.1, 93.2,
 93.3, 93.4,    1,
 93.5, 93.6, 93.7,
 93.8, 93.9,   94,
 94.1, 94.2, 94.3,
 94.4, 94.5, 94.6,
 94.7, 94.8, 94.9,
   95, 95.1, 95.2,
 95.3, 95.4, 95.5,
 95.6,    1, 95.7,
 95.8, 95.9,   96,
 96.1, 96.2, 96.3,
 96.4, 96.5, 96.6,
 96.7, 96.8, 96.9,
   97, 97.1, 97.2,
 97.3, 97.4, 97.5,
 97.6, 97.7, 97.8,
 97.9,   98, 98.1,
 98.2, 98.3, 98.4,
 98.5, 98.6, 98.7,
 98.8, 98.9,   99,
    1, 99.1, 99.2,
 99.3, 99.4, 99.5,
 99.6, 99.7, 99.8,
 99.9,  100,100.1,
100.2,100.3,100.4,
100.5,100.6,100.7,
100.8,100.9,  101,
101.1,101.2,101.3,
101.4,101.5,    1,
101.6,101.7,101.8,
101.9,  102,102.1,
102.2,102.3,102.4,
102.5,102.6,102.7,
102.8,102.9,  103,
103.1,103.2,103.3,
103.4,103.5,103.6,
103.7,103.8,103.9,
  104,104.1,104.2,
104.3,104.4,104.5,
104.6,104.7,104.8,
104.9,    1,  105,
105.1,105.2,105.3,
105.4,105.5,105.6,
105.7,105.8,105.9,
  106,106.1,106.2,
106.3,106.4,106.5,
106.6,106.7,106.8,
106.9,  107,107.1,
107.2,107.3,107.4,
    1,107.5,107.6,
107.7,107.8,107.9,
  108,108.1,108.2,
108.3,108.4,108.5,
108.6,108.7,108.8,
108.9,  109,109.1,
109.2,109.3,109.4,
109.5,109.6,109.7,
109.8,109.9,  110,
110.1,110.2,110.3,
110.4,110.5,110.6,
110.7,110.8,110.9,
  111,111.1,    1,
111.2,111.3,111.4,
111.5,111.6,111.7,
111.8,111.9,  112,
112.1,112.2,112.3,
112.4,112.5,112.6,
112.7,112.8,112.9,
  113,113.1,113.2,
113.3,    1,113.4,
113.5,113.6,113.7,
113.8,113.9,  114,
114.1,114.2,114.3,
114.4,114.5,114.6,
114.7,114.8,114.9,
  115,115.1,115.2,
115.3,115.4,115.5,
115.6,115.7,115.8,
115.9,  116,116.1,
116.2,116.3,116.4,
116.5,116.6,116.7,
    1,116.8,116.9,
  117,117.1,117.2,
117.3,117.4,117.5,
117.6,117.7,117.8,
117.9,  118,118.1,
118.2,118.3,118.4,
118.5,118.6,118.7,
118.8,118.9,  119,
119.1,119.2,    1,
119.3,119.4,119.5,
119.6,119.7,119.8,
119.9,  120,120.1,
120.2,120.3,120.4,
120.5,120.6,120.7,
120.8,120.9,  121,
121.1,121.2,121.3,
121.4,121.5,121.6,
121.7,121.8,121.9,
  122,122.1,122.2,
122.3,122.4,122.5,
122.6,    1,122.7,
122.8,122.9,  123,
123.1,123.2,123.3,
123.4,123.5,123.6,
123.7,123.8,123.9,
  124,124.1,124.2,
124.3,124.4,124.5,
124.6,124.7,124.8,
124.9,  125,125.1,
    1,125.2,125.3,
125.4,125.5,125.6,
125.7,125.8,125.9,
  126,126.1,126.2,
126.3,126.4,126.5,
126.6,126.7,126.8,
126.9,  127,127.1,
127.2,127.3,127.4,
127.5,127.6,127.7,
127.8,127.9,  128,
128.1,128.2,128.3,
128.4,128.5,128.6,
128.7,128.8,    1,
128.9,  129,129.1,
129.2,129.3,129.4,
129.5,129.6,129.7,
129.8,129.9,  130,
130.1,130.2,130.3,
130.4,130.5,130.6,
130.7,130.8,130.9,
  131,    1,131.1,
131.2,131.3,131.4,
131.5,131.6,131.7,
131.8,131.9,  132,
132.1,132.2,132.3,
132.4,132.5,132.6,
132.7,132.8,132.9,
  133,133.1,133.2,
133.3,133.4,133.5,
133.6,133.7,133.8,
133.9,  134,134.1,
134.2,134.3,134.4,
    1,134.5,134.6,
134.7,134.8,134.9,
  135,135.1,135.2,
135.3,135.4,135.5,
135.6,135.7,135.8,
135.9,  136,136.1,
136.2,136.3,136.4,
136.5,136.6,136.7,
136.8,136.9,    1,
  137,137.1,137.2,
137.3,137.4,137.5,
137.6,137.7,137.8,
137.9,  138,138.1,
138.2,138.3,138.4,
138.5,138.6,138.7,
138.8,138.9,  139,
139.1,139.2,139.3,
139.4,139.5,139.6,
139.7,139.8,139.9,
  140,140.1,140.2,
140.3,    1,140.4,
140.5,140.6,140.7,
140.8,140.9,  141,
141.1,141.2,141.3,
141.4,141.5,141.6,
141.7,141.8,141.9,
  142,142.1,142.2,
142.3,142.4,142.5,
142.6,142.7,142.8,
    1,142.9,  143,
143.1,143.2,143.3,
143.4,143.5,143.6,
143.7,143.8,143.9,
  144,144.1,144.2,
144.3,144.4,144.5,
144.6,144.7,144.8,
144.9,  145,145.1,
145.2,145.3,145.4,
145.5,145.6,145.7,
145.8,145.9,  146,
146.1,146.2,146.3,
146.4,146.5,146.6,
146.7,146.8,146.9,
  147,147.1,147.2,
147.3,147.4,147.5,
147.6,147.7,147.8,
147.9,  148,148.1,
148.2,148.3,148.4,
148.5,148.6,148.7,
148.8,    1,148.9,
  149,149.1,149.2,
149.3,149.4,149.5,
149.6,149.7,149.8,
149.9,  150,150.1,
150.2,150.3,150.4,
150.5,150.6,150.7,
150.8,150.9,  151,
151.1,151.2,151.3,
151.4,151.5,151.6,
151.7,151.8,151.9,
  152,152.1,152.2,
152.3,152.4,152.5,
152.6,152.7,152.8,
152.9,  153,153.1,
153.2,153.3,153.4,
153.5,153.6,153.7,
153.8,153.9,  154,
154.1,154.2,154.3,
154.4,154.5,154.6,
154.7,154.8,    1,
154.9,  155,155.1,
155.2,155.3,155.4,
155.5,155.6,155.7,
155.8,155.9,  156,
156.1,156.2,156.3,
156.4,156.5,156.6,
156.7,156.8,156.9,
  157,157.1,157.2,
157.3,157.4,157.5,
157.6,157.7,157.8,
157.9,  158,158.1,
158.2,158.3,158.4,
158.5,158.6,158.7,
158.8,158.9,  159,
159.1,159.2,159.3,
159.4,159.5,159.6,
159.7,159.8,159.9,
  160,160.1,160.2,
160.3,160.4,160.5,
160.6,160.7,160.8,
    1,160.9,  161,
161.1,161.2,161.3,
161.4,161.5,161.6,
161.7,161.8,161.9,
  162,162.1,162.2,
162.3,162.4,162.5,
162.6,162.7,162.8,
162.9,  163,163.1,
163.2,163.3,163.4,
163.5,163.6,163.7,
163.8,163.9,  164,
164.1,164.2,164.3,
164.4,164.5,164.6,
164.7,164.8,164.9,
  165,165.1,165.2,
165.3,165.4,165.5,
165.6,165.7,165.8,
165.9,  166,166.1,
166.2,166.3,166.4,
166.5,166.6,166.7,
166.8,    1,166.9,
  167,167.1,167.2,
167.3,167.4,167.5,
167.6,167.7,167.8,
167.9,  168,168.1,
168.2,168.3,168.4,
168.5,168.6,168.7,
168.8,168.9,  169,
169.1,169.2,169.3,
169.4,169.5,169.6,
169.7,169.8,169.9,
  170,170.1,170.2,
170.3,170.4,170.5,
170.6,170.7,170.8,
170.9,  171,171.1,
171.2,171.3,171.4,
171.5,171.6,171.7,
171.8,171.9,  172,
172.1,172.2,172.3,
172.4,172.5,172.6,
172.7,172.8,    1,
172.9,  173,173.1,
173.2,173.3,173.4,
173.5,173.6,173.7,
173.8,173.9,  174,
174.1,174.2,174.3,
174.4,174.5,174.6,
174.7,174.8,174.9,
  175,175.1,175.2,
175.3,175.4,175.5,
175.6,175.7,175.8,
175.9,  176,176.1,
176.2,176.3,176.4,
176.5,176.6,176.7,
176.8,176.9,  177,
177.1,177.2,177.3,
177.4,177.5,177.6,
177.7,177.8,177.9,
  178,178.1,178.2,
178.3,178.4,178.5,
178.6,178.7,178.8,
    1,178.9,  179,
179.1,179.2,179.3,
179.4,179.5,179.6,
179.7,179.8,179.9,
  180,180.1,180.2,
180.3,180.4,180.5,
180.6,180.7,180.8,
180.9,  181,181.1,
181.2,181.3,181.4,
181.5,181.6,181.7,
181.8,181.9,  182,
182.1,182.2,182.3,
182.4,182.5,182.6,
182.7,182.8,182.9,
  183,183.1,183.2,
183.3,183.4,183.5,
183.6,183.7,183.8,
183.9,  184,184.1,
184.2,184.3,184.4,
184.5,184.6,184.7,
184.8,    1,184.9,
  185,185.1,185.2,
185.3,185.4,185.5,
185.6,185.7,185.8,
185.9,  186,186.1,
186.2,186.3,186.4,
186.5,186.6,186.7,
186.8,186.9,  187,
187.1,187.2,187.3,
187.4,187.5,187.6,
187.7,187.8,187.9,
  188,188.1,188.2,
188.3,188.4,188.5,
188.6,188.7,188.8,
188.9,  189,189.1,
189.2,189.3,189.4,
189.5,189.6,189.7,
189.8,189.9,  190,
190.1,190.2,190.3,
190.4,190.5,190.6,
190.7,190.8,    1,
190.9,  191,191.1,
191.2,191.3,191.4,
191.5,191.6,191.7,
191.8,191.9,  192,
192.1,192.2,192.3,
192.4,192.5,192.6,
192.7,192.8,192.9,
  193,193.1,193.2,
193.3,193.4,193.5,
193.6,193.7,193.8,
193.9,  194,194.1,
194.2,194.3,194.4,
194.5,194.6,194.7,
194.8,194.9,  195,
195.1,195.2,195.3,
195.4,195.5,195.6,
195.7,195.8,195.9,
  196,196.1,196.2,
196.3,196.4,196.5,
196.6,196.7,196.8,
    1,196.9,  197,
197.1,197.2,197.3,
197.4,197.5,197.6,
197.7,197.8,197.9,
  198,198.1,198.2,
198.3,198.4,198.5,
198.6,198.7,198.8,
198.9,  199,199.1,
199.2,199.3,199.4,
199.5,199.6,199.7,
199.8,199.9,  200,
200.1,200.2,200.3,
200.4,200.5,200.6,
200.7,200.8,200.9,
  201,201.1,201.2,
201.3,201.4,201.5,
201.6,201.7,201.8,
201.9,  202,202.1,
202.2,202.3,202.4,
202.5,202.6,202.7,
202.8,    1,202.9,
  203,203.1,203.2,
203.3,203.4,203.5,
203.6,203.7,203.8,
203.9,  204,204.1,
204.2,204.3,204.4,
204.5,204.6,204.7,
204.8,204.9,  205,
205.1,205.2,205.3,
205.4,205.5,205.6,
205.7,205.8,205.9,
  206,206.1,206.2,
206.3,206.4,206.5,
206.6,206.7,206.8,
206.9,  207,207.1,
207.2,207.3,207.4,
207.5,207.6,207.7,
207.8,207.9,  208,
208.1,208.2,208.3,
208.4,208.5,208.6,
208.7,208.8,    1,
208.9,  209,209.1,
209.2,209.3,209.4,
209.5,209.6,209.7,
209.8,209.9,  210,
210.1,210.2,210.3,
210.4,210.5,210.6,
210.7,210.8,210.9,
  211,211.1,211.2,
211.3,211.4,211.5,
211.6,211.7,211.8,
211.9,  212,212.1,
212.2,212.3,212.4,
212.5,212.6,212.7,
212.8,212.9,  213,
213.1,213.2,213.3,
213.4,213.5,213.6,
213.7,213.8,213.9,
  214,214.1,214.2,
214.3,214.4,214.5,
214.6,214.7,214.8,
    1,214.9,  215,
215.1,215.2,215.3,
215.4,215.5,215.6,
215.7,215.8,215.9,
  216,216.1,   -1,
216.2,216.3,216.4,
216.5,216.6,216.7,
216.8,216.9,  217,
217.1,217.2,217.3,
217.4,217.5,217.6,
217.7,217.8,217.9,
  218,218.1,218.2,
218.3,218.4,218.5,
218.6,218.7,218.8,
218.9,  219,219.1,
219.2,219.3,219.4,
219.5,219.6,219.7,
219.8,219.9,  220,
220.1,220.2,220.3,
220.4,220.5,220.6,
220.7,    1,220.8,
220.9,  221,221.1,
221.2,221.3,221.4,
221.5,221.6,221.7,
   -1,221.8,221.9,
  222,222.1,222.2,
222.3,222.4,222.5,
222.6,222.7,222.8,
222.9,  223,223.1,
223.2,223.3,223.4,
223.5,223.6,223.7,
223.8,223.9,  224,
224.1,224.2,224.3,
224.4,224.5,224.6,
224.7,224.8,224.9,
  225,225.1,225.2,
225.3,225.4,225.5,
225.6,225.7,225.8,
225.9,  226,226.1,
226.2,226.3,226.4,
226.5,226.6,    1,
226.7,226.8,226.9,
  227,227.1,227.2,
227.3,227.4,227.5,
227.6,   -1,227.7,
227.8,227.9,  228,
228.1,228.2,228.3,
228.4,228.5,228.6,
228.7,228.8,228.9,
  229,229.1,229.2,
229.3,229.4,229.5,
229.6,229.7,229.8,
229.9,  230,230.1,
230.2,230.3,230.4,
230.5,230.6,230.7,
230.8,230.9,  231,
231.1,231.2,231.3,
231.4,231.5,231.6,
231.7,231.8,231.9,
  232,232.1,232.2,
232.3,232.4,232.5,
    1,232.6,232.7,
232.8,232.9,  233,
233.1,233.2,233.3,
233.4,233.5,233.6,
233.7,233.8,   -1,
233.9,  234,234.1,
234.2,234.3,234.4,
234.5,234.6,234.7,
234.8,234.9,  235,
235.1,235.2,235.3,
235.4,235.5,235.6,
235.7,235.8,235.9,
  236,236.1,236.2,
236.3,236.4,236.5,
236.6,236.7,236.8,
236.9,  237,237.1,
237.2,237.3,237.4,
237.5,237.6,237.7,
237.8,237.9,  238,
238.1,238.2,238.3,
238.4,    1,238.5,
238.6,238.7,238.8,
238.9,  239,239.1,
239.2,239.3,239.4,
   -1,239.5,239.6,
239.7,239.8,239.9,
  240,240.1,240.2,
240.3,240.4,240.5,
240.6,240.7,240.8,
240.9,  241,241.1,
241.2,241.3,241.4,
241.5,241.6,241.7,
241.8,241.9,  242,
242.1,242.2,242.3,
242.4,242.5,242.6,
242.7,242.8,242.9,
  243,243.1,243.2,
243.3,243.4,243.5,
243.6,243.7,243.8,
243.9,  244,244.1,
244.2,244.3,    1,
244.4,244.5,244.6,
244.7,244.8,244.9,
  245,245.1,245.2,
245.3,   -1,245.4,
245.5,245.6,245.7,
245.8,245.9,  246,
246.1,246.2,246.3,
246.4,246.5,246.6,
246.7,246.8,246.9,
  247,247.1,247.2,
247.3,247.4,247.5,
247.6,247.7,247.8,
247.9,  248,248.1,
248.2,248.3,248.4,
248.5,248.6,248.7,
248.8,248.9,  249,
249.1,249.2,249.3,
249.4,249.5,249.6,
249.7,249.8,249.9,
  250,250.1,250.2,
    1,250.3,250.4,
250.5,250.6,250.7,
250.8,250.9,  251,
251.1,251.2,251.3,
251.4,251.5,   -1,
251.6,251.7,251.8,
251.9,  252,252.1,
252.2,252.3,252.4,
252.5,252.6,252.7,
252.8,252.9,  253,
253.1,253.2,253.3,
253.4,253.5,253.6,
253.7,253.8,253.9,
  254,254.1,254.2,
254.3,254.4,254.5,
254.6,254.7,254.8,
254.9,  255,255.1,
255.2,255.3,255.4,
255.5,255.6,255.7,
255.8,255.9,  256,
256.1,    1,256.2,
256.3,256.4,256.5,
256.6,256.7,256.8,
256.9,  257,257.1,
   -1,257.2,257.3,
257.4,257.5,257.6,
257.7,257.8,257.9,
  258,258.1,258.2,
258.3,258.4,258.5,
258.6,258.7,258.8,
258.9,  259,259.1,
259.2,259.3,259.4,
259.5,259.6,259.7,
259.8,259.9,  260,
260.1,260.2,260.3,
260.4,260.5,260.6,
260.7,260.8,260.9,
  261,261.1,261.2,
261.3,261.4,261.5,
261.6,261.7,261.8,
261.9,  262,    1,
262.1,262.2,262.3,
262.4,262.5,262.6,
262.7,262.8,262.9,
  263,   -1,263.1,
263.2,263.3,263.4,
263.5,263.6,263.7,
263.8,263.9,  264,
264.1,264.2,264.3,
264.4,264.5,264.6,
264.7,264.8,264.9,
  265,265.1,265.2,
265.3,265.4,265.5,
265.6,265.7,265.8,
265.9,  266,266.1,
266.2,266.3,266.4,
266.5,266.6,266.7,
266.8,266.9,  267,
267.1,267.2,267.3,
267.4,267.5,267.6,
267.7,267.8,267.9,
    1,  268,268.1,
268.2,268.3,268.4,
268.5,268.6,268.7,
268.8,268.9,  269,
269.1,269.2,   -1,
269.3,269.4,269.5,
269.6,269.7,269.8,
269.9,  270,270.1,
270.2,270.3,270.4,
270.5,270.6,270.7,
270.8,270.9,  271,
271.1,271.2,271.3,
271.4,271.5,271.6,
271.7,271.8,271.9,
  272,272.1,272.2,
272.3,272.4,272.5,
272.6,272.7,272.8,
272.9,  273,273.1,
273.2,273.3,273.4,
273.5,273.6,273.7,
273.8,    1,273.9,
  274,274.1,274.2,
274.3,274.4,274.5,
274.6,274.7,274.8,
   -1,274.9,  275,
275.1,275.2,275.3,
275.4,275.5,275.6,
275.7,275.8,275.9,
  276,276.1,276.2,
276.3,276.4,276.5,
276.6,276.7,276.8,
276.9,  277,277.1,
277.2,277.3,277.4,
277.5,277.6,277.7,
277.8,277.9,  278,
278.1,278.2,278.3,
278.4,278.5,278.6,
278.7,278.8,278.9,
  279,279.1,279.2,
279.3,279.4,279.5,
279.6,279.7,    1,
279.8,279.9,  280,
280.1,280.2,280.3,
280.4,280.5,280.6,
280.7,   -1,280.8,
280.9,  281,281.1,
281.2,281.3,281.4,
281.5,281.6,281.7,
281.8,281.9,  282,
282.1,    1,282.2,
282.3,282.4,282.5,
282.6,282.7,282.8,
282.9,  283,283.1,
283.2,283.3,283.4,
283.5,283.6,283.7,
283.8,283.9,  284,
284.1,284.2,284.3,
284.4,   -1,284.5,
284.6,284.7,284.8,
284.9,  285,285.1,
285.2,285.3,285.4,
285.5,285.6,285.7,
285.8,285.9,  286,
286.1,286.2,286.3,
286.4,286.5,286.6,
286.7,286.8,286.9,
  287,287.1,287.2,
287.3,287.4,287.5,
287.6,287.7,287.8,
287.9,  288,    1,
288.1,288.2,288.3,
288.4,288.5,288.6,
288.7,288.8,288.9,
  289,289.1,289.2,
289.3,289.4,289.5,
289.6,289.7,289.8,
289.9,  290,290.1,
290.2,290.3,   -1,
290.4,290.5,290.6,
290.7,290.8,290.9,
  291,291.1,291.2,
291.3,291.4,291.5,
291.6,291.7,291.8,
291.9,  292,292.1,
292.2,292.3,292.4,
292.5,292.6,292.7,
292.8,292.9,  293,
293.1,293.2,293.3,
293.4,293.5,293.6,
    1,293.7,293.8,
293.9,  294,294.1,
294.2,294.3,294.4,
294.5,294.6,294.7,
294.8,294.9,  295,
295.1,295.2,295.3,
295.4,295.5,295.6,
295.7,295.8,295.9,
   -1,  296,296.1,
296.2,296.3,296.4,
296.5,296.6,296.7,
296.8,296.9,  297,
297.1,297.2,297.3,
297.4,297.5,297.6,
297.7,297.8,297.9,
  298,298.1,298.2,
298.3,298.4,298.5,
298.6,298.7,298.8,
298.9,  299,299.1,
299.2,299.3,299.4,
299.5,299.6,299.7,
299.8,    1,299.9,
  300,300.1,300.2,
300.3,300.4,300.5,
300.6,300.7,300.8,
300.9,  301,301.1,
301.2,301.3,301.4,
301.5,301.6,301.7,
301.8,301.9,  302,
302.1,   -1,302.2,
302.3,302.4,302.5,
302.6,302.7,302.8,
302.9,  303,303.1,
303.2,303.3,303.4,
303.5,303.6,303.7,
303.8,303.9,  304,
304.1,304.2,304.3,
304.4,304.5,304.6,
304.7,304.8,304.9,
  305,305.1,305.2,
305.3,305.4,305.5,
305.6,305.7,    1,
305.8,305.9,  306,
306.1,306.2,306.3,
306.4,306.5,306.6,
306.7,306.8,306.9,
  307,307.1,307.2,
307.3,307.4,307.5,
307.6,307.7,307.8,
307.9,  308,   -1,
308.1,308.2,308.3,
308.4,308.5,308.6,
308.7,308.8,308.9,
  309,309.1,309.2,
309.3,309.4,309.5,
309.6,309.7,309.8,
309.9,  310,310.1,
310.2,310.3,310.4,
310.5,310.6,310.7,
310.8,310.9,  311,
311.1,311.2,311.3,
    1,311.4,311.5,
311.6,311.7,311.8,
311.9,  312,312.1,
312.2,312.3,312.4,
312.5,312.6,312.7,
312.8,312.9,  313,
313.1,313.2,313.3,
313.4,313.5,313.6,
   -1,313.7,313.8,
313.9,  314,314.1,
314.2,314.3,314.4,
314.5,314.6,314.7,
314.8,314.9,  315,
315.1,315.2,315.3,
315.4,315.5,315.6,
315.7,315.8,315.9,
  316,316.1,316.2,
316.3,316.4,316.5,
316.6,316.7,316.8,
316.9,  317,317.1,
317.2,317.3,317.4,
317.5,    1,317.6,
317.7,317.8,317.9,
  318,318.1,318.2,
318.3,318.4,318.5,
318.6,318.7,318.8,
318.9,  319,319.1,
319.2,319.3,319.4,
319.5,319.6,319.7,
319.8,   -1,319.9,
  320,320.1,320.2,
320.3,320.4,320.5,
320.6,320.7,320.8,
320.9,  321,321.1,
321.2,321.3,321.4,
321.5,321.6,321.7,
321.8,321.9,  322,
322.1,322.2,322.3,
322.4,322.5,322.6,
322.7,322.8,322.9,
  323,323.1,323.2,
323.3,323.4,    1,
323.5,323.6,323.7,
323.8,323.9,  324,
324.1,324.2,324.3,
324.4,324.5,324.6,
324.7,324.8,324.9,
  325,325.1,325.2,
325.3,325.4,325.5,
325.6,325.7,   -1,
325.8,325.9,  326,
326.1,326.2,326.3,
326.4,326.5,326.6,
326.7,326.8,326.9,
  327,327.1,327.2,
327.3,327.4,327.5,
327.6,327.7,327.8,
327.9,  328,328.1,
328.2,328.3,328.4,
328.5,328.6,328.7,
328.8,328.9,  329,
    1,329.1,329.2,
329.3,329.4,329.5,
329.6,329.7,329.8,
329.9,  330,330.1,
330.2,330.3,330.4,
330.5,330.6,330.7,
330.8,330.9,  331,
331.1,331.2,331.3,
   -1,331.4,331.5,
331.6,331.7,331.8,
331.9,  332,332.1,
332.2,332.3,332.4,
332.5,332.6,332.7,
332.8,332.9,  333,
333.1,333.2,333.3,
333.4,333.5,333.6,
333.7,333.8,333.9,
  334,334.1,334.2,
334.3,334.4,334.5,
334.6,334.7,334.8,
334.9,  335,335.1,
335.2,    1,335.3,
335.4,335.5,335.6,
335.7,335.8,335.9,
  336,336.1,336.2,
336.3,336.4,336.5,
336.6,336.7,336.8,
336.9,  337,337.1,
337.2,337.3,337.4,
337.5,   -1,337.6,
337.7,337.8,337.9,
  338,338.1,338.2,
338.3,338.4,338.5,
338.6,338.7,338.8,
338.9,  339,339.1,
339.2,339.3,339.4,
339.5,339.6,339.7,
339.8,339.9,  340,
340.1,340.2,340.3,
340.4,340.5,340.6,
340.7,340.8,340.9,
  341,341.1,    1,
341.2,341.3,341.4,
341.5,341.6,341.7,
341.8,341.9,  342,
342.1,342.2,342.3,
342.4,342.5,342.6,
342.7,342.8,342.9,
  343,343.1,343.2,
343.3,343.4,   -1,
343.5,343.6,343.7,
343.8,343.9,  344,
344.1,344.2,344.3,
344.4,344.5,344.6,
344.7,344.8,344.9,
  345,345.1,345.2,
345.3,345.4,345.5,
345.6,345.7,345.8,
345.9,  346,346.1,
346.2,346.3,346.4,
346.5,346.6,346.7,
    1,346.8,346.9,
  347,347.1,347.2,
347.3,347.4,347.5,
347.6,347.7,347.8,
347.9,  348,348.1,
348.2,348.3,348.4,
348.5,348.6,348.7,
348.8,348.9,  349,
   -1,349.1,349.2,
349.3,349.4,349.5,
349.6,349.7,349.8,
349.9,  350,350.1,
350.2,350.3,350.4,
};

// ----------------------------------------------------------------------
// Computed values
// ----------------------------------------------------------------------

const double pylith::faults::CohesiveDynDataHex8::_orientation[] = {
  0.0, -1.0, 0.0,    0.0, 0.0, -1.0,    -1.0, 0.0, 0.0,
  0.0, -1.0, 0.0,    0.0, 0.0, -1.0,    -1.0, 0.0, 0.0,
  0.0, -1.0, 0.0,    0.0, 0.0, -1.0,    -1.0, 0.0, 0.0,
  0.0, -1.0, 0.0,    0.0, 0.0, -1.0,    -1.0, 0.0, 0.0,
};

const double pylith::faults::CohesiveDynDataHex8::_area[] = {
  1.0, 1.0, 1.0, 1.0
};

const double pylith::faults::CohesiveDynDataHex8::_forcesInitial[] = {
  3.063397471, -1.063397471, -2.063397471, 
  3.121132498, -1.121132498, -2.121132498, 
  3.178867525, -1.178867525, -2.178867525,
  3.236602552, -1.236602552, -2.236602552,
};


const int pylith::faults::CohesiveDynDataHex8::_numConstraintVert = 4;
const int pylith::faults::CohesiveDynDataHex8::_constraintVertices[] = {
  18, 19, 20, 21
};
// ----------------------------------------------------------------------
// Stick case
// ----------------------------------------------------------------------
// Input
const double pylith::faults::CohesiveDynDataHex8::_fieldIncrStick[] = {
  0.1, 2.1, 28.1,
  0.2, 2.2, 28.2,
  0.3, 2.3, 28.3,
  0.4, 2.4, 28.4,
  0.5, 2.5, 28.5, // 6
  0.6, 2.6, 28.6, // 7
  0.7, 2.7, 28.7, // 8
  0.8, 2.8, 28.8, // 9
  0.9, 2.9, 28.9,
  0.0, 2.0, 28.0,
  1.1, 3.1, 29.1,
  1.2, 3.2, 29.2,
  1.3, 3.3, 29.3, // 14
  1.5, 3.5, 29.5, // 15
  1.7, 3.7, 29.7, // 16
  1.9, 3.9, 29.9, // 17
  1.4, 3.4, -89.4, // 18
  1.6, 3.6, -89.6, // 19
  1.8, 3.8, -89.8, // 20
  1.0, 3.0, -89.0, // 21
};

// No change in fieldIncr
// Zero slip

// ----------------------------------------------------------------------
// Slip case
// ----------------------------------------------------------------------
// Input
const double pylith::faults::CohesiveDynDataHex8::_fieldIncrSlip[] = {
  5.1, 7.1, 9.1,
  5.2, 7.2, 9.2,
  5.3, 7.3, 9.3,
  5.4, 7.4, 9.4,
  5.5, 7.5, 9.5, // 6
  5.6, 7.6, 9.6, // 7
  5.7, 7.7, 9.7, // 8
  5.8, 7.8, 9.8, // 9
  5.9, 7.9, 9.9,
  5.0, 7.0, 9.0,
  6.1, 8.1, 10.1,
  6.2, 8.2, 10.2,
  6.3, 8.3, 10.3, // 14
  6.5, 8.5, 10.5, // 15
  6.7, 8.7, 10.7, // 16
  6.9, 8.9, 10.9, // 17
  -6.4, -8.4, -10.4, // 18
  -6.6, -8.6, -10.6, // 19
  -6.8, -8.8, -10.8, // 20
  -6.0, -8.0, -10.0, // 21
};

// Output
const double pylith::faults::CohesiveDynDataHex8::_fieldIncrSlipE[] = {
  5.100000000000000,   7.100000000000000,   9.100000000000000,
  5.200000000000000,   7.200000000000000,  9.199999999999999,
  5.300000000000000,   7.300000000000000,   9.300000000000001,
  5.400000000000000,   7.400000000000000,   9.400000000000000,
  5.497852353458395,   7.498145012421456,   9.500000000000000,
  5.598633150735187,   7.598835934591629,   9.600000000000000,
  5.699186008483079,   7.699335886446804,   9.699999999999999,
  5.799600882460798,   7.799716783656151,   9.800000000000001,
  5.900000000000000,   7.900000000000000,   9.900000000000000,
  5.000000000000000,   7.000000000000000,   9.000000000000000,
  6.100000000000000,   8.100000000000000,  10.100000000000000,
  6.200000000000000,   8.199999999999999,  10.199999999999999,
  6.302147646541605,   8.301854987578544,  10.300000000000001,
  6.501366849264812,   8.501164065408370,  10.500000000000000,
  6.700813991516921,   8.700664113553195,  10.699999999999999,
  6.900399117539203,   8.900283216343849,  10.900000000000000,
  -5.824264070000000,  -7.824264070000000, -10.400000000000000,
  -6.024264069999999, -8.024264069999999, -10.600000000000000,
  -6.224264070000000,  -8.224264070000000, -10.800000000000001,
  -5.424264070000000,  -7.424264070000000, -10.000000000000000,
};

const double pylith::faults::CohesiveDynDataHex8::_slipSlipE[] = {
  0.004295383083209,   0.003709975157088,                   0.0,
  0.002733698529624,   0.002328130816740,                   0.0,
  0.001627983033842,   0.001328227106392,                   0.0,
  0.000798235078404,   0.000566432687698,                   0.0,
};

// ----------------------------------------------------------------------
// Open case
// ----------------------------------------------------------------------
// Input
const double pylith::faults::CohesiveDynDataHex8::_fieldIncrOpen[] = {
  5.1, 7.1, 9.1,
  5.2, 7.2, 9.2,
  5.3, 7.3, 9.3,
  5.4, 7.4, 9.4,
  5.5, 7.5, 9.5, // 6
  5.6, 7.6, 9.6, // 7
  5.7, 7.7, 9.7, // 8
  5.8, 7.8, 9.8, // 9
  5.9, 7.9, 9.9,
  5.0, 7.0, 9.0,
  6.1, 8.1, 10.1,
  6.2, 8.2, 10.2,
  6.3, 8.3, 10.3, // 14
  6.5, 8.5, 10.5, // 15
  6.7, 8.7, 10.7, // 16
  6.9, 8.9, 10.9, // 17
  6.4, 8.4, 10.4, // 18
  6.6, 8.6, 10.6, // 19
  6.8, 8.8, 10.8, // 20
  6.0, 8.0, 10.0, // 21
};

// Output
const double pylith::faults::CohesiveDynDataHex8::_fieldIncrOpenE[] = {
  5.100000000000000,   7.100000000000000,   9.100000000000000,
  5.200000000000000,   7.200000000000000,   9.199999999999999,
  5.300000000000000,   7.300000000000000,   9.300000000000001,
  5.400000000000000,   7.400000000000000,   9.400000000000000,
  5.479235316138440,   7.506377670059572,   9.560964099926785,
  5.566675616466609,   7.590571311651349,   9.633724303282460,
  5.657842717320326,   7.679191979989885,   9.715001379770163,
  5.739890640676003,   7.759602313469650,   9.789443715897779,
  5.900000000000000,   7.900000000000000,   9.900000000000000,
  5.000000000000000,   7.000000000000000,   9.000000000000000,
  6.100000000000000,   8.100000000000000,  10.100000000000000,
  6.200000000000000,   8.199999999999999,  10.199999999999999,
  6.320764683861560,   8.293622329940428,  10.239035900073215,
  6.533324383533390,   8.509428688348651,  10.466275696717540,
  6.742157282679674,   8.720808020010114,  10.684998620229836,
  6.960109359323997,   8.940397686530350,  10.910556284102222,
  -5.400000000000000,  -7.400000000000000,  -9.400000000000000,
  -5.600000000000000, -7.600000000000000,  -9.600000000000000,
  -5.800000000000000,  -7.800000000000001,  -9.800000000000001,
  -5.000000000000000,  -7.000000000000000,  -9.000000000000000,

};

const double pylith::faults::CohesiveDynDataHex8::_slipOpenE[] = {
  0.041529467723119,  -0.012755340119145,  -0.121928199853569,
  0.066648767066781,   0.018857376697302,  -0.067448606564919,
  0.084314565359348,   0.041616040020231,  -0.030002759540328,
  0.120218718647994,   0.080795373060699,   0.021112568204443,
};

// ----------------------------------------------------------------------
pylith::faults::CohesiveDynDataHex8::CohesiveDynDataHex8(void)
{ // constructor
  meshFilename = const_cast<char*>(_meshFilename);
  spaceDim = _spaceDim;
  cellDim = _cellDim;
  numBasis = _numBasis;
  numQuadPts = _numQuadPts;
  quadPts = const_cast<double*>(_quadPts);
  quadWts = const_cast<double*>(_quadWts);
  basis = const_cast<double*>(_basis);
  basisDeriv = const_cast<double*>(_basisDeriv);
  verticesRef = const_cast<double*>(_verticesRef);
  id = _id;
  label = const_cast<char*>(_label);
  initialTractFilename = const_cast<char*>(_initialTractFilename);

  fieldT = const_cast<double*>(_fieldT);
  jacobian = const_cast<double*>(_jacobian);
  orientation = const_cast<double*>(_orientation);
  area = const_cast<double*>(_area);
  forcesInitial = const_cast<double*>(_forcesInitial);

  constraintVertices = const_cast<int*>(_constraintVertices);
  numConstraintVert = _numConstraintVert;  

  // Stick
  fieldIncrStick = const_cast<double*>(_fieldIncrStick);

  // Slip
  fieldIncrSlip = const_cast<double*>(_fieldIncrSlip);
  fieldIncrSlipE = const_cast<double*>(_fieldIncrSlipE);
  slipSlipE = const_cast<double*>(_slipSlipE);

  // Open
  fieldIncrOpen = const_cast<double*>(_fieldIncrOpen);
  fieldIncrOpenE = const_cast<double*>(_fieldIncrOpenE);
  slipOpenE = const_cast<double*>(_slipOpenE);
} // constructor

pylith::faults::CohesiveDynDataHex8::~CohesiveDynDataHex8(void)
{}


// End of file
