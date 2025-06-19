
public class StringUtils {
	

	/**
	 * Deleting an element in a specific index from an array, and returning an array without said
	 * element that is 1-element shorter than the given array.
	 * 
	 * @param str_array		the array which will be shortened; must not be null; must consist of
	 * 						strings.
	 * 
	 * @param index			the index of the element which will be deleted; must not be null;
	 * 						must not be a non-negative integer; must not be greater than the
	 * 						length of the array.
	 * 
	 * @return				an array that is 1-element shorter than the given array, which includes
	 * 						all the elements in the given array besides the element in the given
	 * 						index, in the same order in which they were in the given array.
	 */
	private static String[] Minus1Array(String[] str_array, int index) {
		int len = str_array.length;
		String[] new_array = new String[len - 1];
		for (int i = 0; i < index; i++) { new_array[i] = str_array[i]; }
		for (int j = index; j < (len - 1); j++) { new_array[j] = str_array[j + 1]; }
		return new_array;
	}
	
	
	/**
	 * Creates a new array that is longer by 1 element than a given element, which is identical to the given
	 * array up to the second to last element.
	 * 
	 * @param current_path		the array which would be expanded by a single element; must not be null; must
	 * 							consist of strings.
	 * 
	 * @return					a new array that is 1 element longer than the given array, and that is
	 * 							identical to the given element up to the second to last element.
	 */
	private static String[] Plus1Sequence(String[] current_sequence) {
		/**
		 * We might need to create an array that is only 1 element longer to represent the word sequence
		 * we would receive should we find out the current sequence could be expanded with the next
		 * string in the originally given string (given to the 'findSortedSequence' method).
		 */
		int len = current_sequence.length;
		String[] new_sequence = new String[len + 1];
		for (int i = 0; i < len; i++) { new_sequence[i] = current_sequence[i]; }
		return new_sequence;
	}
	
	
	/**
	 * Validating if two strings are properly lexicographically sorted.
	 * 
	 * @param previous_str		the first string, should come before the second lexicographically; must
	 * 							not be null.
	 * 
	 * @param current_str		the second string, should come after the first lexicographically; must
	 * 							not be null.
	 * 
	 * @return					true if the strings are properly lexicographically sorted and false if not,
	 * 							according to the specified parameters.
	 */
	private static boolean LexicographicallySorted(String previous_str, String current_str) {
		/**
		 * The easiest thing to check is if we're looking on 2 identical strings, if we are - by definition
		 * they are properly lexicographically sorted.
		 */
		if (current_str.equals(previous_str)) { return true; }
		
		int min_len = Math.min(previous_str.length(), current_str.length());
		boolean same_letters_up_to_min_len = true;
		for (int i = 0; i < min_len; i++) {
			char previous_char = previous_str.charAt(i);
			char current_char = current_str.charAt(i);
			if (previous_char > current_char) { return false; }
			if (same_letters_up_to_min_len) {
				if (previous_char < current_char) { return true; }
				if (previous_char != current_char) { same_letters_up_to_min_len = false; }
			}
		}
		
		/**
		 * So far we've made sure that up to the minimal length of both string, the characters in the current string
		 * are properly lexicographically sorted. If that is the case, and the previous string is longer than the
		 * current string, that means that the current string comes before the previous string lexicographically,
		 * thus the strings are not properly lexicographically sorted.
		 */
		if (same_letters_up_to_min_len) { if (previous_str.length() > current_str.length()) { return false; } }
		
		
		return true;
	}
	
	
	/**
	 * Finding the longest lexicographically sorted sequence of strings in an array. Does not support empty
	 * arrays or a non-positive integer.
	 * 
	 * @param str_array						the array of strings where we would look for the longest sequence;
	 * 										must not be null; must consist of non-empty strings.
	 * 
	 * @param current_max_sequence			an array representing the currently found longest
	 * 										lexicographically sorted sequence; must not be null; must consist
	 * 										of non-empty strings.
	 * 
	 * @param current_sequence				an array representing the currently found lexicographically sorted
	 * 										sequence; must not be null; must consist of non-empty strings.
	 * 
	 * @param index							an integer representing the index of the string which will be
	 * 										validated for being lexicographically sorted in relation to the
	 * 										previous string in str_array; must not be null; must be a positive
	 * 										integer; must not be greater than the length of str_array.
	 * 
	 * @return								the longest lexicographically sorted sequence of strings in a given
	 * 										array, according to the specified parameters.
	 */
	private static String[] FindMaxSequence(String[] str_array, String[] current_max_sequence,
			String[] current_sequence, int index) {
		if (LexicographicallySorted(str_array[index -1], str_array[index])) {
			String[] bigger_sequence = Plus1Sequence(current_sequence);
			bigger_sequence[current_sequence.length]= str_array[index]; 
			if (bigger_sequence.length >= current_max_sequence.length) {
				current_max_sequence = bigger_sequence.clone();
			}
			if ((index + 1) < str_array.length) {
				String[] bigger_max_sequence = FindMaxSequence(str_array, current_max_sequence,
						bigger_sequence, index + 1);
				if (bigger_max_sequence.length >= current_max_sequence.length) {
					current_max_sequence = bigger_max_sequence;
				}
			}
		}
		else // str_array[index] comes before str_array[index - 1] lexicographically
		{
			String[] smaller_sequence = new String[] {str_array[index] };
			if (smaller_sequence.length >= current_max_sequence.length) {
				current_max_sequence = smaller_sequence.clone();
			}
			if ((index + 1) < str_array.length) {
				String[] smaller_max_sequence = FindMaxSequence(str_array, current_max_sequence,
						smaller_sequence, index + 1);
				if (smaller_max_sequence.length >= current_max_sequence.length) {
					current_max_sequence = smaller_max_sequence;
				}
			}
		}
		return current_max_sequence;
	}
	
	
	/**
	 * Finding and returning the longest sequence of properly lexicographically sorted sub-strings in a
	 * given string. Supports an empty string.
	 * 
	 * @param str		a string where we will look for a sequence of lexicographically sorted "words"
	 * 					(actually sub-strings of str divided by a space); must not be null; there must not
	 * 					be 2 or more spaces one after the other without at least 1 letter separating them;
	 * 					each sub-string must consist of nothing but lower-case letters.
	 * 
	 * @return			the longest lexicographically sorted sequence of sub-strings, according to the
	 * 					specified parameters.
	 */
	public static String findSortedSequence(String str) {
		if (str.equals("")) { return ""; }
		
		String[] str_array = str.split(" ");
		// Deleting empty strings from str_array:
		for (int i = 0; i < str_array.length; i++) {
			if (str_array[i].equals("")) { str_array = Minus1Array(str_array, i); }
		}
		/**
		 * Validating that we weren't given a string that is composed of spaces alone or one that has only
		 * 1 sub-string:
		 */
		if (str_array.length == 0) { return ""; }
		if (str_array.length == 1) { return str_array[0]; }
		
		String[] max_sequence = FindMaxSequence(str_array, new String[] {str_array[0]},
				new String[] {str_array[0]}, 1);
		String res = String.join(" ", max_sequence);
		return res;
	}
	
	
	
	
	// We finished the first method we wanted to implement, this is just a divider between both methods :)
	
	
	
	
	/**
	 * Checking if the maximal amount of changes needed to ensure that the 2 given strings would become the
	 * same is 1 at most, where one of the given strings is longer than the other by 1 element.
	 * 
	 * @param a		the longer string; must not be null; must be 1 element longer than the other given
	 * 				string;
	 * 
	 * @param b		the shorter string; must not be null; must be 1 element shorter than the other given
	 * 				string.
	 * 
	 * @return		true if the maximal amount of changes needed to ensure that the 2 given strings would
	 * 				become the same is 1 at most, and false otherwise, given the specified parameters.
	 */
	private static boolean OneIsLonger(String a, String b) {
		int min_len = b.length();
		int index_offset = 0;
		int differences_number = 0;
		for (int i = 0; i < min_len; i++) {
			char a_char = a.charAt(i + index_offset);
			char b_char = b.charAt(i);
			if (b_char != a_char) {
				if (index_offset >= 1) { return false; }
				index_offset += 1;
				a_char = a.charAt(i + index_offset);
				if (b_char != a_char) {
					if (differences_number >= 1) { return false; }
					differences_number += 1;
					index_offset -= 1;
				}
			}
		}
		if (index_offset > 1 || differences_number > 1) { return false; }
		
		return true;
	}
	
	
	/**
	 * Checking if the maximal amount of changes needed to ensure that the 2 given strings would become the
	 * same is 1 at most, where both given strings are of the same length.
	 * 
	 * @param a		the first given string; must not be null; must exactly be as long as the second given
	 * 				string;
	 * 
	 * @param b		the second given string; must not be null; must exactly be as long as the first given
	 * 				string;
	 * 
	 * @return		true if the maximal amount of changes needed to ensure that the 2 given strings would
	 * 				become the same is 1 at most, and false otherwise, given the specified parameters.
	 */
	private static boolean SameLength(String a, String b) {
		int len = a.length();
		int differences_number = 0;
		for (int i = 0; i < len; i++) {
			char a_char = a.charAt(i);
			char b_char = b.charAt(i);
			if (a_char != b_char) {
				if (differences_number >= 1) { return false; }
				differences_number += 1;
			}
		}
		if (differences_number > 1) { return false; }
		
		return true;
	}
	
	
	/**
	 * Checking if the maximal amount of changes needed to ensure that the 2 given strings would become the
	 * same is 1 at most. Supports empty strings.
	 * 
	 * @param a		the first given string; must not be null.
	 * 
	 * @param b		the second given string; must not be null.
	 * 
	 * @return		true if the maximal amount of changes needed to ensure that the 2 given strings would
	 * 				become the same is 1 at most, and false otherwise, given the specified parameters.
	 */
	public static boolean isEditDistanceOne(String a, String b){
		int max_len = Math.max(a.length(), b.length());
		int min_len = Math.min(a.length(), b.length());
		if ((max_len - min_len) > 1) { return false; }
		if ((max_len - min_len) == 1) {
			if (min_len == 0) { return true; }
			else if (a.length() > b.length()) { return OneIsLonger(a, b); }
			else // b.length() > a.length()
			{ return OneIsLonger(b, a); }
		}
		else // max_len == min_len
		{
			if (max_len == 0) { return true; }
			else { return SameLength(a, b); }
		}
	}
	
}
