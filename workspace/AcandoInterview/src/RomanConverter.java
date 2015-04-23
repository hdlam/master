
public class RomanConverter {
	
	public int convert(String romertall){
//		String[] liste = romertall.split("");
		int sum = 0, posisjon = romertall.length()-1;
		for (int i = romertall.length()-1; i>=0; i--) {
			if(romertall.charAt(i)=='I'){
				sum++;
			}
			else{
				posisjon = i;
				break;
			}
		}
		while(posisjon >= 0){
			if(romertall.charAt(posisjon) =='V'){
				sum+=5;
			}
			if(romertall.charAt(posisjon) == 'X')
				sum+= 10;
			posisjon--;
		}
		if(romertall.length()>1 && (romertall.charAt(0) == 'I' && romertall.charAt(1) != 'I')){
			sum--;
		}
		
		return sum;
	}

}
