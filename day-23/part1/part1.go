package main

import (
	"fmt"
	"os"
	"strings"
)

type pos struct {
	x, y int
}
type node struct {
	pos  pos
	seen int
}

var intersections map[pos]struct{}
var sx, sy, ex, ey int
var trail []string
var graph map[node]struct{}

func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("AAH")
	}
	trail = strings.Split(string(f), "\n")
	sy = 0
	ey = len(trail) - 1
	for i, t := range trail {
		if i == 0 || i == len(trail)-1{
			sx = strings.Index(t, ".")
		} else if i == len(trail)-1 {
			ex = strings.Index(t, ".")
		}
	}
	fmt.Println(sx, sy, ex, ey)
	parseMapIntoGraph()
}

func parseMapIntoGraph() {
	intersections = make(map[pos]struct{})
	for y, t := range trail {
		if y == len(trail)-1 {
			break
		}
		for x, c := range t {
			neighbours := 0
			if c == '#' {
				continue
			}
			// top
			if y > 0 && trail[y-1][x] != '#' {
				neighbours++
			}
			// bottom
			if y < len(trail)-2 && trail[y+1][x] != '#' {
				neighbours++
			}

			// left
			if x > 0 && trail[y][x-1] != '#' {
				neighbours++
			}

			// right
			if x < len(t)-1 && trail[y][x+1] != '#' {
				neighbours++
			}

			if neighbours >= 3 {
				pos := pos{
					x: x,
					y: y,
				}
				intersections[pos] = struct{}{}
			}
		}
	}
}

func floodfill() {
	dirs := make(map[byte][]pos)
	dirs['>'] = []pos{pos {x: 1, y: 0}}
	dirs['<'] = []pos{pos {x: -1, y: 0}}
	dirs['v'] = []pos{pos {x: 0, y: 1}}
	dirs['^'] = []pos{pos {x: 0, y: -1}}
	dirs['.'] = []pos {
		pos {x: 1, y: 0},
		pos {x: -1, y: 0},
		pos {x: 0, y: 1},
		pos {x: 0, y: -1},
	}
	for intersection, _ := range intersections {
		var stack []node
		seen := make(map[pos]struct{})
		for len(stack) > 0 {
			node := stack[len(stack)-1]
			stack = stack[:len(stack)-2]
			_, exists := intersections[node.pos]
			if node.seen > 0 && exists {
				
			}
		}
	}
}
