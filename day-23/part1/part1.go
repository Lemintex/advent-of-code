package main

import (
	"fmt"
	"os"
	"strings"
)

type node struct {
	x, y  int
	steps int
	nodes []*node
}

var root node
var nodes map[[2]int]*node
var sx, sy int
var trail []string

func main() {
	nodes = make(map[[2]int]*node)
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("AAH")
	}
	trail = strings.Split(string(f), "\n")
	sy = 0
	for i, t := range trail {
		if i == 0 {
			sx = strings.Index(t, ".")
		}
	}
	parseMapIntoGraph()
}


func parseMapIntoGraph() {
	var stack [][2]int
	begin := node {
		x: sx,
		y: sy,
		steps: 0,
	}
	root = begin
	stack = append(stack, [2]int{sx, sy})
	nodes[[2]int{sx, sy}] = &root
	for len(stack) > 0 {
		s := stack[len(stack) - 1]
		stack = stack[:max(0, len(stack) - 2)]
		fmt.Println(s)
	}
}
