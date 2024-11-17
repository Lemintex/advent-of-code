package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

type spring struct {
	spring string
	group  []int
}

var springs []spring

func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("File not found")
	}
	parseInput(f)
	countConfigs()
}

func parseInput(f []byte) {
	lines := strings.Split(strings.TrimSpace(string(f)), "\n")
	for _, l := range lines {
		s, group, _ := strings.Cut(l, " ")
		g := strings.Split(group, ",")
		var springGroups []int
		for _, sg := range g {
			n, _ := strconv.Atoi(sg)
			springGroups = append(springGroups, n)
		}
		spr := spring{spring: s, group: springGroups}
		springs = append(springs, spr)
	}
}

func countConfigs() {
	ans := 0
	for _, s := range springs {
		ans += count(s.spring, s.group)
	}
	fmt.Println(ans)
}

func count(cfg string, n []int) int {
	if cfg == "" {
		if len(n) == 0 {
			return 1
		} else {
			return 0
		}
	}
	if len(n) == 0 {
		for _, c := range cfg {
			if c == '#' {
				return 0
			}
		}
		return 1
	}
	res := 0
	if strings.ContainsRune(".?", rune(cfg[0])) {
		res += count(cfg[1:], n)
	}

	if strings.ContainsRune("#?", rune(cfg[0])) {
		if n[0] <= len(cfg) && !strings.ContainsAny(cfg[:n[0]], ".") && (n[0] == len(cfg) || cfg[n[0]] != '#') {
			if len(cfg) == n[0] {
				cfg = ""
			} else {
				cfg = cfg[n[0] + 1:]
			}
			res += count(cfg, n[1:])
		}
	}
	return res
}
