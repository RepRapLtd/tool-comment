# tool-comment
G Code post-processor

A little program to comment out calls to Tn except for a band of Z values in a G-Code file.

This assumes that the file has a certain structure:

   1. Tool change lines look like: "Tn..." with the T as the first character,
   2. Z increment lines look like: "G1 Zz..." with the G as the first character, and
   3. The Z values increase monotonically.

Slicers like Cura have a very useful Wipe & Ooze tower facility for 3D prints using
more than one material.  But often you know that the second or minor material starts at a certain 
height and is all finished-with by a higher height.  Normally when the tower gets built the material
in it swaps every other layer, which wastes lots of tool-change time in layers where the 
minor material isn't being used.  This program comments out tool changes of the form "Tn"
except between a lower and higher range of Z values.

On Linux systems compile with:

  g++ -o tool-comment tool-comment.cpp

and use with:

  ./tool-comment -i input-file -o output-file -zl z-low-height -zh z-high-height -t tool-number-to-comment-out

You probably want to set the range of heights rather wider than the actual heights of the minor material.
That way the tool will get a bit of a warm-up before it is actually needed, and won't dissappear a layer 
too early because of floating-point accuracy.

To start from the ground up, just set the low height negative.

Adrian Bowyer
http://reprapltd.com
March 2016
