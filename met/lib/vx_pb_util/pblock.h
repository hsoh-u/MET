

////////////////////////////////////////////////////////////////////////


#ifndef  __PBLOCK_H__
#define  __PBLOCK_H__


////////////////////////////////////////////////////////////////////////


enum PadSize {

   padsize_4,
   padsize_8,

   no_padsize

};


////////////////////////////////////////////////////////////////////////


enum Action {

   block,
   unblock,

   no_action

};

////////////////////////////////////////////////////////////////////////

extern void pblock(const char *, const char *, Action);

////////////////////////////////////////////////////////////////////////


#endif   /*  __PBLOCK_H__  */


////////////////////////////////////////////////////////////////////////

