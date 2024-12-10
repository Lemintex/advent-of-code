package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

type unit struct {
	pos  int
	size int
}

var input string
var files map[int]unit //map of ids -> files
var blanks []unit

func ReadFile() {
	var err error
	input, err = Read("input.txt")
	if err != nil {
		log.Fatal(err)
		return
	}
}

func Read(filename string) (string, error) {
	s, err := os.ReadFile(filename)
	if err != nil {
		log.Fatal(err)
		return "", err
	}
	in := strings.TrimSpace(string(s))

	// depending on the grouping of the input we may want to change the seperator
	return in, nil
}

func main() {
	files = make(map[int]unit)
	ReadFile()
	Parse()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Parse() {
	id := 0
	pos := 0
	for i, c := range input {
		n, err := strconv.Atoi(string(c))
		if err != nil {
			log.Fatal(err)
		}
		u := unit{
			pos:  pos,
			size: n,
		}
		if i%2 == 0 {
			files[id] = u
			id++
		} else {
			if n == 0 {
				continue
			}
			blanks = append(blanks, u)
		}
		pos += n
	}
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	for id := len(files) - 1; id >= 0; id--{
		f := files[id]
		for i, b := range blanks {
			if b.pos >= f.pos {
				blanks = blanks[:i]
				break
			}
			if f.size <= b.size {
				u := unit{
					pos:  b.pos,
					size: f.size,
				}
				files[id] = u
				if files[id].size == b.size {
					blanks = append(blanks[:max(i, 0)], blanks[i+1:]...)
				} else {
					blanks[i].pos += files[id].size
					blanks[i].size -= files[id].size
				}
				break
			}
		}
	}

	for id := range files {
		f := files[id]
		for i := range f.size {
			ans += (f.pos + i) * id
		}
	}
	return ans, time.Since(start)
}
