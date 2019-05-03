// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2007
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*



////////////////////////////////////////////////////////////////////////


#ifndef  __64_BIT_CALENDAR_ROUTINES_H__
#define  __64_BIT_CALENDAR_ROUTINES_H__


////////////////////////////////////////////////////////////////////////


   //
   //  These routines use the Gregorian Calendar.
   //
   //     The algorithms used are not valid before
   //
   //     about 4700 BC.
   //


////////////////////////////////////////////////////////////////////////


typedef long long unixtime;


////////////////////////////////////////////////////////////////////////


static const int mjd_ut0 = 40587;   //  mjd of Jan 1, 1970


////////////////////////////////////////////////////////////////////////


extern  unixtime  doyhms_to_unix  (int doy, int year, int hour, int minute, int second);

extern  unixtime  mdyhms_to_unix  (int month, int day, int year, int hour, int minute, int second);

extern  void      unix_to_mdyhms  (unixtime u, int & month, int & day, int & year, int & hour, int & minute, int & second);


extern  int       date_to_mjd     (int m, int d, int y);

extern  void      mjd_to_date     (int mjd, int & month, int & day, int & year);


extern  int       day_dif         (int m1, int d1, int y1, int m2, int d2, int y2);

extern  int       day_of_week     (int month, int day, int year);

extern  int       is_leap_year    (int year);

extern  void      easter          (int & month, int & day, int year);


extern  int       is_dst          (int month, int day, int year, int hour, int minute, int second);

extern  int       is_dst          (unixtime);

extern  void      sec_to_hms      (int in_sec, int & hour, int & min, int & sec);

extern  int       hms_to_sec      (int hour, int min, int sec);

// Parse time strings

extern  void     substring_vx_cal       (const char * text, char * out, int first, int last);

extern  void     sec_to_hhmmss          (int, char *);

extern  int      hhmmss_to_sec          (const char *);

extern  void     unix_to_yyyymmdd_hhmmss(unixtime, char *);

extern  unixtime yyyymmdd_hhmmss_to_unix(const char *);

extern  unixtime yyyymmdd_hh_to_unix(const char *);

extern  unixtime yyyymmdd_to_unix(const char *);

extern  void     unix_to_yyyymmddhh(unixtime, char *);

extern  unixtime yyyymmddhh_to_unix(const char *);

extern  void     make_timestring(unixtime, char *);

extern  unixtime timestring_to_unix(const char *);

extern  int      timestring_to_sec(const char *);


////////////////////////////////////////////////////////////////////////


extern const char * day_name[];

extern const char * short_day_name[];

extern const char * month_name[];

extern const char * short_month_name[];


////////////////////////////////////////////////////////////////////////


static const int Monday    = 1;
static const int Tuesday   = 2;
static const int Wednesday = 3;
static const int Thursday  = 4;
static const int Friday    = 5;
static const int Saturday  = 6;
static const int Sunday    = 7;


////////////////////////////////////////////////////////////////////////


#include "vx_cal/holiday.h"


////////////////////////////////////////////////////////////////////////


#endif   //  __64_BIT_CALENDAR_ROUTINES_H__


////////////////////////////////////////////////////////////////////////


