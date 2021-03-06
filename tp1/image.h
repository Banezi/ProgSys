#ifndef INCLUDE_IMAGE_H
#define INCLUDE_IMAGE_H

#include <stdio.h>
#include <stdint.h>

struct image;
struct image make_image(size_t type, size_t row_count, size_t column_count, size_t max_value);
struct image make_image_from_stream(FILE *stream);
struct image make_image_from_file(char const *file);
struct image make_image_from_memory(size_t type, size_t row_count, size_t column_count, size_t max_value, void *data);
size_t get_image_byte_count(struct image const *image);
void alloc_image(struct image *image);
void clear_image(struct image *image);
size_t write_image_header_to_stream(struct image const *image, FILE *stream);
void write_image_to_stream(struct image const *image, FILE *stream);
void write_image_to_file(struct image const *image, char const *file);
void blur_image(struct image const *input, struct image *output, size_t radius);
void blur_image_layer(struct image const *input, struct image *output, size_t radius, size_t layer);
void blur_image_row(struct image const *input, struct image *output, size_t radius, size_t output_row);
void blur_image_column(struct image const *input, struct image *output, size_t radius, size_t output_column);
void copy_image_layer(struct image const *input, struct image *output, size_t layer);
void copy_image_row(struct image const *input, struct image *output, size_t row);

struct image
{
	size_t type, row_count, column_count, max_value;
	union {
		void *as_bytes;
		uint8_t (*as_rgb8)[3];
	} data;
};

/*

DÃ©finition des fonctions.
Devrait Ãªtre compilÃ© sÃ©parÃ©ment.

*/

#include "debug.h"

size_t get_image_byte_count(struct image const *image)
{
	switch(image->type)
	{
		case 3:
		case 6:
		return image->row_count * image->column_count * 3 * (image->max_value < 256 ? 1 : 2);

		default:
		fprintf(stderr, "type P%zu inconnu.\n", image->type);
		abort();
	}
}

void alloc_image(struct image *image)
{
	image->data.as_bytes = malloc(get_image_byte_count(image));
	CHECK_ERRNO(image->data.as_bytes);
}

void clear_image(struct image *image)
{
	free(image->data.as_bytes);
	image->data.as_bytes = 0;
}

struct image make_image(size_t type, size_t row_count, size_t column_count, size_t max_value)
{
	struct image image = make_image_from_memory(type, row_count, column_count, max_value, 0);
	alloc_image(&image);
	return image;
}

struct image make_image_from_stream(FILE *stream)
{
	struct image image;
	CHECK_ERRNO(fscanf(stream, "P%zu%zu%zu%zu\n", &image.type, &image.column_count, &image.row_count, &image.max_value) == 4);
	alloc_image(&image);
	switch(image.type)
	{
		case 4:
		case 5:
		case 6:
		CHECK_ERRNO(fread(image.data.as_bytes, get_image_byte_count(&image), 1, stream) == 1);
		break;

		default:
		fprintf(stderr, "type P%zu inconnu.\n", image.type);
		abort();
	}
	return image;
}

struct image make_image_from_file(char const *file)
{
	FILE *stream = fopen(file, "rb");
	CHECK_ERRNO(stream);
	struct image image = make_image_from_stream(stream);
	CHECK_ERRNO(!fclose(stream));
	return image;
}

struct image make_image_from_memory(size_t type, size_t row_count, size_t column_count, size_t max_value, void *data)
{
	struct image image = {type, row_count, column_count, max_value, {data}};
	return image;
}

size_t write_image_header_to_stream(struct image const *image, FILE *stream)
{
	int length =fprintf(stream, "P%zu %zu %zu %zu\n", image->type, image->column_count, image->row_count, image->max_value);
	CHECK_ERRNO(length >= 0);
	return (size_t) length;
}

