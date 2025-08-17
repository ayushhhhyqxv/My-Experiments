package main

import (
	"math/rand"
	"fmt"
	"time"
)

func process(num chan int ){
	for n := range num {
		fmt.Println("Pipe processing :",n)
		time.Sleep(time.Second)
	}
	
}

func main() {

	sender := make(chan int)

	go process(sender)

	for {
		sender<-rand.Intn(300)
	}
	


	// sender <- "Check For Sockets" 

	// receiver := <-sender

	// fmt.Println(receiver)
	
	time.Sleep(time.Second * 3)

}