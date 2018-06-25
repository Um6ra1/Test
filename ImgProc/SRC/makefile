OBJS = WinMain.o BmpUtil.o FFTImage.o FwhtImage.o Laplacian.o GaussBlur.o ConvImage.o 
LIBS = 
TARGET = Main.exe
CC = g++

LDFLAGS = 
CFLAGS = -O3
INCLUDE = 
LIBS	= 

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)# $(RCOBJS)
	$(CC) $(MEXFLAGS) $(OBJS) $(LIBS) -o $@
	@echo Done.

.c.o:
	$(CC) $(CFLAGS) $(MEXFLAGS) $(LDFLAGS) $(INCLUDE) -c $<

%.o: %.rc
	windres $^ $@

.PHONY: clean
clean:
	@rm -f *.o*
	@echo Cleaned.
