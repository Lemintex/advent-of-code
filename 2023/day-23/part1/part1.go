package main

import (
	"fmt"
	"os"
	"strings"
)

type pos struct {
	r, c int
}
type node struct {
	pos  pos
	seen int
}

var intersections map[pos]struct{}
var sr, sc, er, ec int
var trail []string
var graph map[pos]map[pos]int
var walkSeen map[pos]struct{}

func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("AAH")
	}
	trail = strings.Split(string(f), "\n")
	sr = 0
	er = len(trail) - 2
	for i, t := range trail {
		if i == 0 {
			sc = strings.Index(t, ".")
		} else if i == len(trail)-2 /*TODO FIX*/ {
			fmt.Println(t)
			ec = strings.Index(t, ".")
		}
	}
	fmt.Println(sr, sc, er, ec)
	parseMapIntoGraph()
}

func parseMapIntoGraph() {
	intersections = make(map[pos]struct{})
	// add start and end to intersections
	start := pos{r: sr, c: sc}
	end := pos{r: er, c: ec}
	intersections[start] = struct{}{}

	intersections[end] = struct{}{}
	for r, t := range trail {
		for c, ch := range t {
			neighbours := 0
			if ch == '#' {
				continue
			}
			// top
			if c > 0 && trail[r][c-1] != '#' {
				neighbours++
			}
			// bottom
			if c < len(trail)-2 && trail[r][c+1] != '#' {
				neighbours++
			}

			// left
			if r > 0 && trail[r-1][c] != '#' {
				neighbours++
			}

			// right
			if r < len(t)-1 && trail[r+1][c] != '#' {
				neighbours++
			}

			if neighbours >= 3 {
				pos := pos{
					r: r,
					c: c,
				}
				intersections[pos] = struct{}{}
			}
		}
	}
	floodfill()
	walkSeen = map[pos]struct{}{}
	startingPos := pos{r: sr, c: sc}
	fmt.Println(findLongestHike(startingPos))
}

func floodfill() {
	graph = make(map[pos]map[pos]int)
	dirs := make(map[byte][]pos)
	dirs['>'] = []pos{pos{c: 1, r: 0}}
	dirs['<'] = []pos{pos{c: -1, r: 0}}
	dirs['v'] = []pos{pos{c: 0, r: 1}}
	dirs['^'] = []pos{pos{c: 0, r: -1}}
	dirs['.'] = []pos{
		pos{c: 1, r: 0},
		pos{c: -1, r: 0},
		pos{c: 0, r: 1},
		pos{c: 0, r: -1},
	}
	// for each intersection
	for intersection, _ := range intersections {
		var stack []node
		beginning := node{
			pos:  intersection,
			seen: 0,
		}
		// stack init
		stack = append(stack, beginning)

		// seen init
		seen := make(map[pos]struct{})
		seen[intersection] = struct{}{}

		// while the stack isn't empty
		for len(stack) > 0 {
			// pop
			n := stack[len(stack)-1]
			stack = stack[:len(stack)-1]

			_, ok := graph[intersection]
			if !ok {
				graph[intersection] = make(map[pos]int)
			}
			// if a different intersection is reached
			_, exists := intersections[n.pos]
			if n.seen > 0 && exists && intersection != n.pos {
				graph[intersection][n.pos] = n.seen
				continue
			}

			// go through neighbours
			xIndex, yIndex := min(n.pos.c, len(trail[0])-1), min(n.pos.r, len(trail)-2)
			for _, position := range dirs[trail[yIndex][xIndex]] {
				nc, nr := n.pos.c+position.c, n.pos.r+position.r
				// clamp the indexes so we dont go out of bounds
				nc = max(0, min(nc, len(trail[0])))
				nr = max(0, min(nr, len(trail)-2))

				pos := pos{
					r: nr,
					c: nc,
				}
				_, visited := seen[pos]
				if nc >= 0 && nc < len(trail[0]) && nr >= 0 && nr < len(trail)-1 && trail[nr][nc] != '#' && !visited {
					stack = append(stack, node{pos: pos, seen: n.seen + 1})
					seen[pos] = struct{}{}
				}
			}
		}
	}
}

func findLongestHike(position pos) int {
	if position.c == ec && position.r == er {
		return 0
	}
	m := -99999999
	walkSeen[position] = struct{}{}
	for n, _ := range graph[position] {
		_, exists := walkSeen[n]
		if exists {
			continue
		}
		m = max(m, findLongestHike(n) + graph[position][n])
	}
	delete(walkSeen, position)
	return m
}
