// This saves the contents of an OpenGL screen to a .ppm picture file.
// Bart Stander
// April, 2008

// Updated March of 2011, so the picture is not upside down (thanks Nick Cox), and
// this version is MANY times faster than the previous, because it uses binary ppm, and fout.write

bool SaveToPPM(char filename[])
{
	cout << "Saving to ppm BINARY file " << filename << " ..." << endl;

	struct Pixel
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	// Allocate enough space to hold the whole screen
	Pixel *pixels = new Pixel[screen_x*screen_y];
	Pixel *pixelsFlipped = new Pixel[screen_x*screen_y];

	// reads from the framebuffer into pixels (from the lower left corner)
	glReadPixels(0,0,screen_x,screen_y,GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// Open the output file
	ofstream fout(filename, ios::binary);
	if(!fout)
		return false;

	// Note that we must flip the vertical order and write from top to bottom.
	for(int y=0; y<screen_y; y++)
	{
		int yflipped = screen_y-y-1;
		int index = y*screen_x;
		int indexFlipped = yflipped*screen_x;
		for(int x=0; x<screen_x; x++)
		{
			pixelsFlipped[indexFlipped+x] = pixels[index+x];
		}
	}

	// write the ppm header
	fout << "P6" << endl; // p3 for text, p6 for binary
	fout << screen_x << " " << screen_y << endl;
	fout << "255" << endl;

	// write the ppm color data. 
	fout.write( (char*)pixelsFlipped, screen_x*screen_y*3 );

	// cleanup
	fout.close();
	delete []pixels;
	delete []pixelsFlipped;

	cout << "Done saving." << endl;

	return true;
}
