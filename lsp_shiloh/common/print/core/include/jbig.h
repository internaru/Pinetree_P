/*	Copyright 1999-2000 Zenographics, Inc.  ALL RIGHTS RESERVED. */

#ifndef JBIG_H
#define JBIG_H

typedef struct {
	uint8_t Dl;
	uint8_t D;
	uint8_t P;
	uint8_t reserved;
	/* the DWORDs are BigEndian */
	uint32_t Xd, Yd;
	uint32_t L0;
	uint8_t Mx, My;
	uint8_t Order;
	uint8_t Options;
	} JBIG_BIH;

/*	Order flags  */

#define JBIG_HITOLO     0x08
#define JBIG_SEQ        0x04
#define JBIG_ILEAVE     0x02
#define JBIG_SMID       0x01

/*	Option flags  */

#define JBIG_LRLTWO     0x40
#define JBIG_VLENGTH    0x20
#define JBIG_TPDON      0x10
#define JBIG_TPBON      0x08
#define JBIG_DPON       0x04
#define JBIG_DPPRIV     0x02
#define JBIG_DPLAST     0x01

//
// Resource interface defines for jbig blocks
// 


#endif /* JBIG_H */
