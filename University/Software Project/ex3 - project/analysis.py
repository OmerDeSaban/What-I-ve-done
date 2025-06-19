import sys
import pandas as pd
import numpy as np
import math
import symnmfmod as symnmfC
from sklearn.metrics import silhouette_score
from typing import List, Dict, Tuple
from symnmf import printError, createPoints, initH


def argvToVars() -> Tuple:
    """Allocate the user CMD arguments into variables and return the variables as a tuple.
    The method extracts the command-line arguments (excluding the script name) and returns 3 variables.
    Args:
        None: This method takes no arguments as it simply reads command-line arguments and parses them.
    Returns:
        Tuple: A tuple containing the following 2 elements:
            Tuple[0] ( int ): The number of desired clusters (or centroids).\
            Tuple[2] ( str ): The filename of the .txt file."""
    if len(sys.argv) != 3:  # Invalid number of CMD arguments:
        printError("An Error Has Occurred")
    try:
        K = float(sys.argv[1])
    except:
        printError("An Error Has Occurred")
    if (K < 0) or (K != int(K)):
        printError("An Error Has Occurred")
    K = int(K)  # Need to validate that K is smaller than N! (This will be done a bit later)
    file_name = sys.argv[2]
    if (len(file_name) < 4) or (file_name[-4:] != ".txt"):
        printError()
    return (K, file_name)

def euclideanDistance(point1: List[float], point2: List[float]) -> float:
    """Calculates and returns the Euclidean Distance between the 2 given datapoints.
    Args:
        point1 (List[float]): A single point.
        point2 (List[float]): A single point.
        N (int): The number of points.
    Returns:
        float: The Euclidean Distance between the 2 given datapoints.
    
    For more information about Euclidean distance, see:
        https://en.wikipedia.org/wiki/Euclidean_distance"""
    res = 0.0
    for index in range(len(point1)):
        res += math.pow((point1[index] - point2[index]), 2)
    res = math.sqrt(res)
    return res

def allocate(points_list: List[List[float]], centroids: List[List[float]], N: int, K: int, \
             allocation: Dict[str, List[List[List[float]]]]) -> Dict[str, List[List[List[float]]]]:
    """Allocating each datapoint to a list representing its closest centroid.
    Args:
        points_list (List[List[float]]): All the points.
        centroids (List[List[float]]): All the centroids.
        N (int): The number of points.
        K (int): The desired number of clusters (or centroids).
        allocation (Dict[str, List[List[List[float]]]]): The allocation dictionary, whose keys are the
                string representation of the relevant centroid's index in centroids, and whose values are
                lists containing the points for which the respective centroid is closest.
    Returns:
        Dict[str, List[List[List[float]]]]: The updated allocation dictionary."""
    for point in points_list:
        centroid_index = 0
        min_distance = euclideanDistance(point, centroids[0])
        if (K > 1):
            for i in range(1, K):
                distance_i = euclideanDistance(point, centroids[i])
                if (distance_i < min_distance):
                    min_distance = distance_i
                    centroid_index = i
        allocation[str(centroid_index)].append(point)
    return allocation

def centroidUpdate(points_list: List[List[float]], dim: int) -> List[float]:
    """Updates and returns the centroid after every loop over all the points.
    Args:
        points_list (List[List[float]]): All the points allocated to this centroid.
        dim (int): The shared dimension of all centroids and points.
    Returns:
        List[float]: The updated centroid."""
    centroid = [0.0]*dim
    for index in range(dim):
        for point in points_list:
            centroid[index] += point[index]
        centroid[index] /= len(points_list)
    return centroid

def iterationEnd(centroids: List[List[float]], allocation: Dict[str, List[List[List[float]]]], \
                         epsilon: float, N: int, K: int) -> \
                            Tuple[List[List[float]], Dict[str, List[List[List[float]]]], bool]:
    """Updates the given centroids to contain the new centroids, the allocation dictionary's values to be
    empty lists, and checks if the new centroid's Euclidean Distance to its respective previous centroid
    is smaller than the given epsilon.
    Args:
        centroids (List[List[float]]): All the current centroids.
        allocation (Dict[str, List[List[List[float]]]]): The allocation dictionary, whose keys are the
                string representation of the relevant centroid's index in centroids, and whose values are
                lists containing the points for which the respective centroid is closest.
        epsilon (float): The value of convergence.
        N (int): The number of points.
        K (int): The desired number of clusters (or centroids).
    Returns:
        Tuple: A tuple containing the following 3 elements:
            Tuple[0] ( List[List[float]] ): The updated centroids.
            Tuple[1] ( Dict[str, List[List[List[float]]]] ): The updated allocation.
            Tuple[2] ( bool ): True if said Euclidean Distance is smaller than epsilon, and False
                                otherwise."""
    epsilon_is_greater = True   # Would only be changed if at least 1 of the centriod deltas is equal to,
                                # or larger than, epsilon.
    for j in range(K):
        old_centroid = centroids[j]
        new_centroid = centroidUpdate(allocation[str(j)], len(centroids[0]))
        allocation[str(j)] = []     # Ensuring that in the next iteration we'd restart the
                                    # datapoint allocation to centroids without remembering
                                    # what was the allocation in the previous iteraion.
        if euclideanDistance(old_centroid, new_centroid) >= epsilon:
            epsilon_is_greater = False
        centroids[j] = new_centroid     # Ensuring that in the beginning of the next iteraion
                                        # we'd use the updated centroids.
    return (centroids, allocation, epsilon_is_greater)

