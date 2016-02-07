OBJS = status.c
CC = clang
PREFIX = /usr/local
CFLAGS = -g -O2 -fPIE -pie -D_FORTIFY_SOURCE=2 -fstack-protector-strong --param=ssp-buffer-size=1 -Weverything -Werror -std=c11 -fsanitize-trap=undefined -lX11
LDFLAGS = -Wl,-z,relro,-z,now

test: $(OBJS)
		$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o status
