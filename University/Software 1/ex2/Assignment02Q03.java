
public class Assignment02Q03 {

	public static void main(String[] args) {
		int numOfOdd = 0;
		int n = -1;
		// *** your code goes here below ***
		n = Integer.parseInt(args[0]);
		String[] fib_nums = new String[n];
		for (int i = 1; i <= n; i++) {
			fib_nums[i-1] = Integer.toString(fib(i));
		}
		String res = String.join(" ", fib_nums);
		for (int j = 0; j < n; j++) {
			int tmp = Integer.parseInt(fib_nums[j]);
			if ((tmp % 2) == 1) {
				numOfOdd += 1;
			}
		}
		
		System.out.println("The first "+ n +" Fibonacci numbers are:");
		// *** your code goes here below ***
		System.out.println(res);
		System.out.println("The number of odd numbers is: "+numOfOdd);

	}
	
	public static int fib(int n) {
		if (n == 0) {
			int res = 0;
			return res;
		}
		else if (n == 1 || n == 2) {
			int res = 1;
			return res;
		}
		int n_minus_2 = 1;
		int n_minus_1 = 1;
		int res = 0;
		for (int i = 2; i < n; i++) {
			res = (n_minus_1 + n_minus_2);
			n_minus_2 = n_minus_1;
			n_minus_1 = res;
		}
		return res;
	}

}
