	package il.ac.tau.cs.sw1.ex4;

	

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;



public class BigramModel {
	public static final int MAX_VOCABULARY_SIZE = 14500;
	public static final String VOC_FILE_SUFFIX = ".voc";
	public static final String COUNTS_FILE_SUFFIX = ".counts";
	public static final String SOME_NUM = "some_num";
	public static final int ELEMENT_NOT_FOUND = -1;
	
	String[] mVocabulary;
	int[][] mBigramCounts;
	
	
	
	// DO NOT CHANGE THIS !!! (I changed only the comments)
	/**
	 * @pre: 'fileName' must not be null.
	 * @pre: 'fileName' is a legal file path.
	 * @pre: 'fileName' must be a name of a .txt file.
	 * 
	 * @param 'fileName': A string representing the name of the file from which we will build our vocabulary
	 * and our 2-dimensional array. 
	 * 
	 * @post: fileName = previous(fileName).
	 */
	public void initModel(String fileName) throws IOException{
		mVocabulary = buildVocabularyIndex(fileName);
		mBigramCounts = buildCountsArray(fileName, mVocabulary);
		
	}
	
	
	
	/**
	 * @pre: for (element : array) { element.getClass() == java.lang.String; }, but 'array' could be an empty
	 * array.
	 * 
	 * @pre: string != null.
	 * 
	 * @param 'array': The array in which we will search for the given string.
	 * @param 'string': The string we will search for in the given array.
	 * 
	 * @return: The index of 'string' in 'array' if 'array' contains 'string', and -1 otherwise.
	 * 
	 * @post: (array = previous(array) && string = previous(string)).
	 * 
	 * This method's worst-case time complexity is O(array.length * m), where m represent the maximum between
	 * the maximal length of an element in the given array, and the length of the given string.
	 */
	private int arrayContains(String[] array, String string) {
	// The worst-case scenario in this method is when 'string' isn't in 'array'.
		
		if (array.length == 0) {	// O(1) time complexity to validate this condition.
			
			return ELEMENT_NOT_FOUND;		// O(1) time complexity. Won't be reached in the worst-case.
			
		// Thus the overall worst-case time complexity of this 'if' loop is O(1).
		}
		
		int array_length = array.length;	// O(1) time complexity.
		
		for (int i = 0; i < array_length; i++) {		// O(1) time complexity in order to calculate each
		// integer 'i'. There are array.length such integers 'i', thus the overall worst-case time complexity
		// for getting elements from the array is O(1) * array.length = O(array.length).
			
			// Denote m = max(the maximal length of an element in 'array', string.length())
			if (array[i].equals(string)) {	// O(m) time complexity at worst-case scenario.
				
				return i;	// O(1) time complexity. Won't be reached in the worst-case.
				
			// Thus the overall worst-case time complexity of this 'if' loop is O(m).
			}
		
		// Thus the overall worst-case time complexity of this 'for' loop is:
		// O(array.length) * O(m) = O(array.length * m).
		}
		
		return ELEMENT_NOT_FOUND;	// O(1) time complexity.
		
	// Thus this method's worst-case time complexity is:
	// O(1) + O(1) + O(array.length * m) + O(1) = O(array.length * m).
	}
	
	
	
