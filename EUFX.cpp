#include <iostream>
#include <fstream>
#include <vector>
#include <time.h> //noise filter


//check if element is contained in an iterable container (used to check validity of filters)
#define contains(v, x) std::find(v.begin(), v.end(), x) != v.end()


const int MAX_WIDTH = 1024;
const int MAX_HEIGHT = 1024;
int MAX_GRAY;


//fills image matrix with values, sets height and width in callee to height/width values
void read_image(int img[MAX_HEIGHT][MAX_WIDTH], int &height, int &width, std::ifstream &fin);

//standard image algorithm, applies func to each pixel and writes
void apply_filter(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout, int(*func)(int));

//nonstandard algorithms (don't simply apply filter to each pixel)
void rotate(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout);
void reflect(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout);
void scale_down(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, int scale, std::ofstream &fout);
void asciify(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, const std::string &fn);
void frame(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout);

//image algorithm helpers (per pixel)
int noise(int pixel) { return (1 + rand() % MAX_GRAY); }
int invert(int pixel) { return MAX_GRAY - pixel; }
int nothing(int pixel) { return pixel; }
int posterize(int pixel);
char asciify(int pixel);

//housekeeping helpers
void list_filters(const std::vector<std::string> &filters);
void handle_errors(int argc, char **argv, const std::vector<std::string> &FILTERS);


/*
======================================
======================================

				MAIN

=====================================
=====================================
*/

int main(int argc, char **argv)
{
	const std::vector<std::string> FILTERS = 
	{"reflect", "rotate", "asciify", "scale_down", "noise", "posterize", "nothing", "invert", "frame"};

	handle_errors(argc, argv, FILTERS);

	std::string input_img = argv[3];
	std::ifstream fin;
	fin.open(input_img);
	if (fin.fail()) 
	{
		std::cout << "Unable to read file\n";
		exit(1);
	}

	std::string output_img = argv[4];
	std::ofstream fout;
	fout.open(output_img);

	int img[MAX_HEIGHT][MAX_WIDTH];
	int height, width; //set by read_image
	read_image(img, height, width, fin);

	std::string filter = argv[2];

	if(filter == "reflect") reflect(img, height, width, fout);
	else if(filter == "rotate") rotate(img, height, width, fout);
	else if(filter == "frame") frame(img, height, width, fout);
	else if(filter == "noise") apply_filter(img, height, width, fout, noise);
	else if(filter == "posterize") apply_filter(img, height, width, fout, posterize);
	else if(filter == "nothing") apply_filter(img, height, width, fout, nothing);
	else if(filter == "invert") apply_filter(img, height, width, fout, invert);

	else if(filter == "asciify")
	{
		//delete output file, we'll rewrite it in the asciify() function
		//in case the file extension was wrong, since we made no checks
		remove(argv[4]);
		asciify(img, height, width, output_img);
	}

	else if(filter == "scale_down")
	{
		int factor;
		std::cout << "Scale down by what factor? (e.g. 2 to half): ";
		std::cin >> factor;
		scale_down(img, height, width, factor, fout);
	}

	else std::cout << "Invalid filter! Use the --help flag for a list of available filters.\n";

	return 0;
}


/*
======================================
======================================

		IMAGE ALGORITHMS

=====================================
=====================================
*/

//fills image matrix with values, sets h and w in callee to height/width values
void read_image(int img[MAX_HEIGHT][MAX_WIDTH], int &height, int &width, std::ifstream &fin)
{
	std::string header;
	getline(fin, header); //should be p5 or p2

	// skip the comments (if any)
	while ((fin >> std::ws).peek() == '#')
		fin.ignore(4096, '\n');

	fin >> width >> height >> MAX_GRAY;
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
			fin >> img[i][j];
	}
 
	if(width > 1024 || height > 1024)
	{
		std::cout << "Your image is too large, the maximum width/height is 1024px\n";
		exit(1);
	}

	fin.close();
}

//standard image algorithm, applies func to each pixel and writes
void apply_filter(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout, int(*func)(int))
{
	fout << "P2\n";
	fout << width << " " << height << std::endl;
	fout << MAX_GRAY << std::endl;

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			fout << func(img[i][j]) << " ";
		}
		fout << std::endl;
	}
}

