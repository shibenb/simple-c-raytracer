CC = cc
LIBS = -lm
MAIN = raytracer.c
OUT = raytracer.out

$(OUT): $(MAIN)
	$(CC) $? -o $@ $(LIBS) 

clean:
	rm -f $(OUT)