void write_image_to_stream(struct image const *image, FILE *stream)
{
	write_image_header_to_stream(image, stream);
	CHECK_ERRNO(fwrite(image->data.as_bytes, get_image_byte_count(image), 1, stream) == 1);
}

void write_image_to_file(struct image const *image, char const *file)
{
	FILE *stream = fopen(file, "wb");
	CHECK_ERRNO(stream);
	write_image_to_stream(image, stream);
	CHECK_ERRNO(!fclose(stream));
}

/*

DÃ©but du traitement d'image brutal.
Il me faut des macros ou je vais devenir fou.

*/

#define FOR_EACH_ROW(...) for(size_t row = 0; row < output->row_count; ++row) {__VA_ARGS__}
#define FOR_EACH_COLUMN(...) for(size_t column = 0; column < output->column_count; ++column) {__VA_ARGS__}
#define FOR_EACH_LAYER(...) for(size_t layer = 0; layer < 3; ++layer) {__VA_ARGS__}

#define FOR_EACH_OUTPUT_ROW(...) for(size_t output_row = 0; output_row < output->row_count; ++output_row) {DEFINE_INPUT_RANGE(row) __VA_ARGS__}
#define FOR_EACH_OUTPUT_COLUMN(...) for(size_t output_column = 0; output_column < output->column_count; ++output_column) {DEFINE_INPUT_RANGE(column) __VA_ARGS__}

#define DEFINE_INPUT_RANGE(X)\
	size_t input_##X##_begin = output_##X > radius ? output_##X - radius : 0;\
	size_t input_##X##_end = output_##X + radius < input->X##_count ? output_##X + radius : input->X##_count;\
	size_t input_##X##_count = input_##X##_end - input_##X##_begin;\

#define DO_BLUR\
	uint_fast32_t sum = 0;\
	for(size_t input_row = input_row_begin; input_row < input_row_end; ++input_row)\
	{\
		for(size_t input_column = input_column_begin; input_column < input_column_end; ++input_column)\
		{\
			sum += (uint_fast32_t) input->data.as_rgb8[input_row * input->column_count + input_column][layer];\
		}\
	}\
	output->data.as_rgb8[output_row * output->column_count + output_column][layer] = (uint8_t)(sum / input_row_count / input_column_count);

#define DO_COPY output->data.as_rgb8[row * output->column_count + column][layer] = input->data.as_rgb8[row * input->column_count + column][layer];

#if !defined DO_PROFILE
#define DO_PROFILE(...)
#endif

void blur_image(struct image const *input, struct image *output, size_t radius) {FOR_EACH_OUTPUT_ROW(FOR_EACH_OUTPUT_COLUMN(FOR_EACH_LAYER(DO_BLUR)))}
void blur_image_layer(struct image const *input, struct image *output, size_t radius, size_t layer) {FOR_EACH_OUTPUT_ROW(FOR_EACH_OUTPUT_COLUMN(DO_BLUR DO_PROFILE(layer+1)))}
void blur_image_row(struct image const *input, struct image *output, size_t radius, size_t output_row) {DEFINE_INPUT_RANGE(row) FOR_EACH_OUTPUT_COLUMN(FOR_EACH_LAYER(DO_BLUR))}
void blur_image_column(struct image const *input, struct image *output, size_t radius, size_t output_column) {DEFINE_INPUT_RANGE(column) FOR_EACH_OUTPUT_ROW(FOR_EACH_LAYER(DO_BLUR))}

void copy_image_layer(struct image const *input, struct image *output, size_t layer) {FOR_EACH_ROW(FOR_EACH_COLUMN(DO_COPY DO_PROFILE(-layer-1)))}
void copy_image_row(struct image const *input, struct image *output, size_t row) {FOR_EACH_COLUMN(FOR_EACH_LAYER(DO_COPY))}
void copy_image_column(struct image const *input, struct image *output, size_t column) {FOR_EACH_ROW(FOR_EACH_LAYER(DO_COPY))}

#endif
