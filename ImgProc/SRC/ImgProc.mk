OBJS = WinMain.obj BmpUtil.obj FFTImage.obj LensBlur.obj Effect3D.obj Laplacian.obj GaussBlur.obj ConvImage.obj Posterization.obj
LIBS = 
LNKFLAGS = 
CFLAGS = /Ox
TARGET = Main.exe

.cpp.obj:
	$(CC) $(CFLAGS) /c $<

$(TARGET): $(OBJS)
	link.exe /out:$(TARGET) $(OBJS) $(LNKFLAGS)
		
clean:
	del $(OBJS) $(TARGET)
