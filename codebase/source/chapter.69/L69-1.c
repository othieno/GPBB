// Quake's recursive subdivision triangle rasterizer; draws all 
// pixels in a triangle other than the vertices by splitting an 
// edge to form a new vertex, drawing the vertex, and recursively
// processing each of the two new triangles formed by using the
// new vertex. Results are less accurate than from a precise
// affine or perspective texture mapper, and drawing boundaries 
// are not identical to those of a precise polygon drawer, although
// they are consistent between adjacent polygons drawn with this
// technique.
//
// Invented and implemented by John Carmack of id Software.

void D_PolysetRecursiveTriangle (int *lp1, int *lp2, int *lp3)
{
	int    *temp;
	int    d;
	int    new[6];
	int    z;
	short  *zbuf;

// try to find an edge that's more than one pixel long in x or y
	d = lp2[0] - lp1[0];
	if (d < -1 || d > 1)
		goto split;
	d = lp2[1] - lp1[1];
	if (d < -1 || d > 1)
		goto split;
	d = lp3[0] - lp2[0];
	if (d < -1 || d > 1)
		goto split2;
	d = lp3[1] - lp2[1];
	if (d < -1 || d > 1)
		goto split2;
	d = lp1[0] - lp3[0];
	if (d < -1 || d > 1)
		goto split3;
	d = lp1[1] - lp3[1];
	if (d < -1 || d > 1)
	{
split3:
	// shuffle points so first edge is edge to split
		temp = lp1;
		lp1 = lp3;
		lp3 = lp2;
		lp2 = temp;
		goto split;
	}

	return;         // no pixels left to fill in triangle

split2:
// shuffle points so first edge is edge to split
	temp = lp1;
	lp1 = lp2;
	lp2 = lp3;
	lp3 = temp;

split:
// split first edge screen x, screen y, texture s, texture t, and z
// to form a new vertex.  Lighting (index 4) is ignored; the 
// difference between interpolating lighting and using the same 
// shading for the entire triangle is unnoticeable for small 
// triangles, so we just use the lighting for the first vertex of 
// the original triangle (which was used during set-up to set 
// d_colormap, used below to look up lit texels)
	new[0] = (lp1[0] + lp2[0]) >> 1;        // split screen x
	new[1] = (lp1[1] + lp2[1]) >> 1;        // split screen y
	new[2] = (lp1[2] + lp2[2]) >> 1;        // split texture s
	new[3] = (lp1[3] + lp2[3]) >> 1;        // split texture t
	new[5] = (lp1[5] + lp2[5]) >> 1;        // split z

// draw the point if splitting a leading edge
	if (lp2[1] > lp1[1])
		goto nodraw;
	if ((lp2[1] == lp1[1]) && (lp2[0] < lp1[0]))
		goto nodraw;


	z = new[5]>>16;

// point to the pixel's z-buffer entry, looking up the scanline start 
// address based on screen y and adding in the screen x coordinate
	zbuf = zspantable[new[1]] + new[0];

// draw the split vertex if it's not obscured by something nearer, as 
// indicated by the z-buffer
	if (z >= *zbuf)
	{
		int     pix;
		
	// set the z-buffer to the new pixel's distance
		*zbuf = z;

	// get the texel from the model's skin bitmap, according to 
	// the s and t texture coordinates, and translate it through 
	// the lighting look-up table set according to the first 
	// vertex for the original (top-level) triangle.  Both s and
	// t are in 16.16 format
		pix = d_pcolormap[skintable[new[3]>>16][new[2]>>16]];

	// draw the pixel, looking up the scanline start address 
	// based on screen y and adding in the screen x coordinate
		d_viewbuffer[d_scantable[new[1]] + new[0]] = pix;
	}

nodraw:
// recursively draw the two new triangles we created by adding the 
// split vertex
	D_PolysetRecursiveTriangle (lp3, lp1, new);
	D_PolysetRecursiveTriangle (lp3, new, lp2);
}
