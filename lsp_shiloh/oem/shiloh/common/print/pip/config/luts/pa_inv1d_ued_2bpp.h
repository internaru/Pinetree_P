/******************************************************************************
 * Copyright (c) 2011-2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef INC_INV1D_UED_2BPP_H
#define INC_INV1D_UED_2BPP_H

/*
 * For UED with SC-CSC: 10-bit in to 10-bit out.
 *
 * 2bpp PWM 0x19,0x26,0x3F, visually match original-product print
 * xsamp = [  0,   7,  63, 127, 255, 511, 767, 895, 959, 1000, 1021, 1024]
 * ysamp = [600, 400, 285, 270, 200, 125,  85,  60,  40,   15,    0,    0]
 */
uint32_t PA_inv1d_ued_2bpp[] =
{
0x00000258,
0x0000023b,
0x0000021e,
0x00000202,
0x000001e5,
0x000001c9,
0x000001ac,
0x00000190,
0x0000018e,
0x0000018c,
0x0000018a,
0x00000188,
0x00000186,
0x00000184,
0x00000182,
0x00000180,
0x0000017e,
0x0000017c,
0x0000017a,
0x00000177,
0x00000175,
0x00000173,
0x00000171,
0x0000016f,
0x0000016d,
0x0000016b,
0x00000169,
0x00000167,
0x00000165,
0x00000163,
0x00000161,
0x0000015f,
0x0000015d,
0x0000015b,
0x00000159,
0x00000157,
0x00000155,
0x00000153,
0x00000150,
0x0000014e,
0x0000014c,
0x0000014a,
0x00000148,
0x00000146,
0x00000144,
0x00000142,
0x00000140,
0x0000013e,
0x0000013c,
0x0000013a,
0x00000138,
0x00000136,
0x00000134,
0x00000132,
0x00000130,
0x0000012e,
0x0000012c,
0x00000129,
0x00000127,
0x00000125,
0x00000123,
0x00000121,
0x0000011f,
0x0000011d,
0x0000011d,
0x0000011c,
0x0000011c,
0x0000011c,
0x0000011c,
0x0000011b,
0x0000011b,
0x0000011b,
0x0000011b,
0x0000011a,
0x0000011a,
0x0000011a,
0x0000011a,
0x00000119,
0x00000119,
0x00000119,
0x00000119,
0x00000119,
0x00000118,
0x00000118,
0x00000118,
0x00000118,
0x00000117,
0x00000117,
0x00000117,
0x00000117,
0x00000116,
0x00000116,
0x00000116,
0x00000116,
0x00000115,
0x00000115,
0x00000115,
0x00000115,
0x00000115,
0x00000114,
0x00000114,
0x00000114,
0x00000114,
0x00000113,
0x00000113,
0x00000113,
0x00000113,
0x00000112,
0x00000112,
0x00000112,
0x00000112,
0x00000111,
0x00000111,
0x00000111,
0x00000111,
0x00000111,
0x00000110,
0x00000110,
0x00000110,
0x00000110,
0x0000010f,
0x0000010f,
0x0000010f,
0x0000010f,
0x0000010e,
0x0000010e,
0x0000010e,
0x0000010e,
0x0000010e,
0x0000010d,
0x0000010c,
0x0000010c,
0x0000010b,
0x0000010b,
0x0000010a,
0x0000010a,
0x00000109,
0x00000109,
0x00000108,
0x00000107,
0x00000107,
0x00000106,
0x00000106,
0x00000105,
0x00000105,
0x00000104,
0x00000104,
0x00000103,
0x00000103,
0x00000102,
0x00000101,
0x00000101,
0x00000100,
0x00000100,
0x000000ff,
0x000000ff,
0x000000fe,
0x000000fe,
0x000000fd,
0x000000fd,
0x000000fc,
0x000000fb,
0x000000fb,
0x000000fa,
0x000000fa,
0x000000f9,
0x000000f9,
0x000000f8,
0x000000f8,
0x000000f7,
0x000000f7,
0x000000f6,
0x000000f5,
0x000000f5,
0x000000f4,
0x000000f4,
0x000000f3,
0x000000f3,
0x000000f2,
0x000000f2,
0x000000f1,
0x000000f1,
0x000000f0,
0x000000ef,
0x000000ef,
0x000000ee,
0x000000ee,
0x000000ed,
0x000000ed,
0x000000ec,
0x000000ec,
0x000000eb,
0x000000eb,
0x000000ea,
0x000000e9,
0x000000e9,
0x000000e8,
0x000000e8,
0x000000e7,
0x000000e7,
0x000000e6,
0x000000e6,
0x000000e5,
0x000000e4,
0x000000e4,
0x000000e3,
0x000000e3,
0x000000e2,
0x000000e2,
0x000000e1,
0x000000e1,
0x000000e0,
0x000000e0,
0x000000df,
0x000000de,
0x000000de,
0x000000dd,
0x000000dd,
0x000000dc,
0x000000dc,
0x000000db,
0x000000db,
0x000000da,
0x000000da,
0x000000d9,
0x000000d8,
0x000000d8,
0x000000d7,
0x000000d7,
0x000000d6,
0x000000d6,
0x000000d5,
0x000000d5,
0x000000d4,
0x000000d4,
0x000000d3,
0x000000d2,
0x000000d2,
0x000000d1,
0x000000d1,
0x000000d0,
0x000000d0,
0x000000cf,
0x000000cf,
0x000000ce,
0x000000ce,
0x000000cd,
0x000000cc,
0x000000cc,
0x000000cb,
0x000000cb,
0x000000ca,
0x000000ca,
0x000000c9,
0x000000c9,
0x000000c8,
0x000000c8,
0x000000c7,
0x000000c7,
0x000000c7,
0x000000c6,
0x000000c6,
0x000000c6,
0x000000c5,
0x000000c5,
0x000000c5,
0x000000c5,
0x000000c4,
0x000000c4,
0x000000c4,
0x000000c3,
0x000000c3,
0x000000c3,
0x000000c3,
0x000000c2,
0x000000c2,
0x000000c2,
0x000000c1,
0x000000c1,
0x000000c1,
0x000000c0,
0x000000c0,
0x000000c0,
0x000000c0,
0x000000bf,
0x000000bf,
0x000000bf,
0x000000be,
0x000000be,
0x000000be,
0x000000be,
0x000000bd,
0x000000bd,
0x000000bd,
0x000000bc,
0x000000bc,
0x000000bc,
0x000000bb,
0x000000bb,
0x000000bb,
0x000000bb,
0x000000ba,
0x000000ba,
0x000000ba,
0x000000b9,
0x000000b9,
0x000000b9,
0x000000b9,
0x000000b8,
0x000000b8,
0x000000b8,
0x000000b7,
0x000000b7,
0x000000b7,
0x000000b7,
0x000000b6,
0x000000b6,
0x000000b6,
0x000000b5,
0x000000b5,
0x000000b5,
0x000000b4,
0x000000b4,
0x000000b4,
0x000000b4,
0x000000b3,
0x000000b3,
0x000000b3,
0x000000b2,
0x000000b2,
0x000000b2,
0x000000b2,
0x000000b1,
0x000000b1,
0x000000b1,
0x000000b0,
0x000000b0,
0x000000b0,
0x000000af,
0x000000af,
0x000000af,
0x000000af,
0x000000ae,
0x000000ae,
0x000000ae,
0x000000ad,
0x000000ad,
0x000000ad,
0x000000ad,
0x000000ac,
0x000000ac,
0x000000ac,
0x000000ab,
0x000000ab,
0x000000ab,
0x000000aa,
0x000000aa,
0x000000aa,
0x000000aa,
0x000000a9,
0x000000a9,
0x000000a9,
0x000000a8,
0x000000a8,
0x000000a8,
0x000000a8,
0x000000a7,
0x000000a7,
0x000000a7,
0x000000a6,
0x000000a6,
0x000000a6,
0x000000a6,
0x000000a5,
0x000000a5,
0x000000a5,
0x000000a4,
0x000000a4,
0x000000a4,
0x000000a3,
0x000000a3,
0x000000a3,
0x000000a3,
0x000000a2,
0x000000a2,
0x000000a2,
0x000000a1,
0x000000a1,
0x000000a1,
0x000000a1,
0x000000a0,
0x000000a0,
0x000000a0,
0x0000009f,
0x0000009f,
0x0000009f,
0x0000009e,
0x0000009e,
0x0000009e,
0x0000009e,
0x0000009d,
0x0000009d,
0x0000009d,
0x0000009c,
0x0000009c,
0x0000009c,
0x0000009c,
0x0000009b,
0x0000009b,
0x0000009b,
0x0000009a,
0x0000009a,
0x0000009a,
0x0000009a,
0x00000099,
0x00000099,
0x00000099,
0x00000098,
0x00000098,
0x00000098,
0x00000097,
0x00000097,
0x00000097,
0x00000097,
0x00000096,
0x00000096,
0x00000096,
0x00000095,
0x00000095,
0x00000095,
0x00000095,
0x00000094,
0x00000094,
0x00000094,
0x00000093,
0x00000093,
0x00000093,
0x00000092,
0x00000092,
0x00000092,
0x00000092,
0x00000091,
0x00000091,
0x00000091,
0x00000090,
0x00000090,
0x00000090,
0x00000090,
0x0000008f,
0x0000008f,
0x0000008f,
0x0000008e,
0x0000008e,
0x0000008e,
0x0000008d,
0x0000008d,
0x0000008d,
0x0000008d,
0x0000008c,
0x0000008c,
0x0000008c,
0x0000008b,
0x0000008b,
0x0000008b,
0x0000008b,
0x0000008a,
0x0000008a,
0x0000008a,
0x00000089,
0x00000089,
0x00000089,
0x00000089,
0x00000088,
0x00000088,
0x00000088,
0x00000087,
0x00000087,
0x00000087,
0x00000086,
0x00000086,
0x00000086,
0x00000086,
0x00000085,
0x00000085,
0x00000085,
0x00000084,
0x00000084,
0x00000084,
0x00000084,
0x00000083,
0x00000083,
0x00000083,
0x00000082,
0x00000082,
0x00000082,
0x00000081,
0x00000081,
0x00000081,
0x00000081,
0x00000080,
0x00000080,
0x00000080,
0x0000007f,
0x0000007f,
0x0000007f,
0x0000007f,
0x0000007e,
0x0000007e,
0x0000007e,
0x0000007d,
0x0000007d,
0x0000007d,
0x0000007d,
0x0000007c,
0x0000007c,
0x0000007c,
0x0000007c,
0x0000007c,
0x0000007c,
0x0000007b,
0x0000007b,
0x0000007b,
0x0000007b,
0x0000007b,
0x0000007b,
0x0000007a,
0x0000007a,
0x0000007a,
0x0000007a,
0x0000007a,
0x0000007a,
0x0000007a,
0x00000079,
0x00000079,
0x00000079,
0x00000079,
0x00000079,
0x00000079,
0x00000078,
0x00000078,
0x00000078,
0x00000078,
0x00000078,
0x00000078,
0x00000078,
0x00000077,
0x00000077,
0x00000077,
0x00000077,
0x00000077,
0x00000077,
0x00000076,
0x00000076,
0x00000076,
0x00000076,
0x00000076,
0x00000076,
0x00000075,
0x00000075,
0x00000075,
0x00000075,
0x00000075,
0x00000075,
0x00000075,
0x00000074,
0x00000074,
0x00000074,
0x00000074,
0x00000074,
0x00000074,
0x00000073,
0x00000073,
0x00000073,
0x00000073,
0x00000073,
0x00000073,
0x00000073,
0x00000072,
0x00000072,
0x00000072,
0x00000072,
0x00000072,
0x00000072,
0x00000071,
0x00000071,
0x00000071,
0x00000071,
0x00000071,
0x00000071,
0x00000070,
0x00000070,
0x00000070,
0x00000070,
0x00000070,
0x00000070,
0x00000070,
0x0000006f,
0x0000006f,
0x0000006f,
0x0000006f,
0x0000006f,
0x0000006f,
0x0000006e,
0x0000006e,
0x0000006e,
0x0000006e,
0x0000006e,
0x0000006e,
0x0000006e,
0x0000006d,
0x0000006d,
0x0000006d,
0x0000006d,
0x0000006d,
0x0000006d,
0x0000006c,
0x0000006c,
0x0000006c,
0x0000006c,
0x0000006c,
0x0000006c,
0x0000006b,
0x0000006b,
0x0000006b,
0x0000006b,
0x0000006b,
0x0000006b,
0x0000006b,
0x0000006a,
0x0000006a,
0x0000006a,
0x0000006a,
0x0000006a,
0x0000006a,
0x00000069,
0x00000069,
0x00000069,
0x00000069,
0x00000069,
0x00000069,
0x00000069,
0x00000068,
0x00000068,
0x00000068,
0x00000068,
0x00000068,
0x00000068,
0x00000067,
0x00000067,
0x00000067,
0x00000067,
0x00000067,
0x00000067,
0x00000066,
0x00000066,
0x00000066,
0x00000066,
0x00000066,
0x00000066,
0x00000066,
0x00000065,
0x00000065,
0x00000065,
0x00000065,
0x00000065,
0x00000065,
0x00000064,
0x00000064,
0x00000064,
0x00000064,
0x00000064,
0x00000064,
0x00000064,
0x00000063,
0x00000063,
0x00000063,
0x00000063,
0x00000063,
0x00000063,
0x00000062,
0x00000062,
0x00000062,
0x00000062,
0x00000062,
0x00000062,
0x00000061,
0x00000061,
0x00000061,
0x00000061,
0x00000061,
0x00000061,
0x00000061,
0x00000060,
0x00000060,
0x00000060,
0x00000060,
0x00000060,
0x00000060,
0x0000005f,
0x0000005f,
0x0000005f,
0x0000005f,
0x0000005f,
0x0000005f,
0x0000005f,
0x0000005e,
0x0000005e,
0x0000005e,
0x0000005e,
0x0000005e,
0x0000005e,
0x0000005d,
0x0000005d,
0x0000005d,
0x0000005d,
0x0000005d,
0x0000005d,
0x0000005c,
0x0000005c,
0x0000005c,
0x0000005c,
0x0000005c,
0x0000005c,
0x0000005c,
0x0000005b,
0x0000005b,
0x0000005b,
0x0000005b,
0x0000005b,
0x0000005b,
0x0000005a,
0x0000005a,
0x0000005a,
0x0000005a,
0x0000005a,
0x0000005a,
0x0000005a,
0x00000059,
0x00000059,
0x00000059,
0x00000059,
0x00000059,
0x00000059,
0x00000058,
0x00000058,
0x00000058,
0x00000058,
0x00000058,
0x00000058,
0x00000057,
0x00000057,
0x00000057,
0x00000057,
0x00000057,
0x00000057,
0x00000057,
0x00000056,
0x00000056,
0x00000056,
0x00000056,
0x00000056,
0x00000056,
0x00000055,
0x00000055,
0x00000055,
0x00000055,
0x00000055,
0x00000055,
0x00000055,
0x00000054,
0x00000054,
0x00000054,
0x00000054,
0x00000054,
0x00000053,
0x00000053,
0x00000053,
0x00000053,
0x00000053,
0x00000052,
0x00000052,
0x00000052,
0x00000052,
0x00000052,
0x00000051,
0x00000051,
0x00000051,
0x00000051,
0x00000051,
0x00000050,
0x00000050,
0x00000050,
0x00000050,
0x00000050,
0x0000004f,
0x0000004f,
0x0000004f,
0x0000004f,
0x0000004f,
0x0000004e,
0x0000004e,
0x0000004e,
0x0000004e,
0x0000004e,
0x0000004d,
0x0000004d,
0x0000004d,
0x0000004d,
0x0000004d,
0x0000004c,
0x0000004c,
0x0000004c,
0x0000004c,
0x0000004c,
0x0000004c,
0x0000004b,
0x0000004b,
0x0000004b,
0x0000004b,
0x0000004b,
0x0000004a,
0x0000004a,
0x0000004a,
0x0000004a,
0x0000004a,
0x00000049,
0x00000049,
0x00000049,
0x00000049,
0x00000049,
0x00000048,
0x00000048,
0x00000048,
0x00000048,
0x00000048,
0x00000047,
0x00000047,
0x00000047,
0x00000047,
0x00000047,
0x00000046,
0x00000046,
0x00000046,
0x00000046,
0x00000046,
0x00000045,
0x00000045,
0x00000045,
0x00000045,
0x00000045,
0x00000044,
0x00000044,
0x00000044,
0x00000044,
0x00000044,
0x00000044,
0x00000043,
0x00000043,
0x00000043,
0x00000043,
0x00000043,
0x00000042,
0x00000042,
0x00000042,
0x00000042,
0x00000042,
0x00000041,
0x00000041,
0x00000041,
0x00000041,
0x00000041,
0x00000040,
0x00000040,
0x00000040,
0x00000040,
0x00000040,
0x0000003f,
0x0000003f,
0x0000003f,
0x0000003f,
0x0000003f,
0x0000003e,
0x0000003e,
0x0000003e,
0x0000003e,
0x0000003e,
0x0000003d,
0x0000003d,
0x0000003d,
0x0000003d,
0x0000003d,
0x0000003c,
0x0000003c,
0x0000003c,
0x0000003c,
0x0000003c,
0x0000003c,
0x0000003b,
0x0000003b,
0x0000003b,
0x0000003a,
0x0000003a,
0x0000003a,
0x00000039,
0x00000039,
0x00000039,
0x00000038,
0x00000038,
0x00000038,
0x00000037,
0x00000037,
0x00000037,
0x00000037,
0x00000036,
0x00000036,
0x00000036,
0x00000035,
0x00000035,
0x00000035,
0x00000034,
0x00000034,
0x00000034,
0x00000033,
0x00000033,
0x00000033,
0x00000032,
0x00000032,
0x00000032,
0x00000032,
0x00000031,
0x00000031,
0x00000031,
0x00000030,
0x00000030,
0x00000030,
0x0000002f,
0x0000002f,
0x0000002f,
0x0000002e,
0x0000002e,
0x0000002e,
0x0000002d,
0x0000002d,
0x0000002d,
0x0000002d,
0x0000002c,
0x0000002c,
0x0000002c,
0x0000002b,
0x0000002b,
0x0000002b,
0x0000002a,
0x0000002a,
0x0000002a,
0x00000029,
0x00000029,
0x00000029,
0x00000028,
0x00000028,
0x00000028,
0x00000027,
0x00000027,
0x00000026,
0x00000025,
0x00000025,
0x00000024,
0x00000023,
0x00000023,
0x00000022,
0x00000022,
0x00000021,
0x00000020,
0x00000020,
0x0000001f,
0x0000001f,
0x0000001e,
0x0000001d,
0x0000001d,
0x0000001c,
0x0000001c,
0x0000001b,
0x0000001a,
0x0000001a,
0x00000019,
0x00000019,
0x00000018,
0x00000017,
0x00000017,
0x00000016,
0x00000015,
0x00000015,
0x00000014,
0x00000014,
0x00000013,
0x00000012,
0x00000012,
0x00000011,
0x00000011,
0x00000010,
0x0000000f,
0x0000000f,
0x0000000f,
0x0000000e,
0x0000000d,
0x0000000c,
0x0000000c,
0x0000000b,
0x0000000a,
0x0000000a,
0x00000009,
0x00000008,
0x00000007,
0x00000007,
0x00000006,
0x00000005,
0x00000005,
0x00000004,
0x00000003,
0x00000002,
0x00000002,
0x00000001,
0x00000000,
0x00000000,
0x00000000,
};

#endif
