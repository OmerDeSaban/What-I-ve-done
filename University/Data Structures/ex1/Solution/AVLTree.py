# username - omerdavidd
# id      - 315635441
# name    - Omer De-Saban
# I was given an approval by Rani Hod to submit the assignment alone, and late by (at most) 1 week from the deadline.


class AVLNode(object):
    """A class representing a node in an AVL tree"""


    def __init__(self, key, value):
        """Constructor, you are allowed to add more fields. No complexity analysis needed.
            @type key: int or None
            @type value: Any
            @param value: Data of your node"""
        self.key = key
        self.value = value
        self.left = None
        self.right = None
        self.parent = None
        self.height = -1
        # ADDED CLASS FIELDS:
        self.min = None
        self.max = None
        self.size = 0


    # ADDED CLASS METHODS:


    def update_fields(self):
        """Updates 'self's height, min, max and size fields. This method's worst-case time complexity is O(1).
            @return: None."""
        left_node = self.left
        right_node = self.right

        if left_node is not None and left_node.is_real_node():
            left_exists = True
        else:
            left_exists = False

        if right_node is not None and right_node.is_real_node():
            right_exists = True
        else:
            right_exists = False

        if left_exists and right_exists:        # 'self' has 2 children.
            self.height = max(left_node.height, right_node.height) + 1
            self.min = left_node.min
            self.max = right_node.max
            self.size = left_node.size + right_node.size + 1
        elif left_exists:       # 'self's left child is its only child.
            self.height = left_node.height + 1
            self.min = left_node.min
            self.max = self
            self.size = left_node.size + 1
        elif right_exists:      # 'self's right child is its only child.
            self.height = right_node.height + 1
            self.min = self
            self.max = right_node.max
            self.size = right_node.size + 1
        else:       # 'self' has no children.
            self.height = 0
            self.min = self
            self.max = self
            self.size = 1

        return None


    def is_node(self, node):
        """Validates is 'self' and 'node' are the same. This method's worst-case time complexity is O(1).
            @type node: AVLNode.
            @precondition: Both 'self' and 'node' are AVLNodes which aren't None and aren't virtual nodes.
            @param node: The node we'll ensure is equal to 'self'.
            @return: True is the nodes are the same and False otherwise."""
        if self.key != node.key or self.value != node.value or self.left != node.left or self.right != node.right or \
                self.parent != node.parent or self.height != node.height or self.min != node.min or \
                self.max != node.max or self.size != node.size:
            return False

        return True


    def sever_ties(self):
        """Makes sure all of 'self's pointers that should point to another AVLNode are set to None. This method's
            worst-case time complexity is O(1).
            @return: None."""
        self.left = None
        self.right = None
        self.parent = None
        self.min = None
        self.max = None
        return None


    def set_virtual(self):
        """Sets 'self' to become a virtual node by having its key set to None, its height set to -1, its size set to 0,
            and severing its pointers to any other AVLNode. This method's worst-case time complexity is O(1).
            @return: None."""
        self.key = None
        self.height = -1
        self.size = 0
        self.sever_ties()       # O(1) at worst-case.
        return None


    def child_parent_pointers(self, parent_node, direction):
        """Sets 'self's parent pointer to point at 'parent_node', and sets 'parent_node's child (in accordance to
            direction) to point at 'self'. This method's worst-case time complexity is O(1).
            @type parent_node: AVLNode.
            @type direction: str.
            @param direction: ("R" or "r") for right and ("L" or "l") for left.
            @return: None."""
        self.parent = parent_node

        if parent_node is not None and parent_node.is_real_node():		# O(1) at worst-case.
            if direction in ["L", "l"]:     # Setting 'self' to become 'parent's left child.
                parent_node.left = self
            else:     # Setting 'self' to become 'parent's right child.
                parent_node.right = self

        return None


    def get_min(self):
        """Returns the minimal node of the AVLTree whose root is 'self'. This method's worst-case time complexity is
            O(1).
            @rtype: AVLNode.
            @return: The AVLNode in the AVLTree, whose root is 'self', that has the minimal key within said AVLTree."""
        if self is not None and self.is_real_node():		# O(1) at worst-case.
            return self.min

        return None


    def set_min(self, node):
        """Sets the minimal node of the AVLTree whose root is 'self' to 'node'. This method's worst-case time complexity
            is O(1).
            @type node: AVLNode.
            @param node: The updated minimal node of the AVLTree whose root is 'self'.
            @return: None."""
        self.min = node
        return None


    def get_max(self):
        """Returns the maximal node of the AVLTree whose root is 'self'. This method's worst-case time complexity is
            O(1).
            @rtype: AVLNode.
            @return: The AVLNode in the AVLTree, whose root is 'self', that has the maximal key within said AVLTree."""
        if self is not None and self.is_real_node():		# O(1) at worst-case.
            return self.max

        return None


    def set_max(self, node):
        """Sets the maximal node of the AVLTree whose root is 'self' to 'node'. This method's worst-case time complexity
            is O(1).
            @type node: AVLNode.
            @param node: The updated maximal node of the AVLTree whose root is 'self'.
            @return: None."""
        self.max = node
        return None


    def get_size(self):
        """Returns the size of the AVLTree whose root is 'self'. This method's worst-case time complexity is O(1).
            @rtype: int."""
        return self.size


    def set_size(self, s):
        """Sets the size of the AVLTree whose root is 'self' to 's'. This method's worst-case time complexity is O(1).
            @type s: int.
            @param s: The updated size of the AVLTree whose root is 'self'.
            @return: None."""
        self.size = s
        return None


    def change_size(self, delta):
        """Updating the size of the AVLTree whose root is 'self' by adding 'delta' to it. This method's worst-case time
            complexity is O(1).
            @type delta: int.
            @param delta: 'self's' needed size (post-update size) - 'self's current size (pre-update size).
            @return: None."""
        self.size = self.size + delta
        return None


    def balance_factor(self):
        """Calculates 'self's balance factor. This method's worst-case time complexity is O(1).
            @rtype: int.
            @return: The height of 'self's left child - the height of 'self's right child."""
        if self is not None and self.is_real_node():     # O(1) at worst-case.
            if self.left is not None and self.left.is_real_node() and self.right is not None and \
                    self.right.is_real_node():
                return self.left.height - self.right.height
            elif self.left is not None and self.left.is_real_node():
                return self.left.height - (-1)
            elif self.right is not None and self.right.is_real_node():
                return -1 - self.right.height

        return 0


    def get_successor(self):
        """Returns 'self's successor node. This method's worst-case time complexity is O(log_2 (n) ), for 'n' being the
            number of nodes in the AVL tree (including all of 'self's forefathers and descendants, and 'self' itself),
            and occurs when 'self' is the minimum node in the root's left child's subtree (then the successor would be
            the root). Equally, if 'd' is the depth of 'self' in said AVL tree, than this method's worst-case time
            complexity is O(d + 1).
            @precondition: 'self' is not None and self.is_virtual() == False.
            @return: The min field of the right child of 'self', otherwise (if 'self's right child is virtual): 'self's
            ancestor (by parentage) whose key is the smallest of all 'self's ancestors (by parentage) whose keys are
            larger than 'self's."""
        if self.right is not None and self.right.is_real_node():		# 'self's right child is not a virtual node.
            # O(1) at worst-case.
            return self.right.min

        node = self
        parent_node = node.parent

        while parent_node is not None and parent_node.is_real_node() and parent_node.right.is_node(node):     # O(1) +
            # + O(1) = O(1) at worst-case.
            node = parent_node
            parent_node = node.parent

        return parent_node


    def rotation_action(self, new_child, boolean):
        """Performs the rotation (as specified by the 'rotation_node' method), and updates the tree structure
            accordingly. This method's worst-case time complexity is O(1).
            @type new_child: AVLNode.
            @precondition: 'new_child' must be a real node.
            @param new_child: The node that will become the tree's new root and 'self's parent ('self' will be it's
            right or left child already)
            @type boolean: bool.
            @param boolean: Whether we need to return the tree's new root.
            @return: 'new_child' if 'boolean' is True and None otherwise."""
        grandparent = self.parent
        self.parent = new_child
        self.update_fields()
        new_child.update_fields()

        if grandparent is not None and grandparent.is_real_node():
            if self.key < grandparent.key:  # 'self' was 'grandparent's left child.
                direction = "L"
            else:  # 'self' was 'grandparent's right child.
                direction = "R"

            new_child.child_parent_pointers(grandparent, direction)
            grandparent.update_fields()
        else:
            new_child.parent = grandparent

        if boolean:
            return new_child

        return None


    def rotation_node(self, direction, boolean):
        """Rotates the tree whose root is 'self' to the direction stated by 'direction', and returns the root of the
            tree post-rotation if asked (otherwise None). This method's worst-case time complexity is O(1).
            @type direction: str.
            @precondition: 'direction' is either "L", "l", "R" or "r".
            @param direction: The direction in which the tree would be rotated, "L" or "l" for a rotation to the left,
            and "R" or "r" for a rotation to the right.
            @type boolean: bool.
            @param boolean: Whether we need to return a node.
            @return: The tree's root post-rotation if Boolean is True, and None otherwise."""
        if direction in ["L", "l"]:        # Rotating the tree to the left.
            new_child = self.right
            self.right = new_child.left

            if self.right is not None and self.right.is_real_node():
                self.right.parent = self

            new_child.left = self
        else:       # Rotating the tree to the right.
            new_child = self.left
            self.left = new_child.right

            if self.left is not None and self.left.is_real_node():
                self.left.parent = self

            new_child.right = self

        return self.rotation_action(new_child, boolean)     # O(1) at worst-case.


    def in_order(self, direction):
        """Returns an in-order sorted array representing 'self's subtree in the direction stated by 'direction'. This
            method's worst-case (happens when self is a tree's root) time complexity is O(n).
            @type direction: str.
            @precondition: 'direction' is either "L", "l", "R", or "r".
            @param direction: The direction of the subtree we would sort into an array.
            @rtype: list.
            @return: A sorted list (according to the nodes' keys) of the nodes in the fore-mentioned subtree."""
        if direction in ["L", "l"]:     # Sorting 'self's left subtree.
            child = self.left
        else:       # Sorting 'self's right subtree.
            child = self.right

        if child is None or (not child.is_real_node()):
            return []
        else:
            return child.in_order("L") + [child] + child.in_order("R")      # O(1) at worst-case per iteration. There
            # would be O(n) such iterations at worst-case (when child is the root and thus there would be an iteration
            # for every node in the tree), thus O(n) at worst-case.


    # ORIGINAL CLASS METHODS:


    def get_left(self):
        """Returns 'self's left child. This method's worst-case time complexity is O(1).
            @rtype: AVLNode.
            @return: 'self's left child, None if there is no left child (if 'self' is virtual)."""
        if self is not None and self.is_real_node():		# O(1) at worst-case.
            return self.left

        return None


    def get_right(self):
        """Returns 'self's right child. This method's worst-case time complexity is O(1).
            @rtype: AVLNode.
            @return: 'self's right child, None if there is no right child (if 'self' is virtual)."""
        if self is not None and self.is_real_node():		# O(1) at worst case.
            return self.right

        return None


    def get_parent(self):
        """Returns 'self's parent. This method's worst-case time complexity is O(1).
            @rtype: AVLNode.
            @return: 'self's parent, None if 'self' has no parent."""
        return self.parent


    def get_key(self):
        """Returns 'self's key. This method's worst-case time complexity is O(1).
            @rtype: int or None.
            @return: 'self's key, None if 'self' is a virtual node."""
        return self.key


    def get_value(self):
        """Returns 'self's value. This method's worst-case time complexity is O(1).
            @rtype: Any.
            @return: 'self's value, None if 'self' is a virtual node."""
        return self.value


    def get_height(self):
        """Returns 'self's height. This method's worst-case time complexity is O(1).
            @rtype: int.
            @return: 'self's height, -1 if 'self' is a virtual node."""
        return self.height


    def set_left(self, node):
        """Sets 'self's left child to node. This method's worst-case time complexity is O(1).
            @type node: AVLNode.
            @param node: 'self's updated left child.
            @return: None."""
        self.left = node
        return None


    def set_right(self, node):
        """Sets 'self's right child to node. This method's worst-case time complexity is O(1).
            @type node: AVLNode.
            @param node: 'self's updated right child.
            @return: None."""
        self.right = node
        return None


    def set_parent(self, node):
        """Sets 'self's parent to node. This method's worst-case time complexity is O(1).
            @type node: AVLNode.
            @param node: 'self's updated parent.
            @return: None."""
        self.parent = node
        return None


    def set_key(self, key):
        """Sets 'self's key to key. This method's worst-case time complexity is O(1).
            @type key: int or None.
            @param key: 'self's updated key.
            @return: None."""
        self.key = key
        return None


    def set_value(self, value):
        """Sets 'self's value to value. This method's worst-case time complexity is O(1).
            @type value: Any.
            @param value: 'self's updated value.
            @return: None."""
        self.value = value
        return None


    def set_height(self, h):
        """Sets 'self's height to h. This method's worst-case time complexity is O(1).
            @type h: int.
            @param h: 'self's updated height.
            @return: None."""
        self.height = h
        return None


    def is_real_node(self):
        """Returns whether 'self' is NOT a virtual node. This method's worst-case time complexity is O(1).
            @rtype: bool.
            @return: False if 'self' is a virtual node (key is None or height is -1), and True otherwise."""
        if self is None or self.key is None or self.height == -1:       # The condition that 'self' is not None is
            # pretty much redundant, but only costs O(1) so I don't mind leaving it as a special case verification that
            # would always return True (probably).
            return False

        return True



