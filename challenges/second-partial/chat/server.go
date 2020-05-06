// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"strconv"
	"flag"
	"time"
	"sort"
	"strings"
	"os"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

type User struct{
	username string
	ip string
	channel client
	logTime string
	connection net.Conn
}

var (
	entering = make(chan User)
	leaving  = make(chan User)
	kicked = make(chan User)
	messages = make(chan string) // all incoming client messages
	prompt = "irc-server > "
	admin = ""
	firstConn = false
	clients = make(map[User]bool)
)

func broadcaster() {

	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli.channel <- msg
			}

		case cli := <-entering:
			if(len(clients) == 0){
				if(!firstConn){
					cli.channel <- prompt + "Congrats, you were the first user"
					firstConn = true
				}
				admin = cli.username
				cli.channel <- prompt + "You're the new IRC Server ADMIN"
				fmt.Println(prompt + "[" + admin + "] was promoted as the channel ADMIN")
			}
			clients[cli] = true

		case cli := <-leaving:
			if _,ok := clients[cli]; ok{
				fmt.Println(prompt + "[" + cli.username + "] left")
			} else{
				fmt.Println(prompt + "[" + cli.username + "] was kicked")
			}
			delete(clients, cli)
			close(cli.channel)
			if(admin == cli.username){
				newAdmin(clients)
			}

		case cli := <-kicked:
			delete(clients, cli)
			cli.channel <- prompt + "You're kicked from this channel"
			cli.channel <- prompt + "An ADMIN decided to kick you for misbehaving"
			cli.connection.Close()
			if(cli.username == admin){
				newAdmin(clients)
			}
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	data := make([]byte, 512)
	n, _ := conn.Read(data)
	who := string(data[:n])

	if(userExists(who)){
		fmt.Fprintln(conn, prompt + "The entered username already exists in the server. Please try another one")
		conn.Close()
		close(ch)
		return
	}

	add := conn.RemoteAddr().String()

	newUser := User{who, add, ch, time.Now().Format("15:04:05"),conn}

	ch <- prompt + "Welcome to the Simple IRC Server"
	ch <- prompt + "Your user [" + who + "] is successfully logged"
	fmt.Println(prompt + "New connected user [" + who + "]")
	messages <- prompt + "New connected user [" + who + "]"
	entering <- newUser

	input := bufio.NewScanner(conn)
	for input.Scan() {
		txtIn := input.Text()
		if((txtIn != "" && txtIn[0:1] != "/") || txtIn == ""){
			messages <- who + " > " + txtIn
			continue
		}
		split := strings.Split(txtIn, " ")
		switch command := split[0]; command {
			case "/users":
				if(len(split) != 1){
					ch <- prompt + "Error. Correct usage: /users"
					break
				}
				usernames := make([]string, 0)
				for k,_ := range clients{
					usernames = append(usernames, k.username)
				}
				res := usernames[0]
				for i := 1; i < len(usernames); i++ {
					res += ", " + usernames[i]
				}
				ch <- prompt + res
			case "/msg":
				if(len(split) < 3){
					ch <- prompt + "Error. Correct usage: /msg <user> <msg>"
					break
				}
				msgUser := findUser(split[1])
				if(msgUser.username == ""){
					ch <- prompt + "Error. User not found."
					break
				}
				res := split[2]
				for i := 3; i < len(split); i++{
					res += " " + split[i]
				}
				msgUser.channel <- "Direct message from " + who + " > " + res
			case "/time":
				if(len(split) != 1){
					ch <- prompt + "Error. Correct usage: /time"
					break
				}
				loc,_ := time.LoadLocation(os.Getenv("TZ"))
				ch <- prompt + "Local Time: " + loc.String() + " " + time.Now().In(loc).Format("15:04")
			case "/user":
				if(len(split) != 2) {
					ch <- prompt + "Error. Correct usage: /user <user>"
					break
				}
				infUser := findUser(split[1])
				if(infUser.username == ""){
					ch <- prompt + "Error. User not found"
					break
				}
				ch <- prompt + "username: " + infUser.username + ", IP: " + infUser.ip
			case "/kick":
				if(who != admin){
					ch <- prompt + "Error. Only the server's ADMIN can use this command"
					break
				}
				if(len(split) != 2){
					ch <- prompt + "Error. Correct usage: /kick <user>"
					break
				}
				kickUser := findUser(split[1])
				if(kickUser.username == ""){
					ch <- prompt + "Error. User not found"
					break
				}
				kicked <- kickUser
				messages <- prompt + "[" + kickUser.username + "] was kicked from channel"
			default:
				ch <- prompt + "Error. Unknown command"
				break
		}

	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- newUser
	messages <- prompt + "[" + who + "] left channel"
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

func findUser(username string) User{
	var retUser User
	for k,_ := range clients{
		if(k.username == username){
			retUser = k
			break
		}
	}
	return retUser
}

//!-handleConn

func newAdmin(list map[User]bool) {

	if(len(list) == 0){
		admin = ""
		fmt.Println(prompt + "No connected clients. There's no designated ADMIN")
		return
	}

	times := make([]string, 0)
	for k, _ := range list {
		times = append(times, k.logTime)
	}

	sort.Strings(times)

	for k, _ := range list {
		if(k.logTime == times[0]){
			admin = k.username
			k.channel <- prompt + "You're the new IRC Server ADMIN"
			fmt.Println(prompt + "[" + admin + "] was promoted as the channel ADMIN")
			return
		}
	}

}

func userExists(user string) bool {
	for k,_ := range clients{
		if(k.username == user){
			return true
		}
	}

	return false
}

//!+main
func main() {

	var hostAdd string
	flag.StringVar(&hostAdd, "host", "localhost", "The host's name or IP address")

	var portNo int
	flag.IntVar(&portNo, "port", 8000, "The server's TCP port")

	flag.Parse()

	_,err := time.LoadLocation(os.Getenv("TZ"))

	if err != nil {
		log.Fatal(err)
	}

	listener, err := net.Listen("tcp", hostAdd + ":" + strconv.Itoa(portNo))
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(prompt + "Simple IRC Server started at " + hostAdd + ":" + strconv.Itoa(portNo))
	fmt.Println(prompt + "Ready for receiving new clients")

	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
