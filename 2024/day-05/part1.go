package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

var input []string
var orderingRules map[int]map[int]struct{}
var pages [][]int

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
	return strings.Split(in, "\n\n"), nil
}

func main() {
	orderingRules = make(map[int]map[int]struct{})
	ReadFile()
	Parse()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Parse() {
	rules := strings.Split(input[0], "\n")
	for _, r := range rules {
		before, after, _ := strings.Cut(r, "|")
		beforeInt, err := strconv.Atoi(before)

		if err != nil {
			log.Fatal(err)
		}

		afterInt, err := strconv.Atoi(after)

		if err != nil {
			log.Fatal(err)
		}

		_, exists := orderingRules[beforeInt]

		if !exists {
			orderingRules[beforeInt] = make(map[int]struct{})
		}
		orderingRules[beforeInt][afterInt] = struct{}{}
	}

	updates := strings.Split(input[1], "\n")
	for i, u := range updates {
		pages = append(pages, []int{})
		pageStrings := strings.Split(u, ",")
		for _, p := range pageStrings {
			page, err := strconv.Atoi(p)

			if err != nil {
				log.Fatal(err)
			}
			pages[i] = append(pages[i], page)
		}
	}
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	for i := range pages {
		ans += isUpdateInOrder(i)
	}
	return ans, time.Since(start)
}

func isUpdateInOrder(i int) int {
	update := pages[i]
	for i, p := range update {
		for j := i+1; j < len(update); j++ {
			q := update[j]
			_, exists := orderingRules[q][p]
			if exists {
				return 0
			}
		}
	}
	return update[len(update)/2]
}