class AVLTree(object):
    """A class implementing the ADT Dictionary, using an AVL tree."""


    def __init__(self):
        """Constructor, you are allowed to add more fields. No complexity analysis needed."""
        self.root = None
        # ADDED CLASS FIELDS:


    # ADDED CLASS METHODS:


    def set_self(self, tree):
        """Sets 'self' to become 'tree'. This method's worst-case time complexity is O(1).
            @type tree: AVLTree.
            @param tree: The tree 'self' should become post-update.
            @return: None."""
        if tree is None:
            self.root = None
        else:
            self.root = tree.root

        return None


    def rotation(self, direction, node):
        """Rotates 'self's subtree whose root is 'node' to the direction stated by 'direction'. This method's worst-case
            time complexity is O(1).
            @type direction: str.
            @precondition: 'direction' is either "L", "l", "R" or "r".
            @param direction: The direction in which the subtree would be rotated, "L" or "l" for a rotation to the
            left, and "R" or "r" for a rotation to the right.
            @type node: AVLNode.
            @param node: The root of the subtree that'll be rotated.
            @return: None."""
        if self.root.get_key() == node.get_key():
            self.root = node.rotation_node(direction, True)     # O(1) at worst-case.
        else:
            node.rotation_node(direction, False)        # O(1) at worst-case.

        return None


    def re_balance(self, parent_node, old_height, post_insertion):
        """Re-balances 'self' and returns the number of re-balancing operations needed to re-balance 'self'.
            Re-balancing operations are as follows:
            Left or right rotations (or both), each rotation counts as 1 re-balancing operation.
            If 'self's parent node's height changed, and NO ROTATION IS NEEDED, 1 re-balancing operation. Note that if
            'self's parent node is None (or virtual), we'll validate if 'self's root's height has changed.
            The worst-case time complexity of each recursive call of this method is O(1). There would be O(log_2 (n) )
            such recursive calls at worst-case (happens when 'paren_node' is a leaf's parent, or even a leaf), and thus
            this method's worst-case time complexity is O(log_2 (n) ).
            @type parent_node: AVLNode.
            @param parent_node: The parent node of the node that changed (either by insertion or deletion).
            @type old_height: int.
            @param old_height: The height of 'parent_node' before ra-balancing the tree.
            @type post_insertion: bool.
            @param post_insertion: Denotes wether we're re-balancing the tree after a node deletion or not.
            @return: The number of re-balancing operations needed to re-balance 'self'."""
        parent_balance_factor = parent_node.balance_factor()
        parent_node.update_fields()
        grandparent = parent_node.get_parent()

        if -2 < parent_balance_factor < 2:		# Either case 3.2 or cse 3.3 (as explained in class).
            if parent_node.get_height() == old_height:		# Case 3.2 (as explained in class).
                carry = 0
            else:		# Case 3.3 (as explained in class).
                carry = 1
        else:		# Case 3.4 (as explained in class).
            left_child = parent_node.get_left()
            right_child = parent_node.get_right()

            if parent_balance_factor == -2:		# Either a left rotation or a (right then left) rotation.
                if (not post_insertion) and right_child.balance_factor() == 0:
                    self.rotation("L", parent_node)  # O(1) at worst-case.
                    carry = 1
                elif right_child.balance_factor() == -1:
                    self.rotation("L", parent_node)     # O(1) at worst-case.
                    carry = 1
                else:		# right_child.balance_factor() == 1.
                    self.rotation("R", parent_node.get_right())     # O(1) at worst-case.
                    self.rotation("L", parent_node)      # O(1) at worst-case.
                    carry = 2
            elif parent_balance_factor == 2:		# Either a (left then right) rotation or a right rotation.
                if left_child.balance_factor() == -1:
                    self.rotation("L", parent_node.get_left())     # O(1) at worst-case.
                    self.rotation("R", parent_node)     # O(1) at worst-case.
                    carry = 2
                elif (not post_insertion) and left_child.balance_factor() == 0:
                    self.rotation("R", parent_node)
                    carry = 1
                else:		# left_child.balance_factor == 1.
                    self.rotation("R", parent_node)
                    carry = 1
            else:
                carry = 0

        if grandparent is None or (not grandparent.is_real_node()):
            return carry
        else:
            return carry + self.re_balance(grandparent, grandparent.get_height(), post_insertion)       # O(1) at
            # worst-case per recursive call.


    def join_with_node(self, tree2, node):
        """Joins 'self' and 'tree2' without creating a new node, rather using the given 'node' instead. This method's
            worst-case time complexity is O(self.root.get_height() - tree2.root.get_height() + 1).
            @type tree2: AVLTree.
            @param tree2: The tree that would be added to 'self' alongside 'node'.
            @type node: AVLNode.
            @param node: The node that would be added to 'self' alongside 'tree2'.
            @precondition: both 'self' and 'tree2' aren't empty, as in their roots are not None nor are they virtual
            nodes.
            @precondition: 'self's height is larger than or equal to 'tree2's height.
            @precondition: 'node' isn't None or a virtual node, but also isn't in 'self' nor in 'tree2'.
            @return: None."""
        child = self.root

        if self.root.get_max().get_key() < tree2.root.get_min().get_key():		# All the keys in 'self' are SMALLER
            # than all the keys in 'tree2'.
            direction = "R"

            while child is not None and child.is_real_node() and tree2.root.get_height() < child.get_height():
                # O(1) at worst-case per iteration. There would be O(self.root.get_height() - tree2.root.get_height())
                # such iteration at the worst-case, and thus O(self.root.get_height() - tree2.root.get_height()) at
                # worst-case.
                child = child.get_right()

            node.set_left(child)
            node.set_right(tree2.root)
        else:		# All the keys in 'self' are LARGER than all the keys in 'tree2' (happens when I flip them to ensure
            # 'self' is longer than 'tree2' in the original 'join' method).
            direction = "L"

            while child is not None and child.is_real_node() and tree2.root.get_height() < child.get_height():
                # O(1) at worst-case per iteration. There would be O(self.root.get_height() - tree2.root.get_height())
                # such iteration at the worst-case, and thus O(self.root.get_height() - tree2.root.get_height()) at
                # worst-case.
                child = child.get_left()

            node.set_left(tree2.root)
            node.set_right(child)

        parent_node = child.get_parent()
        node.child_parent_pointers(parent_node, direction)
        child.set_parent(node)
        tree2.root.set_parent(node)
        node.update_fields()
        parent_node.update_fields()
        self.re_balance(parent_node, parent_node.get_height(), False)
        return None


    # ORIGINAL CLASS METHODS:


    def search(self, key):
        """Searches for an AVLNode in the dictionary, corresponding to 'key'. The worst-case time complexity of each
            recursive call of this method is O(1). There would be O(log_2 (n) such recursive calls at worst-case
            (happens when 'key' is in 'self', and corresponds to a leaf), thus this method's worst-case time complexity
            is O(log_2 (n) ).
            @type key: int.
            @param key: A key to be searched.
            @rtype: AVLNode.
            @return: The AVLNode corresponding to 'key', or None if 'key' isn't found in the dictionary."""
        if self.root is None or (not self.root.is_real_node()):        # The dictionary is empty.
            return None
        elif key < self.root.get_min().get_key() or key > self.root.get_max().get_key():        # 'key' isn't in the
            # dictionary.
            return None
        elif key == self.root.get_key():		# 'key' has been found at the AVLTree's root.
            return self.root
        elif key == self.root.get_min().get_key():		# 'key' has been found at the AVLTree's minimum.
            return self.root.get_min()
        elif key == self.root.get_max().get_key():		# 'key' has been found at the AVLTree's maximum.
            return self.root.get_max()
        elif key > self.root.get_key():		# 'key' is in the AVLTree's root's right subtree.
            right_subtree = AVLTree()
            right_subtree.root = self.root.get_right()
            return right_subtree.search(key)        # O(1) at worst-case per recursive call.
        else:		# 'key' is in the AVLTree's root's left subtree.
            left_subtree = AVLTree()
            left_subtree.root = self.root.get_left()
            return left_subtree.search(key)        # O(1) at worst-case per recursive call.


    def insert(self, key, val):
        """Inserts a new node whose value is 'val' and key is 'key' into the tree. This method's worst-case time
            complexity is O(log_2 (n) ).
            @type key: int.
            @precondition: key currently DOES NOT APPEAR in the dictionary.
            @param key: The key of the new node that's to be inserted to 'self'.
            @type val: Any.
            @param val: The value of the new node that's to be inserted into 'self'.
            @rtype: int.
            @return: The number of re-balancing operations needed to re-balance 'self' post-insertion (see 're-balance'
            method)."""
        new_node = AVLNode(key, val)
        new_node.update_fields()

        if self.root is None or (not self.root.is_real_node()):		# The dictionary is empty.
            self.root = new_node
            return 0
        elif key < self.root.get_min().get_key():		# 'key' is SMALLER than all the keys in the dictionary.
            parent_node = self.root.get_min()
            new_node.child_parent_pointers(parent_node, "L")
        elif key > self.root.get_max().get_key():		# 'key' is LARGER than all the keys in the dictionary.
            parent_node = self.root.get_max()
            new_node.child_parent_pointers(parent_node, "R")
        else:		# self.root.get_min().get_key() < 'key' < self.root.get_max().get_key() .
            parent_node = self.root

            while parent_node is not None and parent_node.is_real_node():       # O(1) at worst-case per iteration.
                # There would be O(log_2 (n) ) such iteration at worst-case, and thus O(log_2 (n) ) at worst-case.
                if key < parent_node.get_key():		# 'new_node' is in 'parent_node's left subtree.
                    if parent_node.get_left() is not None and parent_node.get_left().is_real_node():
                        parent_node = parent_node.get_left()
                        continue
                    else:		# 'parent_node' has no left child - we will insert 'new_node' as its new left child
                        new_node.child_parent_pointers(parent_node, "L")
                        break
                else:		# 'key' > 'parent_node'.get_key(), thus 'new_node' is in 'parent_node's right subtree.
                    if parent_node.get_right() is not None and parent_node.get_right().is_real_node():
                        parent_node = parent_node.get_right()
                        continue
                    else:		# 'parent_node' has no right child - we will insert 'new_node' as its new right child.
                        new_node.child_parent_pointers(parent_node, "R")
                        break

        parent_node.update_fields()
        return self.re_balance(parent_node, parent_node.get_height(), True)       # O(log_2 (n) ) at worst-case.


    def delete(self, node):
        """Deletes 'node' from the dictionary. This method's worst-case time complexity is O(log_2 (n) ).
            @type node: AVLNode.
            @precondition: 'node' is A REAL POINTER TO A NODE IN 'self', as in 'self'.search('node'.key) is not None.
            @rtype: int.
            @return: The number of re-balancing operations needed to re-balance 'self' post-deletion (see 're-balance'
            method)."""
        node_parent = node.get_parent()
        node_left = node.get_left()
        node_right = node.get_right()

        if node_left is not None and node_left.is_real_node() and node_right is not None and node_right.is_real_node():
            # 'node' has 2 children (a left child and a right child). This case's worst-case time complexity is
            # O(log_2 (n) ).
            successor = node.get_successor()		# O(log_2 (n) ) at worst-case. Note that 'successor_parent' must be
            # a real node (as in 'successor' can't be 'self''s root). This is because we know that 'node' has 2
            # children, thus 'successor' must be in 'node's right subtree, since 'node'.get_right() isn't None nor is it
            # a virtual node (see 'get_successor' method). Thus, if 'successor' is 'self' root, then 'self'.root is
            # 'node's right child, which means that 'successor' can't be 'self's root. Therefore, this line's true time
            # complexity is O(1). Also note that 'successor' has no left child, otherwise said left child would have
            # been 'node's successor instead. Thus, 'successor' has 1 child at most, which could only be a right child.
            successor_child = successor.get_right()  # Might not exist (as in might be None or a virtual node).

            if not successor.is_node(node_right):       # 'node's successor isn't its right child, thus 'node_right' has
                # a left child (could also have a right child regardless).
                successor_parent = successor.get_parent()
                node_left.child_parent_pointers(successor, "L")
                node_right.child_parent_pointers(successor, "R")
                successor.update_fields()

                if successor_child is not None and successor_child.is_real_node():      # 'successor' has a child.
                    successor_child.child_parent_pointers(successor_parent, "L")
                else:
                    successor_parent.set_left(successor_child)

                successor_parent.update_fields()
            else:       # 'node's successor is its right child, thus 'node_right' doesn't have a left child (but could
                # still have a right child regardless, just no grandchildren). Note that in this case, 'successor's
                # parent is 'node_parent'.
                node_left.child_parent_pointers(successor, "L")
                successor.update_fields()

            if node_parent is not None and node_parent.is_real_node():      # 'node' wasn't the tree's root.
                if node.get_key() < node_parent.get_key():      # 'node' was 'node_parent's left child.
                    successor.child_parent_pointers(node_parent, "L")
                else:       # 'node' was 'node_parent's right child.
                    successor.child_parent_pointers(node_parent, "R")

                node_parent.update_fields()
            else:
                successor.set_parent(node_parent)

            node.sever_ties()  # Completely optional but why not? Adds just O(1).
            node.set_virtual()
            return self.re_balance(successor, successor.get_height(), False)  # O(log_2 (n) ) at worst-case.
        elif (node_left is not None and node_left.is_real_node()) or \
                (node_right is not None and node_right.is_real_node()):  # 'node' has only 1 child (a left child or a
            # right child). This case's worst-case time complexity is O(1).
            if node_parent is not None and node_parent.is_real_node():		# 'node' wasn't the tree's root.
                if node_left is not None and node_left.is_real_node():      # 'node's left child is its only child.
                    child_node = node_left
                else:       # 'node's right child is its only child.
                    child_node = node_right

                if node.get_key() < node_parent.get_key():      # 'node' is 'node_parent's left child.
                    direction = "L"
                else:  # 'node' is 'node_parent's right child.
                    direction = "R"

                child_node.child_parent_pointers(node_parent, direction)
                node_parent.update_fields()
            else:		# 'node' was the tree's root.
                if node_left is not None and node_left.is_real_node():		# 'node's left child is its only child.
                    self.root = node_left
                else:		# 'node's right child is its only child.
                    self.root = node_right

                self.root.sever_ties()
                self.root.update_fields()
                node.sever_ties()  # Completely optional but why not? Adds just O(1).
                node.set_virtual()
                return 1
        else:		# 'node' has no children. This case's worst-case time complexity is O(1).
            if node_parent is not None and node_parent.is_real_node():		# 'node' wasn't the tree's root.
                if node.get_key() < node_parent.get_key():		# 'node' is 'node_parent's left child.
                    node_parent.set_left(node_left)  # Setting 'node_parent's left child to None (or to a virtual
                    # node).
                else:		# 'node' is 'node_parent's right child.
                    node_parent.set_right(node_right)  # Setting 'node_parent's right child to None (or to a
                    # virtual node).

                node_parent.update_fields()
            else:		# 'node' was the tree's root.
                self.root = node_left		# Setting the tree's root to None (or to a virtual node).
                node.sever_ties()  # Completely optional but why not? Adds just O(1).
                node.set_virtual()
                return 0

        node.sever_ties()		# Completely optional but why not? Adds just O(1).
        node.set_virtual()
        return self.re_balance(node_parent, node_parent.get_height(), False)       # O(log_2 (n) ) at worst-case.


    def avl_to_array(self):
        """Returns an array representing the dictionary (after sorting it). This method's worst-case time complexity is
            O(n).
            @rtype: list.
            @return: A list of the elements in the dictionary post-sorting according to their keys. Each element is
            represented by a tuple (element.key, element.value)."""
        return self.root.in_order("L") + [self.root] + self.root.in_order("R")      # O(n) at worst-case.


    def size(self):
        """Returns the number of items in the dictionary. This method's worst-case time complexity is O(1).
            @rtype: int.
            @return: The number of items in the dictionary."""
        if self.root is None or (not self.root.is_real_node()):
            return 0

        return self.root.get_size()


    def split(self, node):
        """Splits the dictionary at the 'i'th index (here 'i' is the index of 'node' in the array representing the
            dictionary, see 'avl_to_array' method). This method's worst-case time complexity is P((log_2 (n) ) ^ 2), if
            we're being naive. Further examination will reveal the true worst-case time complexity is O(log_2 (n) ), as
            shown in class.
            @type node: AVLNode.
            @precondition: 'node' is IN 'self'.
            @param node: The intended node in the dictionary according to whom we split.
            @rtype: list.
            @return: A list [left_subtree, right_subtree], where 'left_subtree' is an AVLTree representing the keys in
            the dictionary that are SMALLER than 'node'.key, and 'right_subtree' is an AVLTree representing the keys in
            the dictionary that are LARGER than 'node'.key ."""
        if self.search(node.get_key()) is not None:     # O(log_2 (n) ) at worst-case to validate.
            left_subtree = AVLTree()
            left_subtree.root = node.get_left()
            right_subtree = AVLTree()
            right_subtree.root = node.get_right()
            current_node = node
            parent_node = current_node.get_parent()

            while parent_node is not None:      # O(log_2 (n) ) at worst-case per iteration. There would be
                # O(log_2 (n) ) such iterations at worst-case (happens when 'node' is a leaf's parent), and thus
                # O((log_2 (n) ) ^ 2) at worst-case, if we're being naive. But if we look closer we can actually realize
                # the worst-case time complexity is O(log_2 (n) ), as explained in class.
                tmp_tree = AVLTree()

                if current_node.get_key() < parent_node.get_key():		# 'node' is in 'parent_node's left subtree.
                    tmp_tree.root = parent_node.get_right()
                    right_subtree.join_with_node(tmp_tree, parent_node)     # O(log_2 (n) ) at worst-case.
                else:		# node.get_key() > parent_node.get_key(), thus 'node' is in 'parent_node's right subtree.
                    tmp_tree.root = parent_node.get_left()
                    tmp_tree.join_with_node(left_subtree, parent_node)      # O(log_2 (n) ) at worst-case.
                    left_subtree.set_self(tmp_tree)

                current_node = parent_node
                parent_node = current_node.get_parent()

            node.set_virtual()
            return [left_subtree, right_subtree]
        else:
            return [self]


    def join(self, tree2, key, val):
        """Joins 'self' with 'key' and another AVLTree. This method's worst-case time complexity is O(log_2 (n) ).
            @type tree2: AVLTree.
            @param tree2: A dictionary to be joined with 'self'.
            @type key: int.
            @param key: The key separating 'self' with 'tree2'.
            @type val: Any.
            @param val: The value attached to 'key'.
            @precondition: All keys in 'self' are SMALLER than 'key', and all keys in 'tree2' are LARGER than 'key'.
            @rtype: int.
            @return: The absolute value of the difference between the height of the two AVLTrees we've joined."""
        if self.root is None or (not self.root.is_real_node()):		# 'self' is an empty AVLTree. This case's worst-case
            # time complexity is O(log_2 (n) ).
            result = tree2.root.get_height() + 2
            tree2.insert(key, val)      # O(log_2 (n) ) at worst-case.
            self.set_self(tree2)		# We don't need to use it again, just to ensure we updated self and not tree2.
            tree2.set_self(None)		# We don't need to use it again, just to ensure that tree2 is unreachable at the
            # end of the method's run.
            return result
        elif tree2.root is None or (not tree2.root.is_real_node()):		# 'tree2' is an empty AVLTree. This case's
            # worst-case time complexity is O(log_2 (n) ).
            result = self.root.get_height() + 2
            self.insert(key, val)       # O(log_2 (n) ) at worst-case.
            tree2.set_self(None)		# We don't need to use it again, just to ensure that tree2 is unreachable at the
            # end of the method's run.
            return result
        elif self.root.get_height() < tree2.root.get_height():		# Making sure 'self's height is greater or equal to
            # 'tree2's height. This case's worst-case time complexity (for operations inside the 'elif' loop) is O(1).
            tmp = self.root
            self.root = tree2.root
            tree2.root = tmp

        result = self.root.get_height() - tree2.root.get_height() + 1
        node = AVLNode(key, val)
        self.join_with_node(tree2, node)        # O(log_2 (n) ) at worst-case.
        tree2.set_self(None)		# We don't need to use it again, just to ensure that tree2 is unreachable at the end
        # of the method's run.
        return result


    def get_root(self):
        """Returns the root of the tree representing the dictionary. This method's worst-case time complexity is O(1).
            @type: AVLNode.
            @return" The root of the tree representing the dictionary, and None if the dictionary is empty."""
        if self.root is None or (not self.root.is_real_node()):     # O(1) at worst-case.
            return None

        return self.root
