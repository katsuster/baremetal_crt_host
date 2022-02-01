CROSS_COMPILE ?=

CC = $(CROSS_COMPILE)gcc

CMNFLAGS =
CPPFLAGS = -Wall -O2 -g
CFLAGS   = $(CMNFLAGS)
LDFLAGS  = $(CMNFLAGS)
LDADD    = -lOpenCL -lm

BINS = vecadd
VECADD_OBJS = vecadd.o

all: $(BINS)

vecadd: $(VECADD_OBJS)
	$(CC) $(LDFLAGS) $< $(LDADD) -o $@

clean:
	rm -f $(BINS) $(VECADD_OBJS)

.PHONY: all install clean distclean
