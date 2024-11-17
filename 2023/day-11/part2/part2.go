package main

import (
	"fmt"
	"os"
	"strings"
)

type galaxy struct {
	row, col int
}

var rows map[int]struct{}
var cols map[int]struct{}

var galaxies []galaxy

func main() {
	rows = make(map[int]struct{})
	cols = make(map[int]struct{})
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("File can't read")
	}
	str := strings.Split(strings.TrimSpace(string(f)), "\n")
	ParseInput(str)
	Calculate()
}

func ParseInput(str []string) {
	for i := range str[0] {
		emptyCol := true
		for _, g := range str {
			if []byte(g)[i] == '#' {
				emptyCol = false
				break
			}
		}
		if emptyCol {
			cols[i] = struct{}{}
		}
	}
	for r, s := range str {
		emptyRow := true
		for _, ch := range s {
			if ch == '#' {
				emptyRow = false
				break
			}
		}
		if emptyRow {
			rows[r] = struct{}{}
		}
	}
	for r, s := range str {
		for c, ch := range s {
			if ch == '#' {
				galaxies = append(galaxies, galaxy{r, c})
			}
		}
	}
}

func Calculate() {
	ans := 0
	expansion := 1000000
	for i, g := range galaxies {
		for _, pair := range galaxies[:i] {
			for r := min(g.row, pair.row); r < max(g.row, pair.row); r++ {
				_, exists := rows[r]
				n := 1
				if exists {
					n = expansion
				}
				ans += n
			}
			for c := min(g.col, pair.col); c < max(g.col, pair.col); c++ {
				_, exists := cols[c]
				n := 1
				if exists {
					n = expansion
				}
				ans += n
			}
		}
	}
	fmt.Println(ans)
}
