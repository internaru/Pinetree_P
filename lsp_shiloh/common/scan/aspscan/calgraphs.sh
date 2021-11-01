#!/bin/sh

# Run through all supported cal graphs, generating prints.
# davep 15-Oct-2007

ALLDPI="300 600"

# 1==mono with graph
# 3==color with graph
ALLTYPES="1 3"

#DBG=echo

PID=0x5817
MOREOPTS=

for dpi in ${ALLDPI} ; do
    for graphtype in ${ALLTYPES} ; do
        ${DBG} acltester -p ${PID} ${MOREOPTS} -c -r ${dpi} -t $graphtype

        if [ $? -ne 0 ] ; then
            exit -1
        fi
        # wait for the graph to finish
        while [ 1 ] ; do 
            ${DBG} acltester -p ${PID} ${MOREOPTS} -g 
            exitcode=$?
            echo $exitcode
            if [ $exitcode -ne 0 ] ; then
                echo "waiting for calgraph to finish..."
                sleep 5 
            else 
                break
            fi
        done
    done
done

