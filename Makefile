wordhist : lab8_template.c
	clang lab8_template.c `pkg-config --cflags glib-2.0 --libs glib-2.0` -o wordhist

clean :
	rm wordhist