//outputs an image of dimensions (height * 2) x width (reflection on lower half)
void reflect(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{	
	fout << "P2\n";
	fout << width << " " << height * 2 << std::endl;
	fout << MAX_GRAY << std::endl;

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
			fout << img[i][j] << " " << std::endl;

		fout << std::endl;
	}

	for(int i = height - 1; i >= 0; --i)
	{
		for(int j = 0; j < width; j++)
			fout << img[i][j] << " ";
		
		fout << std::endl;
	}
}

//rotate 90 degrees counter clockwise
void rotate(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{	
	fout << "P2\n";
	fout << height << " " << width << std::endl; //flipped
	fout << MAX_GRAY << std::endl;

	for(int i = 0; i < width; ++i)
	{
		for(int j = 0; j < height; ++j)
		{
			fout << img[j][i] << " ";
		}
		fout << std::endl;
	}
}

//outputs a txt file with ascii characters resembling the original image
void asciify(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, const std::string &fn)
{
	std::ofstream fout;

	if(fn.substr(fn.size() - 3) != "txt")
	{
		std::cout << "The asciify filter requires your output file to be a txt file!\n";
		exit(1);
	}
	else
		fout.open(fn);

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			fout << asciify(img[i][j]);
		}
		fout << std::endl;
	}
}

//shrinks the image by a user-specified factor
void scale_down(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, int scale, std::ofstream &fout)
{
	fout << "P2\n";
	fout << width/scale << " " << height/scale << std::endl;
	fout << MAX_GRAY << std::endl;

	for(int i = 0; i < height; i+=scale)
	{
		for(int j = 0; j < width; j+=scale)
		{
			fout << img[i][j] << " ";
		}
		fout << std::endl;
	}
}

void frame(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{
	fout << "P2\n";
	fout << width << " " << height << std::endl;
	fout << MAX_GRAY << std::endl;

	const int BORDER_WIDTH = 5;

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			if(i < BORDER_WIDTH || j < BORDER_WIDTH
				|| i > (height - BORDER_WIDTH) || j > (width - BORDER_WIDTH))
			{
				fout << 0 << " ";
			}
			else
				fout << img[i][j] << " ";
		}
		fout << std::endl;
	}
}


/*
======================================
======================================

		IMAGE ALGORITHM HELPERS

=====================================
=====================================
*/


int posterize(int pixel)
{
	if(pixel > MAX_GRAY/2) return MAX_GRAY;
	else return 0;
}

char asciify(int pixel)
{
	if(pixel > 200) return '.';
	else if(pixel > 100) return '*';
	else return '#';
}


/*
======================================
======================================

	PROGRAM HOUSEKEEPING HELPERS

=====================================
=====================================
*/


void list_filters(const std::vector<std::string> &filters)
{
	for(int i = 0, s = filters.size(); i < s; ++i)
		std::cout << "-" << filters[i] << "\n";
}

void handle_errors(int argc, char **argv, const std::vector<std::string> &FILTERS)
{
	if(argc == 1)
	{
		std::cout << "Usage: " << argv[0] << " --filter <filter_type> input.pgm output.pgm\n";
		std::cout << "Try the --help flag\n";
		exit(1);
	}

	std::string flag = argv[1]; //valid: --filter or --help

	if(flag == "-h" || flag == "--help" || flag == "help")
	{
		std::cout << "Usage: " << argv[0] << " --filter <filter_type> input.pgm output.pgm\n";
		std::cout << "\nAvailable filters:\n";
		list_filters(FILTERS);
		std::cout << "\n";
		exit(1);
	}

	if(argc != 5 || flag != "--filter")
	{
		std::cout << "Usage: " << argv[0] << " --filter <filter_type> input.pgm output.pgm\n";
		exit(1);
	}

	std::string filter = argv[2];

	if(!(contains(FILTERS, filter)))
	{
		std::cout << "Invalid filter! Use the --help flag for a list of available filters.\n";
		exit(1);
	}

	std::string input_img = argv[3];
	std::string output_img = argv[4];

	if(input_img == output_img)
	{
		std::cout << "Input and output images cannot be the same file!\n";
		exit(1);
	}

	if(input_img.substr(input_img.size() - 3) != "pgm")
	{
		std::cout << "Input file must be of type pgm for this filter!\n";
		exit(1);
	}

	if(filter != "asciify" 
		&& output_img.substr(output_img.size() - 3) != "pgm")
	{
		std::cout << "Output file must be of type pgm for this filter!\n";
		exit(1);
	}
}