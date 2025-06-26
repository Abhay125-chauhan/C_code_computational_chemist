###Open the script file distance.tcl with a text editor. You can see that the script does the following:
Choose atom selections
set sel1 [atomselect top "$seltext1"]
set sel2 [atomselect top "$seltext2"]

####Get the number of frames in the trajectory and assign this value to the variable nf
set nf [molinfo top get numframes]

#####Open file specified by the variable f_r_out
set outfile [open $f_r_out w]

########Loop over all frames
for {set i 0} {$i < $nf} {incr i} {

#########Write out the frame number and update the selections to the current frame
puts "frame $i of $nf"
$sel1 frame $i
$sel2 frame $i

######333#Find the center of mass for each selection (com1 and com2 are position vectors)
set com1 [measure center $sel1 weight mass]
set com2 [measure center $sel2 weight mass]

###############At each frame i, find the distance by subtracting one vector from the other (command vecsub) and computing the length of the resulting vector (command veclength), assign that value to an array element simdata($i.r), and print a frame-distance entry to a file
set simdata($i.r) [veclength [vecsub $com1 $com2]]
puts $outfile "$i $simdata($i.r)"
}

Close the file
close $outfile

########The second part of the script is for obtaining the distance distribution. It starts from finding the maximum and minimum values of the distance
set r_min $simdata(0.r)
set r_max $simdata(0.r)
for {set i 0} {$i < $nf} {incr i} {
set r_tmp $simdata($i.r)
if {$r_tmp < $r_min} {set r_min $r_tmp}
if {$r_tmp > $r_max} {set r_max $r_tmp}
}

###########The step over the range of distances is chosen based on the number of bins N_d defined in the beginning, and all values for the elements of the distribution array are set to zero
set dr [expr ($r_max - $r_min) /($N_d - 1)]
for {set k 0} {$k < $N_d} {incr k} {
set distribution($k) 0
}

#########The distribution is obtained by adding 1 (incr ...) to an array element every time the distance is within the respective bin
for {set i 0} {$i < $nf} {incr i} {
set k [expr int(($simdata($i.r) - $r_min) / $dr)]
incr distribution($k)
}

##########3Write out the file with the distribution
set outfile [open $f_d_out w]
for {set k 0} {$k < $N_d} {incr k} {
puts $outfile "[expr $r_min + $k * $dr] $distribution($k)"
}
close $outfile
