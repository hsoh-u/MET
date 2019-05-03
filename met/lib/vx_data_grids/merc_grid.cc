// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2007
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*


////////////////////////////////////////////////////////////////////////


using namespace std;


#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cmath>

#include "vx_math/vx_math.h"
#include <vx_data_grids/merc_grid.h>


////////////////////////////////////////////////////////////////////////


static double     merc_func(double lat_rad);
static double merc_der_func(double lat_rad);

static double merc_inv_func(double r);

static double merc_lon_to_u(double lon_rad);
static double merc_u_to_lon(double u);

static double mercator_segment_area(double u0, double v0, double u1, double v1);


////////////////////////////////////////////////////////////////////////


   //
   //  Code for class MercatorGrid
   //


////////////////////////////////////////////////////////////////////////


MercatorGrid::MercatorGrid()

{

Name = (char *) 0;

clear();

}


////////////////////////////////////////////////////////////////////////


MercatorGrid::~MercatorGrid()

{

clear();

}


////////////////////////////////////////////////////////////////////////


MercatorGrid::MercatorGrid(const MercatorData & data)

{

Name = (char *) 0;

double lll;   //  lower-left longitude
double lur;   //  upper-right longitude

clear();

mc_data = data;

lll = data.lon_ll_deg;
lur = data.lon_ur_deg;

   //
   //  reduce lll and lur to the range -180 ... 180
   //

lll -= 360.0*floor((lll + 180.0)/360.0);
lur -= 360.0*floor((lur + 180.0)/360.0);

   //
   //  make sure that lur >= lll
   //

lur += 360.0*floor((lll - lur)/360.0);

   //
   //  test for full-world grid
   //

if ( fabs(lur - lll) < 1.0e-2 )  lur = lll + 360.0;

   //
   //  calculate stuff
   //

Lat_LL_radians = (data.lat_ll_deg)/deg_per_rad;
Lon_LL_radians = lll/deg_per_rad;

Lat_UR_radians = (data.lat_ur_deg)/deg_per_rad;
Lon_UR_radians = lur/deg_per_rad;

Nx = data.nx;
Ny = data.ny;

Name = new char [1 + strlen(data.name)];

strcpy(Name, data.name);

   //
   //  calculate mx, bx, my, by
   //

double u_first, u_last;
double v_first, v_last;

u_first = merc_lon_to_u(Lon_LL_radians);
u_last  = merc_lon_to_u(Lon_UR_radians);

Mx = (Nx - 1.0)/(u_last - u_first);

Bx = -Mx*u_first;

v_first = merc_func(Lat_LL_radians);
v_last  = merc_func(Lat_UR_radians);

My = (Ny - 1.0)/(v_last - v_first);

By = -My*v_first;

   //
   //  Done
   //

}


////////////////////////////////////////////////////////////////////////


void MercatorGrid::clear()

{

mc_data. name      = (char *) 0;
mc_data.lat_ll_deg = 0.0;
mc_data.lon_ll_deg = 0.0;
mc_data.lat_ur_deg = 0.0;
mc_data.lon_ur_deg = 0.0;
mc_data.nx         = 0;
mc_data.ny         = 0;

Lat_LL_radians = 0.0;
Lon_LL_radians = 0.0;

Lat_UR_radians = 0.0;
Lon_UR_radians = 0.0;

Mx = My = 0.0;

Bx = By = 0.0;

Nx = 0;
Ny = 0;

if ( Name )  { delete [] Name;  Name = (char *) 0; }

return;

}


////////////////////////////////////////////////////////////////////////


double MercatorGrid::f(double lat_deg) const

{

return ( merc_func(lat_deg/deg_per_rad) );

}


////////////////////////////////////////////////////////////////////////


double MercatorGrid::df(double lat_deg) const

{

return ( merc_der_func(lat_deg/deg_per_rad) );

}


////////////////////////////////////////////////////////////////////////


void MercatorGrid::latlon_to_xy(double lat_deg, double lon_deg, double & x, double & y) const

{

double lat_rad, lon_rad;
double u, v;

lat_rad = lat_deg/deg_per_rad;
lon_rad = lon_deg/deg_per_rad;

lon_rad += twopi*floor((Lon_LL_radians - lon_rad)/twopi);

u = merc_lon_to_u(lon_rad);

v = merc_func(lat_rad);

uv_to_xy(u, v, x, y);

return;

}


////////////////////////////////////////////////////////////////////////


void MercatorGrid::xy_to_latlon(double x, double y, double & lat_deg, double & lon_deg) const

{

double lat_rad, lon_rad;
double u, v;


xy_to_uv(x, y, u, v);

lon_rad = merc_u_to_lon(u);

lat_rad = merc_inv_func(v);

lat_deg = deg_per_rad*lat_rad;

lon_deg = deg_per_rad*lon_rad;

lon_deg += 360.0*floor((180.0 - lon_deg)/360.0);   //  reduce lon to range (-180, 180]

return;

}


////////////////////////////////////////////////////////////////////////


double MercatorGrid::calc_area(int x, int y) const

