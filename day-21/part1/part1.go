package main

import (
	"fmt"
	"os"
	"strings"
)

type node struct {
	x, y int
}

type visitedNode struct {
	x, y int
	steps int
}

var steps int = 64
var sX, sY int
var lines []string

func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("Could not read file")
	}
	lines = strings.Split(string(f), "\n")
	for y, l := range lines {
		for x, c := range l {
			if c == 'S' {
				sX, sY = y, x
			}
		}
	}
	traverseGarden()
}

func traverseGarden() {
	// maps and queue
	ans := make(map[[2]int]struct{})
	seen := make(map[[2]int]struct{})
	var q []visitedNode

	initVisitedNode := visitedNode{
		x: sX,
		y: sY,
		steps: steps,
	}
	q = append(q, initVisitedNode)
	for len(q) > 0 {
		// pop the front of the queue
		current := q[0]
		q = q[1:]

		if current.steps % 2 == 0 {
			even := [2]int{current.x, current.y}
			ans[even] = struct{}{}
		}
		if current.steps == 0 {
			continue
		}
		cardinals := [][2]int{{current.x, current.y - 1}, {current.x, current.y + 1}, {current.x - 1, current.y}, {current.x + 1, current.y}} // this slice of [2]int's represents the 4 cardinal directions (up, down, left, right)
		for _, dir := range cardinals {
			_, ok := seen[dir]
			if dir[0] < 0 || dir[0] >= len(lines[0]) || dir[1] < 0 || dir[1] >= len(lines) - 1 || lines[dir[1]][dir[0]] == '#' || ok { // if index is invalid, node is #, or node is seen already, skip it
				continue
			}
			seen[dir] = struct{}{}
			dirVisitedNode := visitedNode {
				x: dir[0],
				y: dir[1],
				steps: current.steps - 1,
			}
			q = append(q, dirVisitedNode)
		}


	}
	fmt.Println("Ans:", len(ans))
}
