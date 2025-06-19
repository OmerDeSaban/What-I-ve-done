import sys
import pandas as pd
import numpy as np
import math
import mykmeanssp as kmeansC
from typing import List, Tuple

def printError(message: str) -> None:
    """Prints the given error message to the standard data stream and exits the program. 

    Args:
        message (str):
            The error message that should be printed. Must not be None.

    Returns:
        None:
            This method does not return a value as it exits the program after printing.
    """
    if message == "":
        printError("An Error Has Occurred")
        exit()
    print(message)
    exit()

def isFloat(arg: str, message: str) -> float:
    """Validate that a given string represents a non-negative float and return the float.

    If the string doesn't represent a float, print the error message and exit the program.
    
    Args:
        arg (str):
            The string that should represent a non-negative float.
        message (str):
            The error message that should be printed. Must not be None!

    Returns:
        float:
            The validated string in the form of a non-negative float.
    """
    try:
        res = float(arg)
    except:
        printError(message)
    if res < 0:
        printError(message)
    return res

def isNum(arg: str, message: str) -> int:
    """Validate that a given string represents an int greater than 1 and return the int.

    If the string doesn't represent an int, print the error message and exit the program.
    
    Args:
        arg (str):
            The string that should represent an int greater than 1.
        message (str):
            The error message that should be printed. Must not be None.

    Returns:
        int:
            The validated string in the form of an int greater than 1.
    """
    res = isFloat(arg, message)
    if (res != int(res)) or (res <= 1):
        printError(message)
    return int(res)

def isFile(arg: str) -> str:
    """Validate that a given string represents a .txt filename and return the string.

    If the string doesn't represent a .txt filename, print the error message and exit the program.
    
    Args:
        arg (str):
            The string that should represent a .txt filename.
        message (str):
            The error message that should be printed. Must not be None.

    Returns:
        str:
            The validated string.
    """
    if (len(arg) < 4) or (arg[-4:] != ".txt"):
        printError("NA")
    return arg

def allocateVars(index: int) -> Tuple:
    """Allocate the last 3 user CMD arguments into variables and return the variables as a tuple.

    Starting at the given index, the method extracts and returns 3 specific command-line arguments.
    
    Args:
        index (int):
            The allocation's starting index. Must satisfy the following conditions:
                - index is equal to 'len(sys.argv) - 3', where:
                    'len(sys.argv)' is the number of CMD arguments, including the script name
                - index is non-negative.

    Returns:
        Tuple:
            A tuple containing the following 3 elements:
                Tuple[0] ( float ):
                    The epsilon value for K-means convergence.
                Tuple[1] ( str ):
                    The filename of the 1st (of 2) .txt file, is to be joined using pandas.inner_join.
                Tuple[2] ( str ):
                    The filename of the 2nd (of 2) .txt file, is to be joined using pandas.inner_join.
    """
    if (index < 0):
        printError("An Error Has Occurred")
    elif index != (len(sys.argv) - 3):
        printError("An Error Has Occurred")
    eps = isFloat(sys.argv[index], "Invalid epsilon!")
    file_name_1 = isFile(sys.argv[index + 1])
    file_name_2 = isFile(sys.argv[index + 2])
    return (eps, file_name_1, file_name_2)

