import sys
import math
from typing import List, Dict, Tuple

def main():
    """ A method that return a K-means partition of the data in the given input_data.txt file.
        Should receive up to 3 arguments in the terminal: 'K', 'iter', 'inut_data.txt' ; Note that the
        order in which they are given is imprtant! Would work if 'iter' is not provided, and set it
        to a default value of 200.
        @param 'K':                 The number of desired centroids. Must be larger than 1 and smaller
                                        than the amount of datapoints in the given .txt file!
        @type 'K':                  A string that represents an integer.
        @param iter:                The maximal number of desired iteration over all the datapoints
                                        in the given .txt file. Must be larger than 1 and smaller
                                        than 1,000!
        @type 'iter':               A string that represents an Integer.
        @param 'input_data.txt':    The name of the .txt file in which all the datapoints are stored.
                                        Must end with ".txt"! All datapoints in this file must represent
                                        floating-point numbers, and consist of the same number of
                                        floating-point numbers! Must not be None!
        @type 'input_data.txt':     String.

        This method would first validate all the restrictions above and then would do the following:
        1. Let 'N' be the number of datapoints in the given .txt file.
        2. Let 'd' be the number of floating-point numbers in each datapoint.
        3. Validating that all datapoints indeed consist of 'd' floating-point numbers. Otherwise,
            return an error message. In addition, validate if 'd' is 0, and if so, print an empty
            string and return None (note explanation in the code).
        4. Generate an array of 'K' datapoints, which will be known as the centroids.
        5. Define Epsilon to be 0.001 .
        6. Define a Euclidean Distance to be the square-root of the sum of the sqare of the result
            of subtracting a floating-point number in datapoint1 with a floating-point number in
            datapoint2, so that both of these floating-point numbers appear in the same index
            (between 0 and 'd'-1) in datapoint1 and datapoint2.
        7. Definde 'centroid_delta' as the Euclidean Distance between the centroid in the previous
            iteration and the centroid in the current iteration.
        8. Do until we've done 'iter' iterations, or until every centroid_delta is smaller than
            Epsilon:
            a) Assign every datapoint in the given .txt file to the centroid for which the
                Euclidean Distance between the datapoint and the centroid is the minimal (out of
            b) Update each of the 'd' floating-point numbers that represent each of the 'K'
                centroids to all the centroids). be the average of the floating-point numbers (of
                the same index as the floating-point number in the centroid's representation that
                we are now looking on) of the datapoints from the given .txt file that were
                assigend to said centroid.
        9. Print the centroids and return None.
    """
    
    # Defining an inner methods that would do the actual calculations, and return the error messages:

    def kmeans(K: int, iter: int, input_data: str) -> None:
        """ This method does exactly what the 'main' method docstring says that 'main' does.
            @param 'K':             The number of desired centroids. Must be larger than 1 and smaller
                                        than the amount of datapoints in the given .txt file!
            @param 'iter':          The maximal number of desired iteration over all the datapoints
                                        in the given .txt file.
            @param 'input_data':    The name of the .txt file in which all the datapoints are stored.
                                        All datapoints in this file must represent floating-point
                                        numbers, and consist of the same number of floating-point
                                        numbers!
            Note that 'main' ensures that this method receives the arguments in the desired types, and
            that 'iter' is larger than 1 and smaller than 1,000, as well that 'input_data' isn't None and
            ends with ".txt", thus this method would not validate these conditions. Since these conditions
            are not validated, this method must not be called directly by the user!
        """
    
        # Defining inner methods to help with the calculations:


        def readFile(input_data: str) -> List[List[str]]:
            """ A method that reads a given .txt file and returns its lines as a list of lists, each
                inner-list consists of strings.
                @param 'input_data':    A .txt file's name. Must end with .txt! Must not be None!
                @type 'input_data':     String.
                Since this method has no input validations and should not be called directed by the user!
            """
            
            with open(input_data, 'r') as input_file:
                data = input_file.read()
                lines = data.split("\n")
                if lines[-1] == "":     # Should always occur according to the PDF iinstructions!
                    lines = lines[:-1]
            return lines
        

        def setDatapoints(datapoints: List[List[str]]) -> List[List[float]]:
            """ A method that sets a given list into a list of lists, each inner-list composed of
                floating-point numbers only.
                @param 'lines':     A list of datapoints. Each datapoint must be a list of strings!
                @type 'lines':      list.
                Since this method has no validation that 'lines' is a list of lists, where each inner-
                -list consists of strings, it should not be called directly by the user!
            """
            
            for i in range(len(datapoints)):      # Setting the datapoints into floating-point coordinates
                datapoints[i] = datapoints[i].split(",")
                for j in range(len(datapoints[i])):
                    try:
                        datapoints[i][j] = float(datapoints[i][j])
                    except:
                        error_message("An Error Has Occurred")
            return datapoints
        

        def validateDimention(datapoints: List[List[float]], d: int) -> None:
            """ A method that validates that each inner-list of the given list consists of the
                same number of floating-point numbers, and that said number is the given integer.
                If said integer is non-positive, the method prints an error and terminates the program.
                @param 'lines':     The given datapoints. Each inner-list must consist of floating-point
                                        numbers! Must not be None!
                @type 'lines':      A list of lists.
                @param 'd':         The dimention of the first datapoint.
                @type 'd':          integer.
                Since this method has no input validations and should not be called directed by the user!
            """

            if d <= 0:      # If d == 0, that means that every data point consists of 0 floating-
                            # -point numbers. Thus there's no possible centroid that subsists the
                            # demands. If d == -1, we had an error getting the dimention of the first
                            # datapoint.
                error_message("An Error Has Occurred")

            # The following was said to always subsist, and thus is commented. If you wish to validate
            # that every datapoint in 'datapoints'' consists of the same number of floating-point
            # numbers, just un-comment this :)
            """
            for datapoint in datapoints:
                if len(datapoint) != d:
                    error_message("An Error Has Occurred")
            """
            return None


        def centroidCalculation(datapoints: List[List[float]], centroids: List[List[float]], \
                                centroid_num: int, dimention: int, epsilon: float, \
                                max_iteration_num: int, allocation: Dict[str, List[List[List[float]]]], \
                                epsilon_is_greater: bool) -> List[List[float]]:
            """ A method that calculates the centroids in each iteration, and returns the final centroids.
                @param 'datapoints':            A list containing all the datapoints which would be used to
                                                    calculate the centroids in every iteration. Must consist
                                                    of lists, where each inner-list consists of floating-point
                                                    numbers! Must not be None!
                @param 'centroids':             A list containing all the centroids as they are at the start
                                                    of the current iteration. Must consist of lists, where
                                                    each inner-list consists of floating-point numbers! Must
                                                    not be None!
                @param 'centroid_num':          The number of desired centroids.
                @param 'dimention':             The number of floating-point that every centroid should
                                                    consist of. Each given datapoint and centroid must already
                                                    consist of this number of floating-point numbers!
                @param 'epsilon':               The desired epsilon, where if the euclidean distance between
                                                    each old centroid (as it was at the start of the current
                                                    iteration) and each respective new centroid (as it is at
                                                    the end of the current iteraton) is smaller than
                                                    'epsilon', we would need not continue to another iteration.
                                                    Must be a positive floating-point number!
                @param 'iteration_num':         The number of iteration we've done so far (not including the
                                                    iteration we are now starting). Must be a non-negative
                                                    integer!
                @param 'max_iteration_num':     The maximal number of desire iterations. Must be a non-
                                                    -negative integet!
                @param 'allocation':            A dictionary that details the allocation of every datapoint
                                                    to its closest centroid. Its keys are strings that
                                                    represent the index of the relevant centroid in the list
                                                    of centroids. Its values are lists of datapoints. Must
                                                    not be None! If 'iteration_num' is 0, each value must
                                                    include the centroid in the relevant index in the list
                                                    of centroids! If 'iteration_num' isn't 0, each value
                                                    must be an empty list!
                @param 'epsilon_is_greater':    A boolean value that states if the centroid delta (the
                                                    euclidean distance mentioned in the explanation of the
                                                    'epsilon' parameter) is smaller than the 'epsilon'
                                                    parameter.
                Since this method has no input validations and should not be called directed by the user!
            """
            
            # Defining inner methods to help with the calculation:


            def euclideanDistance(datapoint1: List[float], datapoint2: List[float], d: int) -> float:
                """ A method that calculates and returns the Euclidean Distance between 2 given lists of
                    floating-point numbers. Please read the 'main' method's docstring to see the definition
                    of Euclidean Distance.
                    @param 'datapoint1':    A datapoint from which we will calculate the Euclidean Distance to
                                                the other datapoint. Must consist of 'd' floating-point
                                                numbers!
                    @param 'datapoint2':    A datapoint to which we will calculate the Euclidean Distance from
                                                the other datapoint. Must consist of 'd' floating-point
                                                numbers!
                    @param 'd':             The number of floating-point numbers from which both datapoints
                                                must consist. Must be a positive number!
                    Note that 'Kmeans' ensures that this method receives arguments that subsist each condition
                    shown above, and that each of the arguments this method receives is in the desired type.
                    Since this method has no input validations and should not be called directed by the user!
                """
                
                res = 0.0
                for index in range(d):
                    res += (datapoint1[index] - datapoint2[index]) ** 2
                res = math.sqrt(res)
                return res


            def allocate(datapoints: List[List[float]], centroids: List[List[float]], d: int, \
                     allocation: Dict[str, List[List[List[float]]]]) -> Dict[str, List[List[List[float]]]]:
                """ Allocating each datapoint to a list representing its closest centroid.
                    @param 'datapoints':        A list containing all the datapoints.
                    @param 'centroids':         A list containing all the centroids.
                    @param 'd':                 The number of floating-point number each centroid and
                                                    datapoint consists of.
                    @param 'allocation':        A dictionary detailing the allocation of every datapoint
                                                    to its closest centroid. It keys are strings that
                                                    represent the index of the relevant centroid in the
                                                    centroids list, and its values are lists containing
                                                    the datapoints for which the respective centroid is
                                                    closest.
                    Since this method has no input validations and should not be called directed by the user!
                """
                
                for datapoint in datapoints:
                    centroid_index = 0
                    min_distance = euclideanDistance(datapoint, centroids[0], d)

                    for i in range(1, K):
                        distance_i = euclideanDistance(datapoint, centroids[i], d)
                        if distance_i < min_distance:
                            min_distance = distance_i
                            centroid_index = i
                    
                    allocation[str(centroid_index)].append(datapoint)
                
                return allocation


            def iterationEnd(centroids: List[List[float]], allocation: Dict[str, List[List[List[float]]]], \
                         epsilon: float) -> Tuple[List[List[float]], Dict[str, List[List[List[float]]]], bool]:
                """ Updating the centroids list to contain the new centroids, the allocation dictionary's
                    values to be empty lists, and also returns a boolean value representing if every new
                    centroid's euclidean distance to its respective previous centroid is smaller than
                    the given epsilon.
                    @param 'centroids':     A list containing all the current centroids (also known as the
                                                previous centroids in relation to the centroids that would be
                                                returned by this method).
                    @param 'allocation':    A dictionary detailing the allocation of each datapoint to the
                                                centroid closest to it. Its keys are strings that represent
                                                the index of the respective centroid in the centroids list,
                                                and its values are lists of datapoints, which the respective
                                                centroid is closest to.
                    @param 'epsilon':       The minimal floating-point number that should be greater than the
                                                euclidean distance between each centroid in the given list and
                                                the respective centroid in the output centroid list.
                    Since this method has no input validations and should not be called directed by the user!
                """

                # Defining inner methods:


                def centroidUpdate(datapoints: List[List[float]], d: int) -> List[float]:
                    """ A method that updates and returns the centroid after every loop over all the given
                        datapoints. Please read the 'main' method's docstring to see how this update is done.
                        @param 'datapoints':        A list of datapoints that will be used to update the centroid.
                        @param 'd':                 The number of floating-point numbers from which all datapoints
                                                        must consist. Must be a positive number!
                        Note that 'Kmeans' ensures that this method receives arguments that subsist each condition
                        shown above, and that each of the arguments this method receives is in the desired type.
                        Since this method has no input validations and should not be called directed by the user!
                    """
                    
                    centroid = [0.0]*d
                    for index in range(d):
                        for datapoint in datapoints:
                            centroid[index] += datapoint[index]
                        centroid[index] /= len(datapoints)
                    return centroid


                def centroidDelta(previous_centroid: List[float], new_centroid: List[float], d: int) -> float:
                    """ A method that calculates the centroid delta afer every loop over all the given
                        datapoints. Please read the 'main' method's docstring to see how this calculation is
                        done.
                        @param 'previous_centroid':     The previous centroid from which we will calculate the
                                                            Euclidean Distance to the new centroid.
                        @param 'new_centroid':          The new centroid to whom we will calculate the Euclidean
                                                            Distance from the old centroid.
                        @param 'd':                     The number of floating-point numbers from which both
                                                            centroids must consist. Must be a positive number!
                        Note that 'Kmeans' ensures that this method receives arguments that subsist each condition
                        shown above, and that each of the arguments this method receives is in the desired type.
                        Since this method has no input validations and should not be called directed by the user!
                    """

                    # This method is unneccesary as I could've easily used the euclideanDistance method without
                    # defining this method, but I believe it'll make this code easier to read.

                    return euclideanDistance(previous_centroid, new_centroid, d)


                # End of inner methods, start of 'iterationEnd's actual code.

                epsilon_is_greater = True     # Would only be changed if at least 1 of the centriod deltas
                                            # is equal to, or larger than, eps.
                
                for j in range(K):
                    old_centroid = centroids[j]
                    new_centroid = centroidUpdate(allocation[str(j)], d)
                    allocation[str(j)] = []     # Ensuring that in the next iteration we'd restart the
                                                # datapoint allocation to centroids without remembering
                                                # what was the allocation in the previous iteraion.

                    if centroidDelta(old_centroid, new_centroid, d) >= epsilon:
                        epsilon_is_greater = False
                    
                    centroids[j] = new_centroid     # Ensuring that in the beginning of the next iteraion
                                                    # we'd use the updated centroids.
                
                return (centroids, allocation, epsilon_is_greater)


            # End of inner methods. Start of 'centroidCalculation's actual code.

            iteration_num = 0

            while (iteration_num < max_iteration_num) and (not epsilon_is_greater):
                if iteration_num == 0:      # If we're at the first iteration, we already allocated K
                                            # datapoints as the centroids
                    allocation = allocate(datapoints[centroid_num:], centroids, dimention, allocation)
                else:
                    allocation = allocate(datapoints, centroids, dimention, allocation)
                
                (centroids, allocation, epsilon_is_greater) = iterationEnd(centroids, allocation, epsilon)
                iteration_num += 1
            
            return centroids
        

        def printResult(centroids: List[List[float]], K: int, d: int) -> None:
            """ Prints each centroid in the given list as a string, with the '\n' character as a
                dividor between each 2 adjacent centroids, and the ',' character as a dividor
                between each 2 adjacent floating-point numbers in each centroid.
                @param 'centroids':     A list containing all the centroids that should be printed.
                @param 'K':             The number of centroids.
                @param 'd':             The number of floating-point number in each centroid.
                Since this method has no input validations and should not be called directed by the user!
            """

            for i in range(K):
                line = ""

                for j in range(d - 1):
                    line += f"{centroids[i][j]:.4f}" + ","

                line += f"{centroids[i][d - 1]:.4f}"
                print(line)
            
            return None

        
        # End of inner methods definitions. Start of 'Kmeans' actual code.
        
        datapoints = readFile(input_data)
        if K >= len(datapoints):        # Finishing the validation regarding 'K':
            error_message("Invalid number of clusters!")
        
        datapoints = setDatapoints(datapoints)
        d = len(datapoints[0])
        validateDimention(datapoints, d)
        centroids = [element for element in datapoints[:K]]      # This will eventually give us the centuroids,
                                                                 # but at the first iteration, this should be
                                                                 # the first 'K' datapoints.

        # Creating a dictionary that shows the current allocation of datapoints to centroids:
        allocation = {str(i) : [centroids[i]] for i in range(K)}
        centroids = centroidCalculation(datapoints, centroids, K, d, 0.001, iter, allocation, False)
        printResult(centroids, K, d)
        return None


    def error_message(message: str) -> exit:
        """ Prints the given error message and exits the program. 
            @param 'message':       The error memssage that should be printed. Must not be None!
            @type 'message':        String.
            Since this method has no input validations and should not be called directed by the user!
        """
        
        print(message)
        exit()

    
    def validateK(arg: str, message: str) -> int:
        """ Validates that a given string represents an integer that is larger than 1, and returns
            said integer.
            @param 'arg':       The string that should be converted into an integer.
        """

        try:        # Validating that 'arg' representes a floating-point number.
            K = float(arg)
        except:
            error_message(message)
        if (K != int(K)) or (K <= 1):     # Validating that 'K' represents an integer that is larger
                                          # than 1.
            error_message(message)
        
        return int(K)
    

    def validateIter(arg: str, message: str) -> int:
        """ Validates that a given string represents an integer that is larger than 1 and smaller
            than 1,000, and returns said integer.
            @param 'arg':       The string that should be converted into an integer.
        """
        
        iter = validateK(arg, message)
        
        if iter >= 1000:
            error_message(message)
        
        return iter
    

    # End of inner methods definition. Beginning 'main's actual code:
    
    if (len(sys.argv) != 3) and (len(sys.argv) != 4):       # Not enough, or too many, given arguments.
        error_message("An Error Has Occurred")
    
    K = validateK(sys.argv[1], "Invalid number of clusters!")      # The only thing left to validate about 'K'
                                                                   # is that it is smaller than the number of
                                                                   # datapoints in the given .txt file!
    
    if len(sys.argv) == 3:      # No argument given for the iteration number, thus it is set to 200.
        iter = 200
        input_data = sys.argv[2]
    else:       # len(sys.argv) == 4, thus an argument for the iteration number was given.
        iter = validateIter(sys.argv[2], "Invalid maximum iteration!")
        input_data = sys.argv[3]
    
    # Validating that we were given a .txt file as an input for 'input_data.txt':
    if (input_data == None) or (input_data[-4:] != ".txt"):
        error_message("NA")
    
    return kmeans(K, iter, input_data)

# End of 'main's definition. The following is the code to ensure that this file knows that it needs to
# use 'main' if it is being called.

if __name__ == "__main__":
    main()
