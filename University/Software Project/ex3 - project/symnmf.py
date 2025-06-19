import sys
import pandas as pd
import numpy as np
import math
import symnmfmod as symnmfC
from typing import Tuple, List, Callable

np.random.seed(1234)

def printError() -> None:
    """Prints the general error message "An Error has Occurred exits the program. 
    Args:
        None: The error message is constant, thus there's no need for arguments.
    Returns:
        None: This method does not return a value as it exits the program after printing."""
    print("An Error Has Occurred", flush=True)
    sys.exit(1)

def argvToVars() -> Tuple:
    """Allocate the user CMD arguments into variables and return the variables as a tuple.
    The method extracts the command-line arguments (excluding the script name) and returns 3 variables.
    Args:
        None: This method takes no arguments as it simply reads command-line arguments and parses them.
    Returns:
        Tuple: A tuple containing the following 3 elements:
            Tuple[0] ( int ): The number of desired clusters (or centroids).
            Tuple[1] ( str ): The desired goal.
            Tuple[3] ( str ): The filename of the .txt file."""
    if len(sys.argv) != 4:  # Invalid number of CMD arguments:
        printError()
    try:
        K = float(sys.argv[1])
    except:
        printError()
    if (K < 0) or (K != int(K)):
        printError()
    K = int(K)  # Need to validate that K is smaller than N! (This will be done a bit later)
    goal = sys.argv[2]
    if ((goal != "sym") and (goal != "ddg") and (goal != "norm") and (goal != "symnmf")):
        printError()
    file_name = sys.argv[3]
    if (len(file_name) < 4) or (file_name[-4:] != ".txt"):
        printError()
    return (K, goal, file_name)

def createPoints(file_name: str, K: int) -> Tuple:
    """Creates the points from the given file.
    Validates that K is smaller than the number of given points.
    Args:
        file_name (str): The name of the given .txt file.
        K (int) : The desired number of symnmf clusters (or centroids).
    Returns:
        Tuple: A tuple containing the following 2 elements:
            Tuple[0] ( List[List[float]] ): The created points.
            Tuple[1] ( int ): The number of created points."""
    df = pd.read_csv(file_name, sep = ",", header = None)
    N = len(df.index)
    if K >= N:  # Finished validating K.
        printError()
    try:
        np_points = [np.array(df.iloc[i]) for i in range(N)]
    except: # Will fail if we received points of different dimensions.
        printError()
    points_list = [np_point.tolist() for np_point in np_points]
    return (points_list, N)

def getRes(funct: Callable, args: List[List[List[float]]]) -> List[List[float]]:
    """Attempts to get a matrix calculated via C.
    Args:
        funct (Callable): The C function that's needed to get the matrix.
        args (List[List[List[float]]]): A list of all the matrices given to C as arguments.
    Returns:
        List[List[float]]: The matrix calculated via C."""
    try:
        if len(args) == 1:
            return funct(args[0])
        else:   # len(args) == 2
            return funct(args[0], args[1])
    except:
        printError()

def initH(K: int, points_list: List[List[float]], N: int) -> Tuple:
    """Initializes the decomposition matrix.
    Args:
        K (int): The desired number of clusters (or centroids).
        point_list (List[List[float]]): All the given points.
        N (int): The number of points.
    Returns:
        Tuple: A tuple containing the following 2 elements:
            Tuple[0] ( List[List[float]] ): The initialized decomposition matrix.
            Tuple[1] ( List[List[float]] ): The normalized similarity matrix."""
    W = getRes(symnmfC.norm, [points_list])
    upper_value = 0
    for i in range(N):  # Getting the sum of all W's entries
        upper_value += sum(W[i])
    upper_value = upper_value / math.pow(N, 2)  # Average of all W's entries
    upper_value = 2 * math.sqrt(upper_value / K)    # m/K
    H_np = np.random.uniform(0, upper_value, size=(N, K))
    H = [H_line.tolist() for H_line in H_np]
    return (H, W)

def main() -> None:
    """Reads the user CMD arguments and performs the symNMF algorithm using them, via a C-Python API.
    Args:
        None: This method takes no arguments as it reads command-line arguments and uses them.
    Returns:
        None: This method does not return a value as it prints the result as long as no error occurred.
    
    For more information about the symNMF algorithm, see:
        https://epubs.siam.org/doi/epdf/10.1137/1.9781611972825.10"""
    (K, goal, file_name) = argvToVars()
    (points_list, N) = createPoints(file_name, K)
    columns = N
    if (goal == "sym"):
        res_mat = getRes(symnmfC.sym, [points_list])
    elif (goal == "ddg"):
        res_mat = getRes(symnmfC.ddg, [points_list])
    elif (goal == "norm"):
        res_mat = getRes(symnmfC.norm, [points_list])
    else:   # goal == "symnmf"
        columns = K
        (H, W) = initH(K, points_list, N)
        res_mat = getRes(symnmfC.symnmf, [H, W])
    res = ""
    for i in range(N):  # Concatenating the result matrix while also rounding all its elements:
        for j in range(columns):
            if j != (columns - 1):
                res += f"{res_mat[i][j]:.4f}" + ","
            elif i == N:
                res += f"{res_mat[i][j]:.4f}"
            else:
                res += f"{res_mat[i][j]:.4f}" + "\n"
    print(res)
    return None

if __name__ == "__main__":
    main()
