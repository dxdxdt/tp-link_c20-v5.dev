#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


int main (const int argc, const char **args) {
	static const uint8_t ZERO = 0;
	const char *program = args[0];
	FILE *fin = NULL, *fout = NULL;
	unsigned long zeros, skip;
	int ret = 0;
	long fin_size;
	size_t load_size;
	uint8_t *buf = NULL;

	if (argc <= 0) {
		fprintf(stderr, "wtf?\n");
		return 2;
	}
	else if (argc < 5) {
		fprintf(stderr, 
			"A simple C program that fabricates a TFTP image out of a stock image.\n"
			"Usage: %s <in file> <out file> <skip> <zero>\n"
			"\t<in file>: path to input file\n"
			"\t<out file>: path to output file\n"
			"\t<skip>: number of bytes to skip before loading <in file>\n"
			"\t<zero>: number of zeros to write to <out file>\n",
		program);
		return 1;
	}

	if (sscanf(args[3], "%lu", &skip) != 1) {
		fprintf(stderr, "Invalid arg: <skip>\n");
		return 1;
	}
	if (sscanf(args[4], "%lu", &zeros) != 1) {
		fprintf(stderr, "Invalid arg: <zero>\n");
		return 1;
	}

	do { // fake loop
		size_t i;

		fin = fopen(args[1], "rb");
		fout = fopen(args[2], "rb"); // just checking

		if (fin == NULL) {
			perror("Failed to open <in file>");
			ret = 2;
			break;
		}
		if (fout != NULL) {
			fprintf(stderr, "%s: file already exists. Refusing to overwrite.\n", args[2]);
			ret = 2;
			break;
		}
		
		fout = fopen(args[2], "wb");
		if (fout == NULL) {
			perror("Failed to create <out file>");
			ret = 2;
			break;
		}

		if (fseek(fin, 0, SEEK_END) != 0 || (fin_size = ftell(fin)) < 0) {
			perror("Failed to tell size of <in file>");
			ret = 2;
			break;
		}
		if ((size_t)fin_size < skip) {
			fprintf(stderr, "size of <in file> is smaller than <skip> (%lu < %ld)\n", fin_size, skip);
			ret = 2;
			break;
		}
		load_size = (size_t)fin_size - skip;

		if (fseek(fin, skip, SEEK_SET) != 0) {
			perror("Failed to skip");
			ret = 2;
			break;
		}

		buf = (uint8_t*)malloc((size_t)load_size);
		if (buf == NULL) {
			perror("Failed to allocate memory");
			ret = 2;
			break;
		}

		if (fread(buf, 1, load_size, fin) != (size_t)load_size) {
			perror("Failed to load <in file>");
			ret = 2;
			break;
		}

		for (i = 0; i < zeros; i += 1) {
			if (fwrite(&ZERO, 1, 1, fout) != 1) {
				break;
			}
		}
		if (i < zeros) {
			perror("Failed to write <out file>");
			ret = 2;
			break;
		}

		if (fwrite(buf, 1, load_size, fout) != (size_t)load_size) {
			perror("Failed to write <out file>");
			ret = 2;
			break;
		}
	} while (false);

	free(buf);
	if (fin != NULL) {
		fclose(fin);
	}
	if (fout != NULL) {
		fclose(fout);
	}

	return ret;
}
