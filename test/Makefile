CROSS_COMPILE ?=

CC = $(CROSS_COMPILE)gcc

CMNFLAGS =
CPPFLAGS = -Wall -O2 -g
CFLAGS   = $(CMNFLAGS)
LDFLAGS  = $(CMNFLAGS)
LDADD    = -lOpenCL -lm

BINS = sgemm \
	vecadd vecaddf
SGEMM_OBJS = sgemm.o
VECADD_OBJS = vecadd.o
VECADDF_OBJS = vecaddf.o
OBJS = $(VECADD_OBJS) $(VECADDF_OBJS) \
	$(SGEMM_OBJS)

all: $(BINS)

sgemm: $(SGEMM_OBJS)
	$(CC) $(LDFLAGS) $^ $(LDADD) -o $@

vecadd: $(VECADD_OBJS)
	$(CC) $(LDFLAGS) $^ $(LDADD) -o $@

vecaddf: $(VECADDF_OBJS)
	$(CC) $(LDFLAGS) $^ $(LDADD) -o $@

clean:
	rm -f $(BINS) $(OBJS)

.PHONY: all install clean distclean
