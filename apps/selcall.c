/*
 *      selcall.c
 *
 *      Copyright (C) 1996
 *          Thomas Sailer (sailer@ife.ee.ethz.ch, hb9jnx@hb9w.che.eu)
 *
 *      Copyright (C) 2013
 *          Elias Oenal    (EliasOenal@gmail.com)
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* ---------------------------------------------------------------------- */

#include "selcall.h"
#include <math.h>
#include <string.h>

static inline float fsqr(float f)
{
        return f*f;
}

selcall_decoder_t * selcall_decoder_new(char *name, const unsigned int *freq)
{
        selcall_decoder_t *decoder;

        decoder = (selcall_decoder_t *)malloc(sizeof(selcall_decoder_t));
        if(!decoder)
                return (selcall_decoder_t *) 0L;

	memset(decoder, 0, sizeof(decoder));

	decoder->name = name;
	decoder->fbuf_cnt = 0;
	decoder->overlap = 0;
	decoder->freq = freq;

	return decoder;
}

int find_max_idx(const float *f) 
{
    float en = 0;
    int idx = -1, i;

    for (i = 0; i < 16; i++)
        if (f[i] > en) {
            en = f[i];
            idx = i;
        }
    if (idx < 0)
        return -1;
    en *= 0.1;
    for (i = 0; i < 16; i++)
        if (idx != i && f[i] > en)
            return -1;
    return idx;
}

static inline int process_block(selcall_decoder_t *s)
{
    float tote;
    float totte[32];
    int i, j;

    tote = 0;
    for (i = 0; i < BLOCKNUM; i++)
        tote += s->selcall.energy[i];
    for (i = 0; i < 32; i++) {
        totte[i] = 0;
        for (j = 0; j < BLOCKNUM; j++)
            totte[i] += s->selcall.tenergy[j][i];
    }
    for (i = 0; i < 16; i++)
        totte[i] = fsqr(totte[i]) + fsqr(totte[i+16]);
    memmove(s->selcall.energy+1, s->selcall.energy,
            sizeof(s->selcall.energy) - sizeof(s->selcall.energy[0]));
    s->selcall.energy[0] = 0;
    memmove(s->selcall.tenergy+1, s->selcall.tenergy,
            sizeof(s->selcall.tenergy) - sizeof(s->selcall.tenergy[0]));
    memset(s->selcall.tenergy, 0, sizeof(s->selcall.tenergy[0]));
    tote *= (BLOCKNUM*BLOCKLEN*0.5);  /* adjust for block lengths */
    if ((i = find_max_idx(totte)) < 0)
        return -1;
    if ((tote * 0.4) > totte[i])
        return -1;
    return i;
}

void selcall_demod(selcall_decoder_t *s)
{
    float s_in;
    int i, length;
    const float *buffer;
    buffer = s->fbuf;
    length = s->fbuf_cnt - s->overlap;

    for (; length > 0; length--, buffer++) {
        s_in = *buffer;
        s->selcall.energy[0] += fsqr(s_in);
        for (i = 0; i < 16; i++) {
            s->selcall.tenergy[0][i] += COS(s->selcall.ph[i]) * s_in;
            s->selcall.tenergy[0][i+16] += SIN(s->selcall.ph[i]) * s_in;
            s->selcall.ph[i] += s->freq[i];
        }
        if ((s->selcall.blkcount--) <= 0) {
            s->selcall.blkcount = BLOCKLEN;
            i = process_block(s);
            if (i != s->selcall.lastch && i >= 0)
            {
		sprintf(&s->tonebuf[strlen(s->tonebuf)], "%1X", i);
                s->selcall.timeout = 1;
            }

            if(i == -1 && s->selcall.timeout != 0)
                s->selcall.timeout++;
            if(s->selcall.timeout > TIMEOUT_LIMIT+1) {
		memset(s->tonebuf,0,sizeof(s->tonebuf));
                s->selcall.timeout = 0;
	    }

            s->selcall.lastch = i;
        }
    }
}
