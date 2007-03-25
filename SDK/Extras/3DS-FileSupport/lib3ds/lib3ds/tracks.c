/*
 * The 3D Studio File Format Library
 * Copyright (C) 1996-2001 by J.E. Hoffmann <je-h@gmx.net>
 * All rights reserved.
 *
 * This program is  free  software;  you can redistribute it and/or modify it
 * under the terms of the  GNU Lesser General Public License  as published by 
 * the  Free Software Foundation;  either version 2.1 of the License,  or (at 
 * your option) any later version.
 *
 * This  program  is  distributed in  the  hope that it will  be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
 * License for more details.
 *
 * You should  have received  a copy of the GNU Lesser General Public License
 * along with  this program;  if not, write to the  Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: tracks.c,v 1.1 2007-03-25 14:51:12 cruanyes Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/tracks.h>
#include <lib3ds/io.h>
#include <lib3ds/chunk.h>
#include <lib3ds/float.h>
#include <lib3ds/vector.h>
#include <lib3ds/quat.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <config.h>
#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


/*!
 * \defgroup tracks Keyframing Tracks
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


/*!
 * \ingroup tracks 
 */
Lib3dsBoolKey*
lib3ds_bool_key_new()
{
  Lib3dsBoolKey* k;
  k=(Lib3dsBoolKey*)calloc(sizeof(Lib3dsBoolKey), 1);
  return(k);
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_bool_key_free(Lib3dsBoolKey *key)
{
  ASSERT(key);
  free(key);
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_bool_track_free_keys(Lib3dsBoolTrack *track)
{
  Lib3dsBoolKey *p,*q;

  ASSERT(track);
  for (p=track->keyL; p; p=q) {
    q=p->next;
    lib3ds_bool_key_free(p);
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_bool_track_insert(Lib3dsBoolTrack *track, Lib3dsBoolKey *key)
{
  ASSERT(track);
  ASSERT(key);
  ASSERT(!key->next);

  if (!track->keyL) {
    track->keyL=key;
    key->next=0;
  }
  else {
    Lib3dsBoolKey *k,*p;

    for (p=0,k=track->keyL; k!=0; p=k, k=k->next) {
      if (k->tcb.frame>key->tcb.frame) {
        break;
      }
    }
    if (!p) {
      key->next=track->keyL;
      track->keyL=key;
    }
    else {
      key->next=k;
      p->next=key;
    }
 
    if (k && (key->tcb.frame==k->tcb.frame)) {
      key->next=k->next;
      lib3ds_bool_key_free(k);
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_bool_track_remove(Lib3dsBoolTrack *track, Lib3dsIntd frame)
{
  Lib3dsBoolKey *k,*p;
  
  ASSERT(track);
  if (!track->keyL) {
    return;
  }
  for (p=0,k=track->keyL; k!=0; p=k, k=k->next) {
    if (k->tcb.frame==frame) {
      if (!p) {
        track->keyL=track->keyL->next;
      }
      else {
        p->next=k->next;
      }
      lib3ds_bool_key_free(k);
      break;
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_bool_track_eval(Lib3dsBoolTrack *track, Lib3dsBool *p, Lib3dsFloat t)
{
  Lib3dsBoolKey *k;
  Lib3dsBool result;

  ASSERT(p);
  if (!track->keyL) {
    *p=LIB3DS_FALSE;
    return;
  }
  if (!track->keyL->next) {
    *p = LIB3DS_TRUE;
    return;
  }

  result=LIB3DS_FALSE;
  k=track->keyL;
  while ((t<(Lib3dsFloat)k->tcb.frame) && (t>=(Lib3dsFloat)k->next->tcb.frame)) {
    if (result) {
      result=LIB3DS_FALSE;
    }
    else {
      result=LIB3DS_TRUE;
    }
    if (!k->next) {
      if (track->flags&LIB3DS_REPEAT) {
        t-=(Lib3dsFloat)k->tcb.frame;
        k=track->keyL;
      }
      else {
        break;
      }
    }
    else {
      k=k->next;
    }
  }
  *p=result;
}


/*!
 * \ingroup tracks 
 */
Lib3dsBool
lib3ds_bool_track_read(Lib3dsBoolTrack *track, Lib3dsIo *io)
{
  int keys;
  int i;
  Lib3dsBoolKey *k;

  track->flags=lib3ds_io_read_word(io);
  lib3ds_io_read_dword(io);
  lib3ds_io_read_dword(io);
  keys=lib3ds_io_read_intd(io);

  for (i=0; i<keys; ++i) {
    k=lib3ds_bool_key_new();
    if (!lib3ds_tcb_read(&k->tcb, io)) {
      return(LIB3DS_FALSE);
    }
    lib3ds_bool_track_insert(track, k);
  }
  
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup tracks 
 */
Lib3dsBool
lib3ds_bool_track_write(Lib3dsBoolTrack *track, Lib3dsIo *io)
{
  Lib3dsBoolKey *k;
  Lib3dsDword num=0;
  for (k=track->keyL; k; k=k->next) {
    ++num;
  }
  lib3ds_io_write_word(io, (Lib3dsWord)track->flags);
  lib3ds_io_write_dword(io, 0);
  lib3ds_io_write_dword(io, 0);
  lib3ds_io_write_dword(io, num);

  for (k=track->keyL; k; k=k->next) {
    if (!lib3ds_tcb_write(&k->tcb,io)) {
      return(LIB3DS_FALSE);
    }
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup tracks 
 */
Lib3dsLin1Key*
lib3ds_lin1_key_new()
{
  Lib3dsLin1Key* k;
  k=(Lib3dsLin1Key*)calloc(sizeof(Lib3dsLin1Key), 1);
  return(k);
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin1_key_free(Lib3dsLin1Key *key)
{
  ASSERT(key);
  free(key);
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin1_track_free_keys(Lib3dsLin1Track *track)
{
  Lib3dsLin1Key *p,*q;

  ASSERT(track);
  for (p=track->keyL; p; p=q) {
    q=p->next;
    lib3ds_lin1_key_free(p);
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin1_key_setup(Lib3dsLin1Key *p, Lib3dsLin1Key *cp, Lib3dsLin1Key *c,
  Lib3dsLin1Key *cn, Lib3dsLin1Key *n)
{
  Lib3dsFloat np,nn;
  Lib3dsFloat ksm,ksp,kdm,kdp;
  
  ASSERT(c);
  if (!cp) {
    cp=c;
  }
  if (!cn) {
    cn=c;
  }
  if (!p && !n) {
    c->ds=0;
    c->dd=0;
    return;
  }

  if (n && p) {
    lib3ds_tcb(&p->tcb, &cp->tcb, &c->tcb, &cn->tcb, &n->tcb, &ksm, &ksp, &kdm, &kdp);
    np = c->value - p->value; 
    nn = n->value - c->value; 

    c->ds=ksm*np + ksp*nn;
    c->dd=kdm*np + kdp*nn;
  }
  else {
    if (p) {
      np = c->value - p->value;
      c->ds = np;
      c->dd = np;
    }
    if (n) {
      nn = n->value - c->value; 
      c->ds = nn;
      c->dd = nn;
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin1_track_setup(Lib3dsLin1Track *track)
{
  Lib3dsLin1Key *pp,*pc,*pn,*pl;

  ASSERT(track);
  pc=track->keyL;
  if (!pc) {
    return;
  }
  if (!pc->next) {
    pc->ds=0;
    pc->dd=0;
    return;
  }

  if (track->flags&LIB3DS_SMOOTH) {
    for (pl=track->keyL; pl->next->next; pl=pl->next);
    lib3ds_lin1_key_setup(pl, pl->next, pc, 0, pc->next);
 }
 else {
   lib3ds_lin1_key_setup(0, 0, pc, 0, pc->next);
 }
  for (;;) {
    pp=pc;
    pc=pc->next;
    pn=pc->next;
    if (!pn) {
      break;
    }
    lib3ds_lin1_key_setup(pp, 0, pc, 0, pn);
  }

  if (track->flags&LIB3DS_SMOOTH) {
    lib3ds_lin1_key_setup(pp, 0, pc, track->keyL, track->keyL->next);
  }
  else {
    lib3ds_lin1_key_setup(pp, 0, pc, 0, 0);
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin1_track_insert(Lib3dsLin1Track *track, Lib3dsLin1Key *key)
{
  ASSERT(track);
  ASSERT(key);
  ASSERT(!key->next);

  if (!track->keyL) {
    track->keyL=key;
    key->next=0;
  }
  else {
    Lib3dsLin1Key *k,*p;

    for (p=0,k=track->keyL; k!=0; p=k, k=k->next) {
      if (k->tcb.frame>key->tcb.frame) {
        break;
      }
    }
    if (!p) {
      key->next=track->keyL;
      track->keyL=key;
    }
    else {
      key->next=k;
      p->next=key;
    }
 
    if (k && (key->tcb.frame==k->tcb.frame)) {
      key->next=k->next;
      lib3ds_lin1_key_free(k);
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin1_track_remove(Lib3dsLin1Track *track, Lib3dsIntd frame)
{
  Lib3dsLin1Key *k,*p;
  
  ASSERT(track);
  if (!track->keyL) {
    return;
  }
  for (p=0,k=track->keyL; k!=0; p=k, k=k->next) {
    if (k->tcb.frame==frame) {
      if (!p) {
        track->keyL=track->keyL->next;
      }
      else {
        p->next=k->next;
      }
      lib3ds_lin1_key_free(k);
      break;
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin1_track_eval(Lib3dsLin1Track *track, Lib3dsFloat *p, Lib3dsFloat t)
{
  Lib3dsLin1Key *k;
  Lib3dsFloat nt;
  Lib3dsFloat u;

  ASSERT(p);
  if (!track->keyL) {
    *p=0;
    return;
  }
  if (!track->keyL->next) {
    *p = track->keyL->value;
    return;
  }

  for (k=track->keyL; k->next!=0; k=k->next) {
    if ((t>=(Lib3dsFloat)k->tcb.frame) && (t<(Lib3dsFloat)k->next->tcb.frame)) {
      break;
    }
  }
  if (!k->next) {
    if (track->flags&LIB3DS_REPEAT) {
      nt=(Lib3dsFloat)fmod(t, k->tcb.frame);
      for (k=track->keyL; k->next!=0; k=k->next) {
        if ((nt>=(Lib3dsFloat)k->tcb.frame) && (nt<(Lib3dsFloat)k->next->tcb.frame)) {
          break;
        }
      }
      ASSERT(k->next);
    }
    else {
      *p = k->value;
      return;
    }
  }
  else {
    nt=t;
  }
  u=nt - (Lib3dsFloat)k->tcb.frame;
  u/=(Lib3dsFloat)(k->next->tcb.frame - k->tcb.frame);

  *p = lib3ds_float_cubic(
    k->value,
    k->dd,
    k->next->ds,
    k->next->value,
    u
  );
}


/*!
 * \ingroup tracks 
 */
Lib3dsBool
lib3ds_lin1_track_read(Lib3dsLin1Track *track, Lib3dsIo *io)
{
  int keys;
  int i;
  Lib3dsLin1Key *k;

  track->flags=lib3ds_io_read_word(io);
  lib3ds_io_read_dword(io);
  lib3ds_io_read_dword(io);
  keys=lib3ds_io_read_intd(io);

  for (i=0; i<keys; ++i) {
    k=lib3ds_lin1_key_new();
    if (!lib3ds_tcb_read(&k->tcb, io)) {
      return(LIB3DS_FALSE);
    }
    k->value=lib3ds_io_read_float(io);
    lib3ds_lin1_track_insert(track, k);
  }
  lib3ds_lin1_track_setup(track);
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup tracks 
 */
Lib3dsBool
lib3ds_lin1_track_write(Lib3dsLin1Track *track, Lib3dsIo *io)
{
  Lib3dsLin1Key *k;
  Lib3dsDword num=0;
  for (k=track->keyL; k; k=k->next) {
    ++num;
  }
  lib3ds_io_write_word(io, (Lib3dsWord)track->flags);
  lib3ds_io_write_dword(io, 0);
  lib3ds_io_write_dword(io, 0);
  lib3ds_io_write_dword(io, num);

  for (k=track->keyL; k; k=k->next) {
    if (!lib3ds_tcb_write(&k->tcb,io)) {
      return(LIB3DS_FALSE);
    }
    lib3ds_io_write_float(io, k->value);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup tracks 
 */
Lib3dsLin3Key*
lib3ds_lin3_key_new()
{
  Lib3dsLin3Key* k;
  k=(Lib3dsLin3Key*)calloc(sizeof(Lib3dsLin3Key), 1);
  return(k);
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin3_key_free(Lib3dsLin3Key *key)
{
  ASSERT(key);
  free(key);
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin3_track_free_keys(Lib3dsLin3Track *track)
{
  Lib3dsLin3Key *p,*q;

  ASSERT(track);
  for (p=track->keyL; p; p=q) {
    q=p->next;
    lib3ds_lin3_key_free(p);
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin3_key_setup(Lib3dsLin3Key *p, Lib3dsLin3Key *cp, Lib3dsLin3Key *c,
  Lib3dsLin3Key *cn, Lib3dsLin3Key *n)
{
  Lib3dsVector np,nn;
  Lib3dsFloat ksm,ksp,kdm,kdp;
  int i;
  
  ASSERT(c);
  if (!cp) {
    cp=c;
  }
  if (!cn) {
    cn=c;
  }
  if (!p && !n) {
    lib3ds_vector_zero(c->ds);
    lib3ds_vector_zero(c->dd);
    return;
  }

  if (n && p) {
    lib3ds_tcb(&p->tcb, &cp->tcb, &c->tcb, &cn->tcb, &n->tcb, &ksm, &ksp, &kdm, &kdp);
    lib3ds_vector_sub(np, c->value, p->value); 
    lib3ds_vector_sub(nn, n->value, c->value); 

    for(i=0; i<3; ++i) {
      c->ds[i]=ksm*np[i] + ksp*nn[i];
      c->dd[i]=kdm*np[i] + kdp*nn[i];
    }
  }
  else {
    if (p) {
      lib3ds_vector_sub(np, c->value, p->value);
      lib3ds_vector_copy(c->ds, np);
      lib3ds_vector_copy(c->dd, np);
    }
    if (n) {
      lib3ds_vector_sub(nn, n->value, c->value); 
      lib3ds_vector_copy(c->ds, nn);
      lib3ds_vector_copy(c->dd, nn);
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin3_track_setup(Lib3dsLin3Track *track)
{
  Lib3dsLin3Key *pp,*pc,*pn,*pl;

  ASSERT(track);
  pc=track->keyL;
  if (!pc) {
    return;
  }
  if (!pc->next) {
    lib3ds_vector_zero(pc->ds);
    lib3ds_vector_zero(pc->dd);
    return;
  }

  if (track->flags&LIB3DS_SMOOTH) {
    for (pl=track->keyL; pl->next->next; pl=pl->next);
    lib3ds_lin3_key_setup(pl, pl->next, pc, 0, pc->next);
 }
 else {
   lib3ds_lin3_key_setup(0, 0, pc, 0, pc->next);
 }
  for (;;) {
    pp=pc;
    pc=pc->next;
    pn=pc->next;
    if (!pn) {
      break;
    }
    lib3ds_lin3_key_setup(pp, 0, pc, 0, pn);
  }

  if (track->flags&LIB3DS_SMOOTH) {
    lib3ds_lin3_key_setup(pp, 0, pc, track->keyL, track->keyL->next);
  }
  else {
    lib3ds_lin3_key_setup(pp, 0, pc, 0, 0);
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin3_track_insert(Lib3dsLin3Track *track, Lib3dsLin3Key *key)
{
  ASSERT(track);
  ASSERT(key);
  ASSERT(!key->next);

  if (!track->keyL) {
    track->keyL=key;
    key->next=0;
  }
  else {
    Lib3dsLin3Key *k,*p;

    for (p=0,k=track->keyL; k!=0; p=k, k=k->next) {
      if (k->tcb.frame>key->tcb.frame) {
        break;
      }
    }
    if (!p) {
      key->next=track->keyL;
      track->keyL=key;
    }
    else {
      key->next=k;
      p->next=key;
    }
 
    if (k && (key->tcb.frame==k->tcb.frame)) {
      key->next=k->next;
      lib3ds_lin3_key_free(k);
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin3_track_remove(Lib3dsLin3Track *track, Lib3dsIntd frame)
{
  Lib3dsLin3Key *k,*p;
  
  ASSERT(track);
  if (!track->keyL) {
    return;
  }
  for (p=0,k=track->keyL; k!=0; p=k, k=k->next) {
    if (k->tcb.frame==frame) {
      if (!p) {
        track->keyL=track->keyL->next;
      }
      else {
        p->next=k->next;
      }
      lib3ds_lin3_key_free(k);
      break;
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_lin3_track_eval(Lib3dsLin3Track *track, Lib3dsVector p, Lib3dsFloat t)
{
  Lib3dsLin3Key *k;
  Lib3dsFloat nt;
  Lib3dsFloat u;

  if (!track->keyL) {
    lib3ds_vector_zero(p);
    return;
  }
  if (!track->keyL->next) {
      lib3ds_vector_copy(p, track->keyL->value);
    return;
  }

  for (k=track->keyL; k->next!=0; k=k->next) {
    if ((t>=(Lib3dsFloat)k->tcb.frame) && (t<(Lib3dsFloat)k->next->tcb.frame)) {
      break;
    }
  }
  if (!k->next) {
    if (track->flags&LIB3DS_REPEAT) {
      nt=(Lib3dsFloat)fmod(t, k->tcb.frame);
      for (k=track->keyL; k->next!=0; k=k->next) {
        if ((nt>=(Lib3dsFloat)k->tcb.frame) && (nt<(Lib3dsFloat)k->next->tcb.frame)) {
          break;
        }
      }
      ASSERT(k->next);
    }
    else {
      lib3ds_vector_copy(p, k->value);
      return;
    }
  }
  else {
    nt=t;
  }
  u=nt - (Lib3dsFloat)k->tcb.frame;
  u/=(Lib3dsFloat)(k->next->tcb.frame - k->tcb.frame);
  
  lib3ds_vector_cubic(
    p,
    k->value,
    k->dd,
    k->next->ds,
    k->next->value,
    u
  );
}


/*!
 * \ingroup tracks 
 */
Lib3dsBool
lib3ds_lin3_track_read(Lib3dsLin3Track *track, Lib3dsIo *io)
{
  int keys;
  int i,j;
  Lib3dsLin3Key *k;

  track->flags=lib3ds_io_read_word(io);
  lib3ds_io_read_dword(io);
  lib3ds_io_read_dword(io);
  keys=lib3ds_io_read_intd(io);

  for (i=0; i<keys; ++i) {
    k=lib3ds_lin3_key_new();
    if (!lib3ds_tcb_read(&k->tcb, io)) {
      return(LIB3DS_FALSE);
    }
    for (j=0; j<3; ++j) {
      k->value[j]=lib3ds_io_read_float(io);
    }
    lib3ds_lin3_track_insert(track, k);
  }
  lib3ds_lin3_track_setup(track);
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup tracks 
 */
Lib3dsBool
lib3ds_lin3_track_write(Lib3dsLin3Track *track, Lib3dsIo *io)
{
  Lib3dsLin3Key *k;
  Lib3dsDword num=0;
  for (k=track->keyL; k; k=k->next) {
    ++num;
  }
  lib3ds_io_write_word(io, (Lib3dsWord)track->flags);
  lib3ds_io_write_dword(io, 0);
  lib3ds_io_write_dword(io, 0);
  lib3ds_io_write_dword(io, num);

  for (k=track->keyL; k; k=k->next) {
    if (!lib3ds_tcb_write(&k->tcb,io)) {
      return(LIB3DS_FALSE);
    }
    lib3ds_io_write_vector(io, k->value);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup tracks 
 */
Lib3dsQuatKey*
lib3ds_quat_key_new()
{
  Lib3dsQuatKey* k;
  k=(Lib3dsQuatKey*)calloc(sizeof(Lib3dsQuatKey), 1);
  return(k);
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_quat_key_free(Lib3dsQuatKey *key)
{
  ASSERT(key);
  free(key);
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_quat_track_free_keys(Lib3dsQuatTrack *track)
{
  Lib3dsQuatKey *p,*q;

  ASSERT(track);
  for (p=track->keyL; p; p=q) {
    q=p->next;
    lib3ds_quat_key_free(p);
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_quat_key_setup(Lib3dsQuatKey *p, Lib3dsQuatKey *cp, Lib3dsQuatKey *c,
  Lib3dsQuatKey *cn, Lib3dsQuatKey *n)
{
  Lib3dsFloat ksm,ksp,kdm,kdp;
  Lib3dsQuat q,qp,qn,qa,qb;
  int i;
  
  ASSERT(c);
  if (!cp) {
    cp=c;
  }
  if (!cn) {
    cn=c;
  }
  if (!p || !n) {
    lib3ds_quat_copy(c->ds, c->q);
    lib3ds_quat_copy(c->dd, c->q);
    return;
  }

  if (p) {
    if (p->angle>LIB3DS_TWOPI-LIB3DS_EPSILON) {
      lib3ds_quat_axis_angle(qp, p->axis, 0.0f);
      lib3ds_quat_ln(qp);
    }
    else {
      lib3ds_quat_copy(q, p->q);
      if (lib3ds_quat_dot(q,c->q)<0) lib3ds_quat_neg(q);
      lib3ds_quat_ln_dif(qp, c->q, q);
    }
  }
  if (n) {
    if (n->angle>LIB3DS_TWOPI-LIB3DS_EPSILON) {
      lib3ds_quat_axis_angle(qn, n->axis, 0.0f);
      lib3ds_quat_ln(qn);
    }
    else {
      lib3ds_quat_copy(q, n->q);
      if (lib3ds_quat_dot(q,c->q)<0) lib3ds_quat_neg(q);
      lib3ds_quat_ln_dif(qn, c->q, q);
    }
  }

  if (n && p) {
    lib3ds_tcb(&p->tcb, &cp->tcb, &c->tcb, &cn->tcb, &n->tcb, &ksm, &ksp, &kdm, &kdp);
    for(i=0; i<4; i++) {
      qa[i]=-0.5f*(kdm*qn[i]+kdp*qp[i]);
      qb[i]=-0.5f*(ksm*qn[i]+ksp*qp[i]);
    }
    lib3ds_quat_exp(qa);
    lib3ds_quat_exp(qb);
    
    lib3ds_quat_mul(c->ds, c->q, qa);
    lib3ds_quat_mul(c->dd, c->q, qb);
  }
  else {
    if (p) {
      lib3ds_quat_exp(qp);
      lib3ds_quat_mul(c->ds, c->q, qp);
      lib3ds_quat_mul(c->dd, c->q, qp);
    }
    if (n) {
      lib3ds_quat_exp(qn);
      lib3ds_quat_mul(c->ds, c->q, qn);
      lib3ds_quat_mul(c->dd, c->q, qn);
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_quat_track_setup(Lib3dsQuatTrack *track)
{
  Lib3dsQuatKey *pp,*pc,*pn,*pl;
  Lib3dsQuat q;

  ASSERT(track);
  for (pp=0,pc=track->keyL; pc; pp=pc,pc=pc->next) {
    lib3ds_quat_axis_angle(q, pc->axis, pc->angle);
    if (pp) {
      lib3ds_quat_mul(pc->q, q, pp->q);
    }
    else {
      lib3ds_quat_copy(pc->q, q);
    }
  }

  pc=track->keyL;
  if (!pc) {
    return;
  }
  if (!pc->next) {
    lib3ds_quat_copy(pc->ds, pc->q);
    lib3ds_quat_copy(pc->dd, pc->q);
    return;
  }

  if (track->flags&LIB3DS_SMOOTH) {
    for (pl=track->keyL; pl->next->next; pl=pl->next);
    lib3ds_quat_key_setup(pl, pl->next, pc, 0, pc->next);
 }
  else {
    lib3ds_quat_key_setup(0, 0, pc, 0, pc->next);
  }
  for (;;) {
    pp=pc;
    pc=pc->next;
    pn=pc->next;
    if (!pn) {
      break;
    }
    lib3ds_quat_key_setup(pp, 0, pc, 0, pn);
  }

  if (track->flags&LIB3DS_SMOOTH) {
    lib3ds_quat_key_setup(pp, 0, pc, track->keyL, track->keyL->next);
  }
  else {
    lib3ds_quat_key_setup(pp, 0, pc, 0, 0);
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_quat_track_insert(Lib3dsQuatTrack *track, Lib3dsQuatKey *key)
{
  ASSERT(track);
  ASSERT(key);
  ASSERT(!key->next);

  if (!track->keyL) {
    track->keyL=key;
    key->next=0;
  }
  else {
    Lib3dsQuatKey *k,*p;

    for (p=0,k=track->keyL; k!=0; p=k, k=k->next) {
      if (k->tcb.frame>key->tcb.frame) {
        break;
      }
    }
    if (!p) {
      key->next=track->keyL;
      track->keyL=key;
    }
    else {
      key->next=k;
      p->next=key;
    }
 
    if (k && (key->tcb.frame==k->tcb.frame)) {
      key->next=k->next;
      lib3ds_quat_key_free(k);
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_quat_track_remove(Lib3dsQuatTrack *track, Lib3dsIntd frame)
{
  Lib3dsQuatKey *k,*p;
  
  ASSERT(track);
  if (!track->keyL) {
    return;
  }
  for (p=0,k=track->keyL; k!=0; p=k, k=k->next) {
    if (k->tcb.frame==frame) {
      if (!p) {
        track->keyL=track->keyL->next;
      }
      else {
        p->next=k->next;
      }
      lib3ds_quat_key_free(k);
      break;
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_quat_track_eval(Lib3dsQuatTrack *track, Lib3dsQuat q, Lib3dsFloat t)
{
  Lib3dsQuatKey *k;
  Lib3dsFloat nt;
  Lib3dsFloat u;

  if (!track->keyL) {
    lib3ds_quat_identity(q);
    return;
  }
  if (!track->keyL->next) {
    lib3ds_quat_copy(q, track->keyL->q);
    return;
  }

  for (k=track->keyL; k->next!=0; k=k->next) {
    if ((t>=k->tcb.frame) && (t<k->next->tcb.frame)) {
      break;
    }
  }
  if (!k->next) {
    if (track->flags&LIB3DS_REPEAT) {
      nt=(Lib3dsFloat)fmod(t, k->tcb.frame);
      for (k=track->keyL; k->next!=0; k=k->next) {
        if ((nt>=k->tcb.frame) && (nt<k->next->tcb.frame)) {
          break;
        }
      }
      ASSERT(k->next);
    }
    else {
      lib3ds_quat_copy(q, k->q);
      return;
    }
  }
  else {
    nt=t;
  }
  u=nt - k->tcb.frame;
  u/=(k->next->tcb.frame - k->tcb.frame);

  lib3ds_quat_squad(
    q,
    k->q,
    k->dd,
    k->next->ds,
    k->next->q,
    u
  );
}


/*!
 * \ingroup tracks 
 */
Lib3dsBool
lib3ds_quat_track_read(Lib3dsQuatTrack *track, Lib3dsIo *io)
{
  int keys;
  int i,j;
  Lib3dsQuatKey *p,*k;

  track->flags=lib3ds_io_read_word(io);
  lib3ds_io_read_dword(io);
  lib3ds_io_read_dword(io);
  keys=lib3ds_io_read_intd(io);

  for (p=0,i=0; i<keys; p=k,++i) {
    k=lib3ds_quat_key_new();
    if (!lib3ds_tcb_read(&k->tcb, io)) {
      return(LIB3DS_FALSE);
    }
    k->angle=lib3ds_io_read_float(io);
    for (j=0; j<3; ++j) {
      k->axis[j]=lib3ds_io_read_float(io);
    }
    lib3ds_quat_track_insert(track, k);
  }
  lib3ds_quat_track_setup(track);
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup tracks 
 */
Lib3dsBool
lib3ds_quat_track_write(Lib3dsQuatTrack *track, Lib3dsIo *io)
{
  Lib3dsQuatKey *k;
  Lib3dsDword num=0;
  for (k=track->keyL; k; k=k->next) {
    ++num;
  }
  lib3ds_io_write_word(io, (Lib3dsWord)track->flags);
  lib3ds_io_write_dword(io, 0);
  lib3ds_io_write_dword(io, 0);
  lib3ds_io_write_dword(io, num);

  for (k=track->keyL; k; k=k->next) {
    if (!lib3ds_tcb_write(&k->tcb,io)) {
      return(LIB3DS_FALSE);
    }
    lib3ds_io_write_float(io, k->angle);
    lib3ds_io_write_vector(io, k->axis);
  }
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup tracks 
 */
Lib3dsMorphKey*
lib3ds_morph_key_new()
{
  Lib3dsMorphKey* k;
  k=(Lib3dsMorphKey*)calloc(sizeof(Lib3dsMorphKey), 1);
  return(k);
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_morph_key_free(Lib3dsMorphKey *key)
{
  ASSERT(key);
  free(key);
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_morph_track_free_keys(Lib3dsMorphTrack *track)
{
  Lib3dsMorphKey *p,*q;

  ASSERT(track);
  for (p=track->keyL; p; p=q) {
    q=p->next;
    lib3ds_morph_key_free(p);
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_morph_track_insert(Lib3dsMorphTrack *track, Lib3dsMorphKey *key)
{
  ASSERT(track);
  ASSERT(key);
  ASSERT(!key->next);

  if (!track->keyL) {
    track->keyL=key;
    key->next=0;
  }
  else {
    Lib3dsMorphKey *k,*p;

    for (p=0,k=track->keyL; k!=0; p=k, k=k->next) {
      if (k->tcb.frame>key->tcb.frame) {
        break;
      }
    }
    if (!p) {
      key->next=track->keyL;
      track->keyL=key;
    }
    else {
      key->next=k;
      p->next=key;
    }
 
    if (k && (key->tcb.frame==k->tcb.frame)) {
      key->next=k->next;
      lib3ds_morph_key_free(k);
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_morph_track_remove(Lib3dsMorphTrack *track, Lib3dsIntd frame)
{
  Lib3dsMorphKey *k,*p;
  
  ASSERT(track);
  if (!track->keyL) {
    return;
  }
  for (p=0,k=track->keyL; k!=0; p=k, k=k->next) {
    if (k->tcb.frame==frame) {
      if (!p) {
        track->keyL=track->keyL->next;
      }
      else {
        p->next=k->next;
      }
      lib3ds_morph_key_free(k);
      break;
    }
  }
}


/*!
 * \ingroup tracks 
 */
void
lib3ds_morph_track_eval(Lib3dsMorphTrack *track, char *p, Lib3dsFloat t)
{
  Lib3dsMorphKey *k;
  char* result;

  ASSERT(p);
  if (!track->keyL) {
    strcpy(p,"");
    return;
  }
  if (!track->keyL->next) {
    strcpy(p,track->keyL->name);
    return;
  }

  result=0;
  k=track->keyL;
  while ((t<k->tcb.frame) && (t>=k->next->tcb.frame)) {
    result=k->name;
    if (!k->next) {
      if (track->flags&LIB3DS_REPEAT) {
        t-=k->tcb.frame;
        k=track->keyL;
      }
      else {
        break;
      }
    }
    else {
      k=k->next;
    }
  }
  if (result) {
    strcpy(p,result);
  }
  else {
    strcpy(p,"");
  }
}


/*!
 * \ingroup tracks
 */
Lib3dsBool
lib3ds_morph_track_read(Lib3dsMorphTrack *track, Lib3dsIo *io)
{
  /* FIXME: */
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup tracks 
 */
Lib3dsBool
lib3ds_morph_track_write(Lib3dsMorphTrack *track, Lib3dsIo *io)
{
  /* FIXME: */
  ASSERT(0);
  return(LIB3DS_FALSE);
}