def centroidCalculation(points_list: List[List[float]], centroids: List[List[float]], K: int, N: int, \
                        epsilon: float, allocation: Dict[str, List[List[List[float]]]], \
                            epsilon_is_greater: bool) -> List[List[float]]:
    """Calculates the centroids in each iteration.
    Args:
        points_list (List[List[float]]): All the points.
        centroids (List[List[float]]): The initial centroids.
        K (int): The desired number of clusters (or centroids).
        N (int): The number of points.
        epsilon (float): The value of convergence.
        allocation (Dict[str, List[List[List[float]]]]): The allocation dictionary, whose keys are the
                string representation of the relevant centroid's index in centroids, and whose values are
                lists containing the points for which the respective centroid is closest.
        epsilon_is_greater (bool): True if the centroid delta is smaller than epsilon, and False
                otherwise.
    Returns:
        List[List[float]]: The final centroids."""
    iteration_num = 0
    while ((iteration_num < 300) and (not epsilon_is_greater)):
        if iteration_num == 0:      # If we're at the first iteration, we already allocated K points as
                                    # the initial centroids
            allocation = allocate(points_list[K:], centroids, N, K, allocation)
        else:
            allocation = allocate(points_list, centroids, N, K, allocation)
        (centroids, allocation, epsilon_is_greater) = iterationEnd(centroids, allocation, epsilon, N, K)
        iteration_num += 1
    return centroids

def kmeans(K: int, points_list: List[List[float]], N: int) -> List[int]:
    """Prints the Kmeans partition of the data in the file_name.txt file.
    Args:
        K (int): The desired number of clusters (or centroids).
        points_list (List[List[float]]): All the points.
        N (int): The shared dimension of every point, and the number of points.
    Returns:
        List[int]: The final centroids' labels.
    
    For more information about the Kmeans algorithm, see:
        https://en.wikipedia.org/wiki/K-means_clustering"""
    centroids = [element for element in points_list[:K]]    # This will eventually give us the centuroids,
                                                            # but at the first iteration, this should be
                                                            # the first 'K' datapoints.
    # Creating a dictionary that shows the current allocation of datapoints to centroids:
    allocation = {str(i) : [centroids[i]] for i in range(K)}
    centroids = centroidCalculation(points_list, centroids, K, N, 0.001, allocation, False)
    labels = []
    for point in points_list:
        min_dist = euclideanDistance(point, centroids[0])
        label = 0
        if (K > 1):
            for i in range(1, K):
                dist = euclideanDistance(point, centroids[i])
                if dist < min_dist:
                    min_dist = dist
                    label = i
        labels.append(label)
    return labels

def labelClusters(matrix: List[List[float]]) -> List[float]:
    """Labels all the matrix's rows to their respective cluster.
    Args:
        matrix (List[List[float]]): The given matrix.
    Returns:
        List[float]: A list of all the labels."""
    labels = []
    for row in matrix:
        row_max = max(row)
        max_indx = row.index(row_max)
        labels.append(max_indx)
    return labels

def main() -> None:
    """Reads the user CMD arguments and performs the analysis.
    Args:
        None: This method takes no arguments as it reads command-line arguments and uses them.
    Returns:
        None: This method does not return a value as it prints the result as long as no error occurred.
    """
    (K, file_name) = argvToVars()
    (points_list, N) = createPoints(file_name, K)
    (H, W) = initH(K, points_list, N)
    H = symnmfC.symnmf(H, W)
    symnmf_labels = labelClusters(H)    # Now we have the symnmf clusters labeled.
    print("nmf: %.4f" % silhouette_score(points_list, symnmf_labels))
    kmeans_labels = kmeans(K, points_list, N)   # Now we have the kmeans clusters labeled.
    print("kmeans: %.4f" % silhouette_score(points_list, kmeans_labels))
    return None

if __name__ == "__main__":
    main()