{

double u[4], v[4];
double sum;


xy_to_uv(x - 0.5, y - 0.5, u[0], v[0]);  //  lower left
xy_to_uv(x + 0.5, y - 0.5, u[1], v[1]);  //  lower right
xy_to_uv(x + 0.5, y + 0.5, u[2], v[2]);  //  upper right
xy_to_uv(x - 0.5, y + 0.5, u[3], v[3]);  //  upper left


sum = uv_closedpolyline_area(u, v, 4);

sum *= earth_radius_km*earth_radius_km;

return ( sum );

}


////////////////////////////////////////////////////////////////////////


double MercatorGrid::calc_area_ll(int x, int y) const

{

cerr << "\n\n  MercatorGrid::calc_area_ll() -> not yet implemented\n\n";

exit ( 1 );

return ( 0.0 );

}


////////////////////////////////////////////////////////////////////////



int MercatorGrid::nx() const

{

return ( Nx );

}


////////////////////////////////////////////////////////////////////////


int MercatorGrid::ny() const

{

return ( Ny );

}


////////////////////////////////////////////////////////////////////////


double MercatorGrid::EarthRadiusKM() const

{

return ( earth_radius_km );

}


////////////////////////////////////////////////////////////////////////


const char * MercatorGrid::name() const

{

return ( Name );

}


////////////////////////////////////////////////////////////////////////


ProjType MercatorGrid::proj_type() const

{

return ( MercatorProj );

}


////////////////////////////////////////////////////////////////////////


double MercatorGrid::rot_grid_to_earth(int x, int y) const

{

//
// The rotation angle from grid relative to earth relative is zero
// for the Mercator projection
//

return ( 0.0 );

}


////////////////////////////////////////////////////////////////////////


double MercatorGrid::uv_closedpolyline_area(const double *u, const double *v, int n) const

{

int j, k;
double sum;


sum = 0.0;

for (j=0; j<n; ++j)  {

   k = (j + 1)%n;

   sum += mercator_segment_area(u[j], v[j], u[k], v[k]);

}   //  for j

sum = fabs(sum);

return ( sum );

}


////////////////////////////////////////////////////////////////////////


double MercatorGrid::xy_closedpolyline_area(const double *x, const double *y, int n) const

{

int j;
double sum;
double *u = (double *) 0;
double *v = (double *) 0;

u = new double [n];
v = new double [n];

if ( !u || !v )  {

   cerr << "\n\n  MercatorGrid::xy_closedpolyline_area() -> memory allocation error\n\n";

   exit ( 1 );

}

for (j=0; j<n; ++j)  {

   xy_to_uv(x[j], y[j], u[j], v[j]);

}

sum = uv_closedpolyline_area(u, v, n);

sum *= earth_radius_km*earth_radius_km;

delete [] u;  u = (double *) 0;
delete [] v;  v = (double *) 0;

return ( sum );

}


////////////////////////////////////////////////////////////////////////


void MercatorGrid::uv_to_xy(double u, double v, double &x, double &y) const

{

x = Mx*u + Bx;

y = My*v + By;

return;

}


////////////////////////////////////////////////////////////////////////


void MercatorGrid::xy_to_uv(double x, double y, double &u, double &v) const

{

u = (x - Bx)/Mx;

v = (y - By)/My;

return;

}


////////////////////////////////////////////////////////////////////////


void MercatorGrid::grid_data(GridData &gdata) const

{

gdata.mc_data = mc_data;

return;

}


////////////////////////////////////////////////////////////////////////


   //
   //  Code for misc functions
   //


////////////////////////////////////////////////////////////////////////


double merc_func(double lat_rad)

{

double v;

v = log(tan(piover4 + 0.5*lat_rad));

return ( v );

}


////////////////////////////////////////////////////////////////////////


double merc_inv_func(double v)

{

double lat_rad;

lat_rad = 2.0*atan(exp(v)) - piover2;

return ( lat_rad );

}


////////////////////////////////////////////////////////////////////////


double merc_der_func(double lat_rad)

{

double a;

a = 1.0/(cos(lat_rad));

return ( a );

}


////////////////////////////////////////////////////////////////////////


double mercator_segment_area(double u0, double v0, double u1, double v1)

{

double delta_u, delta_v;
double b0, b1;
double answer;

delta_u = u1 - u0;
delta_v = v1 - v0;

if ( fabs(delta_v) < 1.0e-4 )  {

   answer = 0.5*delta_u*(1.0 - tanh(v0));

   return ( answer );

}

   //
   //  ok, so now we know that delta_v is nonzero
   //

b0 = v0 - log(cosh(v0));
b1 = v1 - log(cosh(v1));

answer = (delta_u/delta_v)*(b1 - b0);

   //
   //  done
   //

return ( answer );

}

////////////////////////////////////////////////////////////////////////


double merc_lon_to_u(double lon_rad)

{

double u;

u = -lon_rad;

return ( u );

}


////////////////////////////////////////////////////////////////////////


double merc_u_to_lon(double u)

{

double lon_rad;

lon_rad = -u;

return ( lon_rad );

}


////////////////////////////////////////////////////////////////////////


Grid::Grid(const MercatorData & data)

{

rep = (GridRep *) 0;

rep = new MercatorGrid (data);

if ( !rep )  {

   cerr << "\n\n  Grid::Grid(const MercatorData &) -> memory allocation error\n\n";

   exit ( 1 );

}

rep->refCount = 1;

}


////////////////////////////////////////////////////////////////////////

