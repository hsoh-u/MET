// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2007
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

////////////////////////////////////////////////////////////////////////
//
//   Filename:   stat_analysis.h
//
//   Description:
//
//   Mod#   Date      Name            Description
//   ----   ----      ----            -----------
//   000    11/11/08  Halley Gotway   New
//
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

#ifndef  __STAT_ANALYSIS_H__
#define  __STAT_ANALYSIS_H__

////////////////////////////////////////////////////////////////////////

using namespace std;

#include <cmath>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "stat_analysis_Conf.h"

#include "vx_analysis_util/vx_analysis_util.h"
#include "vx_util/vx_util.h"
#include "vx_met_util/vx_met_util.h"
#include "vx_contable/vx_contable.h"

////////////////////////////////////////////////////////////////////////
//
// Constants
//
////////////////////////////////////////////////////////////////////////

static const char * program_name = "stat_analysis";

////////////////////////////////////////////////////////////////////////
//
// Variables for Command Line Arguments
//
////////////////////////////////////////////////////////////////////////

// Search directories and the corresponding files set with -lookin
static StringArray search_dirs;
static StringArray files;

// Output file set with -out
static const char *out_file = (const char *) 0;
static ofstream   *sa_out   = (ofstream *) 0;

// Config file set with -config
static const char *config_file = (const char *) 0;

// Logging verbosity set with -v
static int verbosity = 1;

// Job command which may be set on the command line
static char command_line_job[PATH_MAX] = "";

////////////////////////////////////////////////////////////////////////
//
// Variables for Output Files
//
////////////////////////////////////////////////////////////////////////

// Default temporary path and file
static ConcatString tmp_dir;
static ConcatString tmp_file;
static ConcatString tmp_path;

// Output file stream for the temporary file
static ofstream tmp_out;

// STAT-Analysis configuration object
static stat_analysis_Conf conf;

// STAT-Analysis default job
static STATAnalysisJob    default_job;

////////////////////////////////////////////////////////////////////////

#endif   //  __STAT_ANALYSIS_H__

////////////////////////////////////////////////////////////////////////