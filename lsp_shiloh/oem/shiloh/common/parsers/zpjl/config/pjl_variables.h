/*
 *
 * ============================================================================
 * Copyright (c) 2008-2012  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef PJL_VARIABLES_H
#define PJL_VARIABLES_H


#ifdef __cplusplus
extern "C" {
#endif

/// Add new PJL Variables of integer or enum type here.
/// name must match PJL keyword exactly.
typedef struct pji_ints_s {
    pji_int_t JAMRECOVERY;//0
    pji_int_t TIMEOUT;	//1
    pji_int_t PAGECOUNT;	//2
    pji_int_t COPIES;		//3
    pji_int_t QTY;		//4
    pji_int_t DUPLEX;		//5
    pji_int_t BINDING;	//6
    pji_int_t PAPER; 		//7
    pji_int_t MEDIATYPE;	//8	/// end of common.
    pji_int_t RESOLUTION;//9 /// 600 -> 600x1bit or 1200 -> 600x2bit
    pji_int_t ECONOMODE;	//10	/// Toner save
    pji_int_t TONERDARKNESS;//11
    pji_int_t INTRAY1SIZE;//12		/// USB Status Monitor		2013.12.17	by Juny
    pji_int_t INTRAY2SIZE;//13
    pji_int_t INTRAY3SIZE;//14
    pji_int_t MPTRAYSIZE;//15
    pji_int_t DRUM;		//16
    pji_int_t TONER;		//17
    pji_int_t DUPLEXER;	//18
    pji_int_t INTRAY1;	//19
    pji_int_t INTRAY2;	//20
    pji_int_t INTRAY3;	//21
    pji_int_t MPTRAY;		//22
    pji_int_t MEDIASOURCE;		//22
/*
    pji_int_t MEDIATYPEINTRAY1;
    pji_int_t MEDIATYPEINTRAY2;
    pji_int_t MEDIATYPEINTRAY3;
    pji_int_t MEDIATYPEMPTRAY;
*/

} pji_ints_t;

/// Add new PJL Variables of char[40] string type here.
/// name must match PJL keyword exactly.
typedef struct pji_strs_s {
	pji_str_t JOBNAME; 	//23	/// end of common.
	pji_str_t AUTHENTICATIONUSERID;		//24
	pji_str_t AUTHENTICATIONPASSWORD;	//25
	pji_str_t LJOBINFO;		//26
	pji_str_t SINDOHUSERNAME;	//27
} pji_strs_t;


#define i_elements (sizeof(pji_ints_t)/sizeof(pji_int_t))
#define s_elements (sizeof(pji_strs_t)/sizeof(pji_str_t))
extern const int num_entries_pjl_local_variable_table;

typedef struct pji_local_s {
	bool frozen;
	union { 
		pji_int_t  by_index[i_elements];
		pji_ints_t by_name;
	} i;
	union { 
		pji_str_t  by_index[s_elements];
		pji_strs_t by_name;
	} s;
} pji_local_t;


#ifdef __cplusplus
}
#endif
#endif // PJL_VARIABLES_H
