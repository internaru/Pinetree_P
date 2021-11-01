/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \brief 
 *
 */

#ifdef __cplusplus
extern "C" {
#endif


int ReturnNumVarMin(uint16_t );
int ReturnNumVarMax(uint16_t );


char *get_var_as_str(uint16_t var, PJL_VARIABLE_STORAGE VarStorage);
char *get_var_as_str_Sindoh(uint16_t var, PJL_VARIABLE_STORAGE VarStorage);		// USB Status Monitor
uint32_t num_new_grammar(void);
uint16_t get_grammar_key(uint32_t count);
void run_grammar_program(uint32_t count, char *InputString);
uint32_t call_grammar_function(uint32_t count);

uint32_t GetNumVars();
uint16_t GetVarKey(uint32_t Index);
char *GetVarEnum(uint16_t Key, uint32_t index);
char *GetVarName(uint32_t Index);

#ifdef __cplusplus
}
#endif