def argvToVars() -> Tuple:
    """Allocate the user CMD arguments into variables and return the variables as a tuple.

    The method extracts the command-line arguments (excluding the script name) and returns 5 variables.
    
    Args:
        None:
            This method takes no arguments as it simply reads command-line arguments and parses them.

    Returns:
        Tuple:
            A tuple containing the following 5 elements:
                Tuple[0] ( int ):
                    The number of desired K-means generated clusters (or centroids).
                Tuple[1] ( int ):
                    The maximal desired number of K-means iterations.
                Tuple[2] ( float ):
                    The epsilon value for K-means convergence.
                Tuple[3] ( str ):
                    The filename of the 1st (of 2) .txt file, is to be joined using pandas.inner_join.
                Tuple[4] ( str ):
                    The filename of the 2nd (of 2) .txt file, is to be joined using pandas.inner_join.
    """
    if (len(sys.argv) != 5) and (len(sys.argv) != 6):
        printError("An Error Has Occurred")
    K = isNum(sys.argv[1], "Invalid number of clusters!")
    # All that's left to validate about 'K' is that it's smaller than 'N'! (This will be done a bit later)
    if len(sys.argv) == 5:      # No argument given for 'iter'
        iter = 300
        args_tuple = allocateVars(2)
    else:
        iter = isNum(sys.argv[2], "Invalid maximum iteration!")
        if iter >= 1000:
            printError("Invalid maximum iteration!")
        args_tuple = allocateVars(3)
    return (K, iter) + args_tuple

def createData(file_name_1: str, file_name_2: str, K: int) -> Tuple:
    """Generate K-means datapoints from the 2 .txt files and return the datapoints and their amount as a tuple.

    The method reads files using pandas.read_csv, joins them using pandas.inner_join, and then sorts the data.
    
    Args:
        file_name_1 (str):
            The filename of the 1st .txt file.
        file_name_2 (str):
            The filename of the 2nd .txt file.
        K (int):
            The number of desired K-means clusters.

    Returns:
        Tuple:
            A tuple containing the following 2 elements:
                Tuple[0] ( pandas.DataFrame ):
                    The generated K-means datapoints as a pandas DataFrame.
                Tuple[1] ( int ):
                    The number of generated K-means datapoints.
    """
    df1 = pd.read_csv(file_name_1, sep = ",", header = None)
    df2 = pd.read_csv(file_name_2, sep = ",", header = None)
    if len(df1.index) != len(df2.index):
        printError("An Error Has Occurred")
    df_inner = pd.merge(df1, df2, on = 0, how = 'inner')
    datapoints = df_inner.sort_values(0)        # Note that 'datapoints' are sorted !
    N = len(datapoints.index)
    if K >= N:      # End of user CMD arguments validations
        printError("Invalid number of clusters!")
    return (datapoints, N)

def Dx(datapoints: List[np.ndarray], index: int, chosen: List[int]) -> float:
    """Calculate the minimal Euclidean distance between a specified datapoint and other chosen datapoints.

    Compute the distance between datapoints[index] to every chosen datapoint. Return the minimal distance.
    
    Args:
        datapoints (NumPy.ndarray):
            The datapoints as a NumPy array.
        index (int):
            The index of the datapoint in the array from which we will calculate the distance to the chosen datapoints.
        chosen (List[int]):
            The list of indices of the chosen datapoints.
    
    Returns:
        float:
            The minimal distance between datapoints[index] and a chosen datapoint.
    
    For more information about Euclidean distance, see:
        https://en.wikipedia.org/wiki/Euclidean_distance
    """
    if index < 0:
        printError("An Error Has Occurred")
    min_dis = math.inf
    datapoint1 = datapoints[index]
    for i in chosen:
        if i < 0:
            printError("An Error Has Occurred")
        datapoint2 = datapoints[i]
        dis = np.linalg.norm(datapoint1 - datapoint2)
        if dis < min_dis:
            min_dis = dis
    return min_dis

