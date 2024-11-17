package main

import (
	"fmt"
	"os"
	"strings"
)

type node struct {
	name    string
	edges   map[*edge]*node
	visited bool
	group   int
}

type edge struct {
	visited bool
}

type item struct {
	e    *edge
	n    *node
	prev *item
}
type graph struct {
	nodes map[string]*node
	edges []*edge
}

var nodeMap map[*node]struct{}

// the way I thought of this problem is instead of wires and components I perceived it in terms of "roads" and "junctions"
// so the problem can be viewed as "there are 2 islands connected via 3 bridges, figure out how many junctions are on each island"
// I figured the way to identify whether junction A is on the same island as junction B is to drive there via 4 unique paths
// if this is impossible, they're on different islands
// so essentially pick a junction and for every other one repeat this process grouping them relative to this source

// thanks u/Krethas for 'inspiration'
func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("File not found")
	}
	str := strings.Split(strings.TrimSpace(string(f)), "\n")
	g := parse(str)
	s1, s2 := g.split(3)
	fmt.Println(s1 * s2)
}

func newNode(name string) *node { return &node{name: name, edges: make(map[*edge]*node)} }

func (g *graph) resetNodes() {
	for _, n := range g.nodes {
		n.visited = false
	}
}

func (g *graph) resetEdges() {
	for _, e := range g.edges {
		e.visited = false
	}
}

func (g *graph) bfs(start, dest *node) bool {
	found := false
	var q []*item
	q = append(q, &item{
		n: start,
	})
	for len(q) > 0 {
		curr := q[0]
		q = q[1:]
		if curr.prev != nil {
			curr.n.group = curr.prev.n.group
		}
		if curr.n == dest {
			for i := curr; i.e != nil; i = i.prev {
				i.e.visited = true
			}
			found = true
			break
		}
		for e, n := range curr.n.edges {
			if e.visited || n.visited {
				continue
			}
			n.visited = true
			q = append(q, &item{e: e, n: n, prev: curr})
		}
	}
	g.resetNodes()
	return found
}

func (g *graph) cutPaths(source, dest *node, pathNum int) bool {
	complete := true
	for range pathNum {
		if !g.bfs(source, dest) {
			complete = false
			break
		}
	}
	return complete
}

func (g *graph) split(cuts int) (int, int) {
	var source *node
	for _, n := range g.nodes {
		source = n
	}
	source.group = 1

	for _, dest := range g.nodes {
		if source == dest || dest.group > 0 {
			continue
		}

		if !g.cutPaths(source, dest, cuts+1) {
			// Use disconnected graph to categorize as many nodes as possible
			g.bfs(source, nil)
			dest.group = 2
			g.bfs(dest, nil)
		}
		g.resetEdges()
	}

	disconnected := 0
	for _, n := range g.nodes {
		if n.group != 1 {
			disconnected++
		}
	}
	return len(g.nodes) - disconnected, disconnected
}

func parse(input []string) *graph {
	nodes := make(map[string]*node)
	var edges []*edge
	// first initialise every node in the input that is followed by ": "
	// keep in mind there may be more nodes that aren't listed directly
	for _, line := range input {
		name := strings.Split(line, ": ")
		nodes[name[0]] = newNode(name[0])
	}
	// loop over every line in the input
	for _, l := range input {
		segments := strings.Split(l, ": ")
		src := nodes[segments[0]]
		destNames := strings.Split(segments[1], " ")
		for _, dest := range destNames {
			d, exists := nodes[dest]
			if !exists {
				d = newNode(dest)
				nodes[dest] = d
			}
			edge := &edge{}
			edges = append(edges, edge)
			src.edges[edge] = d
			d.edges[edge] = src
		}
	}
	return &graph{nodes: nodes, edges: edges}
}

