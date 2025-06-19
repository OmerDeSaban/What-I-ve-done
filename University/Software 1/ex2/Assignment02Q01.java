
public class Assignment02Q01 {

	public static void main(String[] args) {
		// *** your code goes here below ***
		for (int i = 0; i < args.length; i++) {
			if (args[i].length() > 0) {
				char c = args[i].charAt(0);
				int c_val = c;
				if ((c_val % 5) == 0) {
					System.out.println(c);
				}	
			}
		}
	}
}
