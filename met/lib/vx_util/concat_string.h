// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2007
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*


////////////////////////////////////////////////////////////////////////


#ifndef  __CONCAT_STRING_H__
#define  __CONCAT_STRING_H__


////////////////////////////////////////////////////////////////////////


#include <iostream>

#include "vx_util/string_array.h"


////////////////////////////////////////////////////////////////////////


   //
   //  minimum and default allocation increment for the ConcatString class
   //


static const int min_cs_alloc_inc     =  32;

static const int default_cs_alloc_inc = 128;


////////////////////////////////////////////////////////////////////////


static const int concat_string_default_precision =  2;

static const int concat_string_max_precision     = 12;


////////////////////////////////////////////////////////////////////////


class ConcatString {

   private:

      void init_from_scratch();

      void assign(const ConcatString &);

      void extend(int);


      int AllocInc;

      int Length;

      int Nalloc;

      int Precision;

      char FloatFormat[16];

      char * s;

   public:

      ConcatString();
      ConcatString(int _alloc_inc);
     ~ConcatString();
      ConcatString(const ConcatString &);
      ConcatString(const char *);
      ConcatString & operator=(const ConcatString &);
      ConcatString & operator=(const char *);

      void clear();

      void set_alloc_inc(int);

      void add(const char *);

      void add(const ConcatString &);

      void chomp();   //  removes possible trailing newline

      void chomp(const char);   //  removes trailing char, if possible

      void chomp(const char *);   //  removes trailing suffix, if possible

      const char * text() const;

      int length() const;   //  not including trailing nul

      int precision() const;

      void set_precision(int);

      const char * float_format() const;

      operator const char * () const;

      bool startswith(const char *) const;
      bool   endswith(const char *) const;

      void ws_strip();   //  strip leading and trailing whitespace, if any

      void strip_cpp_comment();   //  strip trailing C++ comment, if any

      StringArray split(const char * delim) const;



      int alloc_inc() const;

      void erase();

      void set_repeat(char, int count);

};


////////////////////////////////////////////////////////////////////////


inline const char * ConcatString::text()         const { return ( s ); }

inline int          ConcatString::length()       const { return ( Length ); }

inline int          ConcatString::precision()    const { return ( Precision ); }

inline const char * ConcatString::float_format() const { return ( FloatFormat ); }

inline int          ConcatString::alloc_inc()    const { return ( AllocInc ); }


inline              ConcatString::operator const char * () const { return ( s ); }


////////////////////////////////////////////////////////////////////////


   //
   //  writing a ConcatString to an ostream
   //


extern ostream & operator<<(ostream &, const ConcatString &);


////////////////////////////////////////////////////////////////////////


   //
   //  writing things into a ConcatString
   //


extern ConcatString & operator << (ConcatString &, const char);
extern ConcatString & operator << (ConcatString &, const char *);
extern ConcatString & operator << (ConcatString &, const ConcatString &);
extern ConcatString & operator << (ConcatString &, int);
extern ConcatString & operator << (ConcatString &, long long);
extern ConcatString & operator << (ConcatString &, double);


   //
   //  string comparison functions
   //


extern int operator == (const ConcatString &, const ConcatString &);
extern int operator == (const ConcatString &, const char *);
extern int operator == (const char *, const ConcatString &);

extern int operator != (const ConcatString &, const ConcatString &);
extern int operator != (const ConcatString &, const char *);
extern int operator != (const char *, const ConcatString &);

extern int operator <= (const ConcatString &, const ConcatString &);
extern int operator <= (const ConcatString &, const char *);
extern int operator <= (const char *, const ConcatString &);

extern int operator >= (const ConcatString &, const ConcatString &);
extern int operator >= (const ConcatString &, const char *);
extern int operator >= (const char *, const ConcatString &);

extern int operator <  (const ConcatString &, const ConcatString &);
extern int operator <  (const ConcatString &, const char *);
extern int operator <  (const char *, const ConcatString &);

extern int operator >  (const ConcatString &, const ConcatString &);
extern int operator >  (const ConcatString &, const char *);
extern int operator >  (const char *, const ConcatString &);


////////////////////////////////////////////////////////////////////////


#endif   //  __CONCAT_STRING_H__


////////////////////////////////////////////////////////////////////////

