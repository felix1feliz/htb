#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include"htb.h"

void printHelp() {
		printf("Usage: htb [OPTIONS] <SOURCE>\n");
		printf("Options:\n");
		printf(" -h         Prints this\n");
		printf(" -o <PATH>  Specifies the output file\n");
		printf(" -e         Gives output execute permission\n");
}

unsigned char htb(int fd, char *buf) {
	unsigned char ret = 0;
	if(*buf > 47 && *buf < 58)       // Numbers
		ret += (*buf - 48) * 16;
	else if(*buf > 64 && *buf < 71)  // Upercase letters
		ret += (*buf - 55) * 16;
	else if(*buf > 96 && *buf < 103) // Lowercase letters
		ret += (*buf - 87) * 16;
	else if(*buf < 33) {             // Invisible characters
		printf("Byte was cut in half\n");
		exit(1);
	} else {                         // Other characters
		printf("Unexpected character <%c>\n", *buf);
		exit(1);
	}

	if(read(fd, buf, 1) == 0) {
		printf("Byte was cut in half\n");
		exit(1);
	}

	if(*buf > 47 && *buf < 58)                // Number
		ret += *buf - 48;
	else if(*buf > 64 && *buf < 71)           // Upercase letter
		ret += *buf - 55;
	else if(*buf > 96 && *buf < 103)          // Lowercase letter
		ret += *buf - 87;
	else if(*buf < 33) {
		printf("Byte was cut in half\n"); // Invisible characters
		exit(1);
	} else {                                  // Other characters
		printf("Unexpected character <%c>\n", *buf);
		exit(1);
	}

	return ret;
}

int main(int argc, char **argv) {
	if(argc == 1) {
		printHelp();
		return 1;
	}

	int perms = 0;
	char *output = NULL;
	char *source = NULL;

	for(size_t i = 1; i < argc; ++i) {
		if(strcmp(argv[i], "-h") == 0) {
			if(argc != 2) {
				printf("Arguments incompatible with flag '-h'\n");
				return 1;
			}

			printHelp();
			return 0;
		}

		if(strcmp(argv[i], "-o") == 0) {
			if(argc - i < 2) {
				printf("Flag '-o' requires one positional argument\n");
				return 1;
			}

			if(output != NULL) {
				printf("Flag '-o' must only be specified once\n");
				return 1;
			}

			output = argv[i + 1];
			++i;
			continue;
		}

		if(strcmp(argv[i], "-e") == 0) {
			if(perms != 0) {
				printf("Flag '-e' must only be specified once\n");
				return 1;
			}

			perms = S_IRWXU;
			continue;
		}

		if(source != NULL) {
			printf("Source must only be specified once\n");
			return 1;
		}

		source = argv[i];
	}

	if(source == NULL) {
		printf("Source must be specified\n");
		return 1;
	}

	if(output == NULL) {
		output[sizeof(source) * sizeof(char) + 4 * sizeof(char)];
		output[0] = '\0';
		strcat(output, source);
		strcat(output, ".out");
	}

	if(perms == 0)
		perms = S_IRUSR | S_IWUSR;

	// Source File Descriptor
	int sfd = open(source, O_RDONLY);
	if(sfd == -1) {
		printf("Unable to open source file (does it exist?)\n");
		return 1;
	}

	// Output File Descriptor
	int ofd = open(output, O_WRONLY | O_CREAT | O_TRUNC, perms);
	if(ofd == -1) {
		printf("Unable to open output file\n");
		return 1;
	}


	char c = 0;
	unsigned char val = 0;
	while(read(sfd, &c, 1) != 0) {
		if(c < 33) continue;
		if(c == '>') continue;

		if(c == '<') {
			while(read(sfd, &c, 1) != 0) 
				if(c == '>') break;
			continue;
		}
		
		val = htb(sfd, &c);
		write(ofd, &val, 1);
	}

	return 0;
}
