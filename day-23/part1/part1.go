package main

import (
	"fmt"
	"os"
	"strings"
)

type node struct {
	x, y int
}

var intersections map[node]struct{}
var sx, sy, ex, ey int
var trail []string

func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("AAH")
	}
	trail = strings.Split(string(f), "\n")
	sy = 0
	ey = len(trail) - 1
	for i, t := range trail {
		if i == 0 {
			sx = strings.Index(t, ".")
		} else if i == len(trail)-1 {
			ex = strings.Index(t, ".")
		}
	}
	fmt.Println(sx, sy, ex, ey)
	parseMapIntoGraph()
}

func parseMapIntoGraph() {
	intersections = make(map[node]struct{})
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
				n := node{
					x: x,
					y: y,
				}
				intersections[n] = struct{}{}
			}
		}
	}
	fmt.Println(intersections)
}

func floodfill() {
	visited := make(map[[2]int]struct{})
	position := [2]int{sx, sy}
	var stack [][2]int
	for {
		visited[position] = struct{}{}
		x, y := position[0], position[1]
		c := trail[y][x]
		if c == '#' {
			continue
		}
		// top
		if y > 0 && trail[y-1][x] != '#' && trail[y-1][x] != 'V' {

		}
		// bottom
		if y < len(trail)-2 && trail[y+1][x] != '#' && trail[y+1][x] != '^' {
		}

		// left
		if x > 0 && trail[y][x-1] != '#' && trail[y][x-1] != '>' {
		}

		// right
		if x < len(trail[0])-1 && trail[y][x+1] != '#' && trail[y][x+1] != '<' {

		}
	}
}

func flood(x, y int, visited map[[2]int]struct{}) int {
	steps := 0
	for {
		n := node{
			x: x,
			y: y,
		}
		if _, exists := intersections[n]; exists {
			return steps
		}

	}
}
