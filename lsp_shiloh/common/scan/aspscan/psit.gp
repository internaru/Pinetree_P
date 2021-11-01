# write current graph to a postscript file
set term push 
set term postscript color enhanced
#set term postscript 
set output "$0"
replot 
unset output 
set term pop

