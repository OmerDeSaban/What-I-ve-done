

public class Assignment02Q02 {

	public static void main(String[] args) {
		// do not change this part below
		double piEstimation = 0.0;
		
		// *** your code goes here below ***
		int sum_num = Integer.parseInt(args[0]);
		double sum = 0;
		for (int i = 0; i < sum_num; i++) {
			double tmp = 2*(i + 1) - 1;
			tmp = 1/tmp;
			if ((i % 2) == 0) {
				sum += tmp;
			}
			else {
				sum -= tmp;
			}
		}
		piEstimation += 4*sum;
		
		// do not change this part below
		System.out.println(piEstimation + " " + Math.PI);

	}

}
