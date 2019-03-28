CC=gcc
VPPLIB=-lvapiclient

all:
	${CC} ole.c -o remove_tap_by_sw_index ${VPPLIB} -g

clean:
	rm remove_tap_by_sw_index
