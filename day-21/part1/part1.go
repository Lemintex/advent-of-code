package main

import (
	"fmt"
	"os"
	"strings"
)

type visitedNode struct {
	x, y int
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
	nodes := make(map[visitedNode]struct{})
	init := visitedNode{
		x: sX,
		y: sY,
	}
	nodes[init] = struct{}{}
	for range steps {
		newNodes := make(map[visitedNode]struct{})
		// var newNodes []visitedNode
		for n, _ := range nodes {
			//check 4 sides
			//above
			if n.y > 0 && lines[n.y-1][n.x] != '#' {
				newNode := visitedNode{
					x: n.x,
					y: n.y - 1,
				}
				newNodes[newNode] = struct{}{}
			}

			//below
			if n.y < len(lines) && lines[n.y+1][n.x] != '#' {
				newNode := visitedNode{
					x: n.x,
					y: n.y + 1,
				}
				newNodes[newNode] = struct{}{}
			}

			//left
			if n.x > 0 && lines[n.y][n.x-1] != '#' {
				newNode := visitedNode{
					x: n.x - 1,
					y: n.y,
				}
				newNodes[newNode] = struct{}{}
			}

			//right
			if n.x < len(lines[0]) && lines[n.y][n.x+1] != '#' {
				newNode := visitedNode{
					x: n.x + 1,
					y: n.y,
				}
				newNodes[newNode] = struct{}{}
			}
		}
		nodes = newNodes
		// printMap(nodes)
	}
	fmt.Println(len(nodes))
}

func printMap(nodes map[visitedNode]struct{}) {
	fmt.Println(nodes)
	for y, l := range lines {
		for x, c := range l {
			temp := visitedNode{
				x: x,
				y: y,
			}
			_, ok := nodes[temp]
			if ok {
				fmt.Print("0")
			} else {
				fmt.Print(string(c))
			}
		}
		fmt.Println()
	}
}
