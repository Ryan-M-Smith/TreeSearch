//
// FILENAME: treegraph.hpp | Shifting Stones Search
// DESCRIPTION: A tree representation using the Boost Graph Library
// CREATED: 2024-03-12 @ 5:34 PM
//

#pragma once

#include <format>
#include <fstream>
#include <map>
#include <ostream>
#include <queue>
#include <string>
#include <tuple>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <gvc/gvc.h>
#include <cgraph/cgraph.h>

#include "decl.h"
#include "treeutils.h"

/**
 * @brief Implementation details for the `TreeGraph` class
 */
namespace __detail {
	/**
	 * @brief A concept to represent a `boost::adjacency_list`
	 */
	// template<
	// 	class OutEdgeListS   = boost::vecS,
	// 	class VertexListS    = boost::vecS,
	// 	class DirectedS 	 = boost::undirectedS,
	// 	class VertexProperty = boost::no_property,
	// 	class EdgeProperty   = boost::no_property,
	// 	class GraphProperty  = boost::no_property,
	// 	class EdgeListS 	 = boost::listS
	// >
	// concept AdjacencyList = requires {
	// 	typename boost::adjacency_list<
	// 		OutEdgeListS, VertexListS, DirectedS,
	// 		VertexProperty, EdgeProperty,
	// 		GraphProperty, EdgeListS
	// 	>;
	// };

	/**
	 * @struct __vertex_property
	 * @brief A custom vertex value for the tree
	 */
	struct __vertex_property {
		board_t value;  // The board stored in the node
		int 	parent; // The index of the node's parent
		int 	child; 	// The index of the node under its parent
	};

	/**
	 * @struct __tree_node
	 * @brief A basic data-only tree node
	 */
	struct __tree_node {
		int height; // The height the node is stored at
		int parent; // The index of the node's parent
		int child; 	// The index of the node under its parent
	};

	/**
	 * @brief A custom Graphviz label writer for the tree
	 */
	// template<AdjacencyList AdjList>	
	// class __vertex_label_writer {
	// 	using tree_type 		= AdjList;
	// 	using vertex_descriptor = typename boost::graph_traits<tree_type>::vertex_descriptor;
	// 	using label_type 		= std::map<vertex_descriptor, std::string>;

	// public:
	// 	__vertex_label_writer(const label_type& labels):
	// 		labels(labels)
	// 	{}

	// 	void operator()(std::ostream& out, const vertex_descriptor& descriptor) const {
	// 		out << descriptor << "\n";
	// 		out << "[label=\"" << labels.at(descriptor) << "\"]";
	// 	}
	// private:
	// 	const label_type& labels;
    // };
}

template<
	class OutEdgeListS 	 = boost::vecS,
	class VertexListS 	 = boost::vecS,
	class DirectedS 	 = boost::undirectedS,
	class VertexProperty = __detail::__vertex_property,
	class EdgeProperty 	 = boost::no_property,
	class GraphProperty  = boost::no_property,
	class EdgeListS 	 = boost::listS
>
class TreeGraph {
	using tree_type 		= boost::adjacency_list<
								OutEdgeListS, VertexListS, DirectedS,
								VertexProperty, EdgeProperty,
								GraphProperty, EdgeListS
							  >;

	using vertex_type 		= __detail::__vertex_property;
	using vertex_descriptor = typename boost::graph_traits<tree_type>::vertex_descriptor;
	using label_type 		= std::map<vertex_descriptor, std::string>; 

	using vertex_iterator 	= typename tree_type::vertex_iterator;
	using edge_interator 	= typename tree_type::edge_iterator;

	tree_type tree = {};

public:
	/**
	 * @brief Construct a new `Tree` object
	 * 
	 * @note Default constructor
	 */
	TreeGraph() = default;

	/**
	 * @brief Construct a new `Tree` object
	 * 
	 * @param 	tree 	A pointer to a tree buffer
	 */
	explicit TreeGraph(tree_t tree) {
		storeTree(tree);
	}

	/**
	 * @brief Destroy the `Tree` object
	 * 
	 */
	~TreeGraph() {}

	/**
	 * @brief Add a new vertex to the tree
	 * 
	 * @param 	tree 	The tree buffer to add a vertex from
	 * @param 	parent 	The index of the parent in the tree buffer
	 * @param 	child  	The index of the child in the tree buffer
	 * 
	 * @return 			The vertex descriptor
	 */
	vertex_descriptor addVertex(tree_t tree, int parent, int child) {
		board_t board = treeutils::getBoard(tree, parent, child);
		const int PARENT_INDEX = CHILDREN_PER_PARENT * parent;
		
		// Add the node to the tree
		vertex_descriptor descriptor = boost::add_vertex(vertex_type {board, PARENT_INDEX, child}, this->tree);
		
		// Connect the vertex to its parent. The root node shouldn't be connected to itself.
		if (parent != 0 || child != 0) {
			boost::add_edge(parent, PARENT_INDEX + child, this->tree);
		}

		// Map the node to a strigified version of its board
		vertexLabels[descriptor] = std::string(treeutils::getBits(board, 27));

		return descriptor;
	}

