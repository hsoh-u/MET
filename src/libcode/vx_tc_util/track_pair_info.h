// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2012
// ** University Corporation for Atmospheric Research (UCAR)
// ** National Center for Atmospheric Research (NCAR)
// ** Research Applications Lab (RAL)
// ** P.O.Box 3000, Boulder, Colorado, 80307-3000, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

////////////////////////////////////////////////////////////////////////

#ifndef  __VX_TRACK_PAIR_INFO_H__
#define  __VX_TRACK_PAIR_INFO_H__

////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "track_point.h"
#include "track_info.h"
#include "tc_stat_line.h"

#include "vx_cal.h"
#include "vx_math.h"
#include "vx_util.h"

////////////////////////////////////////////////////////////////////////

static const int TrackPairInfoAllocInc = 10;

////////////////////////////////////////////////////////////////////////
//
// TrackPairInfo class stores a matched ADECK and BDECK track and their
// corresponding errors.
//
////////////////////////////////////////////////////////////////////////

class TrackPairInfo {

   private:

      void init_from_scratch();
      void assign(const TrackPairInfo &);
      void extend(int);

      // Number of track points
      int          NPoints;
      
      // ADECK and BDECK tracks
      TrackInfo    ADeck;
      TrackInfo    BDeck;

      // Distances to land
      NumArray     ADeckDLand;
      NumArray     BDeckDLand;
      
      // Along-Track, Cross-Track, and Track Errors
      NumArray     TrackErr;
      NumArray     AlongTrackErr;
      NumArray     CrossTrackErr;

   public:

      TrackPairInfo();
     ~TrackPairInfo();
      TrackPairInfo(const TrackPairInfo &);
      TrackPairInfo & operator=(const TrackPairInfo &);

      void clear();

      void         dump(ostream &, int = 0)  const;
      ConcatString serialize()               const;
      ConcatString serialize_r(int, int = 0) const;

         //
         //  set stuff
         //

      void initialize(const TrackInfo &, const TrackInfo &);
      void initialize(const TCStatLine &);
      
         //
         //  get stuff
         //

      int               n_points()           const;
      const TrackInfo & adeck()              const;
      const TrackInfo & bdeck()              const;
      double            adeck_dland(int)     const;
      double            bdeck_dland(int)     const;
      double            track_err(int)       const;
      double            along_track_err(int) const;
      double            cross_track_err(int) const;

         //
         //  do stuff
         //

      void add(const TrackPoint &, const TrackPoint &,
               double, double, double, double, double);
      void add(const TCStatLine&);

};

////////////////////////////////////////////////////////////////////////

inline int               TrackPairInfo::n_points()             const { return(NPoints);          }
inline const TrackInfo & TrackPairInfo::adeck()                const { return(ADeck);            }
inline const TrackInfo & TrackPairInfo::bdeck()                const { return(BDeck);            }
inline double            TrackPairInfo::adeck_dland(int i)     const { return(ADeckDLand[i]);    }
inline double            TrackPairInfo::bdeck_dland(int i)     const { return(BDeckDLand[i]);    }
inline double            TrackPairInfo::track_err(int i)       const { return(TrackErr[i]);      }
inline double            TrackPairInfo::along_track_err(int i) const { return(AlongTrackErr[i]); }
inline double            TrackPairInfo::cross_track_err(int i) const { return(CrossTrackErr[i]); }

////////////////////////////////////////////////////////////////////////
//
// TrackPairInfoArray class stores an array of TrackPairInfo objects.
//
////////////////////////////////////////////////////////////////////////

class TrackPairInfoArray {

   private:

      void init_from_scratch();
      void assign(const TrackPairInfoArray &);
      void extend(int);

      TrackPairInfo *Pair;
      int            NPairs;
      int            NAlloc;

   public:

      TrackPairInfoArray();
     ~TrackPairInfoArray();
      TrackPairInfoArray(const TrackPairInfoArray &);
      TrackPairInfoArray & operator=(const TrackPairInfoArray &);

      void clear();

      void         dump(ostream &, int = 0) const;
      ConcatString serialize()              const;
      ConcatString serialize_r(int = 0)     const;

         //
         //  set stuff
         //

         //
         //  get stuff
         //

      const TrackPairInfo & operator[](int) const;
      int n_pairs()  const;
      int n_points() const;

         //
         //  do stuff
         //

      void add(const TrackPairInfo &);

};

////////////////////////////////////////////////////////////////////////

inline int TrackPairInfoArray::n_pairs() const { return(NPairs); }

////////////////////////////////////////////////////////////////////////

#endif   /*  __VX_TRACK_PAIR_INFO_H__  */

////////////////////////////////////////////////////////////////////////
