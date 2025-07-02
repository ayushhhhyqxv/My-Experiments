package main

import "fmt"

type payment struct{
	gateway stripe
}

func (p payment) makePayment(amount float32) {
	// newPayment := razorpay{}
	// newPayment.pay(amount)
	p.gateway.pay(amount)
}

type razorpay struct{}

type stripe struct{}

func (r razorpay) pay(amount float32) {
	// write own actual logic with apis
	fmt.Println("Making Payment via Razorpay",amount)
}

func (s stripe) pay(amount float32) {
	// write own actual logic with apis
	fmt.Println("Making Payment via Stripe",amount)
}

func main() {
	mode := stripe{}
	yourPayment := payment{
		gateway: mode,
	}
	yourPayment.makePayment(200)
}