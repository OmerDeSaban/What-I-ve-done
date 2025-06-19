/*
 * Name:			Omer David De-Saban
 * ID:				315635441
 * username:		omerdavidd
 * (Rani Hod approved that I could submit alone)
 */

/**
 * BinomialHeap is a non-lazy binomial heap.
 *
 * An implementation of a non-lazy binomial heap over non-negative integers.
 * Based on exercise from previous semester.
 */
public class BinomialHeap
{
	// ORIGINALLY GIVEN CLASS FIELDS:
	public int size;
	public HeapNode last;
	public HeapNode min;
	// ADDED CLASS FIELDS:
	public HeapNode first;
	public int trees_num;
	
	
	// ADDED CLASS METHODS AND SUBCLASSES:
	
	
	/**
	 * A default class builder for the BinomialHeap class.
	 * This method' time complexity is O(1).
	 * @return:		null.
	 */
	public BinomialHeap()
	{
		/* I could've left this builder empty and Java's default empty builder would've done the same. I
		 * decided to write this anyway for clarity reasons :)
		 */
		this.size = 0;
		this.last = null;
		this.min = null;
		this.first = null;
		this.trees_num = 0;
	}
	
	
	/**
	 * Generates a new BinomialHeap object that represents 'node's children (without 'node' itself).
	 * This method's time complexity is O('node'.rank + 1).
	 * @param 'node':		The HeapNode object whose children would be turned into the new BinomialHeap
	 * object.
	 * @return:				A BinomialHeap representing 'node's children, as described above.
	 */
	private static BinomialHeap heapFromChildren(HeapNode node)
	{
		if (node == null || node.rank == 0) { return new BinomialHeap(); }
		else
		{
			BinomialHeap result = new BinomialHeap();
			result.size = (int) Math.pow(2, node.rank) - 1;
			result.last = node.last;
			result.min = node.min;
			result.first = node.child;
			result.trees_num = node.rank;
			HeapNode current = result.first;
			while (current != null)
			{
				current.parent = null;
				current = current.next;
			}
			return result;
		}
	}
	
	
	/**
	 * Generates a new BinomialHeap object that represents 'this' without the binomial tree whose root is
	 * 'node'.
	 * This method's time complexity is O('this'.trees_num + 1).
	 * @param 'node':		The node representing the binomial tree that'd be "severed" from 'this'.
	 * @return:				A BinomialHeap object representing 'this' after severing the binomial tree whose
	 * root was 'node'.
	 */
	private BinomialHeap heapWithoutATree(HeapNode node)
	{
		if (node == null) { return this; }
		else if (this.empty() || (this.trees_num == 1 && this.first == node)) { return new BinomialHeap(); }
		
		BinomialHeap result = new BinomialHeap();
		result.size = this.size - (int) Math.pow(2,  node.rank) + 1;
		result.trees_num = this.trees_num - 1;
		HeapNode current = this.first;
		while (current != null)
		{
			if (current == node) { current = current.next; }
			else
			{
				if (result.last == null || current.rank > result.last.rank) { result.last = current; }
				
				if (result.min == null || current.item.key < result.min.item.key)
				{	result.min = current; }
				
				if (result.first == null) { result.first = current; }
				
				if (current.next == node) { current.next = current.next.next; }
				
				current = current.next;
			}
		}
		return result;
	}
	
	
	/**
	 * Sets 'this' to have the same fields as a given BinomialHeap object.
	 * @param 'heap':		A BinomialHeap object whose fields would be copied into 'this's fields.
	 */
	private void setSelf(BinomialHeap heap)
	{
		this.size = heap.size;
		this.last = heap.last;
		this.min = heap.min;
		this.first = heap.first;
		this.trees_num = heap.trees_num;
	}
	
	
	// ORIGINALLY GIVEN CLASS METHODS AND SUBCLASSES:
	
	
	/**
	 * Insert ('key', 'info') into the heap.
	 * This method's worst-case time complexity is O('this'.trees_num + 1 + 1) = O('this'.trees_num + 1).
	 * @precondition:		'key' > 0.
	 * @return:				The newly generated HeapItem.
	 */
	public HeapItem insert(int key, String info) 
	{
		HeapItem item;
		if (this.empty())
		{
			item = new HeapItem(key, info);
			HeapNode node = new HeapNode(item);
			this.size = 1;
			this.last = node;
			this.min = node;
			this.first = node;
			this.trees_num = 1;
		}
		else
		{
			BinomialHeap heap2 = new BinomialHeap();
			item = heap2.insert(key,  info);
			this.meld(heap2);
		}
		return item;
	}

	
	/**
	 * Delete the heap's minimal item.
	 * Denote n = number of HeapNode objects in all of the binomial trees in 'this' combined. Thus this
	 * method's worst-case time complexity is O(Math.ceil( Math.log(n) / Math.log(2) ) + 1).
	 */
	public void deleteMin()
	{
		if (this != null || !this.empty())
		{		/* Note that since this method can only be called using a BinomialHeap object which isn't
				 * null, the first condition of this 'if' statement should always be met, it's just here for
				 * extra clarity :)
				 */
			if (this.size > 1)
			{		/* 'this'.size - 1 > 0, thus 'this'.size > 1. This means that 'this' includes at least 1
				 	 * node other than 'this'.min, and thus 'this'.min has at least 1 child or (mathematical
				 	 * or, as in there could be a scenario where both condition are met) 1 sibling. Said
				 	 * sibling could be either 'this'.min.next, or a HeapNode objects whose next
				 	 * pointer-field points on 'this'.min . In terms of this method - either
				 	 * 'heap_from_children' or 'heap_without_min_tree' wouldn't be null or empty.
				 	 */
				BinomialHeap heap_from_children = heapFromChildren(this.min);
				BinomialHeap heap_without_min_tree = this.heapWithoutATree(this.min);
				/* Note that both 'heap_from_children' and 'heap_without_min_tree' cannot be null due to the
				 * methods using which they are generated, I still added a verification to check if either is
				 * null in the same 'if' and 'else if' statements where I verified they aren't empty, but
				 * that's just for clarity :)
				 */
				BinomialHeap new_this;
				if (heap_from_children == null || heap_from_children.empty())
				{	new_this = heap_without_min_tree; }
				else if (heap_without_min_tree == null || heap_without_min_tree.empty())
				{	new_this = heap_from_children; }
				else
				{
					new_this = heap_without_min_tree;
					new_this.meld(heap_from_children);
				}
				this.setSelf(new_this);
			}
			else { this.setSelf(new BinomialHeap()); }
		}
		/* If 'this' is empty, there's nothing to delete so we don't need to do anything.
		 */
	}

	
	/**
	 * Finds the heap's minimal item.
	 * This method's worst-case time complexity is O(1).
	 * @return:		The minimal HeapItem, if it exists and 'null' otherwise.
	 */
	public HeapItem findMin()
	{
		if (min != null) { return min.item; }
		else { return null; }
	}

	
	/**
	 * Decrease the key of 'item' by 'diff' and fix the heap.
	 * This method's worst-case time complexity is O(1).
	 * @precondition:		0 < 'diff' < 'item'.key.
	 */
	public void decreaseKey(HeapItem item, int diff) 
	{
		item.key -= diff;
		item.node.shiftUp();
	}

	
	/**
	 * Delete 'item' from the heap.
	 * This method's worst-case time complexity is O(Math.ceil( Math.log(n) / Math.log(2) ) + 1).
	 * @precondition:		'this' (the heap) isn't empty, and 'item' is within 'this' (the heap).
	 */
	public void delete(HeapItem item) 
	{
		if (item.node != this.min)
		{
			this.decreaseKey(item, item.key - this.min.item.key);
			this.min = item.node;
		}
		this.deleteMin();
	}
	
	
	/**
	 * Meld the heap with 'heap2'.
	 * This method's worst-case time complexity is O('this'.trees_num + 'heap2'.trees_num + 1).
	 * @post:		'heap2' isn't usable, as in wouldn't be called again.
	 */
	public void meld(BinomialHeap heap2)
	{
		if (this.empty() && !heap2.empty())
		{
			this.size = heap2.size;
			this.last = heap2.last;
			this.min = heap2.min;
			this.first = heap2.first;
			this.trees_num = heap2.trees_num;
		}
		else if (!heap2.empty())
		{
			HeapNode node_this = this.first;
			HeapNode node_heap2 = heap2.first;
			HeapNode carry = null;
			int len = this.trees_num + heap2.trees_num + 1;
			HeapNode[] melded = new HeapNode[len];		/* The array of the post-melding binomial trees, will
			 											 * be used to link the next pointer-fields of every
			 											 * one of these trees' roots.
			 											 */
			int melded_max_index = 0;
			
			while (node_this != null || node_heap2 != null)
			{	if (node_this != null && node_heap2 != null)
				{
					if (carry != null)
					{
						if (node_this.rank == node_heap2.rank)
						{
							if (node_this.rank == carry.rank)
							{
								carry.intoMelded(melded, melded_max_index);
								melded_max_index++;
								carry = node_this.linkNodes(node_heap2);
								node_this = node_this.next;
								node_heap2 = node_heap2.next;
								carry.postLinkFix();
							}
							else		/* 'node_this' != null    &&    'node_heap2' != null    &&
										 * 'carry' != null    &&    'node_this'.rank == 'node_heap2'.rank
										 * &&    'node_this'.rank != 'carry'.rank .
										 */
							{
								carry.intoMelded(melded, melded_max_index);
								melded_max_index++;
								carry = null;
							}
						}
						else		/* 'node_this' != null    &&    'node_heap2' != null    &&
									 * 'carry' != null    &&    'node_this'.rank != 'node_heap2'.rank .
						 			 */
						{
							if (HeapNode.smallerRank(node_this, node_heap2).rank != carry.rank)
							{
								carry.intoMelded(melded, melded_max_index);
								melded_max_index++;
								carry = null;
							}
							else if (HeapNode.smallerRank(node_this, node_heap2) == node_this)
							{
								carry = carry.linkNodes(node_this);
								node_this = node_this.next;
								carry.postLinkFix();
							}
							else
							{		/* 'node_this' != null    &&    'node_heap2' != null    &&
									 * 'carry' != null    &&    'node_this'.rank != 'node_heap2'.rank    &&
									 * HeapNode.smallerRank('node_this', 'node_heap2').rank == 'carry'.rank
									 * &&     HeapNode.smallerRank('node_this', 'node_heap2') == 'node_heap2' .
									 */
								carry = carry.linkNodes(node_heap2);
								node_heap2 = node_heap2.next;
								carry.postLinkFix();
							}
						}
					}
					else		/* 'node_this' != null    &&    'node_heap2' != null    &&
								 * 'carry' == null .
								 */
					{
						if (node_this.rank == node_heap2.rank)
						{
							carry = node_this.linkNodes(node_heap2);
							node_this = node_this.next;
							node_heap2 = node_heap2.next;
							carry.postLinkFix();
						}
						else		/* 'node_this' != null    &&    'node_heap2' != null    &&
									 * 'carry' == null    &&    'node_this'.rank != 'node_heap2'.rank .
									 */
						{
							if (HeapNode.smallerRank(node_this, node_heap2) == node_this)
							{
								node_this.intoMelded(melded, melded_max_index);
								node_this = node_this.next;
							}
							else		/* 'node_this' != null    &&    'node_heap2' != null    &&
										 * 'carry' == null    &&
										 * 'node_this'.rank != 'node_heap2'.rank    &&
										 * HeapNode.smallerRank('node_this', 'node_heap2') == 'node_heap2' .
										 */
							{
								node_heap2.intoMelded(melded, melded_max_index);
								node_heap2 = node_heap2.next;
							}
							melded_max_index++;
						}
					}
				}
				else if (carry != null)
				{
					if (node_this != null && node_this.rank == carry.rank)
					{
						carry = carry.linkNodes(node_this);
						node_this = node_this.next;
						carry.postLinkFix();
					}
					else if (node_heap2 != null && node_heap2.rank == carry.rank)
					{
						carry = carry.linkNodes(node_heap2);
						node_heap2 = node_heap2.next;
						carry.postLinkFix();
					}
					else
					{		/* 'carry' != null    &&    (    (  'node_this' != null    &&
							 * 'node_heap2' == null    &&    'node_this'.rank != 'carry'.rank  )    ||
							 * (  'node_this' == null    &&    'node_heap2' != null    &&
							 * 'node_this'.rank != 'carry'.rank  )    ) .
							 */
						carry.intoMelded(melded, melded_max_index);
						melded_max_index++;
						carry = null;
					}
				}
				else if (node_this != null)
				{		/* 'carry' == null    &&    'node_this' != null    &&    'node_heap2' == null .
						 */
					node_this.intoMelded(melded, melded_max_index);
					melded_max_index++;
					node_this = node_this.next;
				}
				else
				{		/* 'carry' == null    &&    'node_this' == null    &&    'node_heap2' != null .
					 	 */
					node_heap2.intoMelded(melded, melded_max_index);
					melded_max_index++;
					node_heap2 = node_heap2.next;
				}
			}
			/* Appending 'carry' to 'melded' if we've reached the end of both 'this' and 'heap2', and 'carry'
			 * wasn't null, as in we've linked 2 HeapNode objects in the last iteration of the 'while' loop: 
			 */
			if (carry != null)
			{
				carry.intoMelded(melded, melded_max_index);
				melded_max_index++;
			}
			/* Fixing 'this's fields properly:
			 */
			this.size += heap2.size;
			this.last = melded[melded_max_index - 1];
			this.min = HeapNode.smallerKey(this.min, heap2.min);
			this.first = melded[0];
			this.trees_num = melded_max_index;
			/* Fixing 'last's next pointer-field:
			 */
			this.last.next = null;
		}
		/* If 'heap2' is empty, we have nothing to meld 'heap1' with, and thus don't need to do anything.
		 */
	}

	
	/**
	 * Find 'this's (the heap) size.
	 * This method's worst-case time complexity is O(1).
	 * @return:		The number of elements in the heap.
	 */
	public int size()
	{	return this.size; }

	
	/**
	 * Checks whether 'this' (the heap) is empty.
	 * This method's worst-case time complexity is O(1).
	 * @precondition:		'this' != null.
	 * @return:				'true' if the heap is empty and 'false' otherwise.
	 */
	public boolean empty()
	{
		if (this.first != null) { return false; }
		else { return true; }
	}

	
	/**
	 * Finds the number of trees in 'this' (the heap).
	 * This method's worst-case time complexity is O(1).
	 * @return:		The number of trees in the heap.
	 */
	public int numTrees()
	{	return this.trees_num; }
	
	
	
	
	/**
	 * A class implementing a node in a Binomial Heap.
	 */
	public class HeapNode
	{
		// ORIGINALLY GIVEN CLASS FIELDS:
		public HeapItem item;
		public HeapNode child;
		public HeapNode next;
		public HeapNode parent;
		public int rank;
		// ADDED CLASS FIELDS:
		public HeapNode last;
		public HeapNode min;
		
		
		// ADDED CLASS METHODS:
		
		
		/**
		 * A default builder of the HeapNode class.
		 * This mehtod's time complexity is O(1).
		 * @return:				null.
		 */
		public HeapNode()
		{	this(new HeapItem()); }
		
		
		/**
		 * A non-default builder of the HeapNode class.
		 * This mehtod's time complexity is O(1).
		 * @precondition:			'item'.key > 0.
		 * @param 'item':			A HeapItem object on which the item pointer-field of the new HeapNode
		 * object that would be created would point.
		 * @return:					null.
		 */
		public HeapNode(HeapItem item)
		{
			this.item = item;
			this.item.node = this;
			/* I could've not added the next lines, as Java's would've done them by default. I did add them
			 * though, just to add more clarity :)
			 */
			this.child = null;
			this.next = null;
			this.parent = null;
			this.rank = 0;
			this.last = null;
			this.min = null;
		}
		
		
		/**
		 * Finds which of the 2 given HeapNode objects has a smaller key.
		 * This method's time complexity is O(1).
		 * @param 'node1':		The first HeapNode object.
		 * @param 'node2':		The second HeapNode object.
		 * @return:				The HeapNode (out of 'node1' and 'node2') whose key was smaller.
		 */
		private static HeapNode smallerKey(HeapNode node1, HeapNode node2)
		{
			if (node1 == null && node2 == null) { return null; }
			else if (node1 == null) { return node2; }
			else if (node2 == null) { return node1; }
			else if (node1.item.key > node2.item.key) { return node2; }
			else
			{		/* 'node1'.item.key <= 'node2'.item.key .
			 		 */
				return node1;
			}
		}
		
		
		/**
		 * Finds which of the 2 given HeapNode objects has a larger key, using the 'smallerKey' method.
		 * This method's time complexity is O(1).
		 * @param 'node1':		The first HeapNode object.
		 * @param 'node2':		The second HeapNode object.
		 * @return:				The HeapNode (out of 'node1' and 'node2') whose key was larger.
		 */
		private static HeapNode largerKey(HeapNode node1, HeapNode node2)
		{
			HeapNode smaller = smallerKey(node1, node2);
			if (smaller == null) { return null; }
			else if (smaller == node1) { return node2; }
			else { return node1; }
		}
		
		
		/**
		 * Links 'this' and the given 'node' HeapNode objects, so that the one with the larger key of the 2
		 * becomes the last child of the one with the smaller key.
		 * This method's time complexity is O(1).
		 * @precondition:		'this' != null .
		 * @precondition:		'node' != null .
		 * @param 'node':		A HeapNode object which would become either 'this's new last child or 'this's
		 * new parent.
		 * @return:				A HeapNode object representing 'this' and 'node' after linking them as
		 * described above.
		 */
		private HeapNode linkNodes(HeapNode node)
		{
			HeapNode root_node = smallerKey(this, node);
			HeapNode child_node = largerKey(this, node);
			child_node.parent = root_node;
			if (root_node.child == null)
			{
				root_node.child = child_node;
				root_node.min = child_node;
			}
			else if (root_node.min.item.key > child_node.item.key) { root_node.min = child_node; }
			if (root_node.last != null) { root_node.last.next = child_node; }
			
			root_node.last = child_node;
			root_node.rank = root_node.last.rank + 1;
			return root_node;
		}
		
		
		/**
		 * Fixes the next pointer-field of the HeapNode that 'this's last pointer-field points on to be null.
		 * This method's time complexity is O(1).
		 */
		private void postLinkFix()
		{
			if (this != null && this.last != null) { this.last.next = null; }
			/* If 'this'.last == null, it won't have a next pointer-field, hence there'll be no field to fix,
			 * and thus we don't to do anything. And if 'this' == null, it won't have a last pointer-field,
			 * hence there'll be no field to fix, and once again - we don't need to do anything.
			 */
		}
		
		
		/**
		 * Finds which of the 2 given HeapNode objects has a smaller rank.
		 * This method's time complexity is O(1).
		 * @param 'node1':		The first HeapNode object.
		 * @param 'node2':		The second HeapNode object.
		 * @return:				The HeapNode (out of 'node1' and 'node2') whose rank was smaller.
		 */
		private static HeapNode smallerRank(HeapNode node1, HeapNode node2)
		{
			if (node1 == null && node2 == null) { return null; }
			else if (node1 == null) { return node2; }
			else if (node2 == null) { return node1; }
			else if (node1.rank > node2.rank) { return node2; }
			else { return node1; }
		}
		
		
		/**
		 * Appends 'this' into 'melded' at the 'melded_max_index' -th index, and if 'melded' already included
		 * an element beforehand, fixes said previously existing element's next pointer-field to point on
		 * 'this'.
		 * This method's time complexity is O(1).
		 * @precondition:					'melded_max_index' >= 0 .
		 * @param 'melded':					An array of HeapNode objects into which 'this' would be appended.
		 * @param 'melded_max_index':		The index where 'this' would appear in within 'melded' after
		 * being appended.
		 */
		private void intoMelded(HeapNode[] melded, int melded_max_index)
		{
			this.parent = null;
			melded[melded_max_index] = this;
			if (melded_max_index > 0) { melded[melded_max_index - 1].next = this; }
			/* If 'melded_max_index' == 0, there's no element in 'melded' and thus there's no element that
			 * needs to have its next pointer-field fixed, and therefore we don't to do anything.
			 */
		}
		
		
		/**
		 * Shifts 'this' up its corresponding binomial tree until 'this' either becomes the new root, or
		 * finds its appropriate place in the tree according to its key (must be so that all of its
		 * children's keys are larger than 'this', and if it has a parent - the parent's key is smaller than
		 * 'this's key).
		 * Denote d = the number of non-null HeapNode objects on the route up (via .parent calls) from 'this'
		 * to the root of 'this's binomial tree. Thus this method's time complexity is O(d).
		 * At the worst-case scenario, 'this' would be the node in the maximal depth within its binomial
		 * tree, and 'this'.key would be the minimal key of all the keys of the non-null HeapNode objects on
		 * the route up (via .parent calls) from 'this' to the of root of 'this's binomial tree. Denote
		 * n_this = number of nodes in 'this's binomial tree. Thus there are log_2 (n_this) non-null HeapNode
		 * objects in the route up (via .parent calls) from 'this' to the root of 'this's binomial tree. Thus
		 * this method's worst-case time complexity is O(log_2 (n_this)).
		 */
		private void shiftUp()
		{
			if (this.parent != null)
			{
				HeapNode current = this;
				HeapNode parent = current.parent;
				while (parent != null)
				{
					if (smallerKey(current, parent) == current)
					{		/* 'current'.key <= 'parent'.key .
							 */
						HeapItem tmp = parent.item;
						
						parent.item = current.item;
						parent.item.node = parent;
						
						current.item = tmp;
						tmp.node = current;
						
						parent.min = current;
						
						current = parent;
						parent = current.parent;
					}
					else
					{		/* Should never be reached, but if it is reached - that means that we've reached
							 * a point where 'current'.key > 'parent'.key, thus we've found the proper new
							 * place for 'current' in its tree, and we don't need to keep trying to shift it
							 * up. This adds possibilities for further developments down the line should
							 * anyone wish to do some.
							 */
						break;
					}
				}
			}
			/* If 'this'.parent == null, we cannot shift 'this' up in its tree anymore that it currently is,
			 * thus we don't need to do anything.
			 */
		}
	}

	
	
	
	/**
	 * A class implementing an item in a Binomial Heap.
	 */
	public class HeapItem
	{
		// ORIGINALLY GIVEN CLASS FIELDS:
		public HeapNode node;
		public int key;
		public String info;
		// ADDED CLASS FIELDS:
		// There weren't any :) How nice !
		
		
		// ADDED CLASS METHODS:
		
		
		/**
		 * A default builder of the HeapItem class.
		 * This mehtod's time complexity is O(1).
		 * @return:					null.
		 */
		public HeapItem()
		{
			this.key = 1;		/* Meant to be changed and not remain 1 ! Note that it's being changed in
			 					 * every method that calls for a new HeapItem object. I decided not to
			 					 * address the assumption that the keys are positive integers, and thus
			 					 * this code also supports non-negative keys :)
			 					 */
			/* I could've left the rest of this builder empty, as Java would've done the same by default, but
			 * I decided to add these lines for extra clarity.
			 */
			this.node = null;
			this.info = null;
		}
		
		
		/**
		 * A non-default builder of the HeapItem class.
		 * This mehtod's time complexity is O(1).
		 * @precondition:			The given 'key' is larger than 0.
		 * @param 'key':			An integer that's to become the value of the key field of 'this'.
		 * @param 'info':			A string to which the info field of 'this' would point.
		 * @return:					null.
		 */
		public HeapItem(int key, String info)
		{
			this.key = key;
			this.info = info;
			/* I could've not added the next line, as Java would've done that by default, but I decided to
			 * add it for clarity :)
			 */
			this.node = null;
		}
	}
}
