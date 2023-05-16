CFLAGS = -O2
LDFLAGS = -lglfw -lGL -lGLEW

sterne:	sterne.c
	$(CC) sterne.c $(CFLAGS) $(LDFLAGS) -o sterne

clean:
	rm -f sterne
