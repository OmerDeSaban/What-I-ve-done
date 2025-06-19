

public class Assignment1 {
	public static void main(String[] args){
		char ch1 = args[0].charAt(0);
		int val1 = ch1;
		char ch2 = args[1].charAt(0);
		int val2 = ch2;
		int offset = val2-val1;
		System.out.println("Offset is " + offset + ".");
	}
}
