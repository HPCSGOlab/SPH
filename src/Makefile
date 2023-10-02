CC=mpicc
CLIBS= -L/usr/local/lib -lglfw -lGL -lGLU -lX11 -lGLEW -lXxf86vm -lXrandr -lXi -lfreetype -lm 
CINCLUDES= -I/usr/include/freetype2 
CFLAGS= -DGLFW -O3 -ffast-math -I/usr/local/include -I/usr/include/libdrm -g 

SOURCES=liquid_gl.c ogl_utils.c dividers_gl.c particles_gl.c mover_gl.c font_gl.c lodepng.c exit_menu_gl.c rectangle_gl.c renderer.c glfw_utils.c image_gl.c cursor_gl.c background_gl.c controls.c geometry.c hash.c communication.c fluid.c

OBJECTS=$(SOURCES:.c=.o)

TARGET=../bin/sph.out

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(CLIBS)

%.o: %.c
	$(CC) $(CINCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

