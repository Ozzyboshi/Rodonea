all :
	cat $$(ls -tr *.bin) > './outputfile.out'
	./uffman2 -c './outputfile.out' './outputfile.Z'
	cat $$(ls -tr *.bin.help) > './outputfile.out.help'
    
clean :
	rm -f *.bin
	rm -f *.bin.help
	rm -f outputfile.Z outputfile.out outputfile.out.help
    
