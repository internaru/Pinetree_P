/******************************************************************************
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file gcpp_test.h
 *
 * \brief 
 *
 */

#ifndef GCPP_TEST_H
#define GCPP_TEST_H

#define SSL_CLIENT_TEST 1
#define SSL_SERVER_TEST 2

#ifdef __cplusplus
extern "C" {
#endif

int gcpp_test( char *jid, char *password );

#ifdef __cplusplus
}
#endif

#endif  // GCPP_TEST_H
