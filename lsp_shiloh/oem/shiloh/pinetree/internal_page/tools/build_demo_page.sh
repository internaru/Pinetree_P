#!/bin/sh
#/******************************************************************************
# * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
# *
# *                         Marvell Confidential
# ******************************************************************************/

input_file="../../../../../../tools/test_docs/smoke_norm.prn"
output_file="../config/demo_page_data.h"

echo "/******************************************************************************" > ${output_file}
echo "* Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved" >> ${output_file}
echo "*" >> ${output_file}
echo "*                         Marvell Confidential" >> ${output_file}
echo "******************************************************************************/" >> ${output_file}
echo "">> ${output_file}
cp ${input_file} demo_page.prn
xxd -i demo_page.prn >> ${output_file}
rm -f demo_page.prn
echo "" >> ${output_file}

