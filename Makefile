CC = cc
LIBS = "-lm"

raytracer.out: raytracer.c
	$(CC) $(LIBS) $? -o $@
