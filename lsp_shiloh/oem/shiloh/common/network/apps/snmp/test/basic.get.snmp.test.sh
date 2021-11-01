#!/bin/sh
# ============================================================================
#
# Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================
#
#  ./basic.get.snmp.test.sh -- demonstrate a sample of Printer-MIB retrievals
#

test -z "${PRJROOT}" && echo "$0: PRJROOT for mib definitions?" && exit

export MIB=ALL
export MIBDIRS=$MIBDIRS:${PRJROOT}/sysapps/snmp/tgt-cache/usr/local/share/snmp/mibs

usage()
{
    echo "$0 <ip>"
    exit
}

TGT=$1 # TGT="<your printer ip here>

test -z "$TGT" && usage

OPTIONS="-v2c -cpublic"
MIB=Printer-MIB
OP=snmpget
OP=snmpgetnext

# the following list of snmp oids generates a sample of successful (and tbd) retrievals
oids_basic="
prtInputMediaName
prtGeneralCurrentOperator
prtInputDefaultIndex
prtGeneralPrinterName
prtGeneralSerialNumber
prtMediaPathMaxSpeedPrintUnit
prtInterpreterDefaultOrientation
prtInputTable
"

oids_interpreter="
prtInterpreterTable
prtInterpreterTableIndex
prtInterpreterDefaultOrientation
"

oids_media="
prtMediaPathTable
prtMediaPathIndex
prtMediaPathMaxSpeedPrintUnit
"

oids_markers="
prtMarkerDefaultIndex
prtMarkerSuppliesMarkerIndex
prtMarkerSuppliesType
prtMarkerSuppliesDescription
prtMarkerSuppliesSupplyUnit
prtMarkerSuppliesMaxCapacity
prtMarkerSuppliesLevel
"

oids_colorant="
prtMarkerColorantTable
prtMarkerColorantIndex
prtMarkerColorantMarkerIndex
prtMarkerColorantRole
prtMarkerColorantValue
prtMarkerColorantTonality
"

# in progress
oids_err="
prtInputDefaultIndex
"

oids_tbd="
prtGeneralCurrentOperator
prtInputIndex
prtInputMaxCapacity
"

# 1st general walk for good measure
snmpwalk $OPTIONS $TGT 2>&1 | grep -v nnot | grep -v "MIB search"

# aggregate oids to retrieve:
oids="$oids_basic $oids_markers $oids_colorant"

# active testing goes here
oid="<select oid>"
## for OP in snmpget snmpgetnext; do
    echo "$OP $OPTIONS $TGT $MIB::$oid"
    for oid in $oids; do
        $OP $OPTIONS $TGT $MIB::$oid 2>&1 | grep -v nnot | grep -v "MIB search"
    done
    echo ""
## done

set_too() {
OP=snmpset
VALUE=4

    index=3
    echo "$OP $OPTIONS $TGT $MIB::$oid.1.$index = $VALUE"
    for oid in $oids; do
      for index in 0 1 2 3 4 5 6; do
        echo "$OP $OPTIONS $TGT $MIB::$oid.1.$index = $VALUE"
        $OP $OPTIONS $TGT $MIB::$oid.1.$index $VALUE 2>&1 | grep -v nnot | grep -v "MIB search"
      done
    done
}

## eof
