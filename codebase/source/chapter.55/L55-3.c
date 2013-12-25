/* Converts a model color (a color in the RGB color cube, in the current
   color model) to a color index for mode X. Pure primary colors are
   special-cased, and everything else is handled by a 2-2-2 model. */
int ModelColorToColorIndex(ModelColor * Color)
{
   if (Color->Red == 0) {
      if (Color->Green == 0) {
         /* Pure blue */
         return(192+(Color->Blue >> 2));
      } else if (Color->Blue == 0) {
         /* Pure green */
         return(128+(Color->Green >> 2));
      }
   } else if ((Color->Green == 0) && (Color->Blue == 0)) {
      /* Pure red */
      return(64+(Color->Red >> 2));
   }
   /* Multi-color mix; look up the index with the two most significant bits
      of each color component */
   return(((Color->Red & 0xC0) >> 2) | ((Color->Green & 0xC0) >> 4) |
         ((Color->Blue & 0xC0) >> 6));
}

