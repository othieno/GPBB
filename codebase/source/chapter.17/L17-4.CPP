/* next_generation(), implemented using fast, all-in-one hard-wired
   neighbor count/update/draw function. Otherwise, the same as
	Listing 17-3. */

/* Calculates the next generation of current_map and stores it in
   next_map. */
void cellmap::next_generation(cellmap& next_map)
{
   unsigned int x, y, neighbor_count;
   unsigned int width_in_bytesX2 = width_in_bytes << 1;
   unsigned char *cell_ptr, *current_cell_ptr, mask, current_mask;
   unsigned char *base_cell_ptr, *row_cell_ptr, base_mask;
   unsigned char *dest_cell_ptr = next_map.cells;

   // Process all cells in the current cellmap
   row_cell_ptr = cells;      // point to upper left neighbor of
                              // first cell in cell map
   for (y=0; y<height; y++) { // repeat for each row of cells
      // Cell pointer and cell bit mask for first cell in row
      base_cell_ptr = row_cell_ptr; // to access upper left neighbor
      base_mask = 0x01;             // of first cell in row
      for (x=0; x<width; x++) {  // repeat for each cell in row
         // First, count neighbors
         // Point to upper left neighbor of current cell
         cell_ptr = base_cell_ptr;  // pointer and bit mask for
         mask = base_mask;          // upper left neighbor
         // Count upper left neighbor
         neighbor_count = (*cell_ptr & mask) ? 1 : 0;
         // Count left neighbor
         if ((*(cell_ptr + width_in_bytes) & mask)) neighbor_count++;
         // Count lower left neighbor
         if ((*(cell_ptr + width_in_bytesX2) & mask))
               neighbor_count++;
         // Point to upper neighbor
         if ((mask >>= 1) == 0) {
            mask = 0x80;
            cell_ptr++;
         }
         // Remember where to find the current cell
         current_cell_ptr = cell_ptr + width_in_bytes;
         current_mask = mask;
         // Count upper neighbor
         if ((*cell_ptr & mask)) neighbor_count++;
         // Count lower neighbor
         if ((*(cell_ptr + width_in_bytesX2) & mask))
               neighbor_count++;
         // Point to upper right neighbor
         if ((mask >>= 1) == 0) {
            mask = 0x80;
            cell_ptr++;
         }
         // Count upper right neighbor
         if ((*cell_ptr & mask)) neighbor_count++;
         // Count right neighbor
         if ((*(cell_ptr + width_in_bytes) & mask)) neighbor_count++;
         // Count lower right neighbor
         if ((*(cell_ptr + width_in_bytesX2) & mask))
               neighbor_count++;
         if (*current_cell_ptr & current_mask) {
            if ((neighbor_count != 2) && (neighbor_count != 3)) {
               *(dest_cell_ptr + (current_cell_ptr - cells)) &=
                     ~current_mask;    // turn off cell
               draw_pixel(x, y, OFF_COLOR);
            }
         } else {
            if (neighbor_count == 3) {
               *(dest_cell_ptr + (current_cell_ptr - cells)) |=
                     current_mask;     // turn on cell
               draw_pixel(x, y, ON_COLOR);
            }
         }
         // Advance to the next cell on row
         if ((base_mask >>= 1) == 0) {
            base_mask = 0x80;
            base_cell_ptr++;  // advance to the next cell byte
         }
      }
      row_cell_ptr += width_in_bytes;  // point to start of next row
   }
}

