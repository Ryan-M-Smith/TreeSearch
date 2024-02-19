//
// FILENAME: tree_node.cpp | Shifting Stones Search
// DESCRIPTION: A node of the tree
//

#include "tree_node.hpp"

TreeNode::TreeNode(const Mask& mask, bool isRoot = false):
	m_Mask(new Mask(mask)),
	m_Children(new TreeNode[masks::TOTAL_MASKS]),
	m_IsRoot(isRoot)
{}

TreeNode::~TreeNode() {
	delete m_Mask;
	delete[] m_Children;
}