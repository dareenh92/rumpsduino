	
	//main loop
	while(1){
		
		ch = _getchar();
		_putchar(ch);
		_putchar(N);
		
		switch(ch){
			case 'a':
				memcpy(&input_seq, coded_seq1, N*2* sizeof(char));
				map_decode(input_seq, apriori_b1, 1, N, LLR);
				break;
				
			case 'b':
				memcpy(&input_seq, coded_seq2, N*2* sizeof(char));
				map_decode(input_seq, apriori_b1, 1, N, LLR);
				break;
				
			case 'c':
				memcpy(&input_seq, coded_seq3, N*2* sizeof(char));
				map_decode(input_seq, apriori_b1, 1, N, LLR);
				break;
		}
		
		
		for(int i=0; i<N*2; i++)
			_putchar(input_seq[i]);	
	}
