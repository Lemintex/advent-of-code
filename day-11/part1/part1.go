package main
import (
	"os"
	"fmt"
	"strings"
)

var rows []int
var cols []int

func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("File can't read")
	}
	str := strings.Split(string(f), "\n")
	for _, s := range str {
		fmt.Println(s)
	}
	ParseInput(str)
}

func ParseInput(str []string) {
	for i, c := range str[0] {
		if c == 'v' {
			rows = append(rows, i)
		}
	}
	for c, s := range str {
		fmt.Println(s)
		i := strings.Index(s, ">")
		if i >= 0 {
			cols = append(cols, c)
		}
		// if s[0] == '>' {
		// 	cols = append(cols, c)
		// }
	}
	fmt.Println(rows, cols)
}
