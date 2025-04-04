//
//  wavefront.h
//

#ifndef _wavefront_h_
#define _wavefront_h_
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>

/*
 You must complete this function (do so in wavefront.cpp).
 The rest is up to you. You may edit this file freely except for changing this function's signature.
 The filename is the name of the file containing the input map.
 startrow, startcol is the starting location.
 goalrow, goalcol is the target location.
 Print the solution map to the ostream output
 and return the distance of the path.
 */

int wavefront(std::string filename,int startrow,int startcol,int goalrow,int goalcol,std::ostream&output);


#endif /* _wavefront_h_ */
