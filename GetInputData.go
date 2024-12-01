package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"regexp"
	"strconv"
	"strings"
)

var cookie *string
var cookieFile *string

func InitFlags() {
	cookie = flag.String("cookie", "", "The cookie for getting the inputs")
	cookieFile = flag.String("file", "cookie.txt", "The file the cookie is stored in")
	flag.Parse()
}

func GetCookieFromFile() {
	if *cookie != "" {
		return
	}
	c, _ := os.Open(*cookieFile)
	b, _ := ioutil.ReadAll(c)
	*cookie = string(b)
}

func main() {
	cookie = new(string)
	cookieFile = new(string)
	InitFlags()
	GetCookieFromFile()
	if *cookie == "" {
		fmt.Println("No cookies :\\(")
		return
	}
	*cookie = strings.TrimSpace(*cookie)
	f, _ := os.Open(".")
	years, _ := f.ReadDir(10)
	for _, year := range years {
		if !year.IsDir() || year.Name()[0] == '.' {
			continue
		}
		y, _ := os.Open("./" + year.Name())
		days, _ := y.ReadDir(0)
		for _, day := range days {
			d, _ := os.Open("./" + year.Name() + "/" + day.Name())
			dir, _ := d.ReadDir(0)
			exists := false
			for _, i := range dir {
				if i.IsDir() || i.Name() != "input.txt" {
					continue
				}
				directory := d.Name() + "/input.txt"
				f, _ := os.Open(directory)
				content, _ := ioutil.ReadAll(f)
				if len(content) > 0 {
					exists = true
					break
				}
			}
			if !exists {
				CreateFileWithContents(d.Name())
			}
		}
	}
	fmt.Println("Got all input files! :\\)")
}

func CreateFileWithContents(dir string) {
	y, d := getInput(dir)
	year, day := strconv.Itoa(y), strconv.Itoa(d)
	url := "https://adventofcode.com/" + year + "/day/" + day + "/input"
	client := http.Client{}

	req, _ := http.NewRequest("GET", url, nil)

	req.Header.Set("Cookie", *cookie)

	fmt.Println("Getting input from " + year + " day " + day)
	resp, err := client.Do(req)
	if err != nil || resp.StatusCode != 200 {
		fmt.Println(err)
		return
	}

	body, _ := ioutil.ReadAll(resp.Body)

	f, _ := os.Create(dir + "/input.txt")

	f.Write(body)
	f.Close()
	resp.Body.Close()
}

func getInput(dir string) (int, int) {
	parts := strings.Split(dir, "/")
	year, day := getNumber(parts[1]), getNumber(parts[2])
	y, _ := strconv.Atoi(year)
	d, _ := strconv.Atoi(day)
	return y, d
}

func getNumber(s string) string {
	regex := regexp.MustCompile(`\d+`)
	str := regex.FindString(s)
	return str
}
