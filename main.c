
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
int get_file_size(char* filename)
{
    struct stat statbuf;
    stat(filename,&statbuf);
    int size=statbuf.st_size;
    return size;
}

enum Mode {
  ENCODE,
  DECODE
};
int main(int argc, const char *argv[])
{
  enum Mode mode = ENCODE;
  int headerSize = 1500;

  char *h = getenv("HEADER_SIZE");
  if (h) {
    headerSize = atoi(h);
  }

  if (argc == 3 && strcmp(argv[2], "--decode") == 0) {
    mode = DECODE;
    printf("DECODE\n");
    argc--;
  }

  if (argc != 2) {
    fprintf(stderr, "Invalid arguments!\n%s image.jpg\n", argv[0]);
    return 0;
  }

  const char *in_fn = argv[1];
  const char *final_fn = argv[1];

  // 临时文件
  char buf[1024];
  strcpy(buf, in_fn);
  strcpy(buf + strlen(in_fn), ".tmp");
  const char *out_fn = (const char *)buf;

  FILE *fp_in = fopen(in_fn, "rb");

  fseek(fp_in, -1, SEEK_END);
  unsigned char c = fgetc(fp_in);
  if (mode == ENCODE) {
    if (0xDA == c) {
      // already compressed
      return 0;
    } else if (0xD9 != c) {
      fprintf(stderr, "Not a valid jpg file: %s\n", in_fn);
      return 0;
    }
  } else if (mode == DECODE) {
    if (0xD9 == c) {
      // already decoded 
      return 0;
    } else if (0xDA != c) {
      fprintf(stderr, "Not a valid encoded jpg file: %s\n", in_fn);
      return 0;
    }
  }

  FILE *fp_out = fopen(out_fn, "wb+");
  int size = ftell(fp_in);
  rewind(fp_in);

  int n = 0;
  int i = 0;

  printf("file size = %d\n", size);

  for (; i < headerSize && i < size - 1; ++i) {
    unsigned char c = fgetc(fp_in);
    fputc(c, fp_out);
  }

  for (; i < size - 1; i++) {
    char c = fgetc(fp_in);
    fputc(c ^ 0x31, fp_out);
  }

  // for last byte
  for (; i < size; ++i) {
    char c = fgetc(fp_in);
    if (mode == DECODE) {
    fputc(0xD9, fp_out);
    } else {
    fputc(0xDA, fp_out);
    }
  }

  printf("file write done\n");

  fclose(fp_in);
  fclose(fp_out);

  rename(out_fn, final_fn);

  return 0;
}
