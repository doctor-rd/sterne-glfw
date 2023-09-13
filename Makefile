EXE = sterne
CFLAGS = -O2
LDFLAGS = -lglfw -lGL -lGLEW

$(EXE):	sterne.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f $(EXE)
