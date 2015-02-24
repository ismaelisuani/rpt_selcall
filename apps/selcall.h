/*
 *      multimon.h -- Monitor for many different modulation formats
 *
 *      Copyright (C) 1996
 *          Thomas Sailer (sailer@ife.ee.ethz.ch, hb9jnx@hb9w.che.eu)
 *
 *      Added eas parts - A. Maitland Bottoms 27 June 2000
 *
 *      Copyright (C) 2012-2014
 *          Elias Oenal    (multimon-ng@eliasoenal.com)
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

#ifndef _SELCALL_H_
#define _SELCALL_H_

#include <stdint.h>
#include <stdbool.h>

#include "costabf.c"

extern const float costabf[0x400];
#define COS(x) costabf[(((x)>>6)&0x3ffu)]
#define SIN(x) COS((x)+0xc000)

#define SAMPLE_RATE 8000
#define PHINC(x) ((x)*0x10000/SAMPLE_RATE)
#define BLOCKLEN (SAMPLE_RATE/100)  /* 10ms blocks */
#define BLOCKNUM 4    /* must match numbers in multimon.h */
#define TIMEOUT_LIMIT 5 //50ms

static const unsigned int ccir_freq[16] = { 
    PHINC(1981), PHINC(1124), PHINC(1197), PHINC(1275),
    PHINC(1358), PHINC(1446), PHINC(1540), PHINC(1640),
    PHINC(1747), PHINC(1860), PHINC(2400), PHINC(930),
    PHINC(2247), PHINC(991), PHINC(2110), PHINC(1055)
};
static const unsigned int eea_freq[16] = {
    PHINC(1981), PHINC(1124), PHINC(1197), PHINC(1275),
    PHINC(1358), PHINC(1446), PHINC(1540), PHINC(1640),
    PHINC(1747), PHINC(1860), PHINC(1055), PHINC(930),
    PHINC(2400), PHINC(991), PHINC(2110), PHINC(2247)
};
static const unsigned int eia_freq[16] = {
    PHINC(600), PHINC(741), PHINC(882), PHINC(1023),
    PHINC(1164), PHINC(1305), PHINC(1446), PHINC(1587),
    PHINC(1728), PHINC(1869), PHINC(2151), PHINC(2433),
    PHINC(2010), PHINC(2292), PHINC(459), PHINC(1091)
};
static const unsigned int zvei1_freq[16] = {
    PHINC(2400), PHINC(1060), PHINC(1160), PHINC(1270),
    PHINC(1400), PHINC(1530), PHINC(1670), PHINC(1830),
    PHINC(2000), PHINC(2200), PHINC(2800), PHINC(810),
    PHINC(970), PHINC(885), PHINC(2600), PHINC(680)
};
static const unsigned int zvei2_freq[16] = {
    PHINC(2400), PHINC(1060), PHINC(1160), PHINC(1270),
    PHINC(1400), PHINC(1530), PHINC(1670), PHINC(1830),
    PHINC(2000), PHINC(2200), PHINC(885), PHINC(825),
    PHINC(740), PHINC(680), PHINC(970), PHINC(2600)
};
static const unsigned int zvei3_freq[16] = {
    PHINC(2400), PHINC(1060), PHINC(1160), PHINC(1270),
    PHINC(1400), PHINC(1530), PHINC(1670), PHINC(1830),
    PHINC(2000), PHINC(2200), PHINC(885), PHINC(810),
    PHINC(2800), PHINC(680), PHINC(970), PHINC(2600)
};

typedef struct {
	const char *name;
	unsigned int fbuf_cnt;
	unsigned int overlap;
	const unsigned int *freq;
        short sbuf[8192];
        float fbuf[16384];
	char tonebuf[50];
	struct state_selcall {
		unsigned int ph[16];
		float energy[4];
		float tenergy[4][32];
		int blkcount;
		int lastch;
		int timeout;
	} selcall;
} selcall_decoder_t;

selcall_decoder_t * selcall_decoder_new(char *name, const unsigned int *freq);
void selcall_demod(selcall_decoder_t *s);
int find_max_idx(const float *f);

#endif
