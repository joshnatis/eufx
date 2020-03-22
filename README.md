# eufx
_/yo͞o,yə fəks/_

## Install and build
1. git clone https://github.com/joshnatis/eufx.git
2. make

## Usage
`./eufx --filter <filter_type> input.pgm output.pgm`

* Sample images to operate on are in the `images/` directory

* The PGM files must be uncompressed (_eufx: yeah! i can't parse bullshidt_ ):<
  * To get an uncompressed PGM file, scour the interwebz for one, or use some sort of utility to convert images to PGM format.
  * Most of them compress the images to shit, so they won't work.
  * Use ImageMagick's `convert` utility: `convert -compress none input.pgm output.pgm`
