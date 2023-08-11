#include "image.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

#define JSAMPLE2MINE(x) (unsigned char)(x)

struct Image * imageCreate(int width, int height, int comps) {
	struct Image *this;
	if ((this = malloc(sizeof(struct Image))) == NULL) {
		Error("Can't allocate struct Image");
	}
	this->height = height;
	this->width = width;
	this->comps = comps;
	int rowLength = width * comps;
	this->image = malloc(height * rowLength * sizeof(JSAMPLE));
	return this;
}

void imageDelete(struct Image *this) {
	if (this == NULL) {
		Error("Trying to delete non created image!");
		return;
	}
	free(this->image);
	free(this);
}

void imageFillLine(struct Image *this, const JSAMPROW line, int linenum) {
	if (this == NULL) {
		Error("Trying to fill non created image!");
		return;
	}

	int rowLen = this->width * this->comps;
	for (int i = 0; i < rowLen; i++) {
		this->image[linenum * rowLen + i] = JSAMPLE2MINE(line[i]);
	}
}

struct Image * loadImage(char * filename) {
	/* load JPEG
	 * Allocate and initialize a JPEG decompression object
	 * Specify the source of the compressed data (eg, a file)
	 * Call jpeg_read_header() to obtain image info
	 * Set parameters for decompression
	 * jpeg_start_decompress(...);
	 * while (scan lines remain to be read)
	 * 	jpeg_read_scanlines(...);
	 * jpeg_finish_decompress(...);
	 * Release the JPEG decompression object
	 */
	FILE *img;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	struct Image *result;

	if ((img = fopen(filename, "rb")) == NULL) {
		Error("Can't open texture image");
		return NULL;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, img);

	Debug("JPEG header reading status: %d", jpeg_read_header(&cinfo, TRUE));

	jpeg_start_decompress(&cinfo);
	result = imageCreate(cinfo.output_width, cinfo.output_height, cinfo.output_components);
	Debug("Reading texture image with size %dx%dx%d", result->width, result->height, result->comps);
	JSAMPARRAY buf = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, cinfo.output_width * cinfo.output_components, 1);
	Debug("Line buffer memory allocated");
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buf, 1);
		imageFillLine(result, buf[0], cinfo.output_scanline - 1);
	}
	Debug("Texture image readed!");

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(img);

	if (jerr.num_warnings > 0) {
		Debug("There are %d JPEG warnings after cleanup", jerr.num_warnings);
	}

	return result;
}

