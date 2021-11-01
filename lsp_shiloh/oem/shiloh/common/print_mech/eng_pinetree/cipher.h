/******************** (C) COPYRIGHT 2009 PLATO CO., LTD. ***********************
* Project			 : Smart IC test project
* Customer			 : SindoH
* Device			 : STM32F10x
* Compiler			 :
* File Name          : main.h
* Author             : 1 team, BR GROUP, PLATO CO., LTD.
*
*
* Description        :
*
* Revision history -------------------------------------------------------------
Version 		Issued date					Changes
--------------------------------------------------------------------------------
preliminary     2009-09-01

*******************************************************************************/
#ifndef	_CIPHER_H_
#define	_CIPHER_H_

void EncMachine1(unsigned char *Key, unsigned char *EncData);
void DecMachine1(unsigned char *Key, unsigned char *EncData);
void EncMachine2(unsigned char *Key, unsigned char *Challange);




#endif

