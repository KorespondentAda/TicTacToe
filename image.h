#pragma once

struct Image {
	unsigned char *image;
	int width;
	int height;
	int comps;
};

struct Image * imageCreate(int width, int height, int comps);
void imageDelete(struct Image *this);
struct Image * loadImage(char * filename);

