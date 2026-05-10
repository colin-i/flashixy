
. ~/games/ffdec
if [ -z "${src}" ]; then
	src=~/Desktop/actionswf
fi
CC=g++ L=-L${src} ffdec=${ffd} extern_ffdec=${effd} LD_LIBRARY_PATH=${src} cdefines=-I/home/bc/Desktop/actionswf/include ./a${nr} $@
#nr ca mai trece si ep/simplu pe aici si are cu nr