def chosenIndices(datapoints: pd.DataFrame, K: int, N: int) -> Tuple:
    """Choose K initial K-means clusters (or centroids) at random.

    The method chooses the initial K-means clusters using randomity and a weighted probability distribution.
    
    Args:
        datapoints (pandas.DataFrame):
            The datapoints as a pandas DataFrame.
        K (int):
            The number of desired K-means clusters (or centroids).
        N (int):
            The number of datapoints.
    
    Returns:
        Tuple:
            A tuple containing the following 2 elements:
                Tuple[0] ( List[int] ):
                    A list of the indices of the chosen clusters (or centroids).
                Tuple[1] ( List[List[float]] ):
                    A list of the datapoints, where each datapoint is a list of float.
    """
    np.random.seed(1234)
    # Using NumPy to randomly choose the first cluster (or centroid):
    chosen = [np.random.choice([index for index in range(N)])]
    chosens = 1
    # Try to convert the datapoints from a pandas DataFrame to a NumPy array:
    try:
        np_datapoints = [np.array(datapoints.iloc[i].drop(0)) for i in range(len(datapoints))]
    except: # Will fail if we received datapoints of different dimensions.
        printError("An Error Has Occurred")
    # Convert the datapoints from a NumPy array to a Python list:
    datapoint_list = [np_datapoint.tolist() for np_datapoint in np_datapoints]
    while chosens < K:
        Dx_array = [Dx(np_datapoints, i, chosen) for i in range(N)]
        # Using NumPy to calculate the wighted probability of choosing each datapoint:
        Dx_array = Dx_array / np.sum(Dx_array)
        # Using NumPy to randomly choose a cluster (or a centroid), taking the calculated
        # wighted probability into account:
        new_cen = np.random.choice(N, 1, p = Dx_array)[0]
        chosen.append(new_cen)
        chosens += 1
    return (chosen, datapoint_list)

def dValidity(datapoint_list: List[List[float]], N: int) -> int:
    """Validate that every datapoint's dimension is the same and that it's a positive int.
    
    Args:
        datapoints (List[List[float]]):
            The list of datapoints, where each is a list of float.
        N (int):
            The number of datapoints.
    
    Returns:
        int:
            The dimension of each datapoint.
    """
    d = len(datapoint_list[0])  # For the reader's understanding of what d is throughout the entire code
    if d <= 0:
        # Note that if d == 0, it means that every data point consists of 0 floating-point
        # numbers. Thus there's no possible centroid that subsists the demands. If d == -1,
        # we had an error getting the dimension of the first datapoint.
        printError("An Error Has Occurred")
    # Note that we are calling this method in the firstLine method, which receives the datapoints
    # argument as a list generated by the NumPy.tolist method. Thus we can be assured that, as long
    # as the user doesn't call this method directly, it (dValidity) will always receive the datapoints
    # as a list of same-dimensional lists of float. Therefore there's no need to validate this.
    # With that said, we did leave that part here, though commented. If someone would wish to do this
    # validation, they will be able to do it by simply uncommenting the following code segment :)
    """
    for i in range(1, N):
        if d != len(datapoint_list[i]):
            printError("An Error Has Occurred")
    """
    return d

def firstLine(K: int, chosen: List[int], datapoint_list: List[List[float]], N: int) -> Tuple:
    """Create a string detailing the elements of chosen and return a tuple of the chosen datapoints, their dimension, and the string.

    Converts chosen into a list of datapoints, similar to datapoint_list, and returns it as the first element of the tuple.
    
    Args:
        K (int):
            The number of desired K-means clusters (or centroids).
        chosen (List[int]):
            A list of the indices of the K chosen datapoints.
        datapoints (List[List[float]]):
            The list of datapoints, where each is a list of float.
        N (int):
            The number of datapoints.
    
    Returns:
        Tuple:
            A tuple containing the following 3 elements:
                Tuple[0] ( List[List[float]] ):
                    A list of the chosen clusters (or centroids), where each is a list of float.
                Tuple[1] ( int ):
                    The dimension of each datapoint and cluster (or centroid).
                Tuple[2] ( str ):
                    A string detailing the indices of the chosen clusters (or centroids), where:
                        - Each pair of adjacent indices are separated by a ','.
                        - The string ends with the new line character.
    """
    res = ""
    for i in range(K):
        if i != (K - 1):
            res += f"{chosen[i]},"
        else:
            res += f"{chosen[i]}\n"
        chosen[i] = datapoint_list[chosen[i]]
    d = dValidity(datapoint_list, N)
    return (chosen, d, res)

