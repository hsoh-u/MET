// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2018
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

////////////////////////////////////////////////////////////////////////


#ifndef  __MET_DATAPLANE_FROM_XARRAY_H__
#define  __MET_DATAPLANE_FROM_XARRAY_H__


////////////////////////////////////////////////////////////////////////


#include "data_plane.h"
#include "vx_grid.h"


extern "C" {

#include "Python.h"

}


////////////////////////////////////////////////////////////////////////


extern void dataplane_from_xarray(PyObject * data_array, DataPlane & dp_out, Grid & grid_out);


////////////////////////////////////////////////////////////////////////


#endif   /*  __MET_DATAPLANE_FROM_XARRAY_H__  */


////////////////////////////////////////////////////////////////////////


