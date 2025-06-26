##### basic of tcl for vmd  ####
################################
set x 10
puts "the value of x is:$x"
set text "some text"
puts "the value of text is:$text."

##################################
expr 3 - 8
set x 10
expr - 3 * $x

##################################
set result [ expr -3 * $x ]
puts $result
#################################
### writing file in tcl
set file [open "myoutput.dat" w]
for {set x 0} {$x <= 10} {incr x} {
puts $file [ expr -3 * $x ]
}
close $file]

#################################
###select atom or group  in vmd
set sel [atomselect top "hydrophobic"]

##################################
###move crystal or rotate using vmd
$crystal moveby {10 0 0}
$crystal move [transaxis x 40 deg]

###################################
### Get multiple property using vmd
$sel get resid
$sel get {resname resid}
$sel get {x y z}

####################################
## some measure properties

measure center $sel
measure minmax $sel

####################################
## Delete selection in vmd

$sel delete

#####################################
