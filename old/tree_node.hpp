//
// FILENAME: tree_node.hpp | Shifting Stones Search
// DESCRIPTION: A node of the tree
//

#pragma once

#include "masks.hpp"

class TreeNode {
public:
	TreeNode() = default;
	TreeNode(const Mask& mask, bool isRoot);
	~TreeNode();

	constexpr inline bool isRoot() const {
		return m_IsRoot;
	}

private:
	Mask* m_Mask;
	TreeNode* m_Children;
	bool m_IsRoot;
};