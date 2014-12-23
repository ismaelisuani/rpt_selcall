/*-
 *  selcall.h
 *  header for selcall.c
 *
 *  Author: Ismael Isuani (iisuani@rlink.com.ar)
 *
 *  Copyright (c) 2014, 2015  Ismael Isuani  All rights reserved.
 * 
 *  The SelCall decoder Library is free software; you can
 *  redistribute it and/or modify it under the terms of version 2 of
 *  the GNU General Public License as published by the Free Software
 *  Foundation.
 *
 *  If you cannot comply with the terms of this license, contact
 *  the author for alternative license arrangements or do not use
 *  or redistribute this software.
 *
 *  The SelCall decoder Library is distributed in the hope
 *  that it will be useful, but WITHOUT ANY WARRANTY; without even the
 *  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 *  USA.
 *
 *  or see http://www.gnu.org/copyleft/gpl.html
 *
-*/

#ifndef _SELCALL_H_
#define _SELCALL_H_

#include "multimon.h"
#include "demod_ccir.c"
#include "demod_eea.c"
#include "demod_eia.c"
#include "demod_zvei1.c"
#include "demod_zvei2.c"
#include "demod_zvei3.c"
#include "costabi.c"
#include "costabf.c"

int find_max_idx(const float *);
selcall_decoder_t * selcall_decoder_new(char *[], int);
void process_selcall(short *sp, int len, selcall_decoder_t *selcall);

#endif