	/**
	 * @pre: character != null.
	 * 
	 * @param 'character': A character to be checked.
	 * 
	 * @return: A boolean value that represents if a given character is a lower-case letter.
	 * 
	 * @post: character = previous(character).
	 * 
	 * This method's worst-case time complexity is O(1).
	 */
	private boolean isLowerCase(char character) {
	// The worst-case scenario in this method is when 'character' isn't a lower-case letter.
		
		char[] legal = new char[] {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',  'k', 'l', 'm', 'n',
				'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};	// O(1) time complexity.
		for (char element : legal) {	// O(1) time complexity to get an element from an array. There are 26
		// such elements, thus the worst-case time complexity for getting elements from the array is:
		// O(1) * 26 = O(1).
			
			if (element == character) {		// O(1) time complexity to validate this.
				
				return true;	// O(1) time complexity. Won't be reached in the worst-case.
				
			// Thus the overall worst-case time complexity of this 'if' loop is O(1).
			}
		
		// Thus the overall worst-case time complexity of this 'for' loop is O(1) * O(1) = O(1).
		}
		
		return false;		// O(1) time complexity.
		
	// Thus this method's overall worst-case time complexity is O(1) + O(1) + O(1) = O(1).
	}
	
	
	
	/**
	 * @param 'string': The string that would be checked for its legality (as explained above).
	 * 
	 * @return: "legal word" if the given string is a legal word. A legal word must contain at least 1 
	 * letter; "SOME_NUM" if the given string is a number. A number must consist of digits alone (no decimal
	 * points, minus or plus signs, etc); "illegal" if the given isn't a legal word or a number.
	 * 
	 * @post: string = previous(string).
	 * 
	 * This method's worst-case time complexity is O(string.length()).
	 */
	private String isLegalWord(String string) {
	// The worst-case scenario for this method is when every character in 'string' is a digit, besides the
	// very last. It doesn't matter if the last character of 'string' is a letter or not (same time
	// complexity for both cases).
		
		int str_len = string.length();		// O(1) time complexity.
		boolean is_some_num = true;		// O(1) time complexity.
		boolean is_word = false;	// O(1) time complexity.
		
		for (int i = 0; i < str_len; i++) {		// O(1) time complexity to calculate each i and ensuring it
		// meets the given conditions. There are string.length() such integers 'i', thus the worst-case
		// time complexity for calculating those integers 'i' is O(1) * string.length() = O(string.length()).
		// Note that the number of integers 'i' which would invoke each inner loop is already calculated in
		// each of the inner loops' worst-case time complexity, thus we need to multiply each of these time
		// complexities by O(1) and not by O(string.length()) when calculating this loop's overall worst-case
		// time complexity.
			
			char ch = string.charAt(i);		// O(1) time complexity.
			
			if (is_some_num) {		//  O(1) time complexity to validate this condition.
				
				if (!Character.isDigit(ch)) {	// O(1) time complexity to validate this condition.
					
					is_some_num = false;	//  O(1) time complexity.
					
				//	Thus the overall time complexity of this 'if' loop is O(1) + O(1) = O(1).
				}
				
			// The the overall worst-case time complexity of this 'if' loop is:
			// ((string.length() - 1) * O(1)) + O(1) = O(string.length()).
			}
			
			if (!is_word && !is_some_num) {	// O(1) time complexity to validate this condition.
				
				if (isLowerCase(Character.toLowerCase(ch))) {	// O(1) time complexity to transfer a character
				// to a corresponding lower-case character. isLowerCase's worst-case time complexity is O(1).
				// Thus the worst-case time complexity of validating this condition is O(1) + O(1) = O(1).
						
					return "legal word";	// O(1) time complexity.
						
				// Thus the overall time complexity of this 'if' loop is O(1) + O(1) = O(1).
				}
				
			// Thus the overall worst-case time complexity of this 'else if' loop is (1 * O(1)) + O(1) = O(1).
			}
			
		// Thus the overall worst-case time complexity of this 'for' loop is:
		// O(1) * ( O(string.length()) + O(1) ) = O(string.length()).
		}
		
		if (is_some_num) {		// O(1) time complexity to validate this condition.
			
			return "SOME_NUM";		// O(1) time complexity.
			
		// Thus the overall worst-case scenario of this 'if' loop is O(1) + O(1) = O(1).
		}
		
		return "illegal";		// O(1) time complexity.
		
	// Thus this method's overall worst-case time complexity is:
	// O(1) + O(1) + O(1) + O(string.length()) + O(1) = O(string.length()).
	}
	
	
	
	/**
	 * @pre: array != null.
	 * @pre: for (element : array) { element.getClass() == java.lang.String; }.
	 * 
	 * @param 'array': The array that'll be copied and expanded by a single element.
	 * 
	 * @return: A new array that is a copy of the given element, with an addition of a single element, which
	 * would be null.
	 * 
	 * @post: k = previous(k).
	 * 
	 * This method's worst-case time complexity is O(array.length + k).
	 */
	private String[] Plus_k_Array(String[] array, int k) {
	// The worst-case scenario in this method is when ((array_len % 2) == 0) and (array_len != 0).
		
		if (array.length == 0) {	// O(1) time complexity to validate this condition.
			
			return new String[k];	// O(k) time complexity. Won't be reached in the worst-case.
			
		// Thus the overall worst-case time-complexity of this 'if' loop O(1).
		}
		
		int array_len = array.length;	// O(1) time complexity.
		String[] new_array = new String[array_len + k];		// This is an array of  strings, which are
		// immutable. Furthermore, we need to allocate memory for (array.length + k) elements (all of which
		// would be null). Thus this line's time complexity is O(array.length + k).
		int mid = (int) array_len / 2;		// O(1) time complexity.
		
		for (int i = 0; i < mid; i++) {		// O(1) time complexity for calculating each integer 'i' and
		// ensuring it meets the given conditions. There are (array.length / 2) such integers 'i', thus the
		// overall time complexity for calculating those integers 'i' is O(1) * (array.length / 2) =
		// = O(array.length).
			
			new_array[i] = array[i];	// O(1) time complexity.
			new_array[array_len - 1 - i] = array[array_len - 1 - i];	// O(1) time complexity.
			
		// Thus the overall time complexity of this 'for' loop is:
		// O(array.length) * ( O(1) + O(1) ) = O(array.length).
		}
		
		if ((array_len % 2) == 1) {		// O(1) time complexity for ensuring this condition.
			
			new_array[mid] = array[mid];		// O(1) time complexity.
		
		// Thus the overall time complexity of this 'if' loop is O(1) * O(1) = O(1).
		}
		
		return new_array;	// O(1) time complexity.
		
	// Thus this method's overall worst-case time complexity is:
	// O(1) + O(array.length + k) + O(1) + O(array.length) + O(1) + O(1) = O(array.length + k).
	}
	
	
	
	/**
	 * @pre: vocabulary != null.
	 * @pre: for (String element : vocabulary) { element.getClass() == java.lang.String; }, but 'vocabulary'
	 * could be an empty array.
	 * @pre: line != null.
	 * 
	 * @param 'vocabulary': The array to which we will add new elements.
	 * @param 'line': The string in which we will search for new elements to add to the array.
	 * 
	 * @return: An array containing all the elements in the given array in the same order up until the
	 * element in the ((given array).length - 1)th index, and where all the elements from that index to the
	 * end of the array are new elements derived from the given string.
	 * 
	 * @post: line = previous(line).
	 * 
	 * This method's worst-case time complexity is O(line.length() * vocabulary.length * m).
	 */
	private String[] generateVocabulary(String[] vocabulary, String line) {
	// The worst-case scenario in this method is when 'line' is composed of single-character sub-strings,
	// divided by a single whitespace, where each of them is a letter which isn't an element of 'vocabulary'
	// when turned into its lower-case corresponding character. Thus 'line' would contain (line.length() / 2)
	// such sub-strings. In addition, it is also important to note that in this worst-case scenario 
	// vocabulary.length subsists (vocabulary.length + (line.length() / 2)) <= MAX_VOCABULARY_SIZE, thus
	// implying that the vocabulary would be able to receive (line.length() / 2) new elements.
		
		String[] line_array = line.trim().split("\\s+");	// line.trim()'s time complexity is:
		// O(line.length()). In addition, split("\\s+")'s time complexity is also O(line.length()). Thus this
		// line's time complexity is O(line.length()) + O(line.length()) = O(line.length()).
		boolean contains_some_num = false;		// O(1) time complexity.
		
		for (String string: line_array) {	// O(1) time complexity to get a string from 'line_array'. Thus
		// the worst-case (as described above) time complexity for getting strings from 'line_array' is:
		// O(1) * (line.length() / 2) = O(line.length()).
			
			if (vocabulary.length <= MAX_VOCABULARY_SIZE) {	// O(1) time complexity to validate this
				// condition.
				
				if (isLegalWord(string).equals("SOME_NUM")) {	// Note that the time complexity of
				// validating this condition is O(max(string.length(), "SOME_NUM".length()). Thus the
				// worst-case (as described above) time complexity of validating this condition is:
				// O(max(1, 8)) = O(1).
					
					if (!contains_some_num) {	// O(1) time complexity to validate this condition. Won't
						// be reached in the worst-case (as described above).
							
							vocabulary = Plus_k_Array(vocabulary, 1);	// Plus_k_Array's worst-case time 
							// complexity is O(vocabulary.length + k). Here k == 1, thus this line's time
							// complexity is O(vocabulary.length + 1) = O(vocabulary.length). Won't be
							// reached in the worst-case (as described above).
							vocabulary[vocabulary.length - 1] = SOME_NUM;	// O(1) time complexity. Won't be
							// reached in the worst-case (as described above).
							contains_some_num = true;	// O(1) time complexity. Won't be reached in the
							// worst-case (as described above).
						
						// Thus the overall time complexity of this 'if' loop is:
						// O(1) + O(vocabulary.length) + O(1) + O(1) = O(vocabulary.length). This entire 'if'
						// loop won't be reached in the worst-case (as described above).
						}
					
				// Thus the overall worst-case (as described before) time complexity of this 'if' loop
				// is O(1).
				}
				
				// Denote m = max(the maximal length of an element in 'vocabulary',
				// string.toLowerCase().length()). In the worst-case (as described above),
				// (string.toLowerCase().length() == 1), thus m >= 1.
				else if (isLegalWord(string).equals("legal word")) {	// Note that the time complexity of
				// validating this condition is O(max(string.length(), "legal word".length()). Thus the
				// worst-case (as described above) time complexity of validating this condition is:
				// O(max(1, 10)) = O(1).
					
					if (arrayContains(vocabulary, string.toLowerCase()) == ELEMENT_NOT_FOUND) {	// Note that
					// string.toLowerCase()'s worst-case time complexity is O(string.length()).
					// arrayContains's worst-case time complexity is O(vocabulary.length * m). Thus the
					// worst-case (as described above) time complexity for validating this condition is (note
					// that string.length() == 1):
					// O(string.length()) + O(vocabulary.length * m) = O(vocabulary.length * m).
							
							vocabulary = Plus_k_Array(vocabulary, 1);	// Plus_k_Array's worst-case time
							// complexity is O(vocabulary.length + k). Here k == 1, thus the time complexity
							// for validating this condition is O(vocabulary.length + 1) =
							// = O(vocabulary.length).
							vocabulary[vocabulary.length - 1] = string.toLowerCase();	// Note that
							// string.toLowerCase()'s worst-case time complexity is O(string.length()). Thus
							// this line's worst-case (as described above) time complexity is:
							// O(string.length()) = O(1).
						
						// Thus the overall time complexity of this 'if' loop is:
						// O(vocabulary.length * m) + O(vocabulary.length) + O(1) = O(vocabulary.length * m).
						}
					
				// Thus the overall worst-case (as described above) time complexity of this 'else if' loop
				// is O(1) + O(vocabulary.length * m) = O(vocabulary.length * m).
				}
				
			// Thus the overall worst-case (as described above) time complexity of this 'if' loop is:
			// O(1) + O(1) + O(vocabulary.length * m) = O(vocabulary.length * m).
			}
			
		// Thus the overall worst-case (as described above) time complexity of this 'for' loop is:
		// O(line.length()) * O(vocabulary.length * m) = O(line.length() * vocabulary.length * m).
		}
		
		return vocabulary;		// O(1) time complexity.
		
	// Thus this method's overall worst-case (as described above) time complexity is:
	// O(line.length()) + O(1) + O(line.length() * vocabulary.length * m) + O(1) =
	// = O(line.length() * vocabulary.length * m).
	}
	
	
	
	/**
	 * @pre: 'fileName' must not be null.
	 * @pre: 'fileName' is a legal file path.
	 * @pre: 'fileName' must be a name of a .txt file.
	 * 
	 * @param 'fileName': A string representing the name of the file from which we will build our vocabulary.
	 * 
	 * @return: An array that represents up to 'MAX_VOCABULARY_SIZE' different words in the given file.
	 * 
	 * @post: mVocabulary = previous(mVocabulary).
	 * @post: mBigramCounts = previous(mBigramCounts).
	 * @post: fileName = previous(fileName).
	 * 
	 * This method's worst-case time complexity is O((n * t)^2), where 'n' represents the number of lines in
	 * 'fileName', and where 't' represents the maximal number of characters in a line within 'fileName'.
	 */
	public String[] buildVocabularyIndex(String fileName) throws IOException{ // Q - 1
	// The worst-case scenario in this method is when each line within the file is composed of
	// single-character sub-strings, divided by a single whitespace, where each of them is a letter which
	// isn't an element of the vocabulary we had up until that moment when turned into its lower-case
	// corresponding character. Thus each line would contain (line.length() / 2) such sub-strings. In addition,
	// it is important to note that in this worst-case scenario, if we denote n = (number of lines in
	// 'fileName'), and t = (maximal number of characters in a line within 'fileName'), then the following
	// subsists: (n * (t / 2)) <= MAX_VOCABULARY_SIZE, thus implying that we could create a vocabulary that
	// includes every sub-string of every line within 'filename'.
		
		File file = new File(fileName);		// O(1) time complexity.
		BufferedReader buffered_reader = new BufferedReader(new FileReader(file));		// O(1) time
		// complexity.
		String[] vocabulary = new String[0];	// O(1) time complexity.
		String line = buffered_reader.readLine();	// O(line.length()) = O(t) time complexity.
		
		while (line != null && vocabulary.length <= MAX_VOCABULARY_SIZE) {		// O(1) time complexity to
		// validate these conditions. This loop would repeat itself n times at the worst-case (as described
		// above), thus we can claim that the worst-case (as described above) time complexity for validating
		// these conditions is O(1) * n = O(n).
			
			// Denote m = max(the maximal length of an element in 'vocabulary', 1). Note that in the
			// worst-case (as described above), m == 1.
			vocabulary = generateVocabulary(vocabulary, line);	// generateVocabulary's worst-case time
			// complexity is O(line.length() * vocabulary.length * m). Thus this line's time complexity
			// at the worst-case (as described above) is O(line.length() * vocabulary.length * m) =
			// = O(t * vocabulary.length). We also know that vocabulary.length <= (n * (t / 2)), thus this
			// line's worst-case time complexity would be O(t * (n * (t / 2))) = O(n * t^2).
			line = buffered_reader.readLine();		// O(line.length()) = O(t) time complexity.
			
		// Thus the overall worst-case (as described above) time complexity of this 'while' loop is:
		// O(n) * ( O(n * t^2) + O(t) ) = O((n * t)^2).
		}
		
		buffered_reader.close();	// O(1) time complexity.
		return vocabulary;		// O(1) time complexity.
		
	// Thus this method's overall worst-case (as described above) time complexity is:
	// O(1) + O(1) + O(1) + O(t) + O((n * t)^2) + O(1) + O(1) = O((n * t)^2).
	}

	
	
	// FROM HERE ON OUT I DID NOT ANALYZE THE METHODS' TIME COMPLEXITY!!
	
	
	
	/**
	 * @pre: CountsArray.length == vocabulary.length .
	 * @pre: for (array : CountsArray) { array.length == vocabulary.length }.
	 * 
	 * @param 'CountsArray': A 2-dimensional array containing, for every word in a vocabulary, a count of how
	 * many times each word in the vocabulary appeared immediately after it. Would be updated in accordance
	 * with each time this method would operate.
	 * @param 'vocabulary': An array containing a vocabulary of words.
	 * @param 'line': A string containing words to be validated as part of the vocabulary.
	 * @param 'contains_some_num': A boolean value that represents if the vocabulary contains SOME_NUM.
	 * @param 'some_num_index': An integer representing in which index does the vocabulary contain SOME_NUM.
	 * This parameter would be -1 if (contains_some_num == false).
	 * 
	 * @return: An updated version of 'CountsArray', after counting, for each element 'word1' of
	 * 'vocabulary', the number of times each element 'word2' of 'vocabulary' appeared right after 'word1'
	 * in 'line', and adding this result to the previous number of times this occurred (as it was in
	 * 'CountsArray' before the method began).
	 * 
	 * @post: vocabulary = previous(vocabulary).
	 * @post: line = previous(line).
	 * @post: contains_some_num = previous(contains_some_num).
	 * @post: some_num_index = previous(some_num_index).
	 */
	private int[][] countWordsInLine(int[][] CountsArray, String[] vocabulary, String line,
			boolean contains_some_num, int some_num_index) {
		String[] line_array = line.trim().split("\\s+");
		int line_length = line_array.length;
		
		for (int i = 0; i < (line_length - 1); i++) {
			if (isLegalWord(line_array[i]).equals("illegal")) { continue; }
			int word1_index = ELEMENT_NOT_FOUND;
			int word2_index = ELEMENT_NOT_FOUND;
			if (isLegalWord(line_array[i]).equals("SOME_NUM")) {
				if (contains_some_num) {
					word1_index = some_num_index;
					if (isLegalWord(line_array[i + 1]).equals("SOME_NUM")) {
						word2_index = some_num_index;
					}
					else if (isLegalWord(line_array[i + 1]).equals("legal word")) {
						word2_index = arrayContains(vocabulary, line_array[i + 1].toLowerCase());
					}
				}
				else { continue; }
			}
			else if (isLegalWord(line_array[i]).equals("legal word")) {
				word1_index = arrayContains(vocabulary, line_array[i].toLowerCase());
				if (word1_index != -1) {
					if (isLegalWord(line_array[i + 1]).equals("SOME_NUM")) {
						if (contains_some_num) { word2_index = some_num_index; }
					}
					else if (isLegalWord(line_array[i + 1]).equals("legal word")) {
						word2_index = arrayContains(vocabulary, line_array[i + 1].toLowerCase());
					}
				}
				else { continue; }
			}
			
			if (word1_index != ELEMENT_NOT_FOUND && word2_index != ELEMENT_NOT_FOUND) {
				CountsArray[word1_index][word2_index]++;
			}
		}
		
		return CountsArray;
	}
	
	
	
	/**
	 * @pre: 'fileName' must not be null.
	 * @pre: 'fileName' is a legal file path.
	 * @pre: 'fileName' must be a name of a .txt file.
	 * @pre: 'vocabulary' must not be null.
	 * @pre: Each element of 'vocabulary' must not appear more than once in 'vocabulary'.
	 * 
	 * @param 'fileName': A string representing the name of the file from which we will build our
	 * 2-dimensional array. 
	 * @param 'vocabulary': An array that contains up to 'MAX_VOCABULARY_SIZE' elements, all of which are
	 * either 'SOME_NUM' or a lower-case version of how they appeared in the given file.
	 * 
	 * @return: A 2-dimensional array of integers called 'CountsArray', where for the indices 'i' and 'j' of
	 * elements in the given array, CountsArray[i][j] represents how many times vocabulary[j] appeared right
	 * after vocabulary[i] in the given file.
	 * 
	 * @post: mVocabulary = previous(mVocabulary).
	 * @post: mBigramCounts = previous(mBigramCounts).
	 * @post: fileName = previous(fileName).
	 * @post: vocabulary = previous(vocabulary).
	 */
	public int[][] buildCountsArray(String fileName, String[] vocabulary) throws IOException{ // Q - 2
		int vocabulary_length = vocabulary.length;
		int[][] CountsArray = new int[vocabulary_length][vocabulary_length];
		boolean contains_some_num = true;	// O(1) time complexity.
		int some_num_index = arrayContains(vocabulary, SOME_NUM);
		
		if (some_num_index == ELEMENT_NOT_FOUND) { contains_some_num = false; }
		
		File file = new File(fileName);
		BufferedReader buffered_reader = new BufferedReader(new FileReader(file));
		String line = buffered_reader.readLine();
		
		while (line != null) {
			CountsArray = countWordsInLine(CountsArray, vocabulary, line, contains_some_num, some_num_index);
			line = buffered_reader.readLine();
		}
		
		buffered_reader.close();
		return CountsArray;
	}
	
	
	
	/**
	 * @param 'vocabulary': An array of strings, representing the first (up to) MAX_VOCABULARY_SIZE words in
	 * the originally given file.
	 * 
	 * @return: A string representing 'vocabulary'.
	 * 
	 * @post: vocabulary = previous(vocabulary).
	 */
	private String vocTextBuilder(String[] vocabulary) {
		int vocabulary_length = vocabulary.length;
		String result = vocabulary_length + " words";
		for (int i = 0; i < vocabulary_length; i++) {
			result = result + System.lineSeparator() + i + "," + vocabulary[i];
		}
		return result;
	}
	
	
	
	/**
	 * @param 'CountsArray': A 2-dimensional array of integers, representing for each element 'word1' of the
	 * vocabulary, how many times does each element 'word2' of the vocabulary appear immediately after
	 * 'word1'.
	 * 
	 * @return: A string representing 'CountsArray'.
	 * 
	 * @post: CountsArray = previous(CountsArray).
	 */
	private String countsTextBuilder(int[][] CountsArray) {
		int vocabulary_length = CountsArray.length;
		String result = "";
		for (int i = 0; i < vocabulary_length; i++) {
			for (int j = 0; j < vocabulary_length; j++) {
				if (CountsArray[i][j] != 0) {
					if (result.length() != 0) {
						result = result + System.lineSeparator();
					}
					result = result + i + "," + j + ":" + CountsArray[i][j];
				}
			}
		}
		return result;
	}
	
	
	
	/**
	 * @pre: 'fileName' must not be null.
	 * @pre: 'fileName' is a legal file path.
	 * @pre: 'fileName' must not contain a suffix.
	 * @pre: (suffix.equals(VOC_FILE_SUFFIX) || suffix.equals(COUNTS_FILE_SUFFIX)).
	 * 
	 * @param 'fileName': A string representing the name of the file that would be generated.
	 * @param 'suffix': A string representing the suffix that the name of the file that would be generated
	 * would have.
	 * 
	 * @post: suffix = previous(suffix).
	 * @post: fileName = previous(fileName).
	 */
	private void modelGenerator(String fileName, String suffix) throws IOException {
		String text;
		if (suffix.equals(VOC_FILE_SUFFIX)) { text = vocTextBuilder(mVocabulary); }
		else // (suffix.equals(COUNTS_FILE_SUFFIX))
		{ text = countsTextBuilder(mBigramCounts); }
		BufferedWriter writer = new BufferedWriter(new FileWriter(fileName + suffix));
		writer.write(text);
		writer.close();
	}
	
	
	
	/**
	 * @pre: The method initModel was called (the language model is initialized).
	 * @pre: 'fileName' must not be null.
	 * @pre: 'fileName' is a legal file path.
	 * @pre: 'fileName' doesn't contain a suffix.
	 * 
	 * @param 'fileName': A string representing the name of the file that would be generated.
	 * 
	 * @post: fileName = previous(fileName).
	 */
	public void saveModel(String fileName) throws IOException{ // Q-3
		modelGenerator(fileName, VOC_FILE_SUFFIX);
		modelGenerator(fileName, COUNTS_FILE_SUFFIX);
	}
	
	
	
	/**
	 * @pre: 'fileName' must not be null.
	 * @pre: 'fileName' is a legal file path.
	 * @pre: 'fileName' doesn't contain a suffix.
	 * 
	 * @param 'fileName': A string representing the name of the file that would be loaded.
	 * 
	 * @return: An integer representing the length of the vocabulary that's represented by the loaded file.
	 * 
	 * @post: fileName = previous(fileName).
	 */
	private int vocabularyLengthLoad(String fileName) throws IOException {
		String voc_file_name = fileName + VOC_FILE_SUFFIX;
		File voc_file = new File(voc_file_name);
		BufferedReader voc_buffered_reader = new BufferedReader(new FileReader(voc_file));
		String voc_line = voc_buffered_reader.readLine();
		int result = Character.getNumericValue(voc_line.charAt(0));
		voc_buffered_reader.close();
		return result;
	}
	
	
	
	/**
	 * @pre: 'fileName' must not be null.
	 * @pre: 'fileName' is a legal file path.
	 * @pre: 'fileName' doesn't contain a suffix.
	 * 
	 * @param 'fileName': A string representing the name of the .voc file that would be loaded.
	 * 
	 * @post: fileName = previous(fileName).
	 */
	private void vocLoadModel(String fileName) throws IOException {
		int vocabulary_length = vocabularyLengthLoad(fileName);
		String[] vocabulary = new String[vocabulary_length];
		String voc_file_name = fileName + VOC_FILE_SUFFIX;
		File voc_file = new File(voc_file_name);
		BufferedReader voc_buffered_reader = new BufferedReader(new FileReader(voc_file));
		String voc_line = voc_buffered_reader.readLine();
		voc_line = voc_buffered_reader.readLine();
		while (voc_line != null) {
			String[] voc_line_array = voc_line.trim().split(",");
			vocabulary[Integer.parseInt(voc_line_array[0])] = voc_line_array[1];
			voc_line = voc_buffered_reader.readLine();
		}
		voc_buffered_reader.close();
		mVocabulary = vocabulary;
	}
	
	
	
	/**
	 * @pre: 'fileName' must not be null.
	 * @pre: 'fileName' is a legal file path.
	 * @pre: 'fileName' doesn't contain a suffix.
	 * 
	 * @param 'fileName': A string representing the name of the .counts file that would be loaded.
	 * 
	 * @post: fileName = previous(fileName).
	 */
	private void countsLoadModel(String fileName) throws IOException {
		int vocabulary_length = vocabularyLengthLoad(fileName);
		String counts_file_name = fileName + COUNTS_FILE_SUFFIX;
		File counts_file = new File(counts_file_name);
		BufferedReader counts_buffered_reader = new BufferedReader(new FileReader(counts_file));
		int[][] counts_array = new int[vocabulary_length][vocabulary_length];
		String counts_line = counts_buffered_reader.readLine();
		while (counts_line != null) {
			String[] first_split = counts_line.trim().split(",");
			int i = Integer.parseInt(first_split[0]);
			String[] second_split = first_split[1].trim().split(":");
			int j = Integer.parseInt(second_split[0]);
			int value = Integer.parseInt(second_split[1]);
			counts_array[i][j] = value;
			counts_line = counts_buffered_reader.readLine();
		}
		counts_buffered_reader.close();
		mBigramCounts = counts_array;
	}
	
	
	
	/**
	 * @pre: 'fileName' must not be null.
	 * @pre: 'fileName' is a legal file path.
	 * @pre: 'fileName' doesn't contain a suffix.
	 * 
	 * @param 'fileName': A string representing the name of the files that would be loaded.
	 * 
	 * @post: fileName = previous(fileName).
	 */
	public void loadModel(String fileName) throws IOException{ // Q - 4
		vocLoadModel(fileName);
		countsLoadModel(fileName);
	}

	
	
	/**
	 * @pre: 'word' is in lower-case.
	 * @pre: The method initModel was called (the language model is initialized).
	 * 
	 * @param 'word': A string representing the word we would search for in 'mVocabulary'.
	 * 
	 * @return: -1 if 'word' is not in 'mVocabulary', and the index of 'word' in 'mVocabulary' otherwise.
	 * 
	 * @post: word = previous(word).
	 */
	public int getWordIndex(String word){  // Q - 5
		return arrayContains(mVocabulary, word);
	}
	
	
	
	/**
	 * @pre: 'word1' and 'word2' are in lower-case.
	 * @pre: The method initModel was called (the language model is initialized).
	 * 
	 * @param 'word1': The first word in the calculated bigram.
	 * @param 'word2': The second word in the calculated bigram.
	 * 
	 * @return: The count for the bigram <word1, word2>, and 0 if one of the words does not exist in
	 * 'mVocabulary'.
	 * 
	 * @post: word1 = previous(word1).
	 * @post: word2 = previous(word2).
	 */
	public int getBigramCount(String word1, String word2){ //  Q - 6
		int word1_index = getWordIndex(word1);
		int word2_index = getWordIndex(word2);
		if (word1_index == ELEMENT_NOT_FOUND || word2_index == ELEMENT_NOT_FOUND) { return 0; }
		return mBigramCounts[word1_index][word2_index];
	}
	
	
	
	/**
	 * @pre: 'word' is in lower-case, and is in 'mVocabulary'.
	 * @pre: The method initModel was called (the language model is initialized).
	 * 
	 * @param 'word': The word with which the bigram should start.
	 * 
	 * @return: A string representing the word with the lowest vocabulary index that appears most frequently
	 * after 'word', and null if a bigram starting with word was never seen.
	 * 
	 * @post: word = previous(word).
	 */
	public String getMostFrequentProceeding(String word){ //  Q - 7
		int word_index = getWordIndex(word);
		if (word_index == ELEMENT_NOT_FOUND) { return null; }
		int vocabulary_length = mVocabulary.length;
		int result_index = 0;
		int max_count = 0;
		for (int i = 0; i < vocabulary_length; i++) {
			if (mBigramCounts[word_index][i] > max_count) {
				result_index = i;
				max_count = mBigramCounts[word_index][result_index];
			}
		}
		if (max_count > 0) { return mVocabulary[result_index]; }
		return null;
	}
	
	
	
	/**
	 * @pre: 'sentence' is in lower-case.
	 * @pre: The method initModel was called (the language model is initialized).
	 * @pre: Each two words in the sentence are are separated with a single space.
	 * 
	 * @param 'sentence': A string which would be checked for it's validity as a legal sentence in accordance
	 * with 'mVocabulary' and 'mBigramCounts'.
	 * 
	 * @return: 'true' of 'sentence' is probable, according to the model, and 'false' otherwise.
	 * 
	 * @post: sentence = previous(sentence).
	 */
	public boolean isLegalSentence(String sentence){  //  Q - 8
		String[] sentence_array = sentence.trim().split("\\s+");
		if (sentence_array.length == 0) { return true; }
		else if (sentence_array.length == 1) {
			if (getWordIndex(sentence_array[0]) == ELEMENT_NOT_FOUND) { return false; }
			return true;
		}
		int sentence_length = sentence_array.length;
		int[] sentence_array_indices = new int[sentence_length];
		for (int i = 0; i < sentence_length; i++) {
			int word_index = getWordIndex(sentence_array[i]);
			if (word_index == ELEMENT_NOT_FOUND) { return false; }
			sentence_array_indices[i] = word_index;
		}
		for (int j = 0; j < (sentence_length - 1); j++) {
			String word1 = mVocabulary[sentence_array_indices[j]];
			String word2 = mVocabulary[sentence_array_indices[j + 1]];
			if (getBigramCount(word1, word2) == 0) { return false; }
		}
		return true;
	}
	
	
	
	/**
	 * @pre: arr1.length = arr2.length.
	 * 
	 * @param 'arr1': The first vector for the calculation.
	 * @param 'arr2': The second vector for the calculation.
	 * 
	 * @return: -1 if 'arr1' or 'arr2' are only filled with zeros, and the result of a calculation otherwise.
	 * 
	 * @post arr1 = previous(arr1).
	 * @post arr2 = previous(arr2).
	 */
	public static double calcCosineSim(int[] arr1, int[] arr2){ //  Q - 9
		boolean arr1_is_zeros = true;
		boolean arr2_is_zeros = true;
		int array_length = arr1.length;
		for (int i = 0; i < array_length; i++) {
			if (!arr1_is_zeros && !arr2_is_zeros) { break; }
			if (arr1[i] != 0) { arr1_is_zeros = false; }
			if (arr2[i] != 0) { arr2_is_zeros = false; }
		}
		if (arr1_is_zeros || arr2_is_zeros) { return ELEMENT_NOT_FOUND; }
		double sum_multiply = 0.0;
		double sum_squared_1 = 0.0;
		double sum_squared_2 = 0.0;
		for (int j = 0; j < array_length; j++) {
			sum_multiply += arr1[j] * arr2[j];
			sum_squared_1 += arr1[j] * arr1[j];
			sum_squared_2 += arr2[j] * arr2[j];
		}
		double denominator = Math.sqrt(sum_squared_1) * Math.sqrt(sum_squared_2);
		return sum_multiply / denominator;
	}

	
	
	/**
	 * @pre: 'word' is in vocabulary.
	 * @pre: The method initModel was called (the language model is initialized).
	 * 
	 * @param 'word': An element of 'mVocabulary' for which we will look for the most similar other element
	 * in 'mVocabulary', according to the result of cosineSimilarity.
	 * 
	 * @return: A string 'result' from 'mVocabulary' for which cosineSimilarity(vector for 'word', vector for
	 * 'result') is the largest among all the other words in 'mVocabulary'.
	 * 
	 * @post: word = previous(word).
	 */
	public String getClosestWord(String word){ //  Q - 10
		if (mVocabulary.length == 0) { return null; }
		else if (mVocabulary.length == 1) { return word; }
		int vocabulary_length = mVocabulary.length;
		int word_index = getWordIndex(word);
		int[] word_vector = mBigramCounts[word_index];
		double max_similarity = 0.0;
		String result = mVocabulary[0];
		for (int i = 0; i < vocabulary_length; i++) {
			if (i == word_index) { continue; }
			int[] tmp_vector = mBigramCounts[i];
			double tmp_similarity = calcCosineSim(word_vector, tmp_vector);
			if (max_similarity < tmp_similarity) {
				max_similarity = tmp_similarity;
				result = mVocabulary[i];
			}
		}
		return result;
	}


	
}