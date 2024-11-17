package main

import (
	"fmt"
	"os"
	"strings"
	"math"
)

type node struct {
	x, y int
}

type visitedNode struct {
	x, y int
	steps int
}

var stepTotal int = 26501365
var gridSize int
var width int
var odd, even int
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
	gridSize = len(lines) - 1
	width = stepTotal / gridSize - 1

	var oddTemp float64 = float64((width / 2) * 2 + 1)
	odd = int(math.Pow(oddTemp, 2))
	oddPoints := traverseGarden(sX, sY, gridSize * 2 + 1)
	
	var evenTemp float64 = float64(((width + 1) / 2) * 2)
	even = int(math.Pow(evenTemp, 2))
	evenPoints := traverseGarden(sX, sY, gridSize * 2)

	//corners
	topCorner := traverseGarden(sX, gridSize - 1,  gridSize - 1)
	bottomCorner := traverseGarden(sX, 0, gridSize - 1)
	leftCorner := traverseGarden(gridSize - 1, sY, gridSize - 1)
	rightCorner := traverseGarden(0, sY, gridSize - 1)
	cornerTotal := topCorner + bottomCorner + leftCorner + rightCorner

	// small segments
	smallTopRight := traverseGarden(gridSize -1, 0, gridSize / 2 - 1)
	smallTopLeft:= traverseGarden(gridSize -1, gridSize - 1, gridSize / 2 - 1)
	smallBottomRight := traverseGarden(0, 0, gridSize / 2 - 1)
	smallBottomLeft := traverseGarden(0, gridSize - 1, gridSize / 2 - 1)
	smalTotal := (width + 1) * (smallTopRight + smallTopLeft + smallBottomRight + smallBottomLeft)

	// large segments
	largeTopRight :=  traverseGarden(gridSize -1, 0, (gridSize * 3) / 2 - 1)
	largeTopLeft := traverseGarden(gridSize -1, gridSize - 1, (gridSize * 3) / 2 - 1) 
	largeBottomRight :=  traverseGarden(0, 0, (gridSize * 3) / 2 - 1)
	largeBottomLeft :=  traverseGarden(0, gridSize - 1, (gridSize * 3) / 2 - 1)
	largeTotal := width * (largeTopRight + largeTopLeft + largeBottomRight + largeBottomLeft)

	fmt.Println("Total:",odd * oddPoints + even * evenPoints	+ cornerTotal + smalTotal + largeTotal)
}

func traverseGarden(ix, iy, isteps int) int {
	// maps and queue
	ans := make(map[[2]int]struct{})
	seen := make(map[[2]int]struct{})
	var q []visitedNode

	initVisitedNode := visitedNode{
		x: ix,
		y: iy,
		steps: isteps,
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
	return len(ans)
}
