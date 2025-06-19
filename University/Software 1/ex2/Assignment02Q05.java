import java.util.Arrays;

public class Assignment02Q05 {

	public static void main(String[] args) {
		// do not change this part below
		int N = Integer.parseInt(args[0]);
		int[][] matrix = new int[N][N]; // the input matrix to be
		for(int i=0;i < N; i++){
			for(int j=0; j < N; j++){
				matrix[i][j] = Integer.parseInt(args[1+(i*N)+j]); // the value at [i][j] is the i*N+j item in args (without considering args[0])
			}
		}
		for(int i=0;i < N; i++)
			System.out.println(Arrays.toString(matrix[i]));
		System.out.println("");
		int[][] rotatedMatrix; // the rotated matrix
		
		// *** your code goes here below ***
		
		// *** for fun, here's how you do it by creating a new array (commented as to not cause a problem):
		
		/**
		rotatedMatrix = new int[N][N];
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				rotatedMatrix[i][j] = matrix[N- j - 1][i];
			}
		}
		*/
		
		// *** and here's how you do it by changing the matrix in-place:
		
		rotatedMatrix = matrix;
		for (int layer = 0; layer < N / 2; layer++) {
		    int first = layer;
		    int last = N - 1 - layer;
		    for (int i = first; i < last; i++) {
		        int offset = i - first;
		        int top = rotatedMatrix[first][i];
		        // left -> top
		        rotatedMatrix[first][i] = rotatedMatrix[last - offset][first];
		        // bottom -> left
		        rotatedMatrix[last - offset][first] = rotatedMatrix[last][last - offset];
		        // right -> bottom
		        rotatedMatrix[last][last - offset] = rotatedMatrix[i][last];
		        // top -> right
		        rotatedMatrix[i][last] = top;
		    }
		}
				
		// do not change this part below
		for(int i=0;i < N; i++){ 
			System.out.println(Arrays.toString(rotatedMatrix[i])); // this would compile after you would put value in transposedMatrix
		}
	}
}
