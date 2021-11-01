
proc _bt_fmt { level } {
  set cmdstr [info level [expr {$level}]]
  if {[lindex $cmdstr 0] == "unknown"} { set cmdstr [lrange $cmdstr 1 end] }
  set cmd [string trim "[lindex $cmdstr 0] [lrange $cmdstr 1 end]"]
  return "\[$cmd\]"
}

proc backtrace {} {
#HELP Emit a backtrace of the current execution stack for debugging.
  set l [expr {[info level]-1}]

  set btstr ""
  for {set l [expr {[info level]-1}]} {$l} {} {
    append btstr "$l: [_bt_fmt $l]"
    incr l -1
    if {$l} {
      append btstr " called by [_bt_fmt $l]\n"
    } else {
      append btstr " called by <<top level>>"
    }
  }
  return $btstr
}

proc unknown { args } {
#HELP Used internally for catching invalid commands.
#HELP It is not intended that this procedure would be called directly by any
#HELP users or scripts. It is a 'special' TCL procedure and should be ignored
#HELP for all intents and purposes.
  error "Invalid Command \"$args\"\n[backtrace]\n"
}

proc _procshow { procname { indent "" } { showhidden 1 } } {
  set args ""
  foreach arg [info args $procname] {
    if [info default $procname $arg def] {
      lappend args "$arg \"$def\""
    } else {
      lappend args "$arg"
    }
  }
  puts $indent "proc $procname { $args } {"
  set x [info body $procname]
  foreach line [split $x "\n"] {
    if {!$showhidden} {
      if [regexp {^[ \t]*#HELP (.*)$} "$line"] continue
    }
    puts $indent "$line"
  }
  puts $indent "}"
}

proc _usage { procname } {
  set r ""
  foreach arg [info args $procname] {
    if [info default $procname $arg def] {
      if [expr {"" == "$def"}] {
        set r [concat $r "\[<$arg>\]"]
      } else {
        set r [concat $r "\[<$arg> = $def\]"]
      }
    } else {
      set r [concat $r "<$arg>"]
    }
  }
  return $r
}

proc _prochelpstr { procname } {
  if { [info procs $procname] == "$procname"} {
    set x [info body $procname]
    set helplines {}
    foreach line [split $x "\n"] {
      if {[regexp {^[ \t]*#HELP (.*)$} "$line" match comment]} {
          lappend helplines "$comment"
      }
    }
    return $helplines
  } else {
    error "I don't think $procname is a procedure"
  }
}

proc _prochelp { procname {verbose 0} {cols 80} } {
  if { [info procs $procname] == "$procname"} {
    set helplines [_prochelpstr $procname]
    puts "$procname [_usage $procname] -- [lindex $helplines 0]"
    if {$verbose > 0} {
      set x ""
      foreach l $helplines { set x "$x[concat $l] " }
      flowtext $cols "  " $x
    }
    if {$verbose == 2} {
      _procshow $procname "  :  " 0
    }
    return "" 
  } else {
    error "I don't think $procname is a procedure"
  }
}

proc prochelp { {srch ""} {verbose 0} {cols 80} } {
#HELP Display all procedures or search for specific procedures.
#HELP If executed with no srch string then display all of the procedures
#HELP known to the system. If a srch string is provided then all matching
#HELP procedures will be displayed. Verbose may be specified as non-zero to
#HELP request that the notes, if any, for each procedure be shown as well.
#HELP If verbose is 2 then the body of the procedure is additionally shown.
#HELP The notes may be flowed to fit a specified number of columns.

  # Display help for all of the procedures that match the search string
  set srch [string tolower $srch]
  foreach p [lsort [info procs]] {
    if ![regexp {^_} $p] {
      if [expr {("$srch" == "") || [regexp -nocase $srch $p]}] {
        _prochelp $p $verbose $cols
      }
    }
  }
}

proc show { _a_Thing_ } {
#HELP Dump a Tcl object, such as the entire definition of a procedure.
#HELP Will give you as much information about its parameter as possible.
  if       {[info procs $_a_Thing_] == "$_a_Thing_"} {
    _procshow $_a_Thing_
  } elseif {[info commands $_a_Thing_]  == "$_a_Thing_"} {
    puts "$_a_Thing_ is a command (try 'help $_a_Thing_')"
  } else {
    global $_a_Thing_
    if [catch {puts "$_a_Thing_ == [expr $$_a_Thing_]"}] {
      puts "I don't know what $_a_Thing_ is"
    }
  }
}

proc grep { srch stuff } {
#HELP Search through "stuff" for the string "srch"
#HELP All matching lines will be output as results
  set res ""
  foreach line [split $stuff "\n"] {
    if ![regexp {^_} $line] {
      if [expr {("$srch" == "") || [regexp $srch $line]}] {
        append res "$line\n"
      }
    }
  }
  return $res
}
