CROSS_COMPILE ?=

CC = $(CROSS_COMPILE)gcc

CMNFLAGS =
CPPFLAGS = -Wall -O2 -g
CFLAGS   = $(CMNFLAGS)
LDFLAGS  = $(CMNFLAGS)
LDADD    = -lOpenCL -lm

BINS = vecadd vecaddf
VECADD_OBJS = vecadd.o
VECADDF_OBJS = vecaddf.o
OBJS = $(VECADD_OBJS) $(VECADDF_OBJS)

all: $(BINS)

vecadd: $(VECADD_OBJS)
	$(CC) $(LDFLAGS) $^ $(LDADD) -o $@
vecaddf: $(VECADDF_OBJS)
	$(CC) $(LDFLAGS) $^ $(LDADD) -o $@

clean:
	rm -f $(BINS) $(OBJS)

.PHONY: all install clean distclean
