# eufx
_/yo͞o fəks/_

## Install and build
1. `git clone https://github.com/joshnatis/eufx.git`
2. `make`

## Usage
`./eufx --filter <filter_type> input.pgm output.pgm`

* Sample images to operate on are in the `images/` directory

* The PGM files must be uncompressed (_eufx: yeah! i can't parse bullshidt_ ):<
  * To get an uncompressed PGM file, scour the interwebz for one, or use some sort of utility to convert images to PGM format.
  * Most of them compress the images to shit, so they won't work.
  * Use ImageMagick's `convert` utility: `convert -compress none input.pgm output.pgm`
  
_eufx: put me in your PATH! let's be friends :D_
  
## Available Filters
* reflect
* rotate
* asciify
* scale_down
* noise
* posterize
* nothing
* invert
* frame

The filters are coming, I assure you.

## Background Information
The PGM (Portable Graymap) format is an image format (like JPG or PNG). PGM formatted images only support black-and-white (there's no way to encode color), and they're not compressed at all, which makes them very easy to parse and work with (not to mention _thicc_). A PGM image is essentially a matrix of pixels, each of which being a value representing how light/dark it is. The images begin with some header information, noteably the specifications of the height and width, as well as the maximum gray value (i.e. what value constitutes as white). This last point means you can have an arbitrary number of shades of gray in your image, which is pretty cool (_well, actually the largest allowed maximum gray value is 65536_).

A nice, succint specification of PGM images lives [here](http://netpbm.sourceforge.net/doc/pgm.html).
