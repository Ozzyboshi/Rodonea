bin_PROGRAMS = rodonea
rodonea_LDADD = music_ptr_linkable.o
rodonea_SOURCES = main.c rodonea.c demointro.c demointro2.c


music_ptr_linkable.o: music_ptr_linkable.s
	vasmm68k_mot -Fhunk -quiet -esc  -DINTERLACE=0 ./music_ptr_linkable.s  -o ./music_ptr_linkable.o -I/usr/local/amiga/os-include