def cValidity(final_cen: List[List[str]], K: int, d: int) -> None:
    """Validate the output of the C program. Prints an error message and exits the program if the output isn't valid.

    Ensures that the C program output is:
        - K clusters (or centroids), where:
        - Each cluster (or centroid) contains d observations (or coordinates), where:
        - Each observation (or coordinate) is a string that represents a float.
    
    Args:
        final_cen (List[List[str]]):
            The number of desired K-means clusters (or centroids).
        K (int):
            The number of desired K-means clusters (or centroids).
        d (int):
            The dimension of each datapoint and cluster (or centroid).
    
    Returns:
        None:
            This method does not return a value as it exits the program if it finds an error.
    """
    if K != len(final_cen):
        printError("An Error Has Occurred")
    for centroid in final_cen:
        if d != len(centroid):
            printError("An Error Has Occurred")
        for coordinate in centroid:
            try:
                float(coordinate)
            except:
                printError("An Error Has Occurred")

def datapointLines(res: str, K:int, d: int, final_cen: List[List[str]]) -> str:
    """Join the res string with a string of the final clusters (or centroids).
    
    The final clusters (or centroids) are the output of the C program.

    Args:
        res (str):
            A string containing the indices of the initially chosen clusters (or centroids), where:
                - Each pair of adjacent indices are separated by a ','.
                - The string ends with the new line character.
        K (int):
            The number of desired K-means clusters (or centroids).
        d (int):
            The dimension of each datapoint and cluster (or centroid).
        final_cen (List[List[str]]):
            A list containing the outputted clusters (or centroids) from the C program.
    
    Returns:
        str:
            A string detailing the following:
                - The indices of the initially chosen clusters (or centroids), as the first line.
                - The clusters (or centroids) returned by the C program, where:
                    - Each cluster (or centroid) is in its own line, where:
                    - Each pair of adjacent observations (or coordinates) is separated by a ','.
    """
    for i in range(K):
        for j in range(d):
            if j != (d - 1):
                res += final_cen[i][j] + ","
            elif i == K:
                res += final_cen[i][j]
            else:
                res += final_cen[i][j] + "\n"
    return res

def Kmeans_pp(datapoints: pd.DataFrame, K: int, iter: int, eps: float, N: int) -> None:
    """Uses the C program to calculate the desired amount of K-means clusters (or centroids) and prints them.

    Args:
        datapoints (pandas.DataFrame):
            The datapoints as a pandas DataFrame.
        K (int):
            The number of desired K-means clusters (or centroids).
        iter (int):
            The maximal desired number of K-means iterations.
        eps (float):
            The epsilon value for K-means convergence.
        N (int):
            The number of datapoints.
    
    Returns:
        None:
            This method does does not return a value as it prints the result as long as no error occurred.
    """
    (chosen, datapoint_list) = chosenIndices(datapoints, K, N)
    (chosen, d, res) = firstLine(K, chosen, datapoint_list, N)
    final_cen = kmeansC.fit(chosen, datapoint_list, K, iter, eps, N, d)
    # If the C program encountered an error, both programs would not continue past said encounter. Thus
    # the following lines wouldn't be done.
    cValidity(final_cen, K, d)
    res = datapointLines(res, K, d, final_cen)
    print(res)
    return None

def main() -> None:
    """Read the user CMD arguments and perform the K-means++ algorithm, with a C-Python API, using them.

    Uses all the other methods to perform the algorithm.

    Args:
        None:
            This method takes no arguments as it reads command-line arguments and uses them.
    
    Returns:
        None:
            This method does not return a value as it prints the result as long as no error occurred.
    
    For more information about the K-means++ algorithm, see:
        https://en.wikipedia.org/wiki/K-means%2B%2B
    """
    (K, iter, eps, file_name_1, file_name_2) = argvToVars()
    (datapoints, N) = createData(file_name_1, file_name_2, K)
    return Kmeans_pp(datapoints, K, iter, eps, N)

if __name__ == "__main__":
    main()
