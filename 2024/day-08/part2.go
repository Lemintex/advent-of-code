package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

type pos struct {
	x int
	y int
}

var input []string
var antennaPos map[pos]struct{}
var antennas map[rune][]pos
var antinodes map[pos]struct{}

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
	antennaPos = map[pos]struct{}{}
	antennas = make(map[rune][]pos)
	antinodes = make(map[pos]struct{})
	ReadFile()
	Parse()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
	Debug()
}

func Parse() {
	for i, r := range input {
		for j, c := range r {
			if c == '.' {
				continue
			}
			pos := pos{
				x: i,
				y: j,
			}
			antennaPos[pos] = struct{}{}
			antennas[c] = append(antennas[c], pos)
		}
	}
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	for k, _ := range antennas {
		CalculateAntinodesForFrequency(k)
	}
	for k, _ := range antennas {
		for _, pos := range antennas[k] {
			_, exists := antinodes[pos]
			if !exists {
				ans++
			}
		}
	}
	ans += len(antinodes)
	return ans, time.Since(start)
}

func CalculateAntinodesForFrequency(freq rune) {
	ants := antennas[freq]
	for i, b := range ants {
		for _, e := range ants[i+1:] {
			diffX, diffY := b.x-e.x, b.y-e.y
			posB := pos{
				x: b.x + diffX,
				y: b.y + diffY,
			}
			for {
				if posB.x < 0 || posB.y < 0 || posB.x >= len(input) || posB.y >= len(input[0]) {
					break
				}
				antinodes[posB] = struct{}{}
				posB.x += diffX
				posB.y += diffY
			}

			posE := pos{
				x: e.x - diffX,
				y: e.y - diffY,
			}
			for {
				if posE.x < 0 || posE.y < 0 || posE.x >= len(input) || posE.y >= len(input[0]) {
					break
				}
				antinodes[posE] = struct{}{}
				posE.x -= diffX
				posE.y -= diffY
			}
		}
	}
}

func Debug() {
	for x, r := range input {
		for y, c := range r {
			pos := pos{
				x: x,
				y: y,
			}
			_, exists := antennaPos[pos]
			if exists {
				fmt.Print(string(c))
			} else {
				_, exists = antinodes[pos]
				if exists {
					fmt.Print("#")
				} else {
					fmt.Print(string(c))
				}
			}
		}
		fmt.Print("\n")
	}
}
