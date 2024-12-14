package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

type pos struct {
	r int
	c int
}

type cornerPos struct {
	r float64
	c float64
}
type edge struct {
	p   pos
	dir byte
}

var input []string
var visited map[pos]struct{}

func ReadFile() {
	var err error
	input, err = Read("input.txt")
	if err != nil {
		log.Fatal(err)
		return
	}
}

func Read(filename string) ([]string, error) {
	s, err := os.ReadFile(filename)
	if err != nil {
		log.Fatal(err)
		return nil, err
	}
	in := strings.TrimSpace(string(s))

	// depending on the grouping of the input we may want to change the seperator
	return strings.Split(in, "\n"), nil
}

func main() {
	visited = make(map[pos]struct{})
	ReadFile()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Parse(in []string) { //edit return type as needed
	// return some kind of data structure once the input has been parsed
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	for i, r := range input {
		for j, _ := range r {
			_, exists := visited[pos{r: i, c: j}]
			if exists {
				continue
			}
			visited[pos{r: i, c: j}] = struct{}{}
			area, perimeter := floodFill(pos{r: i, c: j})
			ans += area * perimeter
		}
	}

	return ans, time.Since(start)
}

func floodFill(init pos) (int, int) {
	region := make([]pos, 0)
	region = append(region, init)
	area, perimeter := 1, 0
	plant := input[init.r][init.c]
	var directions [4]pos
	directions = [4]pos{pos{r: 1, c: 0}, pos{r: -1, c: 0}, pos{r: 0, c: 1}, pos{r: 0, c: -1}}
	var flood func(pos)
	flood = func(p pos) {
		for _, d := range directions {
			r, c := p.r+d.r, p.c+d.c
			newPos := pos{r: r, c: c}
			if r < 0 || r >= len(input) || c < 0 || c >= len(input[0]) {
				perimeter++
				continue
			}
			_, exists := visited[newPos]
			if plant != input[r][c] {
				perimeter++
			} else if !exists {
				area++
				visited[newPos] = struct{}{}
				region = append(region, newPos)
				flood(newPos)
			}
		}
	}
	flood(init)
	perimeter = countCorners(region)
	return area, perimeter
}

func countCorners(region []pos) int {
	corners := 0
	cornerMap := make(map[cornerPos]struct{})
	regionMap := make(map[pos]struct{})
	candidates := make([]cornerPos, 0)
	dirs := [4]cornerPos{cornerPos{0.5, 0.5}, cornerPos{0.5, -0.5}, cornerPos{-0.5, -0.5}, cornerPos{-0.5, 0.5}}
	for _, p := range region {
		regionMap[p] = struct{}{}
		for _, d := range dirs {
			c := cornerPos{float64(p.r) + d.r, float64(p.c) + d.c}
			_, exists := cornerMap[c]
			if exists {
				continue
			}
			cornerMap[c] = struct{}{}
			candidates = append(candidates, c)
		}
	}

	var config [][]bool
	for i, candidate := range candidates {
		config = append(config, make([]bool, 0))
		r, c := candidate.r,  candidate.c
		for _, d := range dirs {
			er, ec := r - d.r, c - d.c
			pos := pos{int(er), int(ec)}
			_, exists := regionMap[pos]
			config[i] = append(config[i], exists)
		}
	}
	for _, c := range config {
		truecount, falseCount := 0, 0
		for _, i := range c {
			if i {
				truecount++
			} else {
				falseCount++
			}
		}
		if truecount == 1 || falseCount == 1 {
			corners++
			continue
		}
		if truecount == 2 && falseCount == 2{
			if c[0] == c[2] && c[1] == c[3] {
				corners+=2
			}
		}
	}
	return corners
}