	/**
	 * @brief Get the underlying tree stored in the class
	 * 
	 * @return A const reference to the underlying tree
	 */
	inline const tree_type& underlying() const {
		return tree;
	}

	/**
	 * @brief Write Graphviz DOT code to an output stream
	 * 
	 * @param 	os 	The `std::ostream` object to write to
	 * 
	 * @note 		To print the code to the console, pass `std::cout` or call `TreeGraph::printGVDOT`	
	 */
	void writeGVDOT(const std::string& filename) const {
		/**
		 * @brief Lambda function to format a 27-bit board into a 3x3 grid of 3-bit tiles
		 * 
		 * @param 	label 	The board to format
		 * @return 			The formatted board
		 */
		auto format3x3 = [](const std::string& label) -> std::string {
			return std::format(
				R"({} {} {}\n{} {} {}\n{} {} {})",

				label.substr(0, 3),		// (0, 0)
				label.substr(3, 3),		// (0, 1)
				label.substr(6, 3),		// (0, 2)
				label.substr(9, 3),		// (1, 0)
				label.substr(12, 3),	// (1, 1)
				label.substr(15, 3),	// (1, 2)
				label.substr(18, 3),	// (2, 0)
				label.substr(21, 3),	// (2, 1)
				label.substr(24, 3)		// (2, 2)
			);
		};

		// Construct a property map of vertex data. This is what gives each vertex its name on the image.
		boost::dynamic_properties properties;
		properties.property("node_id", boost::get(boost::vertex_index, tree));
		properties.property("label", boost::make_transform_value_property_map(
        	[&](auto descriptor) -> std::string {
				return format3x3(vertexLabels.at(descriptor));
			},
        	boost::get(boost::vertex_index, tree)
    	));

		std::fstream file(filename, file.out);
		boost::write_graphviz_dp(file, tree, properties); // Write Graphviz code to the output stream
	}

	/**
	 * @brief Write Graphviz DOT code to stdout (`std::cout`)
	 * 
	 * @note Equavalent to `TreeGraph::writeGVDOT(std::cout)`
	 * @see TreeGraph::writeGVDOT
	 */
	void printGVDOT() const {
		writeGVDOT(std::cout);
	}

	/**
	 * @brief Overload the `>>` operator to print a `TreeGraph` object
	 * 
	 * @param 	os  	The `std::ostream` instance to the left of the operator
	 * @param 	tree 	The `TreeGraph` instance to the right of the operator
	 * @return 			A reference to the left hand `std::ostream` object
	 */
	friend std::ostream& operator<<(std::ostream& os, const TreeGraph& tree) {
		tree.printGVDOT();
		return os;
	}

	/**
	 * @brief Render Graphviz DOT code to a PNG
	 * 
	 * @return 
	 */
	int makePNG(const std::string& filename) const {
		FILE* gvfile = fopen(filename.c_str(), "r");
		FILE* gvpng = fopen((filename + ".png").c_str(), "w");
		
		GVC_t* gvc = gvContext();
		Agraph_t* graph = agread(gvfile, 0);
		
		gvLayout(gvc, graph, "dot");
		gvRender(gvc, graph, "png", gvpng);
		gvFreeLayout(gvc, graph);
		agclose(graph);

		fclose(gvfile);
		fclose(gvpng);

		return gvFreeContext(gvc);
	}

private:
	label_type vertexLabels; // The graph labels to be displayed in the Graphviz output

	void storeTree(tree_t tree) {
		std::queue<__detail::__tree_node> nodes;
		nodes.push({0, 0, 0}); // Store the root node
		int nodesStored = 0;

		while (!nodes.empty() && nodes.front().height <= TREE_GEN_HEIGHT) {
			// Add the next node in the queue to the graph
			auto [height, parent, child] = nodes.front();
			nodes.pop();
			addVertex(tree, parent, child);
			nodesStored++;

			//std::cout << "Height: " << height << " | Parent: " << parent << " | Child: " << child << "\n";

			const int PARENT_INDEX = CHILDREN_PER_PARENT * parent + child;

			// Add that node's children to the queue
			for (int i = 1; i <= CHILDREN_PER_PARENT; i++) {
				nodes.push({height + 1, PARENT_INDEX, i});
			}
		}

		std::cout << "Stored " << nodesStored << " nodes\n";
	}
};