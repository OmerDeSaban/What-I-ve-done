
public class ArrayUtils {
	
	/**
	 * Mirroring a given array in-place and returning it post-mirroring.
	 * 
	 * @param array		the array to be mirrored; must not be null; must consist of integers.
	 * 
	 * @return			the given array after mirroring.
	 */
	private static int[] MirrorArray(int[] array) {
		/**
		 * We might need to flip our array. Best to do so in O(log_2^(array.length)) iterations and not in
		 * O(array.length) iterations by switching an element on the right half of the array with the
		 * corresponding element on the left half of the array until we reach the elements on the middle of
		 * our array.
		 * If our array has an odd number of elements, the middle element wouldn't be moved. 
		 */
		int len = array.length;
		int mid = (int) array.length / 2;
		for (int i = 0; i < mid; i++) {
			int tmp = array[i];
			array[i] = array[len - i - 1];
			array[len - i - 1] = tmp;
		}
		return array;
	}
	
	
	/**
	 * Switching the direction between 'R' and 'L' ('R' would become 'L' and 'L' would become 'R').
	 * 
	 * @param direction		the direction which would be switched; must not be null; must be either 'R' or
	 * 						'L'.
	 * 
	 * @return				the given direction after switching.	
	 */
	private static char SwitchDirection(char direction) {
		/**
		 * We might need to switch the direction in which we are shifting our array, very easy to do so.
		 */
		if (direction == 'R') { direction = 'L'; }
		else if (direction == 'L') { direction = 'R'; }
		return direction;
	}
	
	
	/**
	 * shifts the elements of an array cyclically as many times as requested. Does not support negative
	 * shifts amount or 0 number of shifts.
	 * 
	 * @param array			the array to be shifted cyclically; must not be null; must consist of integers.
	 * 
	 * @param move			the number of times the array would be shifted cyclically; must not be null; must
	 * 						be a positive integer.
	 * 
	 * @param direction		the direction in which the array would be shifted cyclically ('R' for right and
	 * 						'L' for left); must not be null; must be 'R' or 'L'.
	 * 
	 * @return				the array with elements shifted cyclically, according to the specified
	 * 						parameters.
	 */
	private static int[] ShiftArray(int[] array, int move, char direction) {
		/**
		 * When shifting our array to the right 'move' times, our each element from the first up to the
		 * (array.length - move - 1)th element would be moved 'move' times ahead, while all the elements
		 * from that index (denote an index of such element as i) up to (array.length - 1) would be moved to
		 * the (move - (array.length - i - 1))th index.
		 */
		int len = array.length;
		int[] lasts = new int[move];
		for (int i = 0; i < move; i++) { lasts[i] = array[len - move + i]; }
		for (int j = 0; j < (len - move); j ++) { array[len - 1 - j] = array[len - 1 - j - move]; }
		for (int k = 0; k < move; k++) { array[k] = lasts[k]; }
		return array;
	}
	
	
	/**
	 * Shifts the elements of an array cyclically as many times as requested. Supports negative shifts
	 * amounts (using direction shifts), 0 number of shifts and an empty array.
	 * 
	 * @param array			the array to be shifted cyclically; must not be null; must consist of integers.
	 * 
	 * @param move			the number of times the array would be shifted cyclically; must not be null; must
	 * 						be an integer.
	 * 
	 * @param direction		the direction in which the array would be shifted cyclically ('R' for right and
	 * 						'L' for left); must not be null; must be 'R' or 'L'.
	 * 
	 * @return				the array with elements shifted cyclically, according to the specified
	 * 						parameters.
	 */
	public static int[] shiftArrayCyclic(int[] array, int move, char direction) {
		/**
		 * We can return the given array without a change if we move=0, thus no moves are needed, if the
		 * moves aren't to the right (thus 'R') or left (thus 'L'), and if the given array is either empty or
		 * consists of a single element.
		 * If we received a negative number of moves, we will have to shift the array -move times in the
		 * opposite direction, thus we need to update our move to be positive and switch our direction.
		 */
		if (move == 0 || (direction != 'R' && direction != 'L') || array.length == 0 || array.length == 1) {
			return array;
			}
		else if (move < 0) {
			move = - move;
			direction = SwitchDirection(direction);
		}
		
		/**
		 * If move>array.length, we just need to make as many moves as would be left after doing array.length
		 * moves, thus update move to be more accurate.
		 * No need to have code duplications, thus we only need to create a method to shift our array to the
		 * right, and if we have to shift our array to the left we could do so by mirroring our array, then
		 * shifting our new array to the right 'move' times, and finally mirroring it back.
		 */
		move = move % array.length;
		if (direction == 'R') { array = ShiftArray(array, move, direction); }
		else if (direction == 'L') {
			array = MirrorArray(array);
			array = ShiftArray(array, move, direction);
			array = MirrorArray(array);
		}
		
		return array;
	}
	
	
	
	
	// We finished the first method we wanted to implement, this is just a divider between both methods :)
	
	
	
	
	/**
	 * Checks if the given element is in the given array.
	 * 
	 * @param current_path		the array we would check whether the element is in or not; must not be null;
	 * 							must consist of non-negative integers.
	 * 
	 * @param k					the element we would search for in the given array; must not be null; must be
	 * 							a non-negative integer.
	 * 
	 * @return					a boolean value that represents if the element is in the array or not,
	 * 							according to the specified parameters.
	 */
	private static Boolean kInCurrentPath(int[] current_path, int k) {
		/**
		 * We need to ensure we aren't going through a vertex we already visited in our current path, thus
		 * ensuring we avoid cycles which will cause an endless recursion.
		 */
		for (int i = 0; i < current_path.length; i++) { if (current_path[i] == k) { return true; } }
		return false;
	}
	
	
	/**
	 * Creates a new array that is longer by 1 element than a given element, which is identical to the given
	 * array up to the second to last element.
	 * 
	 * @param current_path		the array which would be expanded by a single element; must not be null; must
	 * 							consist of non-negative integers.
	 * 
	 * @return					a new array that is 1 element longer than the given array, and that is
	 * 							identical to the given element up to the second to last element.
	 */
	private static int[] Plus1Path(int[] current_path) {
		/**
		 * We might need to create an array that is only 1 element longer to represent the path we would take
		 * should we take the path we currently took and then took another "step" to another vertex in the
		 * graph.
		 * Yes, this pretty much destroys the entire method's time complexity, but we want to keep using
		 * arrays and I don't want to create an array of Boolean values to represent which vertices we
		 * visited and another array of integers to represent the possible vertices or something of the
		 * sort... It just felt like it would be easier to create memorization this way.
		 */
		int len = current_path.length;
		int[] new_path = new int[len + 1];
		for (int i = 0; i < len; i++) { new_path[i] = current_path[i]; }
		return new_path;
	}
	
	
	/**
	 * Finds the length of the shortest path between 1 vertex to the other in a given 2-dimensional array
	 * that represents a directional graph. Does not support i<0, j<0, i>=m.length, j>=m.length, i=j.
	 * 
	 * @param m					the 2-dimensional array that represents a directional graph; must not be
	 * 							null; each inner array must consist of 0s and 1s (m[k][l]=0 represents that
	 * 							there isn't an edge from k to l, and m[k][l]=1 represents that there is an
	 * 							edge from k to l).
	 * 
	 * @param i					the starting point vertex; must not be null; must be a non-negative integer;
	 * 							must not be greater than m.length.
	 * 
	 * @param j					the ending point vertex; must not be null; must be a non-negative integer;
	 * 							must not be greater than m.length.
	 * 
	 * @param current_path		an array representing the vertices already visited in the current attempt to
	 * 							reach the ending point vertex from the starting point vertex; must not be
	 * 							null; must consist of non-negative integers.
	 * 
	 * @param min_len			the length of the shortest found path from the starting point vertex to the
	 * 							ending point vertex that; must not be null, must be either -1 or a positive
	 * 							integer.
	 * 
	 * @return					the length of the shortest path from the starting point vertex to the ending
	 * 							point vertex in a directional graph, according to the specified parameters;
	 * 							if there isn't such a path, return -1;
	 */
	private static int PathLen(int[][] m, int i, int j, int[] current_path, int min_len) {
		/**
		 * We will use this function to find the length of the shortest path between the given i and the
		 * given j. By using memorization to ensure we won't go through the same vertex twice in a single
		 * path we would ensure we aren't going through cycles in the graph, which will cause a longer path
		 * and a possibility for endless recursion. By also remembering our current minimal path length
		 * between i and j, we could simply validate if we found a shorter path as we find a new path between
		 * i and j.
		 * We would simply check if there is a direct path between i and j at first, as we would not be
		 * given i or j that are out of bound (this would first be validated using the 'findShortestPath'
		 * method, and then maintained by using a recursion over a given i that exists within m).
		 * By validating this simple condition we would ensure we would never reach a point where we are
		 * given i=j. Therefore our recursion would be using 'k' that we haven't yet visited in our current
		 * path, that is within m, and that isn't j.
		 */
		int n = m.length;
		if (m[i][j] == 1) { return 1; }
		for (int k = 0; k < n; k++) {
			if (!kInCurrentPath(current_path, k)) {
				if (k != j) {
					if (m[i][k] == 1) {
						int[] k_path = Plus1Path(current_path);
						k_path[current_path.length]= k;
						int k_len = PathLen(m, k, j, k_path, min_len);
						/**
						 * Dealing with a case where we found a path to j:
						 * If we haven't yet found a path to j (thus min_len = -1), then we need to update
						 * our min_len to be the length of the path we've taken until now (minus 1 since we
						 * started a path with the first i), add 1 (the length of the path from the current i
						 * to k), and then add the length of the path from k to j (thus k_len).
						 * If we've already found a path to j in the past, we'd already have min_len != -1,
						 * thus we just need to validate if the length of path we found to j (thus
						 * k_len != -1) is shorter than the current minimal length of a path from i to j. As
						 * explained, the length of the path we just found is the length of the current
						 * path (minus 1 since we started a path with the first i) plus 1 (the length of the
						 * path from the current i to k) plus k_len. If we indeed found a shorter path to j,
						 * we just need to update our min_len.
						 */
						if (m[k][j] == 1 && k_len != -1 && (min_len == -1 ||
								(k_len + current_path.length) < min_len)) {
								min_len = k_len + current_path.length;
								}
						else if ((k_len < min_len || min_len == -1) && k_len != -1) {
							min_len = k_len;
							}
					}
				}
			}
		}
		return min_len;
	}
	
	
	/**
	 * Finds the length of the shortest path between 1 vertex to the other in a given 2-dimensional array
	 * that represents a directional graph. Supports i<0, j<0, i>=m.length, j>=m.length, i=j.
	 * 
	 * @param m			the 2-dimensional array that represents a directional graph; must not be null; each
	 * 					inner array must consist of 0s and 1s (m[k][l]=0 represents that there isn't an edge
	 * 					from k to l, and m[k][l]=1 represents that there is an edge from k to l).
	 * 
	 * @param i			the starting point vertex; must not be null; must be an integer.
	 * 
	 * @param j			the ending point vertex; must not be null; must be must be an integer.
	 * 
	 * @return			the length of the shortest path from the starting point vertex to the ending point
	 * 					vertex in a directional graph, according to the specified parameters; if there isn't
	 * 					such a path, return -1; if the starting point is the same as the ending point, return
	 * 					0; if either the starting point vertex or the ending point vertex are negative or
	 * 					greater than m.length, return -1 (as there isn't a possibility for such a path).
	 */
	public static int findShortestPath(int[][] m, int i, int j) {
        /** 
         * We have a problem if either i or j are out of bounds, thus there is no doubt that
         * there isn't a path between i and j.
         * We can also make sure immediately that we aren't looking for a path from one vertex to itself,
         * such a path's length is 0.
         * Finally we can check if there's a direct path between i and j, if there is we can return 1
         * immediately without going through too many iterations.
         */
        if (i < 0 || j < 0 || i >= m.length || j >= m.length) { return -1; }
        else if (i == j) { return 0; }
        else if (m[i][j] == 1) { return 1; }
        
        int res = PathLen(m, i, j, new int[]{i}, -1);
        return res;
    
	}
}
