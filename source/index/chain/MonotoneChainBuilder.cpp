/**********************************************************************
 * $Id$
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.refractions.net
 *
 * Copyright (C) 2001-2002 Vivid Solutions Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation. 
 * See the COPYING file for more information.
 *
 **********************************************************************/

#include <geos/indexChain.h>
#include <geos/geomgraph.h>
#include <stdio.h>

#define DEBUG 0

namespace geos {

vector<indexMonotoneChain*>*
MonotoneChainBuilder::getChains(CoordinateSequence *pts)
{
	return getChains(pts,NULL);
}

/**
 * Return a list of the MonotoneChain
 * for the given list of coordinates.
 */
vector<indexMonotoneChain*>*
MonotoneChainBuilder::getChains(CoordinateSequence *pts, void* context)
{
	vector<indexMonotoneChain*> *mcList=new vector<indexMonotoneChain*>();
	vector<int> *startIndex=getChainStartIndices(pts);
	unsigned int n=startIndex->size()-1;
	for(unsigned int i=0; i<n; i++)
	{
		indexMonotoneChain *mc=new indexMonotoneChain(pts,
			(*startIndex)[i], (*startIndex)[i+1], context);
		mcList->push_back(mc);
	}
	delete startIndex;
	return mcList;
}

/**
 * Return an array containing lists of start/end indexes of the monotone chains
 * for the given list of coordinates.
 * The last entry in the array points to the end point of the point array,
 * for use as a sentinel.
 */
vector<int>*
MonotoneChainBuilder::getChainStartIndices(CoordinateSequence *pts)
{
	// find the startpoint (and endpoints) of all monotone chains
	// in this edge
	int start=0;
	vector<int> *startIndexList=new vector<int>();
	startIndexList->push_back(start);
	unsigned int n=pts->getSize()-1;
	do {
		int last=findChainEnd(pts, start);
		startIndexList->push_back(last);
		start=last;
	} while((unsigned int)start<n);

	// copy list to an array of ints, for efficiency
	//int[] startIndex = toIntArray(startIndexList);
	return startIndexList;
}

/**
 * @return the index of the last point in the monotone chain starting
 * at <code>start</code>.
 */
int
MonotoneChainBuilder::findChainEnd(CoordinateSequence *pts, int start)
{
	// determine quadrant for chain
	int chainQuad=Quadrant::quadrant(pts->getAt(start),pts->getAt(start + 1));
	int last=start+1;
	unsigned int npts=pts->getSize();
	while ((unsigned int)last<npts)
	{
		// compute quadrant for next possible segment in chain
		int quad=Quadrant::quadrant(pts->getAt(last-1),pts->getAt(last));
		if (quad!=chainQuad) break;
		last++;
	}
#if DEBUG
	cerr<<"MonotoneChainBuilder::findChainEnd() returning"<<endl;
#endif
	return last-1;
}

} // namespace geos

/**********************************************************************
 * $Log$
 * Revision 1.14  2005/02/15 17:15:13  strk
 * Inlined most Envelope methods, reserved() memory for some vectors when
 * the usage was known a priori.
 *
 * Revision 1.13  2005/02/05 05:44:47  strk
 * Changed geomgraph nodeMap to use Coordinate pointers as keys, reduces
 * lots of other Coordinate copies.
 *
 * Revision 1.12  2004/12/08 13:54:43  strk
 * gcc warnings checked and fixed, general cleanups.
 *
 * Revision 1.11  2004/11/01 16:43:04  strk
 * Added Profiler code.
 * Temporarly patched a bug in DoubleBits (must check drawbacks).
 * Various cleanups and speedups.
 *
 * Revision 1.10  2004/07/08 19:34:49  strk
 * Mirrored JTS interface of CoordinateSequence, factory and
 * default implementations.
 * Added DefaultCoordinateSequenceFactory::instance() function.
 *
 * Revision 1.9  2004/07/02 13:28:27  strk
 * Fixed all #include lines to reflect headers layout change.
 * Added client application build tips in README.
 *
 * Revision 1.8  2004/03/25 02:23:55  ybychkov
 * All "index/" packages upgraded to JTS 1.4
 *
 * Revision 1.7  2004/03/19 09:48:46  ybychkov
 * "geomgraph" and "geomgraph/indexl" upgraded to JTS 1.4
 *
 * Revision 1.6  2003/11/07 01:23:42  pramsey
 * Add standard CVS headers licence notices and copyrights to all cpp and h
 * files.
 *
 *
 **********************************************************************/

