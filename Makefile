CC = cc
LIBS = -lm
MAIN = raytracer.c
OUT = raytracer.out

$(OUT): $(MAIN)
	$(CC) $(LIBS) $? -o $@

clean:
	rm -f $(OUT)
