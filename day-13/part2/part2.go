package main

import (
	"fmt"
	"os"
	"strings"
)

var mirrors [][]string

func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("File could not be read")
	}
	parseInput(f)
	findTotal()
}

func parseInput(s []byte) {
	maps := strings.Split(strings.TrimSpace(string(s)), "\n\n")
	for _, m := range maps {
		current := strings.Split(m, "\n")
		mirrors = append(mirrors, current)
	}
}

func findTotal() {
	total := 0
	for _, m := range mirrors {
		// horizontal
		total += findMirror(m) * 100
		var b [][]byte
		for _, r := range m {
			b = append(b, []byte(r))
		}
		b = flip(b)
		var s []string
		for _, l := range b {
			s = append(s, string(l))
		}
		// vertical
		total += findMirror(s)
	}
	fmt.Println(total)
}
func findMirror(mirror []string) int {
	for r := 1; r < len(mirror); r++ {
		var above []string
		temp, below := mirror[:r], mirror[r:]
		for i := len(temp) - 1; i >= 0; i-- {
			above = append(above, temp[i])
		}
		size := min(len(above), len(below))
		above, below = above[:size], below[:size]
		diff := 0
		for i := 0; i < size; i++ {
				for b := 0; b < len(above[i]); b++ {
					if above[i][b] != below[i][b] {
						diff++
						if diff > 1 {
							goto next
						}
					}
				}
			}
			next:
		if diff != 1 {
			continue
		}
		return r
	}
	return 0
}

func flip(grid [][]byte) [][]byte {
	rows := len(grid)
	cols := len(grid[0])

	transposed := make([][]byte, cols)
	for i := range transposed {
		transposed[i] = make([]byte, rows)
	}

	for i := 0; i < rows; i++ {
		for j := 0; j < cols; j++ {
			transposed[j][i] = grid[i][j]
		}
	}

	return transposed
}